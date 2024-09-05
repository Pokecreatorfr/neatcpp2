{
    files = {
        [[src\matrix.cpp]]
    },
    values = {
        [[C:\MinGW\bin\x86_64-w64-mingw32-g++]],
        {
            "-m64",
            "-g",
            "-O3",
            "-std=c++17",
            "-Iinclude",
            "-isystem",
            [[C:\Users\MatheoVIGNAUD\AppData\Local\.xmake\packages\l\libsdl\2.30.3\718e3415fffe4fb1b9aecca9fa4ec857\include]],
            "-isystem",
            [[C:\Users\MatheoVIGNAUD\AppData\Local\.xmake\packages\l\libsdl\2.30.3\718e3415fffe4fb1b9aecca9fa4ec857\include\SDL2]]
        }
    },
    depfiles_gcc = "matrix.o: src\\matrix.cpp include/matrix.hpp\
"
}