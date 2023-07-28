#pragma once
#include <string>
#include <glm.hpp>


class Program
{
public:
	//General case for 
	Program(const std::string* shaderPaths, int* shaderTypes, int shaderCount);	


	//Common cases, uses first constructor, but makes writing easier
	Program(const std::string& vertPath, const std::string& fragPath);	
	Program(const std::string& vertPath, const std::string& geomPath, const std::string& fragPath);

	~Program();

	void use();

	void setBool(const char* name, bool value);	//Set uniforms within shaders
	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void setVec2(const char* name, glm::vec2 value);
	void setVec3(const char* name, glm::vec3 value);
	void setVec4(const char* name, glm::vec4 value);
	void setMat4(const char* name, glm::mat4 value);

	unsigned int getProgramID() { return programID; }
private:
	unsigned int programID;	//ID of this specific program

	unsigned int* shaderIDs;//IDs of all shaders within the program, as well as the number of shaders
	int shaderCount;

	void init(const std::string* shaderPaths, int* shaderTypes, int shaderCount);	//Set up the program (cannot call one function from another function)

	//Compile a shader at the given file location and return the ID of the generated shader
	unsigned int generateShader(const std::string& shaderDir, int shaderType);	

	//Link all shaders together into the program, returns if shaders were linked successfully
	bool linkShaders();
};