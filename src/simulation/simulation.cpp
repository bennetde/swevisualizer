#include "simulation.h"
#include <imgui.h>
#include <algorithm>
#include <iostream>

void Simulation::loadSimulation(std::filesystem::path path, Shader &shader)
{
	if (_loaded)
	{
		_reader.close();
	}
	_reader = NetCDFReader();
	_reader.open(path);
	_curPath = path;

	/**
	 * Create Plane with dimensions of the simulation
	 */
	_hPlane = Plane{_reader.getXDimension(), _reader.getYDimension(), _reader.getCellWidth(), _reader.getCellHeight()};
	_loaded = true;

	/**
	 * Load initial wave heights
	 */
	_reader.getHeightsForTimeStep(0, _hPlane.value().displacements.data());
	_hPlane.value().updateDisplacementBuffer();

	/**
	 * Load bathymetry data
	 */
	_reader.getBathymetry(_hPlane.value().bathymetry.data());
	_hPlane.value().updateBathymetryBuffer();

	/**
	 * Load min and max height, hu and hv
	 */
	minHeight = _reader.getMinHeight();
	maxHeight = _reader.getMaxHeight();
	minHu = _reader.getMinHu();
	maxHu = _reader.getMaxHu();
	minHv = _reader.getMinHv();
	maxHv = _reader.getMaxHv();

	minBathymetry = 0.0;
	maxBathymetry = _reader.getMaxBathymetry();

	/**
	 * Set time
	 */
	_fileTimes = _reader.getTimeSteps();

	_maxFileTimeIndex = _fileTimes[_fileTimes.size() - 1];
	_currentFileTimeIndex = 0;
	reset();

	openSettingsWindow();
}

void Simulation::render(Shader &shader)
{

	if (_hPlane.has_value())
	{
		_hPlane.value().render(shader);
	}

	shader.setFloat("minHeight", minHeight);
	shader.setFloat("maxHeight", maxHeight);
	shader.setFloat("minHu", minHu);
	shader.setFloat("maxHu", maxHu);
	shader.setFloat("minHv", minHv);
	shader.setFloat("maxHv", maxHv);
	shader.setFloat("minBathymetry", minBathymetry);
	shader.setFloat("maxBathymetry", maxBathymetry);
	shader.setBool("hu", hu);
	shader.setBool("hv", hv);

	if (_showSimulationSettingsWindow)
	{
		if (ImGui::Begin("Simulation Settings", &_showSimulationSettingsWindow))
		{
			if (_curPath.has_value())
			{
				ImGui::Text("Current Path: %s", _curPath.value().string().data());
			}
			else
			{
				ImGui::Text("No file loaded");
			}

			ImGui::Text("SimTime: %f", this->_curTime);
			if (ImGui::Button(_isPlaying ? "Pause" : "Play"))
			{
				if (_isPlaying)
				{
					pause();
				}
				else
				{
					play();
				}
			}
			// ImGui::SameLine();

			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				reset();
			}

			if (ImGui::InputFloat("Minimum Height Value", &minHeight))
			{
				shader.setFloat("minHeight", minHeight);
			}
			if (ImGui::InputFloat("Maximum Height Value", &maxHeight))
			{
				shader.setFloat("maxHeight", maxHeight);
			}
			if (ImGui::InputFloat("Minimum Hu Value", &minHu))
			{
				shader.setFloat("minHu", minHu);
			}
			if (ImGui::InputFloat("Maximum Hu Value", &maxHu))
			{
				shader.setFloat("maxHu", maxHu);
			}
			if (ImGui::InputFloat("Minimum Hv Value", &minHv))
			{
				shader.setFloat("minHv", minHv);
			}
			if (ImGui::InputFloat("Maximum Hv Value", &maxHv))
			{
				shader.setFloat("maxHv", maxHv);
			}
			if (ImGui::InputFloat("Minimum B Value", &minBathymetry))
			{
				shader.setFloat("minBathymetry", minBathymetry);
			}
			if (ImGui::InputFloat("Maximum B Value", &maxBathymetry))
			{
				shader.setFloat("maxB", maxBathymetry);
			}

			ImGui::InputFloat("Simulation speed", &speed);

			if (ImGui::Checkbox("hv", &hv))
			{
				shader.setBool("bool_hv", hv);
			}
			if (ImGui::Checkbox("hu", &hu))
			{
				shader.setBool("bool_hu", hu);
			}

			ImGui::Checkbox("reverse", &reverse);

			if (ImGui::Button("Color Settings"))
			{
				colorSettings = true;
			}
		}
		ImGui::End();
	}

	if (colorSettings)
	{
		colorSettingsWindow(shader);
	}
}

/**
 * Color settings using ImGui
 */
void Simulation::colorSettingsWindow(Shader &shader)
{
	if (ImGui::Begin("Color Settings", &colorSettings))
	{
		/**
		 * Set uniform values to initial color settings depending on user input
		 */
		if (ImGui::Button("Reset"))
		{
			shader.setFloat4("minCol", minCol);
			shader.setFloat4("maxCol", maxCol);
			shader.setFloat4("minBathymetryCol", minBathymetryCol);
			shader.setFloat4("maxBathymetryCol", maxBathymetryCol);
			shader.setFloat4("minhuCol", minhuCol);
			shader.setFloat4("maxhuCol", maxhuCol);
			shader.setFloat4("minhvCol", minhvCol);
			shader.setFloat4("maxhvCol", maxhvCol);
		}
		ImGui::SameLine();
		/**
		 * ColorPickers for minimum and maximum values of different variables
		 */
		if (ImGui::ColorPicker4("minColor", minCol))
		{
			shader.setFloat4("minCol", minCol);
		}

		if (ImGui::ColorPicker4("maxColor", maxCol))
		{
			shader.setFloat4("maxCol", maxCol);
		}

		if (ImGui::ColorPicker4("minBathymetryColor", minBathymetryCol))
		{
			shader.setFloat4("minBathymetryCol", minBathymetryCol);
		}

		if (ImGui::ColorPicker4("maxBathymetryColor", maxBathymetryCol))
		{
			shader.setFloat4("maxBathymetryCol", maxBathymetryCol);
		}

		if (ImGui::ColorPicker4("minHuColor", minhuCol))
		{
			shader.setFloat4("minhuCol", minhuCol);
		}

		if (ImGui::ColorPicker4("maxHuColor", maxhuCol))
		{
			shader.setFloat4("maxhuCol", maxhuCol);
		}

		if (ImGui::ColorPicker4("minHvColor", minhvCol))
		{
			shader.setFloat4("minhvCol", minhvCol);
		}

		if (ImGui::ColorPicker4("maxHvColor", maxhvCol))
		{
			shader.setFloat4("maxhvCol", maxhvCol);
		}
	}
	ImGui::End();
}

/**
 * Update simulation state based on current time and playback settings
 */
void Simulation::update(double deltaTime)
{
	if (!_isPlaying || !_loaded)
		return;

	if (_currentFileTimeIndex == _fileTimes.size() - 1 && !reverse)
	{
		pause();
		return;
	}

	/**
	 * Handles siumlation playing in reverse
	 */
	if (_currentFileTimeIndex == -1 && reverse)
	{
		pause();
		return;
	}

	/**
	 * Update simulation based on playback direction
	 */
	if (reverse)
	{
		_curTime -= 1 * speed;
	}
	else
	{
		_curTime += 1 * speed;
	}

	if (reverse)
	{
		if (_curTime <= _fileTimes[_currentFileTimeIndex])
		{
			// Load displacement
			_reader.getHeightsForTimeStep(_currentFileTimeIndex, _hPlane.value().displacements.data());
			_hPlane.value().updateDisplacementBuffer();
			// Load hu
			_reader.getHorizontalMomentumForTimeStep(_currentFileTimeIndex, _hPlane.value().hu.data());
			_hPlane.value().updateHuBuffer();
			// Load hv
			_reader.getVerticalMomentumForTimeStep(_currentFileTimeIndex, _hPlane.value().hv.data());
			_hPlane.value().updateHvBuffer();

			_currentFileTimeIndex--;
		}
	}

	else
	{
		if (_curTime >= _fileTimes[_currentFileTimeIndex + 1])
		{
			// Load displacement
			_reader.getHeightsForTimeStep(_currentFileTimeIndex + 1, _hPlane.value().displacements.data());
			_hPlane.value().updateDisplacementBuffer();
			// Load hu
			_reader.getHorizontalMomentumForTimeStep(_currentFileTimeIndex + 1, _hPlane.value().hu.data());
			_hPlane.value().updateHuBuffer();
			// Load hv
			_reader.getVerticalMomentumForTimeStep(_currentFileTimeIndex + 1, _hPlane.value().hv.data());
			_hPlane.value().updateHvBuffer();

			_currentFileTimeIndex++;
		}
	}
}

void Simulation::play()
{
	if (!_loaded)
		return;
	_isPlaying = true;
}

void Simulation::pause()
{
	if (!_loaded)
		return;
	_isPlaying = false;
}

void Simulation::reset()
{
	_curTime = 0.0f;
	_currentFileTimeIndex = 0;
	_isPlaying = false;
}

void Simulation::openSettingsWindow()
{
	_showSimulationSettingsWindow = true;
}

void Simulation::closeSettingsWindow()
{
	_showSimulationSettingsWindow = false;
}

bool Simulation::isPlaying()
{
	return _isPlaying;
}

void Simulation::openColorSettingsWindow()
{
	colorSettings = true;
}

void Simulation::closeColorSettingsWindow()
{
	colorSettings = false;
}

void Simulation::stepForward()
{
	if (!_loaded)
		return;
	if (_currentFileTimeIndex == _fileTimes.size() - 1)
	{
		return;
	}

	// Load displacement
	_reader.getHeightsForTimeStep(_currentFileTimeIndex + 1, _hPlane.value().displacements.data());
	_hPlane.value().updateDisplacementBuffer();
	// Load hu
	_reader.getHorizontalMomentumForTimeStep(_currentFileTimeIndex + 1, _hPlane.value().hu.data());
	_hPlane.value().updateHuBuffer();
	// Load hv
	_reader.getVerticalMomentumForTimeStep(_currentFileTimeIndex + 1, _hPlane.value().hv.data());
	_hPlane.value().updateHvBuffer();

	_curTime = _fileTimes[_currentFileTimeIndex + 1];
	_currentFileTimeIndex++;
}

void Simulation::stepBackwards()
{
	if (!_loaded)
		return;
	if (_currentFileTimeIndex == 0)
	{
		return;
	}

	// Load displacement
	_reader.getHeightsForTimeStep(_currentFileTimeIndex - 1, _hPlane.value().displacements.data());
	_hPlane.value().updateDisplacementBuffer();
	// Load hu
	_reader.getHorizontalMomentumForTimeStep(_currentFileTimeIndex - 1, _hPlane.value().hu.data());
	_hPlane.value().updateHuBuffer();
	// Load hv
	_reader.getVerticalMomentumForTimeStep(_currentFileTimeIndex - 1, _hPlane.value().hv.data());
	_hPlane.value().updateHvBuffer();

	_curTime = _fileTimes[_currentFileTimeIndex - 1];
	_currentFileTimeIndex--;
}