#pragma once
#include <filesystem>
#include <optional>
#include "netcdf_reader.h"
#include "../mesh/plane.h"
#include <vector>

/**
 * Responsible for holding information about the current state of the simulation
 * (e.g. current sim time, reference to the models that will be updated by the simulation, netcdf-reader, if the simulation
 * is currently paused or playing)
 */
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
	bool reverse = false;

	std::optional<std::filesystem::path> _curPath;
	NetCDFReader _reader;
	std::optional<Plane> _hPlane;

	std::vector<float> _fileTimes;
	size_t _currentFileTimeIndex;
	size_t _maxFileTimeIndex;

public:
	/**
	 * Load simulation data from specifies path and initialize the shader
	 * @param path Specifies path to simulation data
	 * @param shader Specifies the shader to be initialized
	 */
	void loadSimulation(std::filesystem::path path, Shader &shader);

	/**
	 * Start simulation
	 */
	void play();

	/**
	 * Pause simulation
	 */
	void pause();

	/**
	 * Reset simulation to start
	 */
	void reset();

	/**
	 * Step simulation forwards or backwards
	 */
	void stepForward();
	void stepBackwards();

	/**
	 * Update simulation
	 */
	void update(double deltaTime);

	/**
	 * Render current state of simulation
	 */
	void render(Shader &shader);

	/**
	 * Display color setting
	 */
	void colorSettingsWindow(Shader &shader);
	/**
	 * Open color settings window in IMGUI
	 */
	void openColorSettingsWindow();
	/**
	 * Close settings window in IMGUI
	*/
	void closeColorSettingsWindow();

	/**
	 * Set the color of the simulation
	 */
	void standardColors(Shader &shader);

	/**
	 * Open settings window in IMGUI
	 */
	void openSettingsWindow();
	/**
	 * Close settings window in IMGUI
	 */
	void closeSettingsWindow();

	/**
	 * Check if simulation is currently playing
	 */
	bool isPlaying();
};