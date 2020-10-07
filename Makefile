CC = ../../gbdk/bin/lcc
CFLAGS = -Wm-ys -Wl-j -Wl-m -Wl-w -Wf'--max-allocs-per-node 50000' -Wf'--peep-file peephole\gbz80.rul'

all:	clean rom

%.gb:
	$(CC) $(CFLAGS) -o $@ src/test_sgb.c src/sgb_border.c

clean:
	rm -rf test_sgb.*

rom: test_sgb.gb

symbols:
	python ./utils/noi2sym.py test_sgb.noi >test_sgb.sym