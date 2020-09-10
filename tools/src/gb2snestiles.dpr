{$APPTYPE CONSOLE}
uses  windows, classes, sysutils;

var   src, dest : tFileStream;
      fn        : ansistring;

const empty : array[0..15] of ansichar = #0#0#0#0#0#0#0#0#0#0#0#0#0#0#0#0;
var   buf   : array[0..15] of ansichar;

begin
  fn:= paramstr(1);

  if fileexists(fn) then begin
    src  := tFileStream.create(fn, fmOpenRead or fmShareDenyWrite);
    dest := tFileStream.create(changefileext(fn, '.4bpp'), fmCreate);
    try
      while (src.Read(buf, sizeof(buf)) = sizeof(buf)) do begin
        dest.Write(buf, sizeof(buf));
        dest.Write(empty, sizeof(empty));
      end;
    finally
      freeandnil(src);
      freeandnil(dest);
    end;
  end else writeln('2bpp to 4bpp tile conversion utility.'#$0d#$0a'USAGE: gb2snestiles <binary file>');
end.
