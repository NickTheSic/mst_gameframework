mkdir lib
pushd lib

set INCLUDE=-I../ -I../../ThirdParty
set DEFINES=-DPLATFORM_WEB

em++ %INCLUDE% %DEFINES% -c ../main.cpp -c ../Core/Engine.cpp -c ../Core/Timer.cpp -c ../Core/Camera.cpp -c ../Renderer/BatchRenderer.cpp -c ../Renderer/RenderUtils.cpp -c ../Renderer/TextRenderer.cpp

popd
pause