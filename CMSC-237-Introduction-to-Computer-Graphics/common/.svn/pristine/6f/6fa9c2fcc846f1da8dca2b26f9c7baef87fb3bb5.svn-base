/*! \file cs237-shader.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline functions for shader operations.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_SHADER_INL_
#define _CS237_SHADER_INL_

#ifndef _CS237_HXX_
#error "cs237-shader.inl should not be included directly"
#endif

namespace cs237 {

  /***** class ShaderProgram member functions *****/

    inline void ShaderProgram::Use ()
    {
	glUseProgram (this->Id());
    }

    inline bool ShaderProgram::hasAttribute (const char *name)
    {
	return (glGetAttribLocation (this->Id(), name) >= 0);
    }

    inline bool ShaderProgram::hasUniform (const char *name)
    {
	return (glGetUniformLocation (this->Id(), name) >= 0);
    }

  //! set the specified int uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v the integer value bound to the variable
    inline void setUniform (int loc, int const &v)
    {
	CS237_CHECK (glUniform1i (loc, v));
    }

  //! set the specified int2 uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v1 the first component of the vector bound to the variable
  //! \param v2 the second component of the vector bound to the variable
    inline void setUniform (int loc, int const &v1, int const &v2)
    {
	CS237_CHECK (glUniform2i (loc, v1, v2));
    }

  //! set the specified int3 uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v1 the first component of the vector bound to the variable
  //! \param v2 the second component of the vector bound to the variable
  //! \param v3 the third component of the vector bound to the variable
    inline void setUniform (int loc, int const &v1, int const &v2, int const &v3)
    {
	CS237_CHECK (glUniform3i (loc, v1, v2, v3));
    }

  //! set the specified int4 uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v1 the first component of the vector bound to the variable
  //! \param v2 the second component of the vector bound to the variable
  //! \param v3 the third component of the vector bound to the variable
  //! \param v4 the fourth component of the vector bound to the variable
    inline void setUniform (int loc, int const &v1, int const &v2, int const &v3, int const &v4)
    {
	CS237_CHECK (glUniform4i (loc, v1, v2, v3, v4));
    }

  //! set the specified unsigned int uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v the unsigned integer value bound to the variable
    inline void setUniform (int loc, unsigned int const &v)
    {
	CS237_CHECK( glUniform1ui (loc, v) );
    }

  //! set the specified uint2 uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v1 the first component of the vector bound to the variable
  //! \param v2 the second component of the vector bound to the variable
    inline void setUniform (int loc, unsigned int const &v1, unsigned int const &v2)
    {
	CS237_CHECK (glUniform2ui (loc, v1, v2));
    }

  //! set the specified uint3 uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v1 the first component of the vector bound to the variable
  //! \param v2 the second component of the vector bound to the variable
  //! \param v3 the third component of the vector bound to the variable
    inline void setUniform (int loc, unsigned int const &v1, unsigned int const &v2, unsigned int const &v3)
    {
	CS237_CHECK (glUniform3ui (loc, v1, v2, v3));
    }

  //! set the specified uint4 uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v1 the first component of the vector bound to the variable
  //! \param v2 the second component of the vector bound to the variable
  //! \param v3 the third component of the vector bound to the variable
  //! \param v4 the fourth component of the vector bound to the variable
    inline void setUniform (int loc, unsigned int const &v1, unsigned int const &v2, unsigned int const &v3, unsigned int const &v4)
    {
	CS237_CHECK (glUniform4ui (loc, v1, v2, v3, v4));
    }

  //! set the specified float uniform variable for current shader
  //! \param loc the location of the uniform variable
  //! \param v the float value bound to the variable
    inline void setUniform (int loc, float const &v)
    {
	CS237_CHECK( glUniform1f (loc, v) );
    }

    inline void setUniform (int loc, vec2f const &v)
    {
	CS237_CHECK( glUniform2fv (loc, 1, __detail::valuePtr(v)) );
    }

    inline void setUniform (int loc, vec3f const &v)
    {
	CS237_CHECK( glUniform3fv (loc, 1, __detail::valuePtr(v)) );
    }

    inline void setUniform (int loc, vec4f const &v)
    {
	CS237_CHECK( glUniform4fv (loc, 1, __detail::valuePtr(v)) );
    }

    inline void setUniform (int loc, mat2x2f const &v)
    {
	CS237_CHECK( glUniformMatrix2fv (loc, 1, GL_FALSE, __detail::valuePtr(v)) );
    }

    inline void setUniform (int loc, mat3x3f const &v)
    {
	CS237_CHECK( glUniformMatrix3fv (loc, 1, GL_FALSE, __detail::valuePtr(v)) );
    }

    inline void setUniform (int loc, mat4x4f const &v)
    {
	CS237_CHECK( glUniformMatrix4fv (loc, 1, GL_FALSE, __detail::valuePtr(v)) );
    }

    inline void setUniform (int loc, color3ub const &v)
    {
	CS237_CHECK( glUniform3fv (loc, 1, __detail::valuePtr(color3f(v))) );
    }

    inline void setUniform (int loc, color3f const &v)
    {
	CS237_CHECK( glUniform3fv (loc, 1, __detail::valuePtr(v)) );
    }

    inline void setUniform (int loc, color4ub const &v)
    {
	CS237_CHECK( glUniform4fv (loc, 1, __detail::valuePtr(color4f(v))) );
    }

    inline void setUniform (int loc, color4f const &v)
    {
	CS237_CHECK( glUniform4fv (loc, 1, __detail::valuePtr(v)) );
    }

  /***** functions to set individual attribute variables *****/

    inline void setAttribute (int loc, int const &v)
    {
	CS237_CHECK( glVertexAttribI1i (loc, v) );
    }

    inline void setAttribute (int loc, unsigned int const &v)
    {
	CS237_CHECK( glVertexAttribI1ui (loc, v) );
    }

    inline void setAttribute (int loc, float const &v)
    {
	CS237_CHECK( glVertexAttrib1f (loc, v) );
    }

    inline void setAttribute (int loc, vec2f const &v)
    {
	CS237_CHECK( glVertexAttrib2fv (loc, __detail::valuePtr(v)) );
    }

    inline void setAttribute (int loc, vec3f const &v)
    {
	CS237_CHECK( glVertexAttrib3fv (loc, __detail::valuePtr(v)) );
    }

    inline void setAttribute (int loc, vec4f const &v)
    {
	CS237_CHECK( glVertexAttrib4fv (loc, __detail::valuePtr(v)) );
    }

} /* namespace cs237 */

#endif /* !_CS237_SHADER_INL_ */
