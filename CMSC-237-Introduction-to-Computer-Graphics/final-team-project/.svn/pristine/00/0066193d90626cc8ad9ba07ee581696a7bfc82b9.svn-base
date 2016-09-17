/* Grass source file */
// This file uses some code from http://learnopengl.com/#!Advanced-OpenGL/Blending

#include "grass.hxx"
#include <vector>

const GLint GrassCoordAttr = 0;
const GLint GrassTexAttr = 1;

GLfloat grassVertices[] = {
        // Positions         // Texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };


/***** class Grass member functions *****/
Grass::Grass()
{
	GLuint id;

	CS237_CHECK( glGenVertexArrays (1, &(this->vaoId)));
	CS237_CHECK( glGenBuffers (1, &id));
	CS237_CHECK( glBindVertexArray (this->vaoId));

	// vertex buffer initialization
	this->vbufId = id;
	CS237_CHECK( glBindBuffer(GL_ARRAY_BUFFER, this->vbufId));
	CS237_CHECK( glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), &grassVertices, GL_STATIC_DRAW));
	CS237_CHECK( glVertexAttribPointer (GrassCoordAttr, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0));
	CS237_CHECK( glEnableVertexAttribArray (GrassCoordAttr) );
	CS237_CHECK( glVertexAttribPointer ( GrassTexAttr, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat))));
	CS237_CHECK( glEnableVertexAttribArray (GrassTexAttr) );

	// unbind the vbo and vao
	CS237_CHECK( glBindVertexArray(0));
	CS237_CHECK( glBindBuffer( GL_ARRAY_BUFFER, 0) );
}

Grass::~Grass()
{ }

void Grass::pushVertices (cs237::vec3f position)
{
	this->grass.push_back(position);	
}

void Grass::loadGrass( View *view)
{
	// create the grass texture

	cs237::image2d *image = new cs237::image2d(grasses[view->grassID], false);
	this->texture = new cs237::texture2D(GL_TEXTURE_2D, image);
	this->texture->Bind();

	this->texture->Parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	this->texture->Parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	this->texture->Parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	this->texture->Parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void Grass::Bind()
{
	this->texture->Bind();
}

void Grass::Draw(cs237::ShaderProgram *_shaderG, View *view)
{
	glBindVertexArray(this->vaoId);
	
	for(int i = 0; i < this->grass.size(); i++)
	{
		cs237::mat4x4f model = cs237::translate(grass[i]) * cs237::rotate(cs237::mat4f(1.0f), 90.0f, cs237::vec3f(0.0,(grass[i]).y,0.0));
		cs237::setUniform(_shaderG->UniformLocation("model"), model);
		cs237::setUniform(_shaderG->UniformLocation("ambientIntensity"), view->AmbientIntensity);
		cs237::setUniform(_shaderG->UniformLocation("sunIntensity"), view->SunIntensity);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	//glBindVertexArray(0);
}










