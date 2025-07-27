
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.h")

add_executable(OpenGL ${SOURCES} ${HEADERS})

target_include_directories(OpenGL PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/glfw/include
)

target_link_directories(OpenGL PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/glfw/lib-vc2022"
)

target_link_libraries(OpenGL PRIVATE
	glfw3.lib
	opengl32.lib
)