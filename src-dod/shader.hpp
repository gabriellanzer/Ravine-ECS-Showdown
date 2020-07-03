#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>

#include "debug.hpp"

using std::string;

bool checkShaderCompile(GLuint shaderId)
{
	//Get Compilation Result Code
	GLint isCompiled = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

	//If got an error
	if (isCompiled == GL_FALSE)
	{
		//Get size of info log
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		GLchar* errorLog = new GLchar[maxLength];
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);

		//Create message
		string msg((const char*)errorLog);

		//Pop last \0 char
		msg.pop_back();

		//Log Error through Debugger
		rvDebugLogError("Error compiling Shader: " + msg);

		//Don't leak the shader
		glDeleteShader(shaderId);

		//Exit with failure.
		return false;
	}

	return true;
}

bool checkProgramLink(GLuint programId)
{
	//Get Linking Result Code
	GLint isLinked = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);

	//If got an error
	if (isLinked == GL_FALSE)
	{
		//Get size of info log
		GLint maxLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		GLchar* errorLog = new GLchar[maxLength];
		glGetProgramInfoLog(programId, maxLength, &maxLength, &errorLog[0]);

		//Create message
		string msg((const char*)errorLog);

		//Pop last \0 char
		msg.pop_back();

		//Log Error through Debugger
		rvDebugLogError("Error compiling Shader: " + msg);

		//Don't leak the shader
		glDeleteProgram(programId);

		//Exit with failure.
		return false;
	}

	return true;
}

#endif