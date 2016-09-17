/*! \file view.c
 *
 * \brief This file defines the viewer operations.
 *
 * \author John Reppy
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "view.hxx"
#include "map-cell.hxx"
#include "buffer-cache.hxx"
#include "texture-cache.hxx"

static void Error (int err, const char *msg);
static GLFWwindow *InitGLFW (int wid, int ht, const char *title);

// animation time step (100Hz)
#define TIME_STEP	0.001

/***** class View member functions *****/

View::View (Map *map)
    : _map(map), _errorLimit(2.0), _isVis(true), _window(nullptr), _wireframe(true),
      _bCache(new BufferCache()), _tCache(new TextureCache())
{
}

void View::Init (int wid, int ht)
{
    this->_window = InitGLFW(wid, ht, this->_map->Name().c_str());

  // attach the view to the window so we can get it from callbacks
    glfwSetWindowUserPointer (this->_window, this);

  // Compute the bounding box for the entire map
    this->_mapBBox = cs237::AABBd(
	cs237::vec3d(0.0, double(this->_map->MinElevation()), 0.0),
	cs237::vec3d(
	    double(this->_map->hScale()) * double(this->_map->Width()),
	    double(this->_map->MaxElevation()),
	    double(this->_map->hScale()) * double(this->_map->Height())));

  // Place the viewer in the center of cell(0,0), just above the
  // cell's bounding box.
    cs237::AABBd bb = this->_map->Cell(0,0)->Tile(0).BBox();
    cs237::vec3d pos = bb.center();
    pos.y = bb.maxY() + 0.01 * (bb.maxX() - bb.minX());

  // The camera's direction is toward the bulk of the terrain
    cs237::vec3d  at;
    if ((this->_map->nRows() == 1) && (this->_map->nCols() == 1)) {
	at = pos + cs237::vec3d(1.0, -0.25, 1.0);
    }
    else {
	at = pos + cs237::vec3d(double(this->_map->nCols()-1), 0.0, double(this->_map->nRows()-1));
    }
    this->_cam.move(pos, at, cs237::vec3d(0.0, 1.0, 0.0));

  // set the FOV and near/far planes
    this->_cam.setFOV (60.0);
    this->_cam.setNearFar (10.0, 1.5 * double(this->_map->CellWidth()) * double(this->_map->hScale()));
    this->Resize (wid, ht);

  // initialize shaders
    /* YOUR CODE HERE */

    /* ADDITIONAL INITIALIZATION */

  // initialize animation state
    this->_lastStep =
    this->_lastFrameTime = glfwGetTime();

}

#define SIN_ONE_DEGREE	0.0174524064373f
#define COS_ONE_DEGREE	0.999847695156f
#define SQRT_2 1.41421356237f

void View::HandleKey (int key, int scancode, int action, int mods)
{

    switch (key) {
      case GLFW_KEY_ESCAPE:
      case GLFW_KEY_Q:
	if (mods == 0)
	    glfwSetWindowShouldClose (this->_window, true);
	break;
      case GLFW_KEY_W: // toggle wireframe mode
	this->_wireframe = !this->_wireframe;
	break;
      case GLFW_KEY_EQUAL:
	if (mods == GLFW_MOD_SHIFT) { // shift+'=' is '+'
	  // decrease error tolerance
	    if (this->_errorLimit > 0.5)
		this->_errorLimit /= SQRT_2;
	}
	break;
      case GLFW_KEY_KP_ADD:  // keypad '+'
	if (mods == 0) {
	  // decrease error tolerance
	    if (this->_errorLimit > 0.5)
		this->_errorLimit /= SQRT_2;
	}
	break;
      case GLFW_KEY_MINUS:
	if (mods == 0) {
	  // increase error tolerance
	    this->_errorLimit *= SQRT_2;
	}
	break;
      case GLFW_KEY_KP_SUBTRACT:  // keypad '-'
	if (mods == 0) {
	  // increase error tolerance
	    this->_errorLimit *= SQRT_2;
	}
	break;
      default: // ignore all other keys
	return;
    }

//std::cout << "after handleKey: " << this->_cam << std::endl;

}

void View::HandleMouseEnter (bool entered)
{
}

void View::HandleMouseMove (double x, double y)
{
}

void View::HandleMouseButton (int button, int action, int mods)
{
}

void View::Animate ()
{
    double now = glfwGetTime();
    double dt = now - this->_lastStep;
    if (dt >= TIME_STEP) {
	this->_lastStep = now;
	/* ANIMATE STUFF */
    }

}

void View::Resize (int wid, int ht)
{
    glfwGetFramebufferSize (this->_window, &this->_fbWid, &this->_fbHt);
    glViewport(0, 0 , this->_fbWid, this->_fbHt);

  // default error limit is 1%
    this->_errorLimit = float(this->_fbHt) / 100.0f;

    this->_cam.setViewport (this->_fbWid, this->_fbHt);

}

/***** Local utility functions *****/

static void Error (int err, const char *msg)
{
    std::cerr << "[GLFW ERROR " << err << "] " << msg << "\n" << std::endl;
}

static GLFWwindow *InitGLFW (int wid, int ht, const char *title)
{
    glfwSetErrorCallback (Error);

    glfwInit ();

  // Check the GLFW version
    {
	int major;
	glfwGetVersion (&major, NULL, NULL);
	if (major < 3) {
	    std::cerr << "CS237 requires GLFW 3.0 or later\n" << std::endl;
	    exit (EXIT_FAILURE);
	}
    }

    glfwWindowHint (GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(wid, ht, title, NULL, NULL);
    if (window == nullptr) {
	exit (EXIT_FAILURE);
    }

    glfwMakeContextCurrent (window);

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

    return window;
}
