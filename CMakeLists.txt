cmake_minimum_required(VERSION 3.26)
project(Lab4 C)

set(CMAKE_C_STANDARD 90)

set(CMAKE_C_FLAGS "/TC /Gz")
set(CMAKE_SHARED_LINKER_FLAGS "/DYNAMICBASE")
set(CMAKE_EXE_LINKER_FLAGS "/DYNAMICBASE")

add_definitions(-DECO_WINDOWS)
add_definitions(-D_DEBUG)
add_definitions(-DECO_DLL)
add_definitions(-DUGUID_UTILITY)
add_definitions(-DECO_WIN32)
add_definitions(-D_WIN32)
add_definitions(-D_WINDOWS)
add_definitions(-D_CONSOLE)

include_directories(src/allocator/HeaderFiles)
include_directories(src/allocator/SharedFiles)
include_directories(src/allocator/SourceFiles)
include_directories(src)

set(ECO_FRAMEWORK_DOT_LIB
    $ENV{ECO_FRAMEWORK}/Eco.InterfaceBus1/BuildFiles/Windows/x86/StaticRelease/00000000000000000000000042757331.lib
    $ENV{ECO_FRAMEWORK}/Eco.MemoryManager1/BuildFiles/Windows/x86/StaticRelease/0000000000000000000000004D656D31.lib
    $ENV{ECO_FRAMEWORK}/Eco.FileSystemManagement1/BuildFiles/Windows/x86/StaticRelease/00000000000000000000000046534D31.lib
    $ENV{ECO_FRAMEWORK}/Eco.System1/BuildFiles/Windows/x86/StaticRelease/00000000000000000000000000000100.lib
    $ENV{ECO_FRAMEWORK}/Eco.List1/BuildFiles/Windows/x86/StaticRelease/53884AFC93C448ECAA929C8D3A562281.lib
)


include_directories(
    $ENV{ECO_FRAMEWORK}/Eco.List1/SharedFiles
    $ENV{ECO_FRAMEWORK}/Eco.Core1/SharedFiles
    $ENV{ECO_FRAMEWORK}/Eco.InterfaceBus1/SharedFiles
    $ENV{ECO_FRAMEWORK}/Eco.System1/SharedFiles
    $ENV{ECO_FRAMEWORK}/Eco.MemoryManager1/SharedFiles
    $ENV{ECO_FRAMEWORK}/Eco.FileSystemManagement1/SharedFiles
)

add_library(
    1F5DF16EE1BF43B999A434ED38FE8F3C
    SHARED
    src/allocator/SourceFiles/CEcoLab4.c
    src/allocator/HeaderFiles/CEcoLab4.h
    src/allocator/HeaderFiles/CEcoLab4Factory.h
    src/allocator/SourceFiles/CEcoLab4Factory.c
    src/allocator/SharedFiles/IdEcoLab4.h
    src/allocator/SharedFiles/IEcoLab4.h
    ${ECO_CALC_LIB}
)

add_executable(
    Lab4
    src/main.c
)

target_link_libraries(
    Lab4
    1F5DF16EE1BF43B999A434ED38FE8F3C
    ${ECO_FRAMEWORK_DOT_LIB}
)