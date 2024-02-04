#include "shader.h"

#include <glad/gl.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

/**
 * Constructor that loads, compiles and links vertex and fragment shader
 */
Shader::Shader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath)
{
	/**
	 * Make sure path exists and otherwise throw errors
	 */
	if (!std::filesystem::exists(vertexPath))
	{
		throw std::runtime_error("Vertex-Shader File not found " + vertexPath.string());
	}
	if (!std::filesystem::exists(fragmentPath))
	{
		throw std::runtime_error("Fragment-Shader File not found " + fragmentPath.string());
	}

	/**
	 * Read shaders from their files, if not successful throw error
	 */
	std::ifstream vShaderFile, fShaderFile;
	std::stringstream vShaderStream, fShaderStream;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();
	}
	catch (std::exception e)
	{
		throw std::runtime_error("Shader could not be read " + vertexPath.string() + ", " + fragmentPath.string());
	}

	std::string vS = vShaderStream.str();
	std::string fS = fShaderStream.str();
	const char *vShaderCode = vS.c_str();
	const char *fShaderCode = fS.c_str();

	unsigned int vertexId, fragmentId;
	int success;
	char infoLog[512];

	/**
	 * Compile vertex shader, handle possible errors
	 */
	vertexId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexId, 1, &vShaderCode, NULL);
	glCompileShader(vertexId);
	glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
		throw std::runtime_error("Vertex Shader compilation failed: " + std::string{infoLog});
	}

	/**
	 * Compile fragment shader, handle possible errors
	 */
	fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentId, 1, &fShaderCode, NULL);
	glCompileShader(fragmentId);
	glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentId, 512, NULL, infoLog);
		throw std::runtime_error("Fragment Shader compilation failed: " + std::string{infoLog});
	}

	/**
	 * Link vertex and fragment shader
	 */
	identifier = glCreateProgram();
	glAttachShader(identifier, vertexId);
	glAttachShader(identifier, fragmentId);
	glLinkProgram(identifier);
	glGetProgramiv(identifier, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(identifier, 512, NULL, infoLog);
		throw std::runtime_error("Shader linking failed: " + std::string{infoLog});
	}

	/**
	 * Delete unused shaders
	 */
	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	void standardColors();
}

/**
 * Set default uniform colors in the shader and activate the shader program
 */
void Shader::standardColors()
{
	float minCol[4] = {0.5f, 0.7f, 0.8f, 1.0f};
	float maxCol[4] = {0.0f, 0.0f, 1.0f, 1.0f};
	float minhuCol[4] = {0.2f, 0.0f, 0.0f, 1.0f};
	float maxhuCol[4] = {0.5f, 0.0f, 0.0f, 1.0f};
	float minhvCol[4] = {0.2f, 0.0f, 0.0f, 1.0f};
	float maxhvCol[4] = {0.5f, 0.0f, 0.0f, 1.0f};
	float minBathymetryCol[4] = {0.0f, 0.5f, 0.0f, 1.0f};
	float maxBathymetryCol[4] = {0.4f, 0.2f, 0.0f, 1.0f};

	use();
	setFloat4("minCol", minCol);
	setFloat4("maxCol", maxCol);
	setFloat4("minhuCol", minhuCol);
	setFloat4("maxhuCol", maxhuCol);
	setFloat4("minhvCol", minhvCol);
	setFloat4("maxhvCol", maxhvCol);
	setFloat4("minBathymetryCol", minBathymetryCol);
	setFloat4("maxBathymetryCol", maxBathymetryCol);
}

/**
 * Activate shader program
 */
void Shader::use() const
{
	glUseProgram(identifier);
}

/**
 * Set boolean uniform value in shader
 */
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(identifier, name.c_str()), (int)value);
}

/**
 * Set int uniform value in shader
 */
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(identifier, name.c_str()), value);
}

/**
 * Set float uniform value in shader
 */
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(identifier, name.c_str()), value);
}

/**
 * Set 4-component float uniform value in shader
 */
void Shader::setFloat4(const std::string &name, float value[4]) const
{
	glUniform4f(glGetUniformLocation(identifier, name.c_str()), value[0], value[1], value[2], value[3]);
}

/**
 * Set 4x4 matrix uniform value in shader
 */
void Shader::setMat(const std::string &name, glm::mat4x4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(identifier, name.c_str()), 1, GL_FALSE, &value[0][0]);
}