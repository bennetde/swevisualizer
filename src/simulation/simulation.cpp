#include "simulation.h"
#include <imgui.h>

// Simulation::Simulation() {

// }
void Simulation::loadSimulation(std::filesystem::path path) {
	if(_loaded) {
		_reader.close();
	}
	_reader = NetCDFReader();
	_reader.open(path);
	_curPath = path;

	// Create Plane with dimensions of the simulation
	_hPlane = Plane{_reader.getXDimension(), _reader.getYDimension()};
	_loaded = true;

	// Load initial wave heights 
	_reader.getHeightsForTimeStep(0, _hPlane.value().displacements.data());
	_hPlane.value().updateDisplacementBuffer();
	// eventuell minHeight, maxHeight werte aus displacements data lesen

	// TODO: Load bathymetry, hu, hv
	// _reader.getBathymetry()

	// Set Time
	_fileTimes = _reader.getTimeSteps();
	_maxFileTimeIndex = _fileTimes[_fileTimes.size()-1];
	_currentFileTimeIndex = 0;
	reset();
}

void Simulation::render(Shader& shader) {
	if(_hPlane.has_value()) {
		_hPlane.value().render(shader);
	}

	if(ImGui::Begin("Simulation Settings")) {
		if(_curPath.has_value()) {
			ImGui::Text("Current Path: %s", _curPath.value().string().data());
		} else {
			ImGui::Text("No file loaded");
		}
	}
	ImGui::Text("SimTime: %f", this->_curTime);
	if(ImGui::Button(_isPlaying ? "Pause" : "Play")) {
		if(_isPlaying) {
			pause();
		} else {
			play();
		}
	}
	ImGui::SameLine();
	bool t = true;
	ImGui::Checkbox("Reverse", &t);
	ImGui::SameLine();
	if(ImGui::Button("Reset")) {
		reset();
	}
	// make this members of the class
	float minHeight, maxHeight;
	if(ImGui::InputFloat("Minimum Height Value", &minHeight)) {
		shader.setFloat("minHeight", minHeight);
	}
	ImGui::InputFloat("Maximum Height Value", &maxHeight);
	// eventuell color wheel
	// ImGui::ColorPicker4("test", );
	ImGui::End();
}

void Simulation::update(double deltaTime) {
	if(!_isPlaying || !_loaded) return;

	if(_currentFileTimeIndex == _fileTimes.size() -1) {
		pause();
		return;
	}
	// maybe simulation speed variable
	_curTime += 1;

	if(_curTime >= _fileTimes[_currentFileTimeIndex + 1]) {
		_reader.getHeightsForTimeStep(_currentFileTimeIndex+1, _hPlane.value().displacements.data());
		_hPlane.value().updateDisplacementBuffer();
		// TODO: Load hu, hv....

		_currentFileTimeIndex++;
	}
}

void Simulation::play() {
	if(!_loaded) return;
	_isPlaying = true;
}

void Simulation::pause() {
	if(!_loaded) return;
	_isPlaying = false;
}

void Simulation::reset() {
	_curTime = 0.0f;
	_currentFileTimeIndex = 0;
	_isPlaying = false;
}