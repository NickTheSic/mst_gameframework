mkdir lib
pushd lib
emcc -c ../stb_image.c && emar rcs libstb_image.a stb_image.o
popd