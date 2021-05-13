@echo off

set freetype_include = /I C:\GitHub\asteroids\free_type\include
set glfw_lib = C:\GitHub\asteroids\glfw-3.3.3.bin.WIN64\lib-vc2017\glfw3.lib
set freetype_lib = C:\GitHub\asteroids\free_type\release_static\win64\freetype.lib 

set code = %cd%
set opts = /EHsc -FC -GR- -EHa- -nologo -Zi /MD %freetype_include%
set libs = OpenGL32.lib gdi32.lib user32.lib kernel32.lib Shell32.lib 

IF NOT EXIST build mkdir build
pushd build

cl %opts% %code%\win32_asteroids.cpp -Feasteroids %libs% %glfw_lib% %freetype_lib%

popd
 