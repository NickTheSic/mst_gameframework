mkdir lib
pushd lib

set INCLUDE=-I../ -I../../ThirdParty -I../../ThirdParty/freetype/include
set DEFINES=-DPLATFORM_WEB

em++ %INCLUDE% %DEFINES% -c ../main.cpp -c ../Core/Engine.cpp -c ../Core/Timer.cpp -c ../Core/Camera.cpp -c ../Renderer/BaseRenderer.cpp -c ../Renderer/QuadRenderer.cpp -c ../Renderer/TextRenderer.cpp -c ../Renderer/SpriteSheetRenderer.cpp ../Utils.cpp ../Renderer/Shader.cpp && emar rcs libMSTEngine.a main.o Engine.o Timer.o Camera.o BaseRenderer.o QuadRenderer.o TextRenderer.o SpriteSheetRenderer.o Utils.o Shader.o

popd
pause