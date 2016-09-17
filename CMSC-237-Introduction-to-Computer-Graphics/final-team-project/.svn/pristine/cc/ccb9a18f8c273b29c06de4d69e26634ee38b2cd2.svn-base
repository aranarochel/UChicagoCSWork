/* The header file for the Grass class
 *
 */

 #ifndef _GRASS_HXX_
 #define _GRASS_HXX_

 #include "cs237.hxx"
 #include "map-cell.hxx"
 #include "view.hxx"
 #include <vector>

class Grass {
	public:

		Grass();	// initializes the vbo buffer
		~Grass();

		void pushVertices (cs237::vec3f position);	// populates the vector which contains the grass texture locations
		void loadGrass( View *view);	// load png file and create the grass texture
		void Bind();

		void Draw(cs237::ShaderProgram *_shaderG, View *view);	// render the grass objects
		cs237::texture2D *texture;
		std::vector<cs237::texture2D*> textures;
		std::vector<std::string> grasses = {"../data/grass/autumn-tree.png","../data/grass/tree1.png","../data/grass/tree.png","../data/teehee/jhr11.png"};

	private:
		std::vector<cs237::vec3f> grass;
		GLuint vaoId;
		GLuint vbufId;
};



#endif // !_GRASS_HXX_