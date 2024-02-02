cmake_minimum_required(VERSION 3.20)

project(
	SWEVisualizer
	VERSION 1.0
	DESCRIPTION "A SWE visualizer"
	LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)
find_package(Doxygen
             OPTIONAL_COMPONENTS mscgen dia)
#find_package(NetCDF REQUIRED)

## Include various dependencies using FetchContent
include(FetchContent)

# Find packages for compilation
# glfw
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw
	GIT_TAG 3.3.9
)
FetchContent_MakeAvailable(glfw)
message("Loaded glfw library")


add_library(glad STATIC
	${PROJECT_SOURCE_DIR}/extern/glad/src/gl.c)

target_include_directories(
	glad PUBLIC
	${PROJECT_SOURCE_DIR}/extern/glad/include
)
message("Loaded glad2")

# glm
FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)
message("Loaded glm")


# # IMGUI
FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG v1.90

)
FetchContent_MakeAvailable(imgui)
FetchContent_GetProperties(imgui)

add_library(
	imgui STATIC
	${imgui_SOURCE_DIR}/imgui.cpp
	${imgui_SOURCE_DIR}/imgui_draw.cpp
	${imgui_SOURCE_DIR}/imgui_demo.cpp
	${imgui_SOURCE_DIR}/imgui_tables.cpp
	${imgui_SOURCE_DIR}/imgui_widgets.cpp
  
	${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_compile_options(
    imgui PRIVATE
    -DIMGUI_IMPL_OPENGL_LOADER_GLAD # configure GLAD as the loader
)

target_include_directories(imgui PRIVATE
	${imgui_SOURCE_DIR}
	${imgui_SOURCE_DIR}/backends
	${glad_BINARY_DIR}/include)

target_link_libraries(imgui PRIVATE glfw glad ${OPENGL_LIBRARY})
message("Loaded imgui")

# target_link_libraries(SWEVisualizer PRIVATE ${NETCDF_LIBRARIES})
# target_include_directories(SWEVisualizer PRIVATE ${NETCDF_INCLUDE_DIRS})

add_custom_target(copy_resources ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${PROJECT_SOURCE_DIR}/resources
	${CMAKE_CURRENT_BINARY_DIR}/resources
	COMMENT "Copying resources into binary directory ${CMAKE_CURRENT_BINARY_DIR}")

add_subdirectory(src)
add_subdirectory(Tests)
