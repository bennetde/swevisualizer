#pragma once
#include <filesystem>

// Responsible for holding information about the current state of the simulation
// (e.g. current sim time, reference to the models that will be updated by the simulation, netcdf-reader, if the simulation
// is currently paused or playing)

class Simulation {
private:
	float _curTime;
	bool _isPlaying;
public:
	void loadSimulation(std::filesystem::path path);
	void play();
	void pause();
	void reset();

	void update();
	void render();
};