set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64-w64-mingw32-cpp)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-c++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

set(CMAKE_FIND_ROOT_PATH  /usr/x86_64-w64-mingw32/sys-root/mingw/)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(OPENAL_INCLUDE_DIR /home/C933925/fi.xnet.dhewm3/target/mingw64/dhewm3-libs/x86_64-w64-mingw32/include/)
set(OPENAL_LIBRARY /home/C933925/fi.xnet.dhewm3/target/mingw64/dhewm3-libs/x86_64-w64-mingw32/lib/libOpenAL32.dll.a)

#set(SDL2_INCLUDE_DIR /usr/x86_64-w64-mingw32/sys-root/mingw/include/)
#set(SDL2_LIBRARY /usr/x86_64-w64-mingw32/sys-root/mingw/lib/libSDL2.dll.a)

set(SDL2_INCLUDE_DIR /home/C933925/Downloads/SDL2-2.0.3/x86_64-w64-mingw32/include)
set(SDL2_LIBRARY /home/C933925/Downloads/SDL2-2.0.3/x86_64-w64-mingw32/lib/libSDL2.a)
set(SDL2MAIN_LIBRARY /home/C933925/Downloads/SDL2-2.0.3/x86_64-w64-mingw32/lib/libSDL2main.a)
