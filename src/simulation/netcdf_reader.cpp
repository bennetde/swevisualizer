#include "netcdf_reader.h"

// TODO: Implementation of the reader

NetCDFReader::NetCDFReader(std::filesystem::path filePath) {
    int retval;
    std::string pathString = filePath.string();
	if((retval = nc_open(pathString.c_str(), NC_NOWRITE, &ncid))) {
		throw std::invalid_argument("Could not open NETCDF-File");
	}
	if((retval = nc_inq_dimid(ncid, "x", &dimXPointer))) {
		throw std::runtime_error("Could not load dimension x");
	}
	if((retval = nc_inq_dimid(ncid, "y", &dimYPointer))) {
		throw std::runtime_error("Could not load dimension x");
	}
	if((retval = nc_inq_dimlen(ncid, dimXPointer, &dimXLength))) {
		throw std::runtime_error("Could not load dimension x length");
	}
	if((retval = nc_inq_dimlen(ncid, dimYPointer, &dimYLength))) {
		throw std::runtime_error("Could not load dimension x length");
	}
	if((retval = nc_inq_varid(ncid, "x", &valXPointer))) {
		throw std::runtime_error("Could not load x variable");
	}
	if((retval = nc_inq_varid(ncid, "y", &valYPointer))) {
		throw std::runtime_error("Could not load y variable");
	}

}

NetCDFReader::~NetCDFReader() {
	int retval;
	if((retval = nc_close(ncid))) {
		throw std::runtime_error("Could not close NETCDF-File");
	}
}


std::vector<float> NetCDFReader::getTimeSteps() {
    int retval;
    std::vector<float> timeSteps;
    if((retval = nc_inq_dimid(ncid, "time", &dimTPointer))) {
        throw std::runtime_error("Could not load dimension t");
    }
    if((retval = nc_inq_dimlen(ncid, dimTPointer, &dimTLength))) {
        throw std::runtime_error("Could not load dimension t length");
    }
    if((retval = nc_inq_varid(ncid, "time", &valTPointer))) {
        throw std::runtime_error("Could not load time variable");
    }
    timeSteps.resize(dimTLength);
    if((retval = nc_get_var_float(ncid, valTPointer, timeSteps.data()))) {
        throw std::runtime_error("Could not load time variable");
    }
    return timeSteps;
}

size_t NetCDFReader::getXDimension() {
    return dimXLength;
}

size_t NetCDFReader::getYDimension() {
    return dimYLength;
}

void NetCDFReader::getHeightsForTimeStep(size_t timeIndex, float heights[]) {
    int retval;
    if((retval = nc_inq_varid(ncid, "h", &valHPointer))) {
        throw std::runtime_error("Could not load h variable");
    }
    size_t start[3] = {timeIndex, 0, 0};
    size_t count[3] = {1, dimYLength, dimXLength};
    if((retval = nc_get_vara_float(ncid, valHPointer, start, count, heights))) {
        throw std::runtime_error("Could not load h variable");
    }
}


void NetCDFReader::getHorizontalMomentumForTimeStep(size_t timeIndex, float hu[]) {
    int retval;
    if((retval = nc_inq_varid(ncid, "hu", &valHuPointer))) {
        throw std::runtime_error("Could not load hu variable");
    }
    size_t start[3] = {timeIndex, 0, 0};
    size_t count[3] = {1, dimYLength, dimXLength};
    if((retval = nc_get_vara_float(ncid, valHuPointer, start, count, hu))) {
        throw std::runtime_error("Could not load hu variable");
    }
}


void NetCDFReader::geVerticalMomentumForTimeStep(size_t timeIndex, float hv[]) {
    int retval;
    if((retval = nc_inq_varid(ncid, "hv", &valHvPointer))) {
        throw std::runtime_error("Could not load hv variable");
    }
    size_t start[3] = {timeIndex, 0, 0};
    size_t count[3] = {1, dimYLength, dimXLength};
    if((retval = nc_get_vara_float(ncid, valHvPointer, start, count, hv))) {
        throw std::runtime_error("Could not load hv variable");
    }
}

void NetCDFReader::getBathymetry(float b[]) {
    int retval;
    if((retval = nc_inq_varid(ncid, "b", &valBPointer))) {
        throw std::runtime_error("Could not load b variable");
    }
    size_t start[2] = {0, 0};
    size_t count[2] = {dimYLength, dimXLength};
    if((retval = nc_get_vara_float(ncid, valBPointer, start, count, b))) {
        throw std::runtime_error("Could not load b variable");
    }
}
