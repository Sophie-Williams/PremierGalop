message ( STATUS "==============SDLGfxChevaux===============" )

project(SDLGFXCHEVAUX)

# Make sure the compiler can find include files from our Hello library.
include_directories (${LOGICCHEVAUX_SOURCE_DIR})
message ( STATUS "LOGICCHEVAUX_SOURCE_DIR=${LOGICCHEVAUX_SOURCE_DIR}" )

# Make sure the linker can find the Hello library once it is built.
link_directories (${LOGICCHEVAUX_BINARY_DIR})
message ( STATUS "LOGICCHEVAUX_BINARY_DIR=${LOGICCHEVAUX_BINARY_DIR}" )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL2_image REQUIRED)
FIND_PACKAGE(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

# Workaround for the non-working REQUIRED flag
if ( NOT SDL2_FOUND )
   message ( FATAL_ERROR "SDL2 not found!" )
endif ( NOT SDL2_FOUND )

include_directories (${SDL2_INCLUDE_DIR})
include_directories (${SDL2_IMAGE_INCLUDE_DIR})
include_directories (${SDL2_TTF_INCLUDE_DIR})

link_libraries (
   ${SDL2_LIBRARY}
   ${SDL2_IMAGE_LIBRARY}
   ${SDL2_TTF_LIBRARY}
)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_SDLGFXCHEVAUX)

add_library(SDLGfxChevaux STATIC ${SOURCE_SDLGFXCHEVAUX})

if(WIN32)
	if(CMAKE_CL_64)
		set(ARCH "x64")
	else()
		set(ARCH "x86")
	endif()
	message(STATUS "Architecture: ${ARCH}")
	install(DIRECTORY "${SDL2_INCLUDE_DIR}/../lib/${ARCH}/" DESTINATION "bin" COMPONENT "runtime" FILES_MATCHING PATTERN "*.dll")
	install(DIRECTORY "${SDL2_IMAGE_INCLUDE_DIR}/../lib/${ARCH}/" DESTINATION "bin" COMPONENT "runtime" FILES_MATCHING PATTERN "*.dll")
	install(DIRECTORY "${SDL2_TTF_INCLUDE_DIR}/../lib/${ARCH}/" DESTINATION "bin" COMPONENT "runtime" FILES_MATCHING PATTERN "*.dll")
endif()
