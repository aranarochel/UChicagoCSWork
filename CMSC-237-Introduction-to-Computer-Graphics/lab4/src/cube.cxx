/*! \file cube.cxx
 *
 * \author Lamont Samuels
 */

/* CMSC23700 Lab 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 Lamont Samuels
 * All rights reserved.
 */

#include "cube.hxx"
#include "wall.hxx"

/* The vertices for the cube */ 
    /*! the five visible walls of the box */  
Wall walls[6] = {
    { 
        cs237::vec3f{  1.0f,  0.0f,  0.0f },  // norm
        {cs237::vec3f{ -1.0f,  1.0f, -1.0f },  // corner[0]
        cs237::vec3f{ -1.0f,  1.0f,  1.0f },
        cs237::vec3f{ -1.0f, -1.0f,  1.0f },
        cs237::vec3f{ -1.0f, -1.0f, -1.0f }},
        {cs237::vec2f{  0.333f,  0.666f},  // texture coordinates 
        cs237::vec2f{  0.666f,  0.666f},
        cs237::vec2f{  0.666f,  0.333f},
        cs237::vec2f{  0.334f,  0.333f}}

    },
    { 
        cs237::vec3f{ -1.0f,  0.0f,  0.0f },
        {cs237::vec3f{  1.0f,  1.0f,  1.0f },
        cs237::vec3f{  1.0f,  1.0f, -1.0f },
        cs237::vec3f{  1.0f, -1.0f, -1.0f },
        cs237::vec3f{  1.0f, -1.0f,  1.0f }}, 
        {cs237::vec2f{  0.666f,  0.666f}, //texture coordinates 
        cs237::vec2f{  1.0f,  0.666f},
        cs237::vec2f{  1.0f,  0.333f},
        cs237::vec2f{  0.666f,  0.333f}}

    },
    { 
        cs237::vec3f{  0.0f, -1.0f,  0.0f },
        {cs237::vec3f{ -1.0f,  1.0f, -1.0f },
        cs237::vec3f{  1.0f,  1.0f, -1.0f },
        cs237::vec3f{  1.0f,  1.0f,  1.0f },
        cs237::vec3f{ -1.0f,  1.0f,  1.0f }}, 
        {cs237::vec2f{  0.333f,  0.333f}, 
        cs237::vec2f{  0.666f,  0.333f},
        cs237::vec2f{  0.666f,  0.0f},
          cs237::vec2f{  0.333f,  0.0f}}
    },
    { 
        cs237::vec3f{  0.0f,  1.0f,  0.0f },
        {cs237::vec3f{ -1.0f, -1.0f,  1.0f },
        cs237::vec3f{  1.0f, -1.0f,  1.0f },
        cs237::vec3f{  1.0f, -1.0f, -1.0f },
        cs237::vec3f{ -1.0f, -1.0f, -1.0f }}, 
        {cs237::vec2f{  0.0f,  0.666f}, //texture coordinates 
        cs237::vec2f{  0.333f,  0.666f},
        cs237::vec2f{  0.333f,  0.333f}, 
        cs237::vec2f{  0.0f,  0.333f}}
    },
    { 
        cs237::vec3f{  0.0f,  0.0f,  1.0f },
        {cs237::vec3f{  1.0f,  1.0f, -1.0f },
        cs237::vec3f{ -1.0f,  1.0f, -1.0f },
        cs237::vec3f{ -1.0f, -1.0f, -1.0f },
        cs237::vec3f{  1.0f, -1.0f, -1.0f }}, 
        {cs237::vec2f{  0.0f,  0.333f}, // texture coordinates 
        cs237::vec2f{  0.333f,  0.333f},
        cs237::vec2f{  0.333f,  0.0f},
        cs237::vec2f{  0.0f,  0.0f}}

    },
    {
        cs237::vec3f{  0.0f,  0.0f, -1.0f },
        {cs237::vec3f{ -1.0f,  1.0f,  1.0f },
        cs237::vec3f{  1.0f,  1.0f,  1.0f },
        cs237::vec3f{  1.0f, -1.0f,  1.0f },
        cs237::vec3f{ -1.0f, -1.0f,  1.0f }}, 
        { cs237::vec2f{  0.666f,  0.333f},
            cs237::vec2f{  1.0f,  0.333f},
            cs237::vec2f{  1.0f,  0.0f}, 
            cs237::vec2f{  0.666f,  0.0f}}

    }
};
Cube::Cube()
{
    this->modelMat = cs237::translate(cs237::vec3f(0.0,0.0,0.0)); 
    this->shouldTexturize = true; 
    this->color = GREEN_COLOR;

    //! initialize the vertex data buffers for the cube
    cs237::image2d * image = new cs237::image2d("../data/crate-tex.png", true); 
    cs237::texture2D * texture = new cs237::texture2D(GL_TEXTURE_2D, image);  
    texture->Bind(); 
    texture->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    texture->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR); 

    /* convert the scene objects to meshes */
    const uint32_t indices[4] = {0, 1, 2, 3};
    for (int i = 0; i < 6; i++){
        Mesh * mesh = new Mesh(GL_TRIANGLE_FAN);
        mesh->LoadVertices(4,walls[i].corners);
        mesh->LoadIndices(4,indices);
        cs237::vec3f normals[4] = {
            walls[i].norm, 
            walls[i].norm, 
            walls[i].norm, 
            walls[i].norm
        };
        mesh->LoadNormals(4, normals);
        cs237::vec2f texCoords[4] = {
            cs237::vec2f{  0.0f,  0.0f}, 
            cs237::vec2f{  1.0f,  0.0f}, 
            cs237::vec2f{  1.0f,  1.0f}, 
            cs237::vec2f{  0.0f,  1.0f} 
        };
        mesh->LoadTexCoords(4,texCoords); 
        this->image = image; 
        this->texture = texture; 
        this->meshes.push_back(mesh); 
    }
}
//! initialize the element array for the mesh
void Cube::Draw()
{
    for (auto it = meshes.begin();  it != meshes.end();  it++) {
        Mesh * mesh = *it;
        mesh->Draw();  
    }
}