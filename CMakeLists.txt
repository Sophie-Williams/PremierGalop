
cmake_minimum_required(VERSION 3.0)

# set a default build type if none was provided
# this has to be done before the project() instruction!
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# project name
project(AUTROT)

# add an option for choosing the SDL support
set(BUILD_SDL_SUPPORT TRUE CACHE BOOL "TRUE to build with SDL support")

# add an option for choosing the SFML support
set(BUILD_SFML_SUPPORT TRUE CACHE BOOL "TRUE to build with SFML support")


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++14")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

add_subdirectory(bin2C)
add_subdirectory(gfx)
add_subdirectory(maps)
add_subdirectory(LogicChevaux)

if(${BUILD_SDL_SUPPORT})
	set (SUFFIX_LIB "SDL")
	message ( STATUS "SDLGfxChevaux" )

	add_subdirectory(SDLGfxChevaux)
elseif(${BUILD_SFML_SUPPORT})
	set (SUFFIX_LIB "SFML")
	message ( STATUS "SFMLGfxChevaux" )
	
	add_subdirectory("SFML")
	add_subdirectory(SFMLGfxChevaux)	
else ()
	message ( FATAL_ERROR "You must select SDL or SFML!!!" )
endif ()

add_subdirectory(PremierGalop)

install(FILES ".PremierGalop" "license.txt" "ReadMe.txt" DESTINATION "." COMPONENT "resource")

set(CPACK_PACKAGE_NAME "PremierGalop")
set(CPACK_PACKAGE_VENDOR "Christian FOUCHER")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PremierGalop - Strategy horse board game")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "3")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${SUFFIX_LIB}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "PremierGalop")
set(CPACK_PACKAGE_EXECUTABLES "PremierGalop" "Premier Galop")
set(CPACK_CREATE_DESKTOP_LINKS "PremierGalop")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/license.txt")
set(CPACK_NSIS_URL_INFO_ABOUT "http://github.com/prisme60/PremierGalop" )
set(CPACK_NSIS_PACKAGE_NAME "Premier Galop")
set(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
set(CPACK_NSIS_MUI_FINISHPAGE_RUN "PremierGalop.exe")

SET(CPACK_NSIS_MENU_LINKS
#  "${CPACK_NSIS_EXECUTABLES_DIRECTORY}/${CPACK_NSIS_MUI_FINISHPAGE_RUN}" "${CPACK_NSIS_PACKAGE_NAME}"
  "https://github.com/prisme60/PremierGalop/" "Website Premier Galop"
  )
  
#SET(CPACK_NSIS_CREATE_ICONS_EXTRA)
  
SET(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/gfx/RedHorse.ico")

# SET(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/gfx/PremierGalopCapture.bmp" )

# Use the icond from cmake-gui for add-remove programs
SET(CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_CURRENT_SOURCE_DIR}/gfx/RedHorse.ico")

set(CPACK_COMPONENT_APPLICATION_DISPLAY_NAME "MyLib Application")
set(CPACK_COMPONENT_BIN_DISPLAY_NAME "Premier Galop Application")
set(CPACK_COMPONENT_RESOURCE_DISPLAY_NAME "bitmaps, maps and font resources")
set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Premier Galop libraries")
set(CPACK_COMPONENT_UNSPECIFIED_DISPLAY_NAME "External dependencies")

set(CPACK_COMPONENT_OPTIONAL_DISPLAY_NAME "Development files")
set(CPACK_COMPONENT_DEVEL_DISPLAY_NAME "C++ Headers of SFML library")

# set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
   # "Premier Galop application that makes use of MyLib")
# set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION
   # "Dynamic libraries used to build programs with MyLib")
# set(CPACK_COMPONENT_DEVEL_DESCRIPTION
   # "C/C++ header files for use with graphic lib (SFML or SDL)")
   
set(CPACK_COMPONENT_BIN_DEPENDS "runtime" "resource" "unspecified")


set(CPACK_COMPONENT_BIN_GROUP "Application")
set(CPACK_COMPONENT_RESOURCE_GROUP "Application")
set(CPACK_COMPONENT_RUNTIME_GROUP "Application")
set(CPACK_COMPONENT_UNSPECIFIED_GROUP "Application")

set(CPACK_COMPONENT_DEVEL_GROUP "Optional")

# set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
	# CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${CPACK_PACKAGE_NAME}.lnk\\\"  \\\"$INSTDIR\\\\${CPACK_NSIS_MUI_FINISHPAGE_RUN}\\\"
# ")
# set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS ${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS} "
# Delete \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${CPACK_PACKAGE_NAME}.lnk\\\"
# ")


# This must always be last!
include(CPack)
