# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


SET(CPACK_BINARY_7Z "")
SET(CPACK_BINARY_BUNDLE "")
SET(CPACK_BINARY_CYGWIN "")
SET(CPACK_BINARY_DEB "OFF")
SET(CPACK_BINARY_DRAGNDROP "")
SET(CPACK_BINARY_IFW "OFF")
SET(CPACK_BINARY_NSIS "OFF")
SET(CPACK_BINARY_OSXX11 "")
SET(CPACK_BINARY_PACKAGEMAKER "")
SET(CPACK_BINARY_RPM "OFF")
SET(CPACK_BINARY_STGZ "ON")
SET(CPACK_BINARY_TBZ2 "OFF")
SET(CPACK_BINARY_TGZ "ON")
SET(CPACK_BINARY_TXZ "OFF")
SET(CPACK_BINARY_TZ "ON")
SET(CPACK_BINARY_WIX "")
SET(CPACK_BINARY_ZIP "")
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
SET(CPACK_COMPONENTS_ALL "Unspecified;bin;devel;resource;runtime")
SET(CPACK_COMPONENT_APPLICATION_DISPLAY_NAME "MyLib Application")
SET(CPACK_COMPONENT_BIN_DEPENDS "runtime;resource;unspecified")
SET(CPACK_COMPONENT_BIN_DISPLAY_NAME "Premier Galop Application")
SET(CPACK_COMPONENT_BIN_GROUP "Application")
SET(CPACK_COMPONENT_DEVEL_DISPLAY_NAME "C++ Headers of SFML library")
SET(CPACK_COMPONENT_DEVEL_GROUP "Optional")
SET(CPACK_COMPONENT_OPTIONAL_DISPLAY_NAME "Development files")
SET(CPACK_COMPONENT_RESOURCE_DISPLAY_NAME "bitmaps, maps and font resources")
SET(CPACK_COMPONENT_RESOURCE_GROUP "Application")
SET(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Premier Galop libraries")
SET(CPACK_COMPONENT_RUNTIME_GROUP "Application")
SET(CPACK_COMPONENT_UNSPECIFIED_DISPLAY_NAME "External dependencies")
SET(CPACK_COMPONENT_UNSPECIFIED_GROUP "Application")
SET(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
SET(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
SET(CPACK_CREATE_DESKTOP_LINKS "PremierGalop")
SET(CPACK_GENERATOR "TBZ2;TGZ;TXZ;TZ")
SET(CPACK_IGNORE_FILES "/CVS/;/\\.svn/;/\\.bzr/;/\\.hg/;/\\.git/;\\.swp\$;\\.#;/#")
SET(CPACK_INSTALLED_DIRECTORIES "/home/christian/PremierGalop;/")
SET(CPACK_INSTALL_CMAKE_PROJECTS "")
SET(CPACK_INSTALL_PREFIX "/usr/local")
SET(CPACK_MODULE_PATH "")
SET(CPACK_NSIS_DISPLAY_NAME "PremierGalop")
SET(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
SET(CPACK_NSIS_INSTALLED_ICON_NAME "/home/christian/PremierGalop/gfx/RedHorse.ico")
SET(CPACK_NSIS_INSTALLER_ICON_CODE "")
SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
SET(CPACK_NSIS_MENU_LINKS "https://github.com/prisme60/PremierGalop/;Website Premier Galop")
SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "PremierGalop.exe")
SET(CPACK_NSIS_MUI_ICON "/home/christian/PremierGalop/gfx/RedHorse.ico")
SET(CPACK_NSIS_PACKAGE_NAME "Premier Galop")
SET(CPACK_NSIS_URL_INFO_ABOUT "http://github.com/prisme60/PremierGalop")
SET(CPACK_OUTPUT_CONFIG_FILE "/home/christian/PremierGalop/CPackConfig.cmake")
SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "/home/christian/PremierGalop/ReadMe.txt")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PremierGalop - Strategy horse board game")
SET(CPACK_PACKAGE_EXECUTABLES "PremierGalop;Premier Galop")
SET(CPACK_PACKAGE_FILE_NAME "PremierGalop-SFML-0.3.0-Source")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "PremierGalop")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "PremierGalop")
SET(CPACK_PACKAGE_NAME "PremierGalop")
SET(CPACK_PACKAGE_RELOCATABLE "true")
SET(CPACK_PACKAGE_VENDOR "Christian FOUCHER")
SET(CPACK_PACKAGE_VERSION "SFML-0.3.0")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "3")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_RESOURCE_FILE_LICENSE "/home/christian/PremierGalop/license.txt")
SET(CPACK_RESOURCE_FILE_README "/usr/share/cmake-3.4/Templates/CPack.GenericDescription.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "/usr/share/cmake-3.4/Templates/CPack.GenericWelcome.txt")
SET(CPACK_SET_DESTDIR "OFF")
SET(CPACK_SOURCE_7Z "")
SET(CPACK_SOURCE_CYGWIN "")
SET(CPACK_SOURCE_GENERATOR "TBZ2;TGZ;TXZ;TZ")
SET(CPACK_SOURCE_IGNORE_FILES "/CVS/;/\\.svn/;/\\.bzr/;/\\.hg/;/\\.git/;\\.swp\$;\\.#;/#")
SET(CPACK_SOURCE_INSTALLED_DIRECTORIES "/home/christian/PremierGalop;/")
SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/home/christian/PremierGalop/CPackSourceConfig.cmake")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "PremierGalop-SFML-0.3.0-Source")
SET(CPACK_SOURCE_TBZ2 "ON")
SET(CPACK_SOURCE_TGZ "ON")
SET(CPACK_SOURCE_TOPLEVEL_TAG "Linux-Source")
SET(CPACK_SOURCE_TXZ "ON")
SET(CPACK_SOURCE_TZ "ON")
SET(CPACK_SOURCE_ZIP "OFF")
SET(CPACK_STRIP_FILES "")
SET(CPACK_SYSTEM_NAME "Linux")
SET(CPACK_TOPLEVEL_TAG "Linux-Source")
SET(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/home/christian/PremierGalop/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
