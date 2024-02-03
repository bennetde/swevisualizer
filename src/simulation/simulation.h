#pragma once
#include <filesystem>
#include <optional>
#include "netcdf_reader.h"
#include "../mesh/plane.h"
#include <vector> // Added vector header

// Responsible for holding information about the current state of the simulation
// (e.g. current sim time, reference to the models that will be updated by the simulation, netcdf-reader, if the simulation
// is currently paused or playing)
class Simulation
{
private:
	float _curTime;
	float speed = 1.0f;
	bool _isPlaying;
	bool _loaded;
	float minHeight;
	float maxHeight;
	float minHu;
	float maxHu;
	float minHv;
	float maxHv;
	float minBathymetry;
	float maxBathymetry;

	bool _showSimulationSettingsWindow = false;
	bool colorSettings;
	float minCol[4];
	float maxCol[4];
	float minhuCol[4];
	float maxhuCol[4];
	float minhvCol[4];
	float maxhvCol[4];
	float minBathymetryCol[4];
	float maxBathymetryCol[4];
	
	bool hu = false;
	bool hv = false;
	bool h = true;
	bool reverse = false;

	std::optional<std::filesystem::path> _curPath;
	NetCDFReader _reader;
	std::optional<Plane> _hPlane;

	std::vector<float> _fileTimes;
	size_t _currentFileTimeIndex;
	size_t _maxFileTimeIndex;

public:
	// Simulation();
	void loadSimulation(std::filesystem::path path, Shader& shader);
	void play();
	void pause();
	void reset();

	void stepForward();
	void stepBackwards();

	void update(double deltaTime);
	void render(Shader &shader);
	void colorSettingsWindow(Shader &shader);

	void openSettingsWindow();
	void closeSettingsWindow();

	void openColorSettingsWindow();
	void closeColorSettingsWindow();

	bool isPlaying();
};