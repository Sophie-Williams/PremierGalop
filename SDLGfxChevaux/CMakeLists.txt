message ( STATUS "==============SDLGfxChevaux===============" )

project(SDLGFXCHEVAUX)

# Make sure the compiler can find include files from our Hello library.
include_directories (${LOGICCHEVAUX_SOURCE_DIR})
message ( STATUS "LOGICCHEVAUX_SOURCE_DIR=${LOGICCHEVAUX_SOURCE_DIR}" )

# Make sure the linker can find the Hello library once it is built.
link_directories (${LOGICCHEVAUX_BINARY_DIR})
message ( STATUS "LOGICCHEVAUX_BINARY_DIR=${LOGICCHEVAUX_BINARY_DIR}" )

FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(SDL_image REQUIRED)
FIND_PACKAGE(SDL_ttf REQUIRED)

# Workaround for the non-working REQUIRED flag
if ( NOT SDL_FOUND )
   message ( FATAL_ERROR "SDL not found!" )
endif ( NOT SDL_FOUND )

include_directories (${SDL_INCLUDE_DIR})
include_directories (${SDLIMAGE_INCLUDE_DIR})
include_directories (${SDLTTF_INCLUDE_DIR})

link_libraries (
   ${SDL_LIBRARY}
   ${SDLIMAGE_LIBRARY}
   ${SDLTTF_LIBRARY}
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
	install(DIRECTORY "${SDL_INCLUDE_DIR}/../lib/${ARCH}/" DESTINATION "bin" COMPONENT "runtime" FILES_MATCHING PATTERN "*.dll")
	install(DIRECTORY "${SDLIMAGE_INCLUDE_DIR}/../lib/${ARCH}/" DESTINATION "bin" COMPONENT "runtime" FILES_MATCHING PATTERN "*.dll")
	install(DIRECTORY "${SDLTTF_INCLUDE_DIR}/../lib/${ARCH}/" DESTINATION "bin" COMPONENT "runtime" FILES_MATCHING PATTERN "*.dll")
endif()
