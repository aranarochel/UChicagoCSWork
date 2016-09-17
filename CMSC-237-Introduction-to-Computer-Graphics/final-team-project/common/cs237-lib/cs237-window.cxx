/*! \file cs237-window.cxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"

namespace cs237 {

  // wrapper function for Refresh callback
    static void RefreshCB (GLFWwindow *win)
    {
	Window *winObj = static_cast<Window *>(glfwGetWindowUserPointer (win));
	winObj->Refresh ();
    }

  // wrapper function for Reshape callback
    static void ReshapeCB (GLFWwindow *win, int wid, int ht)
    {
	Window *winObj = static_cast<Window *>(glfwGetWindowUserPointer (win));
	winObj->Reshape (wid, ht);
    }

  // wrapper function for Reshape callback
    static void IconifyCB (GLFWwindow *win, int iconified)
    {
	Window *winObj = static_cast<Window *>(glfwGetWindowUserPointer (win));
	winObj->Iconify (iconified != 0);
    }

  // helper function for setting the viewport.
    static void SetViewport (GLFWwindow *win)
    {
	int fbWid, fbHt;
	glfwGetFramebufferSize (win, &fbWid, &fbHt);
	glViewport(0, 0 , fbWid, fbHt);
    }

    Window::~Window ()
    {
	glfwDestroyWindow (this->_win);
    }

  //! Refresh the contents of the window.  This method is also invoked
  //! on Refresh events.
    void Window::Refresh ()
    {
	if (this->_isVis) {
	    GLFWwindow *oldCxt = glfwGetCurrentContext();

	    glfwMakeContextCurrent (this->_win);
	    this->Draw();
	    glfwSwapBuffers (this->_win);

	    glfwMakeContextCurrent (oldCxt);
	}
    }

    Window::Window (std::string title, int wid, int ht, bool resizable, GLFWwindow *share)
    {
	glfwWindowHint (GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(wid, ht, title.c_str(), NULL, share);
	if (window == nullptr)
	    exit (EXIT_FAILURE);

      // set the user data for the window to this object
	glfwSetWindowUserPointer (window, this);

	glfwMakeContextCurrent (window);

	SetViewport (window);

      // set up callbacks
	glfwSetWindowRefreshCallback (window, RefreshCB);
	glfwSetWindowSizeCallback (window, ReshapeCB);
	glfwSetWindowIconifyCallback (window, IconifyCB);

	this->_win = window;
	this->_wid = wid;
	this->_ht = ht;
	this->_isVis = true;

    }

  //! method invoked on Reshape events.  It resets the viewport
    void Window::Reshape (int wid, int ht)
    {
	this->_wid = wid;
	this->_ht = ht;
	SetViewport (this->_win);

      // reset the projection matrix
	this->SetProjectionMatrix();
    }

  //! method invoked on Iconify events.
    void Window::Iconify (bool iconified)
    {
	this->_isVis = !iconified;
    }

} // namespace cs237
