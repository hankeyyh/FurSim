#include "Program.h"
#include <glm/gtc/type_ptr.hpp>

Program::Program(const vector<ShaderYH>& shaders) :
_object(0)
{
	if (shaders.size() <= 0)
		throw std::runtime_error("No shaders were provided to create the program");

	//create the program object
	_object = glCreateProgram();
	if (_object == 0)
		throw std::runtime_error("glCreateProgram failed");

	//attach all the shaders
	for (unsigned i = 0; i < shaders.size(); ++i)
		glAttachShader(_object, shaders[i].Object());

	//link the shaders together
	glLinkProgram(_object);

	//detach all the shaders
	for (unsigned i = 0; i < shaders.size(); ++i)
		glDetachShader(_object, shaders[i].Object());

	//throw exception if linking failed
	GLint status;
	glGetProgramiv(_object, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) 
	{
		std::string msg("Program linking failure: ");

		GLint infoLogLength;
		glGetProgramiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(_object, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteProgram(_object); _object = 0;
		throw std::runtime_error(msg);
	}
}


Program::~Program()
{
	//might be 0 if ctor fails by throwing exception
	if (_object != 0)
		glDeleteProgram(_object);
}

GLuint Program::Object() const
{
	return _object;
}

void Program::Use() const 
{
	glUseProgram(_object);
}

bool Program::IsInUse() const 
{
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	return (currentProgram == (GLint)_object);
}

void Program::End() const 
{
	assert(IsInUse());
	glUseProgram(0);
}

void Program::DeleteProgram() const
{
	if (_object != 0)
		glDeleteProgram(_object);
}

GLint Program::Attrib(const GLchar* attribName) const 
{
	if (!attribName)
		throw std::runtime_error("attribName was NULL");

	GLint attrib = glGetAttribLocation(_object, attribName);
	/*if (attrib == -1)
		throw std::runtime_error(std::string("Program attribute not found: ") + attribName);*/

	return attrib;
}

GLint Program::Uniform(const GLchar* uniformName) const
{
	if (!uniformName)
		throw std::runtime_error("uniformName was NULL");

	GLint uniform = glGetUniformLocation(_object, uniformName);
	/*if (uniform == -1)
		throw std::runtime_error(std::string("Program uniform not found: ") + uniformName);*/

	return uniform;
}

#define ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE, TYPE_PREFIX, TYPE_SUFFIX) \
	\
	void Program::SetAttrib(const GLchar* name, OGL_TYPE v0) \
		{ assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX(Attrib(name), v0); } \
	void Program::SetAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
		{ assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX(Attrib(name), v0, v1); } \
	void Program::SetAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
		{ assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX(Attrib(name), v0, v1, v2); } \
	void Program::SetAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
		{ assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX(Attrib(name), v0, v1, v2, v3); } \
	\
	void Program::SetAttrib1v(const GLchar* name, const OGL_TYPE* v) \
		{ assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX ## v(Attrib(name), v); } \
	void Program::SetAttrib2v(const GLchar* name, const OGL_TYPE* v) \
		{ assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX ## v(Attrib(name), v); } \
	void Program::SetAttrib3v(const GLchar* name, const OGL_TYPE* v) \
		{ assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX ## v(Attrib(name), v); } \
	void Program::SetAttrib4v(const GLchar* name, const OGL_TYPE* v) \
		{ assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX ## v(Attrib(name), v); } \
	\
	void Program::SetUniform(const GLchar* name, OGL_TYPE v0) \
		{ assert(IsInUse()); glUniform1 ## TYPE_SUFFIX(Uniform(name), v0); } \
	void Program::SetUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
		{ assert(IsInUse()); glUniform2 ## TYPE_SUFFIX(Uniform(name), v0, v1); } \
	void Program::SetUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
		{ assert(IsInUse()); glUniform3 ## TYPE_SUFFIX(Uniform(name), v0, v1, v2); } \
	void Program::SetUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
		{ assert(IsInUse()); glUniform4 ## TYPE_SUFFIX(Uniform(name), v0, v1, v2, v3); } \
	\
	void Program::SetUniform1v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
		{ assert(IsInUse()); glUniform1 ## TYPE_SUFFIX ## v(Uniform(name), count, v); } \
	void Program::SetUniform2v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
		{ assert(IsInUse()); glUniform2 ## TYPE_SUFFIX ## v(Uniform(name), count, v); } \
	void Program::SetUniform3v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
		{ assert(IsInUse()); glUniform3 ## TYPE_SUFFIX ## v(Uniform(name), count, v); } \
	void Program::SetUniform4v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
		{ assert(IsInUse()); glUniform4 ## TYPE_SUFFIX ## v(Uniform(name), count, v); }

ATTRIB_N_UNIFORM_SETTERS(GLfloat, , f);
//ATTRIB_N_UNIFORM_SETTERS(GLdouble, , d); //glUniform1d is only provided in glew
ATTRIB_N_UNIFORM_SETTERS(GLint, I, i);
ATTRIB_N_UNIFORM_SETTERS(GLuint, I, ui);

void Program::SetUniformMatrix2(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) 
{
	assert(IsInUse());
	glUniformMatrix2fv(Uniform(name), count, transpose, v);
}

void Program::SetUniformMatrix3(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose)
{
	assert(IsInUse());
	glUniformMatrix3fv(Uniform(name), count, transpose, v);
}

void Program::SetUniformMatrix4(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose)
{
	assert(IsInUse());
	glUniformMatrix4fv(Uniform(name), count, transpose, v);
}

void Program::SetUniform(const GLchar* name, const glm::mat2& m, GLboolean transpose) 
{
	assert(IsInUse());
	glUniformMatrix2fv(Uniform(name), 1, transpose, glm::value_ptr(m));
}

void Program::SetUniform(const GLchar* name, const glm::mat3& m, GLboolean transpose) 
{
	assert(IsInUse());
	glUniformMatrix3fv(Uniform(name), 1, transpose, glm::value_ptr(m));
}

void Program::SetUniform(const GLchar* name, const glm::mat4& m, GLboolean transpose) 
{
	assert(IsInUse());
	glUniformMatrix4fv(Uniform(name), 1, transpose, glm::value_ptr(m));
}

void Program::SetUniform(const GLchar* uniformName, const glm::vec3& v) 
{
	SetUniform3v(uniformName, glm::value_ptr(v));
}

void Program::SetUniform(const GLchar* uniformName, const glm::vec4& v) 
{
	SetUniform4v(uniformName, glm::value_ptr(v));
}