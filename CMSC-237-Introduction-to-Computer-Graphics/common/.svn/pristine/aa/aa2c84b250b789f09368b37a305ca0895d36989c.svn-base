/*! \file cs237-texture.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_TEXTURE_HXX_
#define _CS237_TEXTURE_HXX_

#ifndef _CS237_HXX_
#error "cs237-texture.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

	class texture_base {
	  public:
	    GLuint Id() const { return this->_obj->_id; }
	    GLenum Target() const { return this->_target; }

	  //! bind this texture as the current texture
	    void Bind ();

	  //! set texture parameter values
	    void Parameter (GLenum param, GLenum value);
	    void Parameter (GLenum param, GLint value);
	    void Parameter (GLenum param, color4f value);

	  protected:
	    texture_base (GLenum target);

	    GLenum _target;

	  private:
	    // wrapper class around GL Texture ID
	    struct Obj {
		GLuint	_id;
		Obj (GLuint id) : _id(id) { }
		~Obj ();
	    };

	    std::shared_ptr<Obj> _obj;
	};

    } /* namespace __detail */

  //! wrapper class for OpenGL 1D textures
    class texture1D : public __detail::texture_base {
      public:

      //! \brief create a 1D texture initialized to the given 2D image data.
      //! \param img specifies the initial texture image data
	texture1D (image1d *img);

      //! \brief create a 1D texture and allocate uninitialized storage for it.
      //! \param target specifies the target of the operation
      //! \param ifmt specifies the internal texture format
      //! \param wid specifies the texture width
      //! \param fmt specifies the image format
      //! \param ty specifies the type of the image elements
	texture1D (GLenum target, GLenum ifmt, GLenum wid, GLenum fmt, GLenum ty);

    };

  //! wrapper class for OpenGL 2D textures
    class texture2D : public __detail::texture_base {
      public:

      //! \brief create a 2D texture initialized to the given 2D image data.
      //! \param target specifies the target of the operation, which should be either
      //!        GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_1D_ARRAY,
      //!	 GL_PROXY_TEXTURE_1D_ARRAY, GL_TEXTURE_RECTANGLE, GL_PROXY_TEXTURE_RECTANGLE,
      //!	 or GL_PROXY_TEXTURE_CUBE_MAP
      //! \param img specifies the initial texture image data
	texture2D (GLenum target, image2d *img);

      //! \brief create a 2D texture and allocate uninitialized storage for it.
      //! \param target specifies the target of the operation
      //! \param ifmt specifies the internal texture format
      //! \param wid specifies the texture width
      //! \param ht specifies the texture height
      //! \param fmt specifies the image format
      //! \param ty specifies the type of the image elements
	texture2D (GLenum target, GLenum ifmt, GLenum wid, GLenum ht, GLenum fmt, GLenum ty);

    };

} /* namespace cs237 */

#endif /* !_CS237_TEXTURE_HXX_ */
