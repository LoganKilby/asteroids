@echo off

set opts= /EHsc -FC -GR- -EHa- -nologo -Zi /MD /I C:\Library\glfw-3.3.3.bin.WIN64\include\GLFW /I C:\Work\Asteroids_OpenGL\glm\glm /I C:\Library\free_type\include

set code=%cd%
set libs=  C:\Library\glfw-3.3.3.bin.WIN64\lib-vc2017\glfw3.lib OpenGL32.lib gdi32.lib user32.lib kernel32.lib Shell32.lib C:\Library\free_type\release_static\win64\freetype.lib 

IF NOT EXIST build mkdir build
pushd build

cl %opts% %code%\practice.cpp -Feasteroids %libs%
popd
 