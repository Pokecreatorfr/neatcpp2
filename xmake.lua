add_rules("mode.debug", "mode.release")

set_languages("c++17")
set_optimize("fastest")

add_requires("libsdl")

target("Neat_CPP2")
    set_kind("binary")
    add_includedirs("include")
    add_files("src/*.cpp")
    add_packages("libsdl")
    add_links("ws2_32")

target("snake_sdl")
    set_kind("binary")
    add_files("snake_sdl/src/*.cpp")
    add_packages("libsdl")
    add_links("ws2_32")