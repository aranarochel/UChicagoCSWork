/* The header file for the skybox class
 *
 */

 #ifndef _SKYBOX_HXX_
 #define _SKYBOX_HXX_

 #include "cs237.hxx"
 #include "view.hxx"

class SkyBox {
	public:

		SkyBox();	// initializes the vbo buffer
		~SkyBox();

		void loadSkyBox(View *view);	// load png files and creates textures

		void Bind();

		void Draw();	// render the skybox

	private:
		GLuint vaoId;
		GLuint vbufId;
		GLuint skyTex;
};



#endif // !_SKYBOX_HXX_