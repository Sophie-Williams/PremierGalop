message ( STATUS "==============PremierGalop==============" )

# Make sure the compiler can find include files from our Hello library.
include_directories (${LOGICCHEVAUX_SOURCE_DIR})

# Make sure the linker can find the Hello library once it is built.
#link_directories (${LOGICCHEVAUX_BINARY_DIR}) 

if(BUILD_SDL_SUPPORT)
	message ( STATUS "PremierGalop : support SDLGfxChevaux" )
	include_directories (${SDLGFXCHEVAUX_SOURCE_DIR})
	#link_directories (${SDLGFXCHEVAUX_BINARY_DIR})
elseif(BUILD_SFML_SUPPORT)
	message ( STATUS "PremierGalop : support SFMLGfxChevaux" )
	include_directories (${SFMLGFXCHEVAUX_SOURCE_DIR})
	#link_directories (${SFMLGFXCHEVAUX_BINARY_DIR}) 
endif()

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_PREMIERGALOP)

add_executable(PremierGalop ${SOURCE_PREMIERGALOP})

if(BUILD_SDL_SUPPORT)
	target_link_libraries(PremierGalop LogicChevaux SDLGfxChevaux)
elseif(BUILD_SFML_SUPPORT)
	target_link_libraries(PremierGalop LogicChevaux SFMLGfxChevaux sfml-window sfml-graphics)
endif()

# setup the install rule
install(TARGETS PremierGalop RUNTIME DESTINATION "bin" COMPONENT "runtime")
