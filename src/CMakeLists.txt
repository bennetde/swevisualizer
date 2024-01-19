add_executable(SWEVisualizer main.cpp mesh/plane.cpp renderer/shader.cpp renderer/camera.cpp simulation/netcdf_reader.cpp
${CMAKE_SOURCE_DIR}/extern/ImGuiFileDialog/ImGuiFileDialog.cpp)
target_compile_features(SWEVisualizer PRIVATE cxx_std_17)
target_link_libraries(SWEVisualizer PUBLIC glfw imgui glad glm)
target_include_directories(SWEVisualizer PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
add_dependencies(SWEVisualizer copy_resources)

if(WIN32) # Windows
	target_link_directories(SWEVisualizer PUBLIC ${CMAKE_SOURCE_DIR}/extern/netcdf/lib/)
	target_link_libraries(SWEVisualizer PUBLIC ${CMAKE_SOURCE_DIR}/extern/netcdf/lib/netcdf.lib)
	target_include_directories(SWEVisualizer PUBLIC ${CMAKE_SOURCE_DIR}/extern/netcdf/include)
	message("linked netcdf")
else() # Linux
	find_package(NetCDF REQUIRED)
	target_link_libraries(SWEVisualizer PUBLIC ${NETCDF_LIBRARIES} ${NETCDF_LIBRARIES_CXX})
	target_include_directories(SWEVisualizer PUBLIC ${NetCDF_INCLUDE_DIRS})
endif()