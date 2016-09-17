/*! \file view.hxx
 *
 * \author John Reppy
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _VIEW_HXX_
#define _VIEW_HXX_

#include "cs237.hxx"
#include "map.hxx"
#include "camera.hxx"
#include "map-cell.hxx"
#include "frustum.hxx"
#include "skybox.hxx"
#include "grass.hxx"
#include <vector>

struct ShaderInfo {
  cs237::ShaderProgram *_shader;
  int _mvMatLoc;
};

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

class View {
  public:

  //! construct a viewer for the map
    View (Map *map);

  //! return the view's window
    GLFWwindow *Window() const { return this->_window; }

  //! initialize the view (i.e., allocate its window)
    void Init (int wid, int ht);

  //! method to handle display of the view
    void Render ();

  //! animation method
    void Animate ();

  //! handle kwyboard input
    void HandleKey (int key, int scancode, int action, int mods);

  //! handle mouse entering/leaving the view's window
    void HandleMouseEnter (bool entered);

  //! handle mouse movement
    void HandleMouseMove (double x, double y);

  //! handle mouse buttons
  //! \param button the mouse button that was pressed
  //! \param action was the button pressed or released?
  //! \param mods   the state of the modifier keys
    void HandleMouseButton (int button, int action, int mods);

  //! handle resizing the view
    void Resize (int wid, int ht);

  //! handle visibility change
    void SetVisible (bool isVis) { this->_isVis = isVis; }

  //! should this view close?
    bool shouldClose () const { return glfwWindowShouldClose(this->_window); }

  //! is the view in wireframe mode?
    bool wireframeMode () const { return this->_wireframe; }
    bool flatshadingMode () const { return this->_flat; }
    bool lightingMode () const { return this->_lighting; }
    bool fogMode () const { return this->_fog; }
    bool horizonMode () const { return this->_horizon; }
    bool skyboxMode () const { return this->_skybox; }
    bool reppyMode () const { return this->_reppy; }
    bool sunMode () const { return this->_sun; }
    bool guiMode () const { return this->_gui; }
    bool grassMode () const { return this->_grass; }


  //! the view's current camera state
    class Camera const &Camera () const { return this->_cam; }
    class Camera &CameraC () const { return const_cast<class Camera &>(this->_cam); }

  //! the view's current error limit
    float ErrorLimit () const { return this->_errorLimit; }

  //! the cache of VAO objects for representing chunks
    class BufferCache *VAOCache () const { return this->_bCache; }

  //! the cache of OpenGL textures for the map tiles
    class TextureCache *TxtCache () const { return this->_tCache; }

    cs237::ShaderProgram * InitShader(std::string name);

    // additional state
    int wid;
    int ht;
    Map *map;
    cs237::ShaderProgram *_shader;
    cs237::ShaderProgram *_shaderL;
    cs237::ShaderProgram *_shaderH;
    cs237::ShaderProgram *_shaderS;
    cs237::ShaderProgram *_shaderG;
    cs237::ShaderProgram *_shaderInUse;
    cs237::ShaderProgram *_shaderSpeed;
    cs237::ShaderProgram *_shaderGUI;
    int currentTexture;
    float ScreenSpaceError(Tile const &tile);

    int calcCol(uint32_t id);
    int calcRow(uint32_t id);

    void FindVisibleTiles (class Tile &tile); 

    void getUniforms();
    void setUniforms(cs237::mat4x4f projection, const class Tile *tile);

    void camTransformation(class Camera &camera, float degrees, cs237::vec3f axis);

    void smoothLook(cs237::vec3f finalDirection);
    void smoothMove(cs237::vec3d moveTo,cs237::vec3d moveFrom);

    void RenderVisibleTiles(class Tile &tile);
    //frustum culling frustum
    GFrustum *gfrustum;

    // infinite texutres
    class Cell * FindCameraCell();
    class Cell * lastVisitedCell;
    bool isSimulatedCell;
    int renderCount;
    int texCount;

    // skybox
    class SkyBox *skybox;

    // grass
    class Grass *grass;
    int grassID;

    //horizon curvature
    float curvatureValue;
    float isSpherical;

    //mouse movement
    double oldMouseX;
    double oldMouseY;
    bool leftMouseDown, rightMouseDown;

    //right click to move
    bool shouldMove;
    cs237::vec3d moveTo;    
    cs237::vec3d moveFrom;    
    cs237::vec3d moveDir;    

    //smooth turning
    bool turningCamera;
    cs237::vec3f smoothLookDirection;
    float smoothLookAngle;
    cs237::vec3f smoothLookFrom;
    cs237::vec3f smoothLookTo;

    //sun movement
    cs237::vec3f SunDirection;
    cs237::color3f SunIntensity;
    cs237::color3f AmbientIntensity;
    float night;


  private:
    Map		*_map;		//!< the map being rendered
    class Camera _cam;		//!< tracks viewer position, etc.
    float	_errorLimit;	//!< screen-space error limit
    bool	_isVis;		//!< true when this window is visible
    GLFWwindow	*_window;	//!< the main window 
    int		_fbWid;		//!< current framebuffer width
    int		_fbHt;		//!< current framebuffer height
    bool	_wireframe;	//!< true if we are rendering the wireframe
    bool  _flat;
    bool  _lighting;
    bool  _fog;
    bool  _horizon;
    bool  _skybox;
    bool  _reppy;
    bool  _grass;
    bool _sun;
    bool _gui;
    bool _shiftDown;
    double	_lastFrameTime;	//!< time of last frame
    double	_lastStep;	//!< time of last animation step
    cs237::AABBd _mapBBox;	//!< a bounding box around the entire map

  // resource management
    class BufferCache	*_bCache;	//! cache of OpenGL VAO objects used for chunks
    class TextureCache	*_tCache;	//! cache of OpenGL textures

  /* ADDITIONAL STATE HERE */
    GLint modelViewMat;
    GLint projection;
    GLint Vcolor;
    GLint hasTexture;
    GLint colorTexture;
    GLint normalTexture;
    GLint skyTexture;
    GLint tileWidth;
    GLint direction;
    GLint ambient;
    GLint intensity;
    GLint NWCorner;
    GLint hasFog;
    GLint fogFactors;
    GLint camPos;
    GLint modelMatLoc;
    GLint tau;
    GLint vertexError;
    //horizon
    GLint isSphericalLoc;
    GLint CurvatureLoc;
    //skybox
    GLint viewSky;
    GLint projSky;

};

#endif // !_VIEW_HXX_
