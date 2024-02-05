#pragma once
#include <netcdf.h>
#include <vector>
#include <filesystem>
#include <array>

/**
 * Handles NetCDF files and provides getters for reading them.
*/
class NetCDFReader
{
private:
	int ncid;
	int dimXPointer, dimYPointer;
	int valXPointer, valYPointer, valZPointer;
	size_t dimXLength, dimYLength;
	float cellWidth, cellHeight;
	float xMin, yMin, xMax, yMax;

	int dimTPointer;
	size_t dimTLength;
	int valHPointer, valHuPointer, valHvPointer, valBPointer, valTPointer, valEdgePointer;
	int maxSimulationTimePointer;

public:
	/**
	 * Initializes the reader.
	 * @param filePath The path to the NetCDF-file.
	 */
	void open(std::filesystem::path filePath);

	/**
	 * Closes the file.
	*/
	void close();

	/**
	 * Returns a vector containing the time steps (in seconds) stored in the file.
	 * By accessing the time variable.
	 */
	std::vector<float> getTimeSteps();

	/**
	 * Returns the length of the X dimension of the file.
	 * By accessing the x dimension.
	 */
	size_t getXDimension();

	/**
	 * Returns the length of the Y dimension of the file.
	 * By accessing the y dimension.
	 */
	size_t getYDimension();

	/**
	 * Gets the heights of the simulation for a specific time step and sets the values inside the heights array.
	 * The heights array has to be of size XDim * YDim to be able to store all the heights for a specific time index.
	 * By accessing the h variable and using nc_get_vara_float to read a variable as fast as possible.
	 */
	void getHeightsForTimeStep(size_t timeIndex, float heights[]);

	/**
	 * Gets the minimum height value stored in the file.
	*/
	float getMinHeight();

	/**
	 * Gets the maximum height value stored in the file.
	*/
	float getMaxHeight();

	/**
	 * Gets the minimum hu value stored in the file.
	*/
	float getMinHu();

	/**
	 * Gets the maximum hu value stored in the file.
	*/
	float getMaxHu();

	/**
	 * Gets the minimum hv value stored in the file.
	*/
	float getMinHv();

	/**
	 * Gets the maximum hv value stored in the file.
	*/
	float getMaxHv();

	/**
	 * Gets the minimum bathymetry value stored in the file.
	*/
	float getMinBathymetry();

	/**
	 * Gets the maximum bathymetry value stored in the file.
	*/
	float getMaxBathymetry();

	/**
	 * Gets the horizontal momentum of the simulation for a specific time step and sets the values inside the heights array.
	 * The hu array has to be of size XDim * YDim to be able to store all the momentums for a specific time index.
	 * By accessing the hu variable and using nc_get_vara_float to read a variable as fast as possible.
	 */
	void getHorizontalMomentumForTimeStep(size_t timeIndex, float hu[]);

	/**
	 * Gets the vertical momentum of the simulation for a specific time step and sets the values inside the heights array.
	 * The hv array has to be of size XDim * YDim to be able to store all the momentums for a specific time index.
	 * By accessing the hv variable and using nc_get_vara_float to read a variable as fast as possible.
	 */
	void getVerticalMomentumForTimeStep(size_t timeIndex, float hv[]);

	/**
	 * Gets the bathymetry of the simulation and stores the relative heights inside the passed bathymetry array.
	 * The bathymetry array has to be of size XDim * YDim to be able to store all the bathymetry values.
	 * By accessing the b variable and using nc_get_vara_float to read a variable as fast as possible.
	 */
	void getBathymetry(float bathymetry[]);

	/**
	 * Gets x for a given index.
	 */
	float getX(size_t index);

	/**
	 * Gets y for given index.
	 */
	float getY(size_t index);

	/**
	 * Calculates the width of a cell. Cell widths are considered to be the same across the entire file.
	 */
	float getCellWidth();

	/**
	 * Calculates the height of a cell. Cell heights are considered to be the same across the entire file.
	 */
	float getCellHeight();
};