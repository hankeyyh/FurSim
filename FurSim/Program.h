#pragma once
#include "ShaderYH.h"
#include <glm\glm.hpp>
#include <vector>
using namespace std;

class Program
{
public:
	Program(const vector<ShaderYH>& shaders);
	~Program();

	GLuint Object() const;

	void Use() const;
	bool IsInUse() const;
	void End() const;
	void DeleteProgram() const;

	GLint Attrib(const GLchar* attribName) const; //get attrib location
	GLint Uniform(const GLchar* uniformName) const; //get uniform location

	/*
	Setters for attribute and uniform variables.
	These are convenience methods for the glVertexAttrib* and glUniform* functions.
	*/
#define _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE) \
	void SetAttrib(const GLchar* attribName, OGL_TYPE v0); \
	void SetAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1); \
	void SetAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
	void SetAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
	\
	void SetAttrib1v(const GLchar* attribName, const OGL_TYPE* v); \
	void SetAttrib2v(const GLchar* attribName, const OGL_TYPE* v); \
	void SetAttrib3v(const GLchar* attribName, const OGL_TYPE* v); \
	void SetAttrib4v(const GLchar* attribName, const OGL_TYPE* v); \
	\
	void SetUniform(const GLchar* uniformName, OGL_TYPE v0); \
	void SetUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1); \
	void SetUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
	void SetUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
	\
	void SetUniform1v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count = 1); \
	void SetUniform2v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count = 1); \
	void SetUniform3v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count = 1); \
	void SetUniform4v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count = 1); \

	_TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLfloat)
	_TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLdouble)
	_TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLint)
	_TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLuint)

	void SetUniformMatrix2(const GLchar* uniformName, const GLfloat* v, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void SetUniformMatrix3(const GLchar* uniformName, const GLfloat* v, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void SetUniformMatrix4(const GLchar* uniformName, const GLfloat* v, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void SetUniform(const GLchar* uniformName, const glm::mat2& m, GLboolean transpose = GL_FALSE);
	void SetUniform(const GLchar* uniformName, const glm::mat3& m, GLboolean transpose = GL_FALSE);
	void SetUniform(const GLchar* uniformName, const glm::mat4& m, GLboolean transpose = GL_FALSE);
	void SetUniform(const GLchar* uniformName, const glm::vec3& v);
	void SetUniform(const GLchar* uniformName, const glm::vec4& v);

private:
	GLuint _object;

	//copying disabled
	Program(const Program&);
	const Program& operator=(const Program&);
};

