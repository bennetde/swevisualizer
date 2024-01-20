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

    simulation::NetCDFReader cdfManager(path);

    SECTION("Test Dimensionen")
    {
        REQUIRE(cdfManager.getXDimension() == 10);
        REQUIRE(cdfManager.getYDimension() == 10);
    }

    SECTION("Teste getBathymetry")
    {
        float expectedValue = 10.0f; // Erwarteter Wert an dieser Position
        float b[10 * 10];
        cdfManager.getBathymetry(b);
        REQUIRE(static_cast<bool>(b[0] == expectedValue));
        // REQUIRE(b[0] == expectedValue);
    }

    // SECTION("Teste ungültige Indizes")
    // {
    //     size_t invalidXIndex = 10; // Ungültiger Index (außerhalb der Grenzen)
    //     size_t invalidYIndex = 10; // Ungültiger Index (außerhalb der Grenzen)

    //     REQUIRE_THROWS_AS(cdfManager.getBathymetry(invalidXIndex, 0), std::invalid_argument);
    //     REQUIRE_THROWS_AS(cdfManager.getBathymetry(0, invalidYIndex), std::invalid_argument);
    // }
}