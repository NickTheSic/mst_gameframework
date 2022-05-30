mkdir Build\WEB
pushd Build\WEB

set INCLUDE=-I../../ThirdParty/ -I../../ThirdParty/stb_image/ -I../../ThirdParty/freetype -I../../MSTEngine -I../../ThirdParty/freetype/include
set MYGAME=../../Sandbox/Demo.cpp 

set MSTENGINE=../../MSTEngine/main.cpp ../../MSTEngine/Core/Engine.cpp ../../MSTEngine/Core/Timer.cpp ../../MSTEngine/Core/Camera.cpp ../../MSTEngine/Renderer/BaseRenderer.cpp ../../MSTEngine/Renderer/QuadRenderer.cpp ../../MSTEngine/Renderer/TextRenderer.cpp ../../MSTEngine/Renderer/SpriteSheetRenderer.cpp ../../ThirdParty/stb_image/stb_image.c

set FREETYPE= -L../../ThirdParty/freetype/lib -lfreetype

set FLAGS= -O2 -s ALLOW_MEMORY_GROWTH=1 -DPLATFORM_WEB -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2
echo -s FULL_ES2 -s FULL_ES3

call em++ %INCLUDE% %FREETYPE% %FLAGS% %MSTENGINE% %MYGAME% -o index.html --shell-file ../../MinimalShell.html --preload-file Data

popd

pause

