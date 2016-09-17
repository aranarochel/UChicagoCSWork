 /*! \file cube.hxx
 *
 * \brief this file contains functions for creating and rendering a Cube
 *
 * \author Lamont Samuels 
 */
#ifndef _TRIANGLE_HXX_
#define _TRIANGLE_HXX_ 

#include "cs237.hxx" 
#include "vertex.hxx"

struct Triangle { 
		GLuint		vaoId;			//!< vertex-array-object ID for the simple triangle 
    	GLuint		vbufId;			//! vertex-buffer ID for the vertex data
    	
		Vertex_t vertices[3] = {  
								  {cs237::vec3f( 0.0f,  1.0f,   1.0f), SKY_BLUE_COLOR},  //0
								  {cs237::vec3f( -1.0f,  -1.0f,   1.0f), PINK_COLOR},  //2
								  {cs237::vec3f( 1.0f,  -1.0f,   1.0f), YELLOW_COLOR},  //1
							   };	  
		GLint		posLoc;			  //!< the vertex attribute location for the position 
    	GLint		colorLoc;		  //!< the vertex attribute location for the color  	
		void Init(); 				  //!< the default constructor 
		void Render(); 				  //!< renders the cube to the screen 
}; 

#endif /* _TRIANGLE_HXX_ */ 