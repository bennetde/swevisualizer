#pragma once
#include <filesystem>

/**
 * RAII-Wrapper for using OpenGL shaders.
*/
class Shader {
private:
	unsigned int identifier;
public:

	/**
	 * Load and compile a shader using paths to the respective vertex and fragment shaders.
	*/
	Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

	/**
	 * Enables the current shader for rendering
	*/
	void use() const;

	/**
	 * Sets a boolean variable
	*/
	void setBool(const std::string& name, bool value) const;

	/**
	 * Sets a integer variable
	*/
	void setInt(const std::string& name, int value) const;

	/**
	 * Sets a floating point variable
	*/
	void setFloat(const std::string& name, float value) const;
};