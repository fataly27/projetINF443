
#include "uniform.hpp"

#include "cgp/base/base.hpp"
#include "cgp/display/opengl/debug/debug.hpp"


namespace cgp
{
	static bool check_location(GLint location, std::string const& name, GLuint shader, bool expected)
	{
		if (location == -1 && expected == true)
		{
			std::string const error_str = "Try to send uniform variable [" + name + "] to a shader that doesn't use it.\n Either change the uniform variable to expected=false, or correct the associated shader (id=" + str(shader) + ").";
#ifdef CHECK_OPENGL_UNIFORM_STRICT
			error_cgp(error_str);
#else
			warning_cgp(error_str,"");
#endif
		}
		if(location == -1)
			return false;
		else
			return true;

	}

	void opengl_uniform(GLuint shader, std::string const& name, int value, bool expected)
	{
		assert_cgp(shader!=0, "Try to send uniform "+name+" to unspecified shader");
		GLint const location = glGetUniformLocation(shader, name.c_str()); opengl_check;
		if(check_location(location, name, shader, expected))
			glUniform1i(location, value); opengl_check;
	}

	void opengl_uniform(GLuint shader, std::string const& name, GLuint value, bool expected)
	{
		assert_cgp(shader != 0, "Try to send uniform " + name + " to unspecified shader");
		GLint const location = glGetUniformLocation(shader, name.c_str()); opengl_check;
		if (check_location(location, name, shader, expected))
			glUniform1i(location, value); opengl_check;
	}
	void opengl_uniform(GLuint shader, std::string const& name, float value, bool expected)
	{
		assert_cgp(shader!=0, "Try to send uniform "+name+" to unspecified shader");
		GLint const location = glGetUniformLocation(shader, name.c_str()); opengl_check;
		if(check_location(location, name, shader, expected))
			glUniform1f(location, value); opengl_check;
	}
	void opengl_uniform(GLuint shader, std::string const& name, vec3 const& value, bool expected)
	{
		assert_cgp(shader!=0, "Try to send uniform "+name+" to unspecified shader");
		GLint const location = glGetUniformLocation(shader, name.c_str()); opengl_check;
		if(check_location(location, name, shader, expected))
			glUniform3f(location, value.x,value.y, value.z); opengl_check;
	}
	void opengl_uniform(GLuint shader, std::string const& name, vec4 const& value, bool expected)
	{
		assert_cgp(shader!=0, "Try to send uniform "+name+" to unspecified shader");
		GLint const location = glGetUniformLocation(shader, name.c_str()); opengl_check;
		if(check_location(location, name, shader, expected))
			glUniform4f(location, value.x,value.y, value.z, value.w); opengl_check;
	}
	void opengl_uniform(GLuint shader, std::string const& name, float x, float y, float z, bool expected)
	{
		assert_cgp(shader!=0, "Try to send uniform "+name+" to unspecified shader");
		GLint const location = glGetUniformLocation(shader, name.c_str()); opengl_check;
		if(check_location(location, name, shader, expected))
			glUniform3f(location, x, y, z);  opengl_check;
	}
	void opengl_uniform(GLuint shader, std::string const& name, float x, float y, float z, float w, bool expected)
	{
		assert_cgp(shader!=0, "Try to send uniform "+name+" to unspecified shader");
		GLint const location = glGetUniformLocation(shader, name.c_str()); opengl_check;
		if(check_location(location, name, shader, expected))
			glUniform4f(location, x, y, z, w);  opengl_check;
	}
	void opengl_uniform(GLuint shader, std::string const& name, mat4 const& m, bool expected)
	{
		assert_cgp(shader!=0, "Try to send uniform "+name+" to unspecified shader");
		GLint const location = glGetUniformLocation(shader, name.c_str()); opengl_check;
		if(check_location(location, name, shader, expected))
			glUniformMatrix4fv(location, 1, GL_TRUE, ptr(m));  opengl_check;
	}
	void opengl_uniform(GLuint shader, std::string const& name, mat3 const& m, bool expected)
	{
		assert_cgp(shader!=0, "Try to send uniform "+name+" to unspecified shader");
		GLint const location = glGetUniformLocation(shader, name.c_str()); opengl_check;
		if(check_location(location, name, shader, expected))
			glUniformMatrix3fv(location, 1, GL_TRUE, ptr(m)); opengl_check;
	}

}