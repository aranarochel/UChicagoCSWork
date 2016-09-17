 /*! \file vertex.hxx
 *
 * \brief this file contains the vertex definition and colors for the Cube classes. 
 *
 * \author Lamont Samuels 
 */
#ifndef _VERTEX_HXX_
#define _VERTEX_HXX_ 

#include "cs237.hxx" 


#define RED_COLOR  cs237::color4f(1.0f, 0.0f, 0.0f, 1.0f)  		/*!< Red color */ 
#define GREEN_COLOR cs237::color4f(0.0f, 1.0f, 0.0f, 1.0f)		/*!< Green color */
#define YELLOW_COLOR	cs237::color4f(1.0f, 1.0f, 0.0f, 1.0f)	/*!< Yellow color */
#define BLUE_COLOR cs237::color4f(0.0f, 0.0f, 1.0f, 1.0f)		/*!< Blue color */
#define SKY_BLUE_COLOR cs237::color4f(0.0f, 1.0f, 1.0f, 1.0f)	/*!< Sky Blue color */
#define PINK_COLOR cs237::color4f(1.0f, 0.0f, 0.5f, 1.0f)		/*!< Pink */ 

/*! \breif a struct for representing a Vertex */ 
typedef struct
{
	cs237::vec3f pos;		/*!< the position of a vertex in world space */ 
	cs237::color4f color;	/*!< the color of the vertex */ 
}Vertex_t;

#endif // !_VERTEX_HXX_  