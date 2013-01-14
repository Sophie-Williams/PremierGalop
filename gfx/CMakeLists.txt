project(GFXRESOURCES)

get_property(bin2C_location TARGET bin2C PROPERTY LOCATION)
message (STATUS "bin2C_location == ${bin2C_location}")

file(GLOB_RECURSE file_list "${CMAKE_CURRENT_SOURCE_DIR}/*")
add_custom_target(GFXResources
				COMMENT "Generates Header files of the resources")
foreach(f ${file_list})
	if((${f} MATCHES ".*/.*\\.png$")
		OR (${f} MATCHES ".*/.*\\.ttf$"))
		message( STATUS "bin2C ${f}" )
		add_custom_command(TARGET GFXResources
		                   PRE_BUILD
						   COMMAND ${bin2C_location} ${f}
						   DEPENDS Bin2C
						   COMMENT "Bin2C : Generate Header file for ${f}"
						   VERBATIM)
	endif()
endforeach(f)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION "." COMPONENT "resource" FILES_MATCHING PATTERN "*.png")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION "." COMPONENT "resource" FILES_MATCHING PATTERN "*.ttf")