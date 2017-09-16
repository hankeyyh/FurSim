#include "ShaderYH.h"
#include <cassert>


ShaderYH::ShaderYH(const std::string& shaderCode, GLenum shaderType) :
_object(0),
_refCount(NULL)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	//create the shader object
	_object = glCreateShader(shaderType);
	if (_object == 0)
		throw std::runtime_error("glCreateShader failed");

	//set the source code
	const char* code = shaderCode.c_str();
	glShaderSource(_object, 1, (const GLchar**)&code, NULL);

	//compile
	glCompileShader(_object);

	//throw exception if compile error occurred
	GLint status;
	glGetShaderiv(_object, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) 
	{
		std::string msg("Compile failure in shader:\n");

		GLint infoLogLength;
		glGetShaderiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(_object, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteShader(_object); _object = 0;
		throw std::runtime_error(msg);
	}

	_refCount = new unsigned;
	*_refCount = 1;
}

ShaderYH::ShaderYH(const ShaderYH& other) :
_object(other._object),
_refCount(other._refCount)
{
	_retain();
}

ShaderYH::~ShaderYH() 
{
	//_refCount will be NULL if constructor failed and threw an exception
	if (_refCount) 
		_release();
}

GLuint ShaderYH::Object() const 
{
	return _object;
}

ShaderYH& ShaderYH::operator = (const ShaderYH& other) 
{
	_release();
	_object = other._object;
	_refCount = other._refCount;
	_retain();
	return *this;
}

ShaderYH ShaderYH::ShaderYHFromFile(const std::string& filePath, GLenum shaderType) 
{
	//open file
	std::ifstream f;
	f.open(filePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open()){
		throw std::runtime_error(std::string("Failed to open file: ") + filePath);
	}

	//read whole file into stringstream buffer
	std::stringstream buffer;
	buffer << f.rdbuf();

	//return new shader
	ShaderYH shader(buffer.str(), shaderType);
	return shader;
}

void ShaderYH::_retain() 
{
	assert(_refCount);
	*_refCount += 1;
}

void ShaderYH::_release()
{
	assert(_refCount && *_refCount > 0);
	*_refCount -= 1;
	if (*_refCount == 0){
		glDeleteShader(_object); _object = 0;
		delete _refCount; _refCount = NULL;
	}
}
