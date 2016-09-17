/*! \file axes.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "axes.hxx"

static const char *vshSrc[] = {
	"#version 410\n"
	"uniform mat4 mvp;\n"
	"layout (location = 0) in vec3 coord;\n"
	"layout (location = 1) in vec3 color;\n"
	"out vec3 col;\n"
	"void main ()\n"
	"{\n"
	"  gl_Position = mvp * vec4(coord, 1);\n"
	"  col = color;\n"
	"}\n", 0};

static const char *fshSrc[] = {
	"#version 410\n"
	"in vec3 col;\n"
	"layout (location = 0) out vec4 fragColor;\n"
	"void main ()\n"
	"{\n"
	"  fragColor = vec4(col, 1);\n"
	"}\n", 0};

static cs237::ShaderProgram	*Shader = nullptr;
static int			UseCnt = 0;

static bool InitShader ()
{
    if (Shader == nullptr) {
	cs237::VertexShader vsh(vshSrc);
	cs237::FragmentShader fsh(fshSrc);

	Shader = new cs237::ShaderProgram (vsh, fsh);
	if (Shader == nullptr) {
	    return true;
	}
    }
    return false;
}

/***** class Axes methods *****/

Axes::Axes (float scale)
{
  // First we initialize the shader for drawing the axes
    if (InitShader()) {
	std::cerr << "Axes::Axes: cannot build shader\n" << std::endl;
	exit (1);
    }

    UseCnt++;
    Shader->Use();

    cs237::vec3f	verts[6] = {
	    cs237::vec3f(0,0,0), cs237::vec3f(1,0,0),		// X-axis
	    cs237::vec3f(0,0,0), cs237::vec3f(0,1,0),		// Y-axis
	    cs237::vec3f(0,0,0), cs237::vec3f(0,0,1),		// Z-axis
	};
    cs237::color3f	colors[6] = {
	    cs237::color3f(1, 0, 0), cs237::color3f(1, 0, 0),
	    cs237::color3f(0, 1, 0), cs237::color3f(0, 1, 0),
	    cs237::color3f(0.1, 0.1, 1), cs237::color3f(0.1, 0.1, 1),
	};

    if (scale != 1.0f) {
      // scale the vertices
	for (int i = 1;  i < 6;  i += 2) {
	    verts[i] = scale * verts[i];
	}
    }

  // initialize the VAO
    CS237_CHECK( glGenVertexArrays (1, &this->_vaoId) );
    CS237_CHECK( glBindVertexArray (this->_vaoId) );

    GLuint buffers[2];
    CS237_CHECK( glGenBuffers (2, buffers) );

  // intialize the vertex buffer
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, buffers[0]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (0) );

  // initialize the color buffer
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, buffers[1]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (1) );

}

Axes::~Axes ()
{
    if (--UseCnt == 0) {
	delete Shader;
	Shader = nullptr;
    }
    CS237_CHECK( glDeleteVertexArrays (1, &this->_vaoId) );
}

void Axes::Draw (cs237::mat4f projMat, cs237::mat4f viewMat)
{
    Shader->Use();
    cs237::setUniform (Shader->UniformLocation ("mvp"), projMat * viewMat);

    CS237_CHECK( glBindVertexArray (this->_vaoId) );
    CS237_CHECK( glDrawArrays (GL_LINES, 0, 6) );

}

/***** class Line methods *****/

static GLuint InitLineVAO (cs237::vec3f p1, cs237::vec3f p2, cs237::color3f color)
{
    cs237::vec3f	verts[2] = { p1, p2 };
    cs237::color3f	colors[2] = { color, color };
    GLuint		vaoId;

  // First we initialize the shader for drawing the axes
    if (InitShader()) {
	std::cerr << "Line::Line: cannot build shader\n" << std::endl;
	exit (1);
    }

    UseCnt++;
    Shader->Use();

  // initialize the VAO
    CS237_CHECK( glGenVertexArrays (1, &vaoId) );
    CS237_CHECK( glBindVertexArray (vaoId) );

    GLuint buffers[2];
    CS237_CHECK( glGenBuffers (2, buffers) );

  // intialize the vertex buffer
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, buffers[0]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (0) );

  // initialize the color buffer
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, buffers[1]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (1) );

    return vaoId;
}

Line::Line (cs237::vec3f dir, cs237::color3f color, float length)
    : _vaoId(InitLineVAO(cs237::vec3f(0,0,0), length*dir, color))
{ }

Line::Line (cs237::vec3f p1, cs237::vec3f p2, cs237::color3f color)
    : _vaoId(InitLineVAO(p1, p2, color))
{ }

Line::~Line ()
{
    if (--UseCnt == 0) {
	delete Shader;
	Shader = nullptr;
    }
    CS237_CHECK( glDeleteVertexArrays (1, &this->_vaoId) );
}

void Line::Draw (cs237::mat4f projMat, cs237::mat4f viewMat)
{
    Shader->Use();
    cs237::setUniform (Shader->UniformLocation ("mvp"), projMat * viewMat);

    CS237_CHECK( glBindVertexArray (this->_vaoId) );
    CS237_CHECK( glDrawArrays (GL_LINES, 0, 2) );

}
