mkdir lib
pushd lib
emcc -c ../stb_truetype.c && emar rcs libstb_truetype.a stb_truetype.o
popd