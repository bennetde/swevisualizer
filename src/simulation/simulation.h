#pragma once
#include <filesystem>
#include <optional>
#include "netcdf_reader.h"
#include "../mesh/plane.h"
// Responsible for holding information about the current state of the simulation
// (e.g. current sim time, reference to the models that will be updated by the simulation, netcdf-reader, if the simulation
// is currently paused or playing)

class Simulation {
private:
	float _curTime;
	bool _isPlaying;
	bool _loaded;
	
	std::optional<std::filesystem::path> _curPath;
	NetCDFReader _reader;
	std::optional<Plane> _hPlane;

	std::vector<float> _fileTimes;
	size_t _currentFileTimeIndex;
	size_t _maxFileTimeIndex;
public:
	// Simulation();
	void loadSimulation(std::filesystem::path path);
	void play();
	void pause();
	void reset();

	void update(double deltaTime);
	void render(Shader& shader);
};