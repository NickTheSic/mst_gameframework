# mst_gameframework
An open GL game framework.  I used this to create my GMTK 2022 Jam entry https://nickthesic.itch.io/dicey-sokoban

# About
Inspired by One Lone Coders Pixel Engine, I liked the header only aspect of it.
Can compile for Windows and Web

## How to compile
The easiest way is to download and run premake to generate the files for your specific platform.  This has only been tested on windows.
Alternatively if you have emscripten setup you can run the bat files to compile the bat files.

## How to use
Inherit from the mst::Engine and overide UserStartup(), UserUpdate() and UserRender().

No need for a main function as it is included in the library.  This is so you only have to focus on your game class

## Third party libraries used
stb_image
Freetype
glm
