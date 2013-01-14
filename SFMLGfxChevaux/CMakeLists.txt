message ( STATUS "==============SDLGfxChevaux===============" )

project(SFMLGFXCHEVAUX)

# Make sure the compiler can find include files from our Hello library.
include_directories (${LOGICCHEVAUX_SOURCE_DIR})
message ( STATUS "LOGICCHEVAUX_SOURCE_DIR=${LOGICCHEVAUX_SOURCE_DIR}" )

# Make sure the linker can find the Hello library once it is built.
link_directories (${LOGICCHEVAUX_BINARY_DIR})
message ( STATUS "LOGICCHEVAUX_BINARY_DIR=${LOGICCHEVAUX_BINARY_DIR}" )

SET(SFML_INCLUDE_DIR "${SFML_SOURCE_DIR}/include")
SET(SFML_LIBRARY_DIR "${SFML_BINARY_DIR}/lib")

include_directories (${SFML_INCLUDE_DIR})

# specific to Win32 (need to find a generic method!!!!)
# link_libraries (
   # "${SFML_LIBRARY_DIR}/sfml-graphics.lib"
   # "${SFML_LIBRARY_DIR}/sfml-system.lib"
   # "${SFML_LIBRARY_DIR}/sfml-window.lib"
# )

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_SFMLGFXCHEVAUX)

add_library(SFMLGfxChevaux STATIC ${SOURCE_SFMLGFXCHEVAUX})

add_dependencies(SFMLGfxChevaux GFXResources)

if(WIN32)
	install(DIRECTORY "${SSFML_LIB_DIR}/../lib/" DESTINATION "." COMPONENT "runtime" FILES_MATCHING PATTERN "*.dll")
endif()
