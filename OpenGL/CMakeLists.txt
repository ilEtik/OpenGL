file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.h")

add_executable(OpenGL ${SOURCES} ${HEADERS})

target_include_directories(OpenGL PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/glfw/include
	${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/glew/include
	${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/glm
	${CMAKE_CURRENT_SOURCE_DIR}/src/vendor
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_directories(OpenGL PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/glfw/lib-vc2022"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/glew/lib/Release/Win32"
)

target_link_libraries(OpenGL PRIVATE
	glfw3.lib
	opengl32.lib
	glew32s.lib
)

target_compile_definitions(OpenGL PRIVATE 
	GLEW_STATIC
	$<$<CONFIG:Debug>:DEBUG>
	FMT_UNICODE=0
)

set_target_properties(OpenGL PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/OpenGL"
)

file(GLOB_RECURSE RES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/res/*")

target_sources(OpenGL PRIVATE ${RES_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/res" PREFIX "Resources" FILES ${RES_FILES})

foreach(file ${RES_FILES})
    set_source_files_properties(${file} PROPERTIES HEADER_FILE_ONLY ON)
endforeach()