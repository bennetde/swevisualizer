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

	// Load Bathymetry
	_reader.getBathymetry(_hPlane.value().bathymetry.data());
	_hPlane.value().updateBathymetryBuffer();

	// Load min and max height, hu, hv
	minHeight = _reader.getMinHeight();
	maxHeight = _reader.getMaxHeight();
	minHu = _reader.getMinHu();
	maxHu = _reader.getMaxHu();
	minHv = _reader.getMinHv();
	maxHv = _reader.getMaxHv();

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
	
	shader.setFloat("minHeight", minHeight);
	shader.setFloat("maxHeight", maxHeight);
	shader.setFloat("minHu", minHu);
	shader.setFloat("maxHu", maxHu);
	shader.setFloat("minHv", minHv);
	shader.setFloat("maxHv", maxHv);
	shader.setBool("hv", hu);
	shader.setBool("hv", hv);
	shader.setBool("hv", h);

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
	// bool t = true;
	// ImGui::Checkbox("Reverse", &t);
	ImGui::SameLine();
	if(ImGui::Button("Reset")) {
		reset();
	}
	
	if(ImGui::InputFloat("Minimum Height Value", &minHeight)) {
		shader.setFloat("minHeight", minHeight);
	}
	if(ImGui::InputFloat("Maximum Height Value", &maxHeight)) {
		shader.setFloat("maxHeight", maxHeight);
	}
	if(ImGui::InputFloat("Minimum Hu Value", &minHu)) {
		shader.setFloat("minHu", minHu);
	}
	if(ImGui::InputFloat("Maximum Hu Value", &maxHu)) {
		shader.setFloat("maxHu", maxHu);
	}
	if(ImGui::InputFloat("Minimum Hv Value", &minHv)) {
		shader.setFloat("minHv", minHv);
	}
	if(ImGui::InputFloat("Maximum Hv Value", &maxHv)) {
		shader.setFloat("maxHv", maxHv);
	}
	if(ImGui::Checkbox("hu", &hu)) {
		shader.setBool("hu", hu);
	}
	if(ImGui::Checkbox("hv", &hv)) {
		shader.setBool("hv", hv);
	}
	if(ImGui::Checkbox("h", &h)) {
		shader.setBool("h", h);
	}

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
		// Load displacement
		_reader.getHeightsForTimeStep(_currentFileTimeIndex+1, _hPlane.value().displacements.data());
		_hPlane.value().updateDisplacementBuffer();
		// Load hu
		_reader.getHorizontalMomentumForTimeStep(_currentFileTimeIndex+1, _hPlane.value().hu.data());
		_hPlane.value().updateHuBuffer();
		// Load hv
		_reader.getVerticalMomentumForTimeStep(_currentFileTimeIndex+1, _hPlane.value().hv.data());
		_hPlane.value().updateHvBuffer();

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