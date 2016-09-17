/* SkyBox source file */
// This file uses some code from http://learnopengl.com/#!Advanced-OpenGL/Cubemaps

#include "skybox.hxx"
#include <vector>

const GLint SkyCoordAttr = 0;

GLfloat skyboxVertices[] = {
        // Positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
   
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


/***** class SkyBox member functions *****/
SkyBox::SkyBox()
{
	GLuint id;

	CS237_CHECK( glGenVertexArrays (1, &(this->vaoId)));
	CS237_CHECK( glGenBuffers (1, &id));
	CS237_CHECK( glBindVertexArray (this->vaoId));

	// vertex buffer initialization
	this->vbufId = id;
	CS237_CHECK( glBindBuffer(GL_ARRAY_BUFFER, this->vbufId));
	CS237_CHECK( glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW));
	CS237_CHECK( glVertexAttribPointer (SkyCoordAttr, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0));
	CS237_CHECK( glEnableVertexAttribArray (SkyCoordAttr) );

	// unbind the vbo and vao
	CS237_CHECK( glBindVertexArray(0));
	CS237_CHECK( glBindBuffer( GL_ARRAY_BUFFER, 0) );

}

SkyBox::~SkyBox()
{ }

void SkyBox::loadSkyBox(View *view)
{
	glGenTextures(1, &(this->skyTex));

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyTex);
	
	//initialize textures for each face of the cube
	if (!view->reppyMode())
	{
		cs237::image2d *imager = new cs237::image2d("../data/interstellar_skybox/xpos.png", false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, imager->format(), imager->width(), imager->height(), 0, imager->format(), imager->type(), imager->data());

		cs237::image2d *imagel = new cs237::image2d("../data/interstellar_skybox/xneg.png", false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, imagel->format(), imagel->width(), imagel->height(), 0, imagel->format(), imagel->type(), imagel->data());

		cs237::image2d *imageu = new cs237::image2d("../data/interstellar_skybox/ypos.png", false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, imageu->format(), imageu->width(), imageu->height(), 0, imageu->format(), imageu->type(), imageu->data());

		cs237::image2d *imaged = new cs237::image2d("../data/interstellar_skybox/yneg.png", false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, imaged->format(), imaged->width(), imaged->height(), 0, imaged->format(), imaged->type(), imaged->data());

		cs237::image2d *imagef = new cs237::image2d("../data/interstellar_skybox/zpos.png", false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, imager->format(), imagef->width(), imagef->height(), 0, imagef->format(), imagef->type(), imagef->data());

		cs237::image2d *imageb = new cs237::image2d("../data/interstellar_skybox/zneg.png", false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, imageb->format(), imageb->width(), imageb->height(), 0, imageb->format(), imageb->type(), imageb->data());
	}
	else
	{
	// REPPY MODE 
		std::cout<<"reppy mode engage\n";
	//initialize textures for each face of the cube
		cs237::image2d *imager = new cs237::image2d("../data/teehee/jhr11.png", false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, imager->format(), imager->width(), imager->height(), 0, imager->format(), imager->type(), imager->data());

		//cs237::image2d *imagel = new cs237::image2d("../data/.png", false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, imager->format(), imager->width(), imager->height(), 0, imager->format(), imager->type(), imager->data());

		cs237::image2d *imageu = new cs237::image2d("../data/teehee/lamonts.png", false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, imageu->format(), imageu->width(), imageu->height(), 0, imageu->format(), imageu->type(), imageu->data());

		//cs237::image2d *imaged = new cs237::image2d("../data/mudskipper/mudskipper_.png", false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, imager->format(), imager->width(), imager->height(), 0, imager->format(), imager->type(), imager->data());

		//cs237::image2d *imagef = new cs237::image2d("../data/mudskipper/mudskipper_front.png", false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, imager->format(), imager->width(), imager->height(), 0, imager->format(), imager->type(), imager->data());

		//cs237::image2d *imageb = new cs237::image2d("../data/mudskipper/mudskipper_back.png", false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, imager->format(), imager->width(), imager->height(), 0, imager->format(), imager->type(), imager->data());
	}
	// set cube map texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 2);

}

void SkyBox::Bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyTex);
}

void SkyBox::Draw()
{
	glBindVertexArray(this->vaoId);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}










