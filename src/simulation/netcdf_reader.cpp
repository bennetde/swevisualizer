#include "netcdf_reader.h"
#include <netcdf.h>
#include <iostream>
#include <algorithm>
// TODO: Implementation of the reader

void NetCDFReader::open(std::filesystem::path filePath)
{
    int retval;
    std::string pathString = filePath.string();
    if ((retval = nc_open(pathString.c_str(), NC_NOWRITE, &ncid)))
    {
        throw std::invalid_argument("Could not open NETCDF-File");
    }
    std::cout << "Opened with id " << ncid << std::endl;
    if ((retval = nc_inq_dimid(ncid, "x", &dimXPointer)))
    {
        throw std::runtime_error("Could not load dimension x");
    }
    if ((retval = nc_inq_dimid(ncid, "y", &dimYPointer)))
    {
        throw std::runtime_error("Could not load dimension x");
    }
    if ((retval = nc_inq_dimlen(ncid, dimXPointer, &dimXLength)))
    {
        throw std::runtime_error("Could not load dimension x length");
    }
    if ((retval = nc_inq_dimlen(ncid, dimYPointer, &dimYLength)))
    {
        throw std::runtime_error("Could not load dimension x length");
    }
    if ((retval = nc_inq_varid(ncid, "x", &valXPointer)))
    {
        throw std::runtime_error("Could not load x variable");
    }
    if ((retval = nc_inq_varid(ncid, "y", &valYPointer)))
    {
        throw std::runtime_error("Could not load y variable");
    }
    if ((retval = nc_inq_dimid(ncid, "time", &dimTPointer)))
    {
        throw std::runtime_error("Could not load dimension t");
    }
    if ((retval = nc_inq_dimlen(ncid, dimTPointer, &dimTLength)))
    {
        throw std::runtime_error("Could not load dimension t length");
    }
    if ((retval = nc_inq_varid(ncid, "h", &valHPointer)))
    {
        throw std::runtime_error("Could not load h variable");
    }
}

void NetCDFReader::close()
{
    nc_close(ncid);
}

std::vector<float> NetCDFReader::getTimeSteps()
{
    int retval;
    std::vector<float> timeSteps;
    timeSteps.resize(dimTLength);
    if ((retval = nc_get_var_float(ncid, valTPointer, timeSteps.data())))
    {
        throw std::runtime_error("Could not load time variable");
    }
    return timeSteps;
}

size_t NetCDFReader::getXDimension()
{
    return dimXLength;
}

size_t NetCDFReader::getYDimension()
{
    return dimYLength;
}

void NetCDFReader::getHeightsForTimeStep(size_t timeIndex, float heights[])
{
    int retval;

    size_t start[3] = {timeIndex, 0, 0};
    size_t count[3] = {1, dimYLength, dimXLength};
    if ((retval = nc_get_vara_float(ncid, valHPointer, start, count, heights)))
    {
        std::cout << retval << std::endl;
        throw std::runtime_error("Could not get h variable");
    }
}

void NetCDFReader::getHorizontalMomentumForTimeStep(size_t timeIndex, float hu[])
{
    int retval;
    if ((retval = nc_inq_varid(ncid, "hu", &valHuPointer)))
    {
        throw std::runtime_error("Could not load hu variable");
    }
    size_t start[3] = {timeIndex, 0, 0};
    size_t count[3] = {1, dimYLength, dimXLength};
    if ((retval = nc_get_vara_float(ncid, valHuPointer, start, count, hu)))
    {
        throw std::runtime_error("Could not load hu variable");
    }
}

float NetCDFReader::getMinHeight()
{
    int retval;
    size_t totalSize = dimTLength * dimYLength * dimXLength;
    std::vector<float> heights(totalSize);
    size_t start[3] = {0, 0, 0};
    size_t count[3] = {dimTLength, dimYLength, dimXLength};

    if ((retval = nc_get_vara_float(ncid, valHPointer, start, count, heights.data())))
    {
        throw std::runtime_error("Could not get h variable");
    }

    return *std::min_element(heights.begin(), heights.end());
}

float NetCDFReader::getMaxHeight()
{
    int retval;
    size_t totalSize = dimTLength * dimYLength * dimXLength;
    std::vector<float> heights(totalSize);
    size_t start[3] = {0, 0, 0};
    size_t count[3] = {dimTLength, dimYLength, dimXLength};

    if ((retval = nc_get_vara_float(ncid, valHPointer, start, count, heights.data())))
    {
        throw std::runtime_error("Could not get h variable");
    }

    return *std::max_element(heights.begin(), heights.end());
}

float NetCDFReader::getMinHu()
{
    int retval;
    size_t totalSize = dimTLength * dimYLength * dimXLength;
    std::vector<float> hus(totalSize);
    size_t start[3] = {0, 0, 0};
    size_t count[3] = {dimTLength, dimYLength, dimXLength};

    if ((retval = nc_get_vara_float(ncid, valHuPointer, start, count, hus.data())))
    {
        throw std::runtime_error("Could not get hu variable");
    }

    return *std::min_element(hus.begin(), hus.end());
}

float NetCDFReader::getMaxHu()
{
    int retval;
    size_t totalSize = dimTLength * dimYLength * dimXLength;
    std::vector<float> hus(totalSize);
    size_t start[3] = {0, 0, 0};
    size_t count[3] = {dimTLength, dimYLength, dimXLength};

    if ((retval = nc_get_vara_float(ncid, valHuPointer, start, count, hus.data())))
    {
        throw std::runtime_error("Could not get hu variable");
    }

    return *std::max_element(hus.begin(), hus.end());
}

float NetCDFReader::getMinHv()
{
    int retval;
    size_t totalSize = dimTLength * dimYLength * dimXLength;
    std::vector<float> hvs(totalSize);
    size_t start[3] = {0, 0, 0};
    size_t count[3] = {dimTLength, dimYLength, dimXLength};

    if ((retval = nc_get_vara_float(ncid, valHvPointer, start, count, hvs.data())))
    {
        throw std::runtime_error("Could not get hv variable");
    }

    return *std::min_element(hvs.begin(), hvs.end());
}

float NetCDFReader::getMaxHv()
{
    int retval;
    size_t totalSize = dimTLength * dimYLength * dimXLength;
    std::vector<float> hvs(totalSize);
    size_t start[3] = {0, 0, 0};
    size_t count[3] = {dimTLength, dimYLength, dimXLength};

    if ((retval = nc_get_vara_float(ncid, valHvPointer, start, count, hvs.data())))
    {
        throw std::runtime_error("Could not get hv variable");
    }

    return *std::max_element(hvs.begin(), hvs.end());
}

float NetCDFReader::getMinBathymetry()
{
    size_t totalSize = dimYLength * dimXLength;
    std::vector<float> b(totalSize);
    int retval;
    if ((retval = nc_inq_varid(ncid, "b", &valBPointer)))
    {
        throw std::runtime_error("Could not load b variable");
    }
    size_t start[2] = {0, 0};
    size_t count[2] = {dimYLength, dimXLength};
    if ((retval = nc_get_vara_float(ncid, valBPointer, start, count, b.data())))
    {
        throw std::runtime_error("Could not load b variable");
    }
    return *std::min_element(b.begin(), b.end());
}

float NetCDFReader::getMaxBathymetry()
{
    size_t totalSize = dimYLength * dimXLength;
    std::vector<float> b(totalSize);
    int retval;
    if ((retval = nc_inq_varid(ncid, "b", &valBPointer)))
    {
        throw std::runtime_error("Could not load b variable");
    }
    size_t start[2] = {0, 0};
    size_t count[2] = {dimYLength, dimXLength};
    if ((retval = nc_get_vara_float(ncid, valBPointer, start, count, b.data())))
    {
        throw std::runtime_error("Could not load b variable");
    }
    return *std::max_element(b.begin(), b.end());
}

void NetCDFReader::getVerticalMomentumForTimeStep(size_t timeIndex, float hv[])
{
    int retval;
    if ((retval = nc_inq_varid(ncid, "hv", &valHvPointer)))
    {
        throw std::runtime_error("Could not load hv variable");
    }
    size_t start[3] = {timeIndex, 0, 0};
    size_t count[3] = {1, dimYLength, dimXLength};
    if ((retval = nc_get_vara_float(ncid, valHvPointer, start, count, hv)))
    {
        throw std::runtime_error("Could not load hv variable");
    }
}

void NetCDFReader::getBathymetry(float b[])
{
    int retval;
    if ((retval = nc_inq_varid(ncid, "b", &valBPointer)))
    {
        throw std::runtime_error("Could not load b variable");
    }
    size_t start[2] = {0, 0};
    size_t count[2] = {dimYLength, dimXLength};
    if ((retval = nc_get_vara_float(ncid, valBPointer, start, count, b)))
    {
        throw std::runtime_error("Could not load b variable");
    }
}

float NetCDFReader::getX(size_t index) {
	int retval;
	if(index < 0 || index >= dimXLength) {
		throw std::invalid_argument("Index too small or too big");
	}
	int xValue;
	if((retval = nc_get_var1_int(ncid, valXPointer, &index, &xValue))) {
		throw std::runtime_error("Could not load x value");
	}
	return xValue;

}

float NetCDFReader::getY(size_t index) {
	int retval;
	if(index < 0 || index >= dimYLength) {
		throw std::invalid_argument("yIndex too small or too big");
	}
	int yValue;
	if((retval = nc_get_var1_int(ncid, valYPointer, &index, &yValue))) {
		throw std::runtime_error("Could not load y value");
	}
	return yValue;
}

float NetCDFReader::getCellWidth() {
	return (getX(1) - getX(0));
}

float NetCDFReader::getCellHeight() {
	return (getY(1) - getY(0));
}

