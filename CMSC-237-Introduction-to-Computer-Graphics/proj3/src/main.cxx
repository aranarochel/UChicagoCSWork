/*! \file main.c
 *
 * \brief this file contains the main function and the GLFW callbacks.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "scene.hxx"
#include "view.hxx"



/* function that abstracts rotation matrix multiplication for each key
 *
 */
void meshTransformation(View *view, float degrees, cs237::vec3f axis)
{
  //create temporary vec4 vectors from the camera vectors
  // to facilitate transformation
  cs237::vec4f cPos = cs237::vec4f(view->camPos, 1.0f);
  cs237::vec4f cAt = cs237::vec4f(view->camAt,1.0f);
  cs237::vec4f cUp = cs237::vec4f(view->camUp,1.0f);
  
  // create rotation matrix
  cs237::mat4x4f T = cs237::rotate (degrees,axis);
  view->camAt = (cs237::vec3f)(T * cAt);
  view->camPos = (cs237::vec3f)(T * cPos);
  view->camUp = (cs237::vec3f)(T * cUp); 
}


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
    if ((action != GLFW_RELEASE) || (mods & (GLFW_MOD_CONTROL|GLFW_MOD_ALT|GLFW_MOD_SUPER))) {
	    View *view = (View *)glfwGetWindowUserPointer(win);

    	switch (key) {
    	  case GLFW_KEY_Q:  // 'q' or 'Q' ==> quit
    	    glfwSetWindowShouldClose (win, true);
    	    break;
    	  case GLFW_KEY_W:  // 'w' or 'W' ==> switch to wireframe mode
    	    view->needsRedraw = (view->mode != WIREFRAME);
    	    view->mode = WIREFRAME;
    	    break;
    	  case GLFW_KEY_F:  // 'f' or 'F' ==> switch to flat-shading mode
    	    view->needsRedraw = (view->mode != FLAT_SHADING);
    	    view->mode = FLAT_SHADING;
    	    break;
        case GLFW_KEY_D:  // 'd' or 'D' ==> switch to diffuse rendering mode
          view->needsRedraw = true;
          view->mode = LIGHTING;
          break;
        case GLFW_KEY_T:  // 't' or 'T' ==> switch to texture rendering mode
          view->needsRedraw = true;
          view->mode = TEXTURING;
          break;
        case GLFW_KEY_S:  // 's' or 'S' ==> switch to shadow rendering mode
          view->needsRedraw = true;
          view->mode = SHADOWS;
          break;
        case GLFW_KEY_KP_ADD: {
          view->needsRedraw = true;
          cs237::vec3f forward(0.0f,0.0f,1.0f);

          // set new camera position
          view->camPos = view->camPos + forward;
          view->InitModelViewMatrix ();
          }
          break;
        case GLFW_KEY_KP_SUBTRACT: {
          view->needsRedraw = true;
          cs237::vec3f backward(0.0f,0.0f,-1.0f);

          // set new camera position
          view->camPos = view->camPos + backward;
          view->InitModelViewMatrix ();
          }
          break;
        case GLFW_KEY_UP: { // this rotates the camera around the y axis
          view->needsRedraw = true;

          // x axis
          cs237::vec3f xaxis(1.0f,0.0f,0.0f);

          // apply rotation transformation to camera
          meshTransformation(view,5.0f,xaxis); 
          view->InitModelViewMatrix ();
          }
          break;
        case GLFW_KEY_DOWN: { // this rotates the camera around the y axis
          view->needsRedraw = true;
          
          // x axis
          cs237::vec3f xaxis(-1.0f,0.0f,0.0f);

          // apply rotation transformation to camera
          meshTransformation(view,5.0f,xaxis);
          view->InitModelViewMatrix ();
          }
          break;
        case GLFW_KEY_RIGHT: { // this rotates the camera around x axis
          view->needsRedraw = true;
          
          // y axis
          cs237::vec3f yaxis(0.0f,1.0f,0.0f);

          // apply rotation transformation to camera
          meshTransformation(view,5.0f,yaxis);
          view->InitModelViewMatrix ();
          }
          break;
        case GLFW_KEY_LEFT: { // this rotates the camera around x axis
          view->needsRedraw = true;
          
          // y axis
          cs237::vec3f yaxis(0.0f,-1.0f,0.0f);

          // apply rotation transformation to camera
          meshTransformation(view,5.0f,yaxis);
          view->InitModelViewMatrix ();
          }
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
  // first we load the scene file
    Scene scene;
    if (argc != 2) {
	std::cerr << "usage: proj2 <scene>" << std::endl;
	return EXIT_FAILURE;
    }
    else if (scene.Load(argv[1])) {
	return EXIT_FAILURE;
    }

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
	scene.Width(), scene.Height(),
	"CMSC 23700: Project 2", NULL, NULL);
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

    View *view = new View(scene, window);

  // NOTE: the shaders can ony be initialized after a GL context is in place.
    view->InitRenderers ();

    view->InitProjMatrix ();
    view->InitDepthBuffer(); 
    view->InitLightMatrices(); 
    view->InitModelViewMatrix ();

  // set up callbacks
    glfwSetWindowRefreshCallback (window, Display);
    glfwSetWindowSizeCallback (window, Reshape);
    glfwSetWindowIconifyCallback (window, Visible);
    glfwSetKeyCallback (window, Key);

    while (! glfwWindowShouldClose(window)) {
	if (view->needsRedraw) {
	    Display (window);
	}
	glfwWaitEvents ();
    }

    glfwTerminate ();

    return EXIT_SUCCESS;
    
} /* end of main */
