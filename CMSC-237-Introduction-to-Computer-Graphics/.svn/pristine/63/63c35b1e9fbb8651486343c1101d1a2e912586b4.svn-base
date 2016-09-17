/*! \file floor.cxx
 *
 * \author Lamont Samuels
 */

/* CMSC23700 Lab 4 sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 Lamont Samuels
 * All rights reserved.
 */

#include "floor.hxx"
#include "wall.hxx"

/* The vertices for the cube */ 
    /*! the five visible walls of the box */  
Wall wall = 
    { 
        cs237::vec3f{  0.0f, 1.0f,  0.0f },
        {cs237::vec3f{ -1.0f,  0.0f, -1.0f },
        cs237::vec3f{  1.0f,  0.0f, -1.0f },
        cs237::vec3f{  1.0f,  0.0f,  1.0f },
        cs237::vec3f{ -1.0f,  0.0f,  1.0f }}, 
        {   cs237::vec2f{  0.0f,  0.0f}, 
            cs237::vec2f{  1.0f,  0.0f}, 
            cs237::vec2f{  1.0f,  1.0f}, 
            cs237::vec2f{  0.0f,  1.0f}}
    }; 
Floor::Floor()
{
    this->modelMat = cs237::translate(cs237::vec3f(0.0,-1.0,0.0)); 
    this->modelMat = cs237::scale( this->modelMat, cs237::vec3f(3.5f,0.0f,3.5f));
    this->shouldTexturize = false; 
    this->color = LIGHT_GREEN_COLOR; 

    //! initialize the vertex data buffers for the floor
    const uint32_t indices[4] = {0, 1, 2, 3};
    Mesh * mesh = new Mesh(GL_TRIANGLE_FAN);
    mesh->LoadVertices(4,wall.corners);
    mesh->LoadIndices(4,indices);
    cs237::vec3f normals[4] = {
            wall.norm, 
            wall.norm, 
            wall.norm, 
            wall.norm
    };
    mesh->LoadNormals(4, normals);
    mesh->LoadTexCoords(4,wall.texCoords); 
    this->image = image; 
    this->texture = texture; 
    this->meshes.push_back(mesh); 
}
//! initialize the element array for the mesh
void Floor::Draw()
{
    for (auto it = meshes.begin();  it != meshes.end();  it++) {
        Mesh * mesh = *it;
        mesh->Draw();  
    }
}