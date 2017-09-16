#pragma once
//#include <GLEW/glew.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

#define GLSL(x) #x

class ShaderYH
{
public:
	ShaderYH(const std::string& ShaderYHCode, GLenum ShaderYHType);
	ShaderYH(const ShaderYH& other);
	~ShaderYH();

	GLuint Object() const;
	ShaderYH& operator =(const ShaderYH& other);
	static ShaderYH ShaderYHFromFile(const std::string& filePath, GLenum ShaderYHType);

private:
	GLuint _object;
	unsigned* _refCount;

	void _retain();
	void _release();
};

