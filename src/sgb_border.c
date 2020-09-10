#include "sgb_border.h"

#include <gb/sgb.h>
#include <string.h>

unsigned char map_buf[20];

#define SGB_TRANSFER(A,B) map_buf[0]=(A),map_buf[1]=(B),sgb_transfer(map_buf) 

#define SGB_CHR_BLOCK0 0
#define SGB_CHR_BLOCK1 1

void transfer_tiles(unsigned char * data, size_t size) {
    UBYTE ntiles;
    if (size > 256 * 32) ntiles = 0; else ntiles = size >> 5;
    
    if ((!ntiles) || (ntiles > 128U)) { 
        set_bkg_data(0, 0, data); 
        if (ntiles) ntiles -= 128U; 
        data += (128 * 32);
    } else { 
        set_bkg_data(0, ntiles << 1, data); 
        SGB_TRANSFER((SGB_CHR_TRN << 3) | 1, SGB_CHR_BLOCK0);
        return;
    }
    SGB_TRANSFER((SGB_CHR_TRN << 3) | 1, SGB_CHR_BLOCK0);

    set_bkg_data(0, ntiles << 1, data); 
    SGB_TRANSFER((SGB_CHR_TRN << 3) | 1, SGB_CHR_BLOCK1);
}

void set_sgb_border(unsigned char * tiledata, size_t tiledata_size,
                    unsigned char * tilemap, size_t tilemap_size,
                    unsigned char * palette, size_t palette_size) {
    if (sgb_check()) {
        BGP_REG = OBP0_REG = OBP1_REG = 0xE4U;
        SGB_TRANSFER((SGB_MASK_EN << 3) | 1, 1); 

        UBYTE tmp_lcdc = LCDC_REG;

        HIDE_SPRITES, HIDE_WIN, SHOW_BKG;
        DISPLAY_ON;

        SCX_REG = SCY_REG = 0U;

        // prepare tilemap for SGB_BORDER_CHR_TRN (should display all 256 tiles)
        UBYTE i = 0U;
        for (UBYTE y = 0; y != 14U; ++y) {
            UBYTE * dout = map_buf;
            for(UBYTE x = 0U; x != 20U; ++x, *dout++ = i++);
            set_bkg_tiles(0, y, 20, 1, map_buf);
        }

        transfer_tiles(tiledata, tiledata_size);

        // transfer map and palettes
        set_bkg_data(0, (UBYTE)(tilemap_size >> 4), tilemap);
        set_bkg_data(128, (UBYTE)(palette_size >> 4), palette);
        SGB_TRANSFER((SGB_PCT_TRN << 3) | 1, 0);

        LCDC_REG = tmp_lcdc;

        // clear SCREEN
        memset(map_buf, 0, sizeof(map_buf));
        set_bkg_data(0, 1, map_buf);
        for (UBYTE y = 0; y != 18U; ++y)
            set_bkg_tiles(0, y, 20, 1, map_buf);
        
        SGB_TRANSFER((SGB_MASK_EN << 3) | 1, 0); 
    }
}
