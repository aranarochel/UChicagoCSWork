/*! \file cs237-shader.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_SHADER_HXX_
#define _CS237_SHADER_HXX_

#ifndef _CS237_HXX_
#error "cs237-shader.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

	class Shader {
	  public:
	    GLuint Id() const { return this->_obj->_id; }

	  protected:
	    Shader (GLenum kind);

	    bool LoadFromFile (const char *file);
	    bool LoadFromString (const char **prog);

	  private:
	    // wrapper class around GL Shader ID
	    struct Obj {
		GLuint	_id;
		Obj (GLuint id) : _id(id) { }
		~Obj();
	    };

	    std::shared_ptr<Obj> _obj;
	    GLenum _kind;
	};

    } // namespace __detail

    class VertexShader : public __detail::Shader {
      public:

	/*! \brief load a vertex shader from a file.
	 *  \param file the name of the shader source file
	 *  \returns a pointer to the vertex shader.
	 *
	 * This function loads and compiles a vertex shader from a file.  If there is
	 * an error, it will print information to stderr and return 0.
	 */
	VertexShader (const char *file);

	/*! \brief define a shader from an array of strings
	 *  \param prog the shader source code represented as an array of char pointers.
	 *              The last element of the array should be nullptr.
	 *  \returns a pointer to the vertex shader.
	 *
	 * This function loads and compiles a vertex shader from a file.  If there is
	 * an error, it will print information to stderr and return 0.
	 */
	VertexShader (const char **prog);

    }; /* VertexShader */

    class GeometryShader : public __detail::Shader {
      public:

	/*! \brief load a fragment shader from a file.
	 *  \param file the name of the shader source file
	 *  \returns a pointer to the fragment shader.
	 *
	 * This function loads and compiles a geometry shader from a file.  If there is
	 * an error, it will print information to stderr and return 0.
	 */
	GeometryShader (const char *file);

	/*! \brief define a shader from an array of strings
	 *  \param prog the shader source code represented as an array of char pointers.
	 *              The last element of the array should be nullptr.
	 *  \returns a pointer to the fragment shader.
	 *
	 * This function loads and compiles a geometry shader from a file.  If there is
	 * an error, it will print information to stderr and return 0.
	 */
	GeometryShader (const char **prog);

    }; /* GeometryShader */

    class FragmentShader : public __detail::Shader {
      public:

	/*! \brief load a fragment shader from a file.
	 *  \param file the name of the shader source file
	 *  \returns a pointer to the fragment shader.
	 *
	 * This function loads and compiles a fragment shader from a file.  If there is
	 * an error, it will print information to stderr and return 0.
	 */
	FragmentShader (const char *file);

	/*! \brief define a shader from an array of strings
	 *  \param prog the shader source code represented as an array of char pointers.
	 *              The last element of the array should be nullptr.
	 *  \returns a pointer to the fragment shader.
	 *
	 * This function loads and compiles a fragment shader from a file.  If there is
	 * an error, it will print information to stderr and return 0.
	 */
	FragmentShader (const char **prog);

    }; /* FragmentShader */

    class ShaderProgram {
      public:

	ShaderProgram (VertexShader const &vsh, FragmentShader const &fsh);
	ShaderProgram (VertexShader const &vsh, GeometryShader const &gsh, FragmentShader const &fsh);

      /*! \brief use the given shader program for rendering.
       */
	void Use ();

      /*! \brief does the shader have the given attribute variable?
       *  \param name the name of the attribute
       *  \return true if the attribute is present, false otherwise
       */
	bool hasAttribute (const char *name);

      /*! \brief return the location of the given attribute in the given program.
       *  \param name the name of the attribute
       *  \return the location of the attribute
       */
	int AttributeLocation (const char *name);

      /*! \brief does the shader have the given uniform variable?
       *  \param name the name of the uniform
       *  \return true if the uniform is present, false otherwise
       */
	bool hasUniform (const char *name);

      /*! \brief return the location of the given uniform variable in the given program.
       *  \param name the name of the uniform variable
       *  \return the location of the uniform variable
       */
	int UniformLocation (const char *name);

      //! return the underlying OpenGL shader program ID
	GLuint Id () { return this->_obj->_id; }

      private:

      // wrapper class around GL Shader Program ID
	struct Obj {
	    GLuint	_id;
	    Obj (GLuint id) : _id(id) { }
	    ~Obj();
	};

	std::shared_ptr<Obj> _obj;

	void _Link ();

    }; // ShaderProgram

} /* namespace cs237 */

#endif /* !_CS237_SHADER_HXX_ */
