set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64-w64-mingw32-cpp)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-c++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

#set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32/sys-root/mingw/)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(OPENAL_INCLUDE_DIR /home/user/fi.xnet.dhewm3/target/mingw64/dhewm3-libs/x86_64-w64-mingw32/include)
set(OPENAL_LIBRARY /home/user/fi.xnet.dhewm3/target/mingw64/dhewm3-libs/x86_64-w64-mingw32/lib)

set(SDL2_INCLUDE_DIR /home/user/fi.xnet.dhewm3/target/mingw64/dhewm3-libs/x86_64-w64-mingw32/include)