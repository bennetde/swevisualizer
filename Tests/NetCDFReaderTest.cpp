#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>
#include "../src/simulation/netcdf_reader.h"

TEST_CASE("CDFManagerTest", "[NetCDFReader]")
{
    std::string netCDFPath = "../Tests/TestData.nc";
    std::filesystem::path path(netCDFPath);

    NetCDFReader cdfManager;
    cdfManager.open(path);

    SECTION("Test Dimensionen")
    {
        REQUIRE(cdfManager.getXDimension() == 10);
        REQUIRE(cdfManager.getYDimension() == 10);
    }

    SECTION("Teste getBathymetry")
    {
        float expectedValue = 10.0f;
        float b[10 * 10];
        cdfManager.getBathymetry(b);
        REQUIRE(static_cast<bool>(b[0] == expectedValue));
    }
}