 /*! \file cube.cxx
 *
 * \brief this file contains function definitions for creating and rendering a triangle
 *
 * \author Lamont Samuels 
 */ 

#include "triangle.hxx" 

void Triangle::Init() { 

    ///generate the vertex array object for this mesh (i.e. the simple triangle)
	CS237_CHECK(glGenVertexArrays (1,&(this->vaoId)));
	CS237_CHECK(glBindVertexArray (this->vaoId));
	    
	//Generate a name for the vertices VBO 
	CS237_CHECK(glGenBuffers(1,&(this->vbufId))); 

	//Bind the vertices buffer to the context using the GL_ARRAY_BUFFER target 
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,this->vbufId)); 
	  
	// Specify the amount of storage we want to use for the vertices buffer 
	CS237_CHECK(glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW));

}
void Triangle::Render(){

	//Bind the meshes VAO id to get the state information 
	CS237_CHECK(glBindVertexArray (vaoId));

	//Bind our VBO (vertex buffer object) to the GL_ARRAY_BUFFER target 
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,vbufId)); 

	//Describe the data that will fill each vertex attribute. In this case, it will be positions and colors. 
	CS237_CHECK(glVertexAttribPointer(posLoc,3,GL_FLOAT,GL_FALSE,sizeof(vertices[0]),(GLvoid *)0));
	CS237_CHECK(glVertexAttribPointer(colorLoc,4,GL_FLOAT,GL_FALSE,sizeof(vertices[0]),(GLvoid *)sizeof(vertices[0].pos))); 

	//Enable our vertex attributes using the data from the GL_ARRAY_BUFFER 
	CS237_CHECK(glEnableVertexAttribArray(posLoc)); 
	CS237_CHECK(glEnableVertexAttribArray(colorLoc)); 

	//Render the triangle 
	CS237_CHECK(glDrawArrays(GL_TRIANGLES,0,3));

}