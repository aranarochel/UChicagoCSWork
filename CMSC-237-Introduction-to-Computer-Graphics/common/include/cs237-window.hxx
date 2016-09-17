/*! \file cs237-window.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_WINDOW_HXX_
#define _CS237_WINDOW_HXX_

#ifndef _CS237_HXX_
#error "c237-vec2.hxx should not be included directly"
#endif

namespace cs237 {

  //! abstract base class for simple GLFW windows used to view buffers, etc.
    class Window {
      public:

      //! destructor: it destroys the underlying GLFW window
	virtual ~Window ();

      //! Refresh the contents of the window.  This method is also invoked
      //! on Refresh events.
	void Refresh ();

      //! Hide the window
	void Hide ()
	{
	    glfwHideWindow (this->_win);
	    this->_isVis = false;
	}

      //! Show the window (a no-op if it is already visible)
	void Show ()
	{
	    glfwShowWindow (this->_win);
	    this->_isVis = true;
	}

      //! virtual draw method provided by derived classes to draw the contents of the
      //! window.  It is called by Refresh.
	virtual void Draw () = 0;

      //! virtual method for setting the projection matrix.  This method should
      //! be called once after the window is created.
	virtual void SetProjectionMatrix () = 0;

      //! method invoked on Reshape events.  It resets the viewport and the
      //! projection matrix (see SetProjectionMatrix)
	void Reshape (int wid, int ht);

      //! method invoked on Iconify events.
	void Iconify (bool iconified);

      protected:
      //! base-class constructor
      //! \param title the string to display in the title bar of the window
      //! \param wid the desired window width
      //! \param ht the desired window height
      //! \param resizable if true, then the window will be resizable
      //! \param share a pointer to a window to share resources with (default is nullptr)
      //!
      //! Note that after creating a Window object, the Window will be the current context
	Window (std::string title, int wid, int ht, bool resizable, GLFWwindow *share = nullptr);

	GLFWwindow	*_win;		//!< the underlying window
	int		_wid, _ht;	//!< window dimensions
	bool		_isVis;		//!< true, when the window is visible
    };

} // namespace cs237

#endif // !_CS237_WINDOW_HXX_
