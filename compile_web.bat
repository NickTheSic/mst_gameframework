pushd Build
mkdir WEB
pushd WEB

set THIRDPARTY=../../ThirdParty
set INCLUDES=-I%THIRDPARTY% -I../../MSTEngine -I%THIRDPARTY%
set DEFINES=
set LIBS=-L%THIRDPARTY%/freetype/lib -lfreetype -L%THIRDPARTY%
set FLAGS=-O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -sFULL_ES2 -sFULL_ES3

em++ %DEFINES% %INCLUDES% -c ../../Sandbox/Demo.cpp 

em++ %FLAGS% Demo.o %LIBS% -o mygame.html --preload-file ../../Data

pause