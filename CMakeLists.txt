CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(trillek-server-core)

# Set the directory of cmake modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/common/modules")

# Put the executable in the bin folder
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Install the libraries using bin folder as root
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")

# Set the directory for the header files
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/common/include)

# set options early
SET(TCC_TEST_NETWORK_DISABLE OFF CACHE BOOL "Disable the network unit test")

# find all source files in the src directory
FILE(GLOB_RECURSE TCC_SRC "src/*.cpp" "common/src/*.cpp")

# Put the main_XXX.cpp files into a filter
FILE(GLOB TCC_MAIN_CPP "main/*.cpp")

# find all the files in the includes directory
FILE(GLOB_RECURSE TCC_INCLUDES "include/*.h" "include/*.hpp" "common/include/*.h"
	"common/include/*.hpp")

#Check if we can build with the current chaintool
INCLUDE(SetCompilerFlags)

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/" "${CMAKE_SOURCE_DIR}/common/include/")

# If we are on windows add in the local search directories as well.
IF (WIN32 AND NOT MINGW) # Windows
    SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/lib/include/)
    INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib/include/")
    IF (CMAKE_CL_64)
    LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib/x64/debug" "${CMAKE_SOURCE_DIR}/lib/x64/release")
    SET(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/x64/debug ${CMAKE_SOURCE_DIR}/lib/x64/release)
    ELSE (CMAKE_CL_64)
    LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib/x86/debug" "${CMAKE_SOURCE_DIR}/lib/x86/release")
    SET(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/x86/debug ${CMAKE_SOURCE_DIR}/lib/x86/release)
    ENDIF (CMAKE_CL_64)
ENDIF (WIN32 AND NOT MINGW)

FIND_PACKAGE(GLM REQUIRED)
# Force using radians as degrees is deprecated
# Bullet is built using doubles
ADD_DEFINITIONS(-DGLM_FORCE_RADIANS -DBT_USE_DOUBLE_PRECISION)

IF (MSVC) # Visual C++
    FIND_PACKAGE(GLFW3 REQUIRED)
ENDIF (MSVC)

FIND_PACKAGE(RapidJSON REQUIRED)
FIND_PACKAGE(RakNet REQUIRED PATH ${CMAKE_SOURCE_DIR}/common/)
FIND_PACKAGE(Bullet REQUIRED)
FIND_PACKAGE(Lua REQUIRED)
FIND_PACKAGE(LuaWrapper REQUIRED)
INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})

# Give these some dummy values and if the platform is LINUX or OSX they will be set accordingly.
SET(X11_LIBRARIES "")
SET(OSX_LIBRARIES "")

IF (APPLE) # Mac OSX
    SET(GLEW_LIBRARY "") # Set a dummy value for GLEW.

    SET(CMAKE_XCODE_ATTRIBUTE_SUPPORTED_PLATFORMS macosx)

    # Need the 10.7 SDK or later.
    EXECUTE_PROCESS(COMMAND xcodebuild -sdk macosx -version SDKVersion OUTPUT_VARIABLE OSX_SDK_VERSION)
    IF (NOT (OSX_SDK_VERSION VERSION_GREATER 10.7 OR OSX_SDK_VERSION VERSION_EQUAL 10.7))
        MESSAGE(FATAL_ERROR "The installed version of Xcode does not support the 10.7 SDK or later.  Please upgrade Xcode and try again.")
    ENDIF (NOT (OSX_SDK_VERSION VERSION_GREATER 10.7 OR OSX_SDK_VERSION VERSION_EQUAL 10.7))

    # Configure the project to use the correct SDK.
    IF (XCODE_VERSION)
        SET(CMAKE_OSX_SYSROOT macosx)
    ELSE (XCODE_VERSION)
        # Non-Xcode generators need the full path.
        EXECUTE_PROCESS(COMMAND xcodebuild -sdk macosx -version Path | head -n 1 OUTPUT_VARIABLE CMAKE_OSX_SYSROOT)
        STRING(REGEX REPLACE "(\r?\n)+$" "" CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}")
    ENDIF (XCODE_VERSION)

    # Can deploy back to 10.7, the first OS X to support the GL Core.
    SET(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)

    # Need Core Foundation and libobjc.
    SET(OSX_LIBRARIES "-framework CoreFoundation /usr/lib/libobjc.dylib")
ENDIF (APPLE)

LINK_DIRECTORIES(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# define all required external libraries
SET(Backend_LIBS
    ${RAKNET_LIBRARIES}
    ${BULLET_DYNAMICS_LIBRARIES}
    ${BULLET_COLLISION_LIBRARIES}
    ${BULLET_LINEARMATH_LIBRARIES}
    ${LUA_LIBRARIES}
    )

ADD_SUBDIRECTORY (${CMAKE_SOURCE_DIR}/common/trillek-vcomputer-module)
#Dirty addition of vcomputer
#TODO: make a cmake module
IF (MSVC) # Visual C++
    SET(Backend_LIBS
        ${Backend_LIBS}
        VCOMPUTER.lib
        )
ELSE (MSVC)
    SET(Backend_LIBS
        ${Backend_LIBS}
	VCOMPUTER
        )
ENDIF (MSVC) # Visual C++

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/common/trillek-vcomputer-module/include")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/common/trillek-vcomputer-module/src")

SET(Trillek_Server_LIBS ${Backend_LIBS})

# Visio C++ needs GLFW for the clock
IF (MSVC) # Visual C++
    SET(Trillek_Server_LIBS ${Trillek_Server_LIBS} ${GLFW3_LIBRARIES})
ENDIF (MSVC)

FOREACH(MAIN_CPP ${TCC_MAIN_CPP})
    GET_FILENAME_COMPONENT(T_NAME ${MAIN_CPP} NAME_WE)
    IF(T_NAME STREQUAL "server-main")
        SET(T_NAME "Trillek_Server")
	SET(BUILDTARGET "BUILD_SERVER")
    ENDIF(T_NAME STREQUAL "server-main")

    IF(BUILDTARGET)
        MESSAGE(STATUS "Processing: ${T_NAME}")

        # if just building an exe use all the source and libraries
        ADD_EXECUTABLE(${T_NAME}
            ${MAIN_CPP}
            ${TCC_SRC}
            ${TCC_INCLUDES}
            )

        # Set a preprocessor variable
        SET_PROPERTY(TARGET ${T_NAME} PROPERTY COMPILE_DEFINITIONS ${BUILDTARGET} APPEND)
        # this other form is only supported in new versions of cmake
        #target_compile_definitions(${T_NAME} PRIVATE ${BUILDTARGET})

	ADD_DEPENDENCIES(${T_NAME} VCOMPUTER)

        # Link the executable to all required libraries
        TARGET_LINK_LIBRARIES(${T_NAME} ${${T_NAME}_LIBS})
    ENDIF(BUILDTARGET)

    UNSET(BUILDTARGET)
ENDFOREACH(MAIN_CPP)
