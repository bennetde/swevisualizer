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
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.7
)
FetchContent_MakeAvailable(catch2)
message("Loaded Catch2")

# Definieren Sie explizit Ihre Test-Quelldateien
set(TEST_SOURCES
    NetCDFReaderTest.cpp
    # Fügen Sie hier weitere Testdateien hinzu
)

# foreach(file ${TEST_SOURCES})
#     get_filename_component(filename ${file} NAME_WLE)
#     add_executable(${filename} ${file})
#     add_test(NAME ${filename} COMMAND ${filename})
#     target_link_libraries(${filename} PRIVATE ${META_PROJECT_NAME} Catch2::Catch2)
#     include_directories(${CMAKE_SOURCE_DIR}/src/simulation)
# endforeach()

add_executable(NetCDFReader_test ${TEST_SOURCES})
target_link_libraries(NetCDFReader_test PRIVATE Catch2::Catch2)

if(MSVC) # Windows
    target_link_directories(NetCDFReader_test ${CMAKE_SOURCE_DIR}/extern/netcdf/lib/)
    target_include_directories(NetCDFReader_test PUBLIC ${CMAKE_SOURCE_DIR}/extern/netcdf/include)
else() # Linux
    find_package(NetCDF REQUIRED)
    target_link_libraries(NetCDFReader_test PUBLIC ${NETCDF_LIBRARIES} ${NETCDF_LIBRARIES_CXX})
    target_include_directories(NetCDFReader_test PUBLIC ${NETCDF_INCLUDE_DIRS})
endif()


