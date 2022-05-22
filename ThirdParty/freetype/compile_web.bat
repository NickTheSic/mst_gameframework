mkdir lib
pushd lib

set INCLUDES=-I../include
set DEFINES=-DFT2_BUILD_LIBRARY

emcc %INCLUDES% %DEFINES% -c ../src/autofit/autofit.c -c ../src/base/ftbase.c  -c ../src/base/ftbbox.c -c ../src/base/ftbdf.c -c ../src/base/ftbitmap.c  -c ../src/base/ftcid.c  -c ../src/base/ftdebug.c -c ../src/base/ftfstype.c  -c ../src/base/ftgasp.c  -c ../src/base/ftglyph.c  -c ../src/base/ftgxval.c  -c ../src/base/ftinit.c  -c ../src/base/ftmm.c  -c ../src/base/ftotval.c  -c ../src/base/ftpatent.c  -c ../src/base/ftpfr.c  -c ../src/base/ftstroke.c  -c ../src/base/ftsynth.c  -c ../src/base/ftsystem.c -c ../src/base/fttype1.c  -c ../src/base/ftwinfnt.c  -c ../src/bdf/bdf.c  -c ../src/bzip2/ftbzip2.c  -c ../src/cache/ftcache.c  -c ../src/cff/cff.c  -c ../src/cid/type1cid.c  -c ../src/gzip/ftgzip.c  -c ../src/lzw/ftlzw.c  -c ../src/pcf/pcf.c  -c ../src/pfr/pfr.c  -c ../src/psaux/psaux.c  -c ../src/pshinter/pshinter.c  -c ../src/psnames/psnames.c  -c ../src/raster/raster.c  -c ../src/sdf/sdf.c  -c ../src/sfnt/sfnt.c  -c ../src/smooth/smooth.c  -c ../src/svg/svg.c  -c ../src/truetype/truetype.c  -c ../src/type1/type1.c  -c ../src/type42/type42.c  -c ../src/winfonts/winfnt.c && emar rcs libfreetype.a autofit.o ftbase.o ftbbox.o ftbdf.o ftbitmap.o ftcid.o ftdebug.o ftfstype.o ftgasp.o ftglyph.o ftgxval.o ftinit.o ftmm.o ftotval.o ftpatent.o ftpfr.o ftstroke.o ftsynth.o ftsystem.o fttype1.o ftwinfnt.o bdf.o ftbzip2.o ftcache.o cff.o type1cid.o ftgzip.o ftlzw.o pcf.o pfr.o psaux.o pshinter.o psnames.o raster.o sdf.o sfnt.o smooth.o svg.o truetype.o type1.o type42.o winfnt.o


popd
pause
 