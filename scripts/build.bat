@echo off

mkdir bin > NUL 2>&1

set APPLICATION_NAME=huffman

cl src\*.cpp ^
        /std:c++17 ^
        /EHsc ^
        /Fe: bin\%APPLICATION_NAME%.exe ^
        /Fo: bin\ ^
        /Fd: bin\%APPLICATION_NAME%.pdb ^
        /Zi ^
        /I"src" ^
        /nologo