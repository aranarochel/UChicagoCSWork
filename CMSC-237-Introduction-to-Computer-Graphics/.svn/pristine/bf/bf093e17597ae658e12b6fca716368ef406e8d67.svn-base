/*! \file view.hxx
 *
 * Handles functions for window and rendering objects 
 * \author Lamont Samuels 
 */
#ifndef _VIEW_HXX_
#define _VIEW_HXX_

#include "cs237.hxx"
#include "triangle.hxx" 

struct View 
{
		cs237::ShaderProgram  * shader;			//!< the shader program for triangle 
		cs237::vec3f camPos;   					//!< camera position in world space 
    	cs237::vec3f camDir;					//!< camera direction in world space
		cs237::vec3f camUp;    					//!< camera up vector in world space 
		
		cs237::mat4f camera; 					//!< the camera matrix 
		cs237::mat4f projectionMat; 			//!< the projection matrix  
			
		GLint cameraLoc; 					    /*!< the uniform location for camera matrix in the shaders*/ 
		GLint projectionLoc; 					/*!< the uniform location for project view matrix in the shaders*/ 
		
		Triangle * tri;							/*! \brief a triangle to render to the screen */ 
		
		int width, height; 						//!< window dimensions
		
		/*! \brief Initialize the view */ 
		void Init(int w, int h); 					

		/*! \brief init triangle mesh */
		void InitTriangle();  

		/*! \brief compiles the shaders */ 
		void InitShaders(); 
		  
  		/*! \brief initialize the projection matrix based on the view state. */
    	void InitProjMatrix ();

  		/*! \brief initialize the camera matrix based on the view state. */
    	void InitCamera ();

		/*! \brief renders the triangle to the screen */
		void Render(); 

}; 
#endif /* !_VIEW_HXX_*/ 