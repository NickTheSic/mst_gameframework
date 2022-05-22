mkdir Build\WEB
pushd Build\WEB

set INCLUDE=-I../../ThirdParty/ -I../../ThirdParty/freetype -I../../MSTEngine -I../../ThirdParty/freetype/include
set MYGAME=../../Sandbox/Demo.cpp 

set MSTENGINE=../../MSTEngine/main.cpp ../../MSTEngine/Core/Engine.cpp ../../MSTEngine/Core/Timer.cpp ../../MSTEngine/Core/Camera.cpp ../../MSTEngine/Renderer/BaseRenderer.cpp ../../MSTEngine/Renderer/QuadRenderer.cpp ../../MSTEngine/Renderer/TextRenderer.cpp

set FREETYPE= -L../../ThirdParty/freetype/lib -lfreetype

set FLAGS= -O2 -sALLOW_MEMORY_GROWTH=1 -DPLATFORM_WEB -sMAX_WEBGL_VERSION=2 -sMIN_WEBGL_VERSION=2 -sFULL_ES2
rem -sFULL_ES2 -sFULL_ES3

em++ %INCLUDE% %FREETYPE% %FLAGS% %MSTENGINE% %MYGAME% -o index.html --preload-file Data





rem../../ThirdParty/freetype/src/autofit/autofit.c
rem../../ThirdParty/freetype/src/base/ftbase.c 
rem../../ThirdParty/freetype/src/base/ftbbox.c
rem../../ThirdParty/freetype/src/base/ftbdf.c
rem../../ThirdParty/freetype/src/base/ftbitmap.c 
rem../../ThirdParty/freetype/src/base/ftcid.c 
rem../../ThirdParty/freetype/src/base/ftdebug.c
rem../../ThirdParty/freetype/src/base/ftfstype.c 
rem../../ThirdParty/freetype/src/base/ftgasp.c 
rem../../ThirdParty/freetype/src/base/ftglyph.c 
rem../../ThirdParty/freetype/src/base/ftgxval.c 
rem../../ThirdParty/freetype/src/base/ftinit.c 
rem../../ThirdParty/freetype/src/base/ftmm.c 
rem../../ThirdParty/freetype/src/base/ftotval.c 
rem../../ThirdParty/freetype/src/base/ftpatent.c 
rem../../ThirdParty/freetype/src/base/ftpfr.c 
rem../../ThirdParty/freetype/src/base/ftstroke.c 
rem../../ThirdParty/freetype/src/base/ftsynth.c 
rem../../ThirdParty/freetype/src/base/ftsystem.c
rem../../ThirdParty/freetype/src/base/fttype1.c 
rem../../ThirdParty/freetype/src/base/ftwinfnt.c 
rem../../ThirdParty/freetype/src/bdf/bdf.c 
rem../../ThirdParty/freetype/src/bzip2/ftbzip2.c 
rem../../ThirdParty/freetype/src/cache/ftcache.c 
rem../../ThirdParty/freetype/src/cff/cff.c 
rem../../ThirdParty/freetype/src/cid/type1cid.c 
rem../../ThirdParty/freetype/src/gzip/ftgzip.c 
rem../../ThirdParty/freetype/src/lzw/ftlzw.c 
rem../../ThirdParty/freetype/src/pcf/pcf.c 
rem../../ThirdParty/freetype/src/pfr/pfr.c 
rem../../ThirdParty/freetype/src/psaux/psaux.c 
rem../../ThirdParty/freetype/src/pshinter/pshinter.c 
rem../../ThirdParty/freetype/src/psnames/psnames.c 
rem../../ThirdParty/freetype/src/raster/raster.c 
rem../../ThirdParty/freetype/src/sdf/sdf.c 
rem../../ThirdParty/freetype/src/sfnt/sfnt.c 
rem../../ThirdParty/freetype/src/smooth/smooth.c 
rem../../ThirdParty/freetype/src/svg/svg.c 
rem../../ThirdParty/freetype/src/truetype/truetype.c 
rem../../ThirdParty/freetype/src/type1/type1.c 
rem../../ThirdParty/freetype/src/type42/type42.c 
rem../../ThirdParty/freetype/src/winfonts/winfnt.c

