/*! \file main.c
 *
 * \brief this file contains the main function and the GLFW callbacks.
 *
 * \author John Reppy
 * \author Lamont Samuels
 */

/* CMSC23700 Lab 2 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr) & Lamont Samuels
 * All rights reserved.
 */

#include "cs237.hxx"
#include "view.hxx"

/*! \brief Set the viewport to the current size of the framebufffer.
 *         We use the framebuffer size instead of the window size, because it
 *         is different on Apple's retina displays.
 */
static void SetViewport (GLFWwindow *win)
{
    int fbWid, fbHt;
    glfwGetFramebufferSize (win, &fbWid, &fbHt);
    glViewport(0, 0 , fbWid, fbHt);

}

/***** Callback functions *****/

static void Error (int err, const char *msg)
{
    std::cerr << "[GLFW ERROR " << err << "] " << msg << std::endl;
}

/*! \brief Run the simulation and then redraw the animation.
 */
void Display (GLFWwindow *win)
{
    View *view = (View *)glfwGetWindowUserPointer(win);

  /* avoid drawing when we are invisible */
    if (! view->isVis)
	return;

    view->Render ();
    glfwSwapBuffers (win);

    view->needsRedraw = false;

} /* end of Display */

/*! \brief Window resize callback.
 *  \param wid the new width of the window.
 *  \param ht the new height of the window. 
 */
void Reshape (GLFWwindow *win, int wid, int ht)
{
    View *view = (View *)glfwGetWindowUserPointer(win);

    view->wid = wid;
    view->ht = ht;

  /* set the viewport */
    SetViewport (win);

  /* recompute the projection matrix */
    view->InitProjMatrix ();

} /* end of Reshape */
/*! \brief Keyboard-event callback.
 *  \param win The window receiving the event
 *  \param key key token
 *  \param scancode unique scancode for physical key (ignored)
 *  \param action PRESS, REPEAT, or RELEASE
 *  \param mods modifier bits
 */
void Key (GLFWwindow *win, int key, int scancode, int action, int mods)
{
  // ignore releases, control keys, command keys, etc.

    GLfloat cameraSpeed = 3.0f;

    if ((action != GLFW_RELEASE) || (mods & (GLFW_MOD_CONTROL|GLFW_MOD_ALT|GLFW_MOD_SUPER))) {
		
		View *view = (View *)glfwGetWindowUserPointer(win);

		switch (key) {
	  		case GLFW_KEY_Q:  // 'q' or 'Q' ==> quit
	    		glfwSetWindowShouldClose (win, true);
	    	break;
	  		case GLFW_KEY_LEFT:
	  		 	view->angle = view->angle - cameraSpeed;
	  		 	view->MoveAround(); 
                view->needsRedraw = true;
	    	break;
	  		case GLFW_KEY_RIGHT:
                view->angle = view->angle + cameraSpeed;
	  		 	view->MoveAround(); 
                view->needsRedraw = true;
	    	break;

	  		default: // ignore all other keys
	    		return;
		}
    }
} /* end of Key */

/*! \brief Visibility-change callback: sets the visibility state of the view.
 *  \param state the current state of the window; GL_TRUE for iconified, GL_FALSE otherwise.
 */
void Visible (GLFWwindow *win, int state)
{
    View *view = (View *)glfwGetWindowUserPointer(win);

    view->isVis = (state == GL_FALSE);

} /* end of Visible */

/*! \brief the main function for the program
 *  \param argc the count of arguments in \a argv
 *  \param argv the array of command-line arguments.
 */
int main(int argc, const char **argv)
{

    glfwSetErrorCallback (Error);

    glfwInit ();

  // Check the GLFW version
    {
	int major;
	glfwGetVersion (&major, NULL, NULL);
	if (major < 3) {
	    std::cerr << "CS237 requires GLFW 3.0 or later" << std::endl;
	    exit (EXIT_FAILURE);
	}
    }

    glfwWindowHint (GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(
	800, 600,
	"CMSC 23700: Lab 4", NULL, NULL);
    if (window == nullptr)
	exit (EXIT_FAILURE);

    glfwMakeContextCurrent (window);

    SetViewport (window);

  // Check the OpenGL version
    {
	GLint major, minor;
	glGetIntegerv (GL_MAJOR_VERSION, &major);
	glGetIntegerv (GL_MINOR_VERSION, &minor);
	if ((major < 4) || ((major == 4) && (minor < 1))) {
	    std::cerr << "CS237 requires OpenGL 4.1 or later; found " << major << "." << minor << std::endl;
	    exit (EXIT_FAILURE);
	}
    }

    View * view = new View(window,800,600);

  // NOTE: the shaders can ony be initialized after a GL context is in place.
    view->InitRenderers();
    view->InitProjMatrix();
    view->InitDepthBuffer(); 
    view->InitLightMatrices(); 
    view->InitModelViewMatrix ();

  // set up callbacks
    glfwSetWindowRefreshCallback (window, Display);
    glfwSetWindowSizeCallback (window, Reshape);
    glfwSetWindowIconifyCallback (window, Visible);
    glfwSetKeyCallback (window, Key);

    std::cout << "\n******* Basic Lab4 Controls *************\n"; 
    std::cout << "Left Arrow Key  - move counter-clockwise\n"; 
    std::cout << "Right Arrow Key - move clockwise\n"; 
    std::cout << "*****************************************\n\n"; 

    while (! glfwWindowShouldClose(window)) {
	if (view->needsRedraw) {
	    Display (window);
	}
	glfwWaitEvents ();
    }

    glfwTerminate ();

    return EXIT_SUCCESS;
    
} /* end of main */
