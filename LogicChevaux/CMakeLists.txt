message ( STATUS "==============LogicChevaux===============" )

project(LOGICCHEVAUX)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_LOGICCHEVAUX)

add_library(LogicChevaux STATIC ${SOURCE_LOGICCHEVAUX})