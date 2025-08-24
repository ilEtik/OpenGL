
set(IMGUI_DIR "${CMAKE_CURRENT_LIST_DIR}/../imgui" CACHE PATH "")

file(GLOB SOURCES "${IMGUI_DIR}/*.cpp")
file(GLOB HEADERS "${IMGUI_DIR}/*.h")
file(GLOB BACKEND 
	"${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
	"${IMGUI_DIR}/backends/imgui_impl_glfw.h"
	"${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
	"${IMGUI_DIR}/backends/imgui_impl_opengl3.h"
	"${IMGUI_DIR}/backends/imgui_impl_opengl3_loader.h"
)

add_library(imgui STATIC ${SOURCES} ${HEADERS} ${BACKEND})

target_include_directories(imgui PUBLIC
	${IMGUI_DIR}
	${IMGUI_DIR}/backends
	${CMAKE_CURRENT_SOURCE_DIR}/../glfw/include
)

target_link_directories(imgui PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/../glfw/lib-vc2022"
)

target_link_libraries(imgui PUBLIC glfw3)