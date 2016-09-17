/*! \file main.c
 *
 * \brief this file contains the main function and the GLFW callbacks.
 *
 * \author John Reppy
 * \author Lamont Samuels 
 */

/* CMSC23700 Lab 1 sample code (Fall 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr) & Lamont Samuels  (http://www.cs.uchicago.edu/~lamonts)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "view.hxx"
#include <unistd.h>

/* the viewer state */
View  * view = nullptr;

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
    std::cerr << "[GLFW ERROR " << err << "] " << msg << "\n" << std::endl;
}

/*! \brief Run the simulation and then redraw the animation.
 */
void Display (GLFWwindow *win)
{
    view->Render();
    glfwSwapBuffers (win);

} /* end of Display */

/*! \brief Window resize callback.
 *  \param wid the new width of the window.
 *  \param ht the new height of the window. 
 */
void Reshape (GLFWwindow *win, int wid, int ht)
{
    view->width = wid;
    view->height = ht;

   /* set the viewport */
    SetViewport (win);

  /* recompute the projection matrix */
    view->InitProjMatrix ();

} /* end of Reshape */

/*! \brief Keyboard-event callback.
 *  \param key The Unicode code point of the character.
 */
void Key (GLFWwindow *win, unsigned int key)
{

} /* end of Key */

/*! \brief Visibility-change callback: sets the visibility state of the view.
 *  \param state the current state of the window; GL_TRUE for iconified, GL_FALSE otherwise.
 */
void Visible (GLFWwindow *win, int state)
{

} /* end of Visible */


/*! \brief the main function for the program
 *  \param argc the count of arguments in \a argv
 *  \param argv the array of command-line arguments.
 */
int main(int argc, const char **argv)
{
    // # 1
    glfwSetErrorCallback (Error);

    // # 2
    glfwInit ();

    // #3 Check the GLFW version 
    {
      int major;
      glfwGetVersion (&major, NULL, NULL);
      if (major < 3) {
        std::cerr << "CS237 requires GLFW 3.0 or later\n" << std::endl;
       exit (EXIT_FAILURE);
      }
    }

    view = new struct View();
    view->Init(1280,720);

    glfwWindowHint (GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //#4
    GLFWwindow *window = glfwCreateWindow(view->width, view->height, "Simple Triangle", NULL, NULL);
    if (window == nullptr)
        exit (EXIT_FAILURE);

    //#5
    glfwMakeContextCurrent (window);

    //#6
    SetViewport (window);

    //Initialize the view members  
    view->InitShaders();
    view->InitProjMatrix();
    view->InitCamera();
    view->InitTriangle(); 

    // set up callbacks
    glfwSetWindowRefreshCallback (window, Display);
    glfwSetWindowSizeCallback (window, Reshape);
    glfwSetWindowIconifyCallback (window, Visible);
    glfwSetCharCallback (window, Key);


    while (! glfwWindowShouldClose(window)) {
          Display (window);
          glfwPollEvents ();
        }

        glfwTerminate();

        return EXIT_SUCCESS;

} /* end of main */
