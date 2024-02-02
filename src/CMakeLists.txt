add_executable(SWEVisualizer main.cpp mesh/plane.cpp renderer/shader.cpp renderer/camera.cpp simulation/netcdf_reader.cpp simulation/simulation.cpp
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
elseif(APPLE) # Mac
	set(NETCDF_ROOT "${CMAKE_SOURCE_DIR}/extern/netcdf/mac")
	target_link_directories(SWEVisualizer PUBLIC ${NETCDF_ROOT}/lib)
	target_link_libraries(SWEVisualizer PUBLIC netcdf)
	target_include_directories(SWEVisualizer PUBLIC ${NETCDF_ROOT}/include)
else() # Linux
	message("Linux")
	find_package(NetCDF REQUIRED)
	target_link_libraries(SWEVisualizer PUBLIC ${NETCDF_LIBRARIES} ${NETCDF_LIBRARIES_CXX} netcdf)
	target_include_directories(SWEVisualizer PRIVATE ${NETCDF_INCLUDES})
endif()

doxygen_add_docs(doxygen ${PROJECT_SOURCE_DIR}/src COMMENT "Generate docs")