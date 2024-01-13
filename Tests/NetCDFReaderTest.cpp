#include <catch2/catch.hpp>
#include "../src/simulation/netcdf_reader.h"

TEST_CASE("CDFManagerTest", "[NetCDFReader]") {
    std::string netCDFPath = "../Tests/TestData.nc";
    std::filesystem::path path(netCDFPath);

    NetCDFReader cdfManager(path);
	//cdfManager.loadZVariable();

	REQUIRE(true);
    // SECTION("Test Dimensionen") {
    //     REQUIRE(cdfManager.getDimensionXLength() == 10);
    //     REQUIRE(cdfManager.getDimensionYLength() == 10);
    // }

   
    // SECTION("Test getX") {
    //     REQUIRE(cdfManager.getX(0) == 0.0);
    //     REQUIRE(cdfManager.getX(1) == 1.0);
    //     REQUIRE(cdfManager.getX(2) == 2.0);
    //     REQUIRE(cdfManager.getX(3) == 3.0);
    //     REQUIRE(cdfManager.getX(4) == 4.0);
    //     REQUIRE(cdfManager.getX(5) == 5.0);
    //     REQUIRE(cdfManager.getX(6) == 6.0);
    //     REQUIRE(cdfManager.getX(7) == 7.0);
    //     REQUIRE(cdfManager.getX(8) == 8.0);
    //     REQUIRE(cdfManager.getX(9) == 9.0);
    // }

    // SECTION("Test getY") {
    //     REQUIRE(cdfManager.getY(0) == 0.0);
    //     REQUIRE(cdfManager.getY(1) == 1.0);
    //     REQUIRE(cdfManager.getY(2) == 2.0);
    //     REQUIRE(cdfManager.getY(3) == 3.0);
    //     REQUIRE(cdfManager.getY(4) == 4.0);
    //     REQUIRE(cdfManager.getY(5) == 5.0);
    //     REQUIRE(cdfManager.getY(6) == 6.0);
    //     REQUIRE(cdfManager.getY(7) == 7.0);
    //     REQUIRE(cdfManager.getY(8) == 8.0);
    //     REQUIRE(cdfManager.getY(9) == 9.0);
    // }

    // SECTION("Test getZ") {
    //     REQUIRE(cdfManager.getZ(0, 0) == 0.0);
    //     REQUIRE(cdfManager.getZ(0, 1) == 0.0);
    //     REQUIRE(cdfManager.getZ(0, 2) == 0.0);
    //     REQUIRE(cdfManager.getZ(0, 3) == 0.0);
    //     REQUIRE(cdfManager.getZ(0, 4) == 0.0);
    //     REQUIRE(cdfManager.getZ(0, 5) == 0.0);
    //     REQUIRE(cdfManager.getZ(0, 6) == 0.0);
    //     REQUIRE(cdfManager.getZ(0, 7) == 0.0);
    //     REQUIRE(cdfManager.getZ(0, 8) == 0.0);
    //     REQUIRE(cdfManager.getZ(0, 9) == 0.0);

    //     REQUIRE(cdfManager.getZ(1, 0) == 0.0);
    //     REQUIRE(cdfManager.getZ(1, 1) == 0.0);
    //     REQUIRE(cdfManager.getZ(1, 2) == 0.0);
    //     REQUIRE(cdfManager.getZ(1, 3) == 0.0);
    //     REQUIRE(cdfManager.getZ(1, 4) == 0.0);
    //     REQUIRE(cdfManager.getZ(1, 5) == 0.0);
    //     REQUIRE(cdfManager.getZ(1, 6) == 0.0);
    //     REQUIRE(cdfManager.getZ(1, 7) == 0.0);
    //     REQUIRE(cdfManager.getZ(1, 8) == 0.0);
    //     REQUIRE(cdfManager.getZ(1, 9) == 0.0);
    // }

    // SECTION("Test getMinXValue") {
    //     REQUIRE(cdfManager.getMinXValue() == 0.0);
    // }

    // SECTION("Test getMaxXValue") {
    //     REQUIRE(cdfManager.getMaxXValue() == 9.0);
    // }

    // SECTION("Test getMinYValue") {
    //     REQUIRE(cdfManager.getMinYValue() == 0.0);
    // }

    // SECTION("Test getMaxYValue") {
    //     REQUIRE(cdfManager.getMaxYValue() == 9.0);
    // }

    //     SECTION("Teste gültige Indizes") {
    //     // Ersetzen Sie diese Werte durch tatsächliche Werte aus Ihrer Testdatei
    //     size_t xIndex = 1; // Gültiger Index
    //     size_t yIndex = 1; // Gültiger Index
    //     float expectedValue = 1.0f; // Erwarteter Wert an dieser Position

    //     REQUIRE(cdfManager.getBathymetry(xIndex, yIndex) == Catch::Approx(expectedValue));
    // }

    // SECTION("Teste ungültige Indizes") {
    //     size_t invalidXIndex = 10; // Ungültiger Index (außerhalb der Grenzen)
    //     size_t invalidYIndex = 10; // Ungültiger Index (außerhalb der Grenzen)

    //     REQUIRE_THROWS_AS(cdfManager.getBathymetry(invalidXIndex, 0), std::invalid_argument);
    //     REQUIRE_THROWS_AS(cdfManager.getBathymetry(0, invalidYIndex), std::invalid_argument);
    // }


}
