{
    values = {
        [[C:\MinGW\bin\x86_64-w64-mingw32-g++]],
        {
            "-m64",
            "-g",
            "-O3",
            "-std=c++17",
            "-Iinclude",
            "-isystem",
            [[C:\Users\mathe\AppData\Local\.xmake\packages\l\libsdl\2.30.6\a0b5b76acc3449dab95ee806391380c5\include]],
            "-isystem",
            [[C:\Users\mathe\AppData\Local\.xmake\packages\l\libsdl\2.30.6\a0b5b76acc3449dab95ee806391380c5\include\SDL2]]
        }
    },
    files = {
        [[src\network.cpp]]
    },
    depfiles_gcc = "network.o: src\\network.cpp include/network.hpp include/activation.hpp  include/matrix.hpp\
"
}