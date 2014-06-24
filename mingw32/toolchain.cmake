set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64-w64-mingw32-cpp)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-c++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

set(CMAKE_FIND_ROOT_PATH  /usr/x86_64-w64-mingw32/sys-root/mingw/)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(OPENAL_INCLUDE_DIR /usr/share/tomcat/.jenkins/workspace/fi.xnet.dhewm3-mingw32-w64/mingw32/openal-soft-1.15.1-x86_64-w64-mingw32/include/)
set(OPENAL_LIBRARY /usr/share/tomcat/.jenkins/workspace/fi.xnet.dhewm3-mingw32-w64/mingw32/openal-soft-1.15.1-x86_64-w64-mingw32/lib/libOpenAL32.dll.a)

set(SDL2_INCLUDE_DIR /usr/share/tomcat/.jenkins/workspace/fi.xnet.dhewm3-mingw32-w64/mingw32/SDL2-2.0.3-x86_64-w64-mingw32/include/)
set(SDL2_LIBRARY /usr/share/tomcat/.jenkins/workspace/fi.xnet.dhewm3-mingw32-w64/mingw32/SDL2-2.0.3-x86_64-w64-mingw32/lib/libSDL2.a)
set(SDL2DLL_LIBRARY /usr/share/tomcat/.jenkins/workspace/fi.xnet.dhewm3-mingw32-w64/mingw32/SDL2-2.0.3-x86_64-w64-mingw32/lib/libSDL2.dll.a)
set(SDL2MAIN_LIBRARY -Wl,--whole-archive /usr/share/tomcat/.jenkins/workspace/fi.xnet.dhewm3-mingw32-w64/mingw32/SDL2-2.0.3-x86_64-w64-mingw32/lib/libSDL2main.a -Wl,--no-whole-archive)
