# find_package(Catch2 REQUIRED)

# file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
# foreach(file ${SOURCES})
#     get_filename_component(filename ${file} NAME_WLE)
#     #display_header("Creating Makefile of ${filename}")
#     add_executable(${filename} ${file})
#     add_executable(NetCDFReader_test NetCDFReaderTest.cpp)
#     add_test(NAME ${filename} COMMAND ${filename})
#     target_link_libraries(${filename} PRIVATE ${META_PROJECT_NAME})
#     #target_link_system_libraries(${filename} PRIVATE Catch2 Catch2WithMain)
#     target_link_libraries(NetCDFReader_test Catch2::Catch2)

# endforeach()

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.7
)
FetchContent_MakeAvailable(catch2)
message("Loaded Catch2")

# Definieren Sie explizit Ihre Test-Quelldateien
set(TEST_SOURCES
    NetCDFReaderTest.cpp
	../simulation/netcdf_reader.cpp
    # Fügen Sie hier weitere Testdateien hinzu
)

add_executable(NetCDFReaderTest NetCDFReaderTest.cpp ../src/simulation/netcdf_reader.cpp)
target_link_libraries(NetCDFReaderTest PRIVATE Catch2::Catch2)

if(MSVC) # Windows
    target_link_directories(NetCDFReaderTest PUBLIC ${CMAKE_SOURCE_DIR}/extern/netcdf/lib/)
	target_link_libraries(SWEVisualizer PUBLIC ${CMAKE_SOURCE_DIR}/extern/netcdf/lib/netcdf.lib)
    target_include_directories(NetCDFReaderTest PUBLIC ${CMAKE_SOURCE_DIR}/extern/netcdf/include)
	message("added netcdf ${CMAKE_SOURCE_DIR}")
else() # Linux
    find_package(NetCDF REQUIRED)
    target_link_libraries(NetCDFReaderTest PUBLIC ${NETCDF_LIBRARIES} ${NETCDF_LIBRARIES_CXX})
    target_include_directories(NetCDFReaderTest PUBLIC ${NETCDF_INCLUDE_DIRS})
endif()


