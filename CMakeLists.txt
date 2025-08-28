cmake_minimum_required(VERSION 3.10...4.0)
project(sgl)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
#MESSAGE(STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})

# Eventually, we will want to switch to C++20 for Emscripten to be able to use co_await and co_yield as a replacement for
# emscripten_sleep with "-sASYNCIFY".
#if (EMSCRIPTEN)
#    set(CMAKE_CXX_STANDARD 20)
#else()
set(CMAKE_CXX_STANDARD 17)
#endif()
set(CMAKE_DEBUG_POSTFIX d)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c src/*.hpp src/*.h)

if (APPLE)
    # Disable '-Wnullability-completeness' warnings caused by VMA using '_Nonnull'.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-nullability-completeness")
endif()

set(BUILD_STATIC_LIBRARY OFF)
if (${BUILD_STATIC_LIBRARY})
    add_library(sgl STATIC ${SOURCES})
    target_compile_definitions(sgl INTERFACE SGL_STATIC_LIBRARY)
else()
    add_library(sgl SHARED ${SOURCES})
endif()

if (UNIX AND NOT APPLE)
    # As of 2024-05-02, the following issue appeared on some CI pipeline configurations:
    # /usr/bin/ld: libsgl.a(imgui_impl_opengl3.cpp.o): undefined reference to symbol 'dlclose@@GLIBC_2.2.5'
    target_link_libraries(sgl PUBLIC ${CMAKE_DL_LIBS})
endif()
if (APPLE)
    target_link_libraries(sgl PRIVATE "-framework CoreFoundation" "-framework CoreServices")
endif()

target_compile_definitions(sgl PUBLIC DLL_OBJECT=)

# According to https://devblogs.microsoft.com/cppblog/improved-openmp-support-for-cpp-in-visual-studio/,
# support for LLVM OpenMP was added with Visual Studio 2019 version 16.9. According to
# https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170,
# both Visual Studio 2019 version 16.8 and 16.9 have _MSC_VER / MSVC_VERSION 1928, so we need to check
# for a minimum version 1929.
if (EMSCRIPTEN)
    # TODO: https://github.com/emscripten-core/emscripten/issues/13892
elseif (APPLE)
    # There are some problems with Homebrew OpenMP as of 2022-10-20.
    find_package(OpenMP QUIET)
    if (OpenMP_FOUND)
        MESSAGE(STATUS "Found OpenMP version ${OpenMP_CXX_VERSION}. Enabling OpenMP threading support.")
    else()
        MESSAGE(STATUS "OpenMP not found. Disabling OpenMP threading support.")
    endif()
else()
    find_package(OpenMP REQUIRED)
endif()
if (OpenMP_FOUND)
    if(NOT TARGET OpenMP::OpenMP_CXX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    else()
        target_link_libraries(sgl PRIVATE OpenMP::OpenMP_CXX)
    endif()
endif()
