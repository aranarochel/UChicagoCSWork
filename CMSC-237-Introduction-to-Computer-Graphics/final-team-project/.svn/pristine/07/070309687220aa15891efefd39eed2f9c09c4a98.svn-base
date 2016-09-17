/*! \file view.c
 *
 * \brief This file defines the viewer operations.
 *
 * \author John Reppy
 */

/* CMSC23700 Final Project sample code (Autumn 2015)
 *
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "view.hxx"
#include "map-cell.hxx"
#include "buffer-cache.hxx"
#include "texture-cache.hxx"

 static cs237::color4ub MeshColor[8] = {
  cs237::color4ub{ 255, 255,   0, 255 },
  cs237::color4ub{ 255,   0, 255, 255 },
  cs237::color4ub{   0, 255, 255, 255 },
  cs237::color4ub{ 255, 128, 128, 255 },
  cs237::color4ub{ 128, 255, 128, 255 },
  cs237::color4ub{ 128, 128, 255, 255 },
  cs237::color4ub{ 255,   0,   0, 255 },
  cs237::color4ub{   0, 255,   0, 255 }
    };

void View::camTransformation(class Camera &camera, float degrees, cs237::vec3f axis)
{
  //create temporary vec4 vectors from the camera vectors
  // to facilitate transformation
  cs237::vec4f cPos = cs237::vec4f(cs237::vec3f(camera.position().x,camera.position().y,camera.position().z), 1.0f);
  cs237::vec4f cAt = cs237::vec4f(camera.direction(),1.0f);
  cs237::vec4f cUp = cs237::vec4f(camera.up(),1.0f);
  
  // create rotation matrix
  cs237::mat4x4f T = cs237::rotate (degrees,axis);
  cs237::vec3f new_pos = (cs237::vec3f)(T * cPos);
  cs237::vec3f new_at = (cs237::vec3f)(T * cAt);
  cs237::vec3f new_up = (cs237::vec3f)(T * cUp);

  camera.move(cs237::vec3d(new_pos.x,new_pos.y,new_pos.z), cs237::vec3d(new_at.x,new_at.y,new_at.z), cs237::vec3d(new_up.x,new_up.y,new_up.z));
}


static void Error (int err, const char *msg);
static GLFWwindow *InitGLFW (int wid, int ht, const char *title);

// animation time step (100Hz)
#define TIME_STEP	0.001

/***** class View member functions *****/

View::View (Map *map)
    : _map(map), _errorLimit(2.0), _isVis(true), _window(nullptr), _wireframe(true), _lighting(false), _skybox(false), _reppy(false), _grass(false),_sun(false),_gui(false),
      grassID(1), _bCache(new BufferCache()), _tCache(new TextureCache())
{
  this->map = map;
  renderCount = 0;
  texCount = 0;
}

cs237::ShaderProgram * View::InitShader(std::string name){
    std::string shaderPrefix = "../shaders/" + name;
    cs237::VertexShader vsh((shaderPrefix + ".vsh").c_str());
    cs237::FragmentShader fsh((shaderPrefix + ".fsh").c_str());
    cs237::ShaderProgram *shader = new cs237::ShaderProgram (vsh, fsh);
    if (shader == nullptr) {
      std::cerr << "Cannot build " << shaderPrefix << std::endl;
      exit (1);
    }
    return shader;
}

void View::Init (int wid, int ht)
{
    this->_window = InitGLFW(wid, ht, this->_map->Name().c_str());
    this->currentTexture = 0;
  // attach the view to the window so we can get it from callbacks
    glfwSetWindowUserPointer (this->_window, this);

  // Compute the bounding box for the entire map
    this->_mapBBox = cs237::AABBd(
    	cs237::vec3d(0.0, double(this->_map->MinElevation()), 0.0),
    	cs237::vec3d(
  	    double(this->_map->hScale()) * double(this->_map->Width()),
  	    double(this->_map->MaxElevation()),
  	    double(this->_map->hScale()) * double(this->_map->Height())));

  // Place the viewer in the center of cell(0,0), just above the
  // cell's bounding box.
    cs237::AABBd bb = this->_map->Cell(0,0)->Tile(0).BBox();
    cs237::vec3d pos = bb.center();
    pos.y = bb.maxY() + 0.01 * (bb.maxX() - bb.minX());

  // The camera's direction is toward the bulk of the terrain
    cs237::vec3d  at = pos + cs237::vec3d(1.0,0.0,0.0);
    /*
    if ((this->_map->nRows() == 1) && (this->_map->nCols() == 1)) {
      at = pos + cs237::vec3d(1.0, -0.25, 1.0);
    }
    else {
      at = pos + cs237::vec3d(double(this->_map->nCols()-1), 0.0, double(this->_map->nRows()-1));
    }
    */
    this->_cam.move(pos, at, cs237::vec3d(0.0, 1.0, 0.0));

  // set the FOV and near/far planes
    this->_cam.setFOV (60.0);
    this->_cam.setNearFar (10.0, /*1.5*/3.5 * double(this->_map->CellWidth()) * double(this->_map->hScale()));
    this->Resize (wid, ht);

  // initialize shaders
    // initialize wireframe/flat shading shader
    this->_shader = InitShader("shader");
    // lighting mode shader (do we even do this?)
    this->_shaderL = InitShader("shaderL");
    // skybox mode shader
    this->_shaderS = InitShader("shaderS");
    // horizon mode shader
    this->_shaderH = InitShader("shaderH");
    this->_shaderG = InitShader("shaderG");
    this->_shaderSpeed = InitShader("shaderSpeed");
    this->_shaderGUI = InitShader("shaderGUI");

    getUniforms();

    /* ADDITIONAL INITIALIZATION */
    this->wid = wid;
    this->ht = ht;
  // initialize animation state
    this->_lastStep =
    this->_lastFrameTime = glfwGetTime();

    for(int i = 0; i < map->nRows(); i++) {
      for(int j = 0; j < map->nCols(); j++) {
          map->Cell(i,j)->InitTextures(this);
      }
    }

    this->grass = new class Grass();
    grass->loadGrass(this);
    //grass->pushVertices();

    this->skybox = new class SkyBox();
    skybox->loadSkyBox(this);

    this->SunDirection = this->map->SunDirection();
    this->SunIntensity = this->map->SunIntensity();
    this->AmbientIntensity = this->map->AmbientIntensity();
    this->night = 1.0;

}

float angle(cs237::vec3f a, cs237::vec3f b)
{
  float dot = cs237::__detail::dot(a,b);
  float cross = cs237::__detail::cross(a,b).y ;
  return -acos(dot / (a.length() * b.length())) * (-1.0 * sgn(cross));
}

//calculate the screenspace error of a chunk
float View::ScreenSpaceError(Tile const &tile)
{
  float delta = (float) tile.Chunk()._maxError;
  float D = tile.BBox().distanceToPt(this->Camera().position());
  float rho = this->Camera().screenError(D, delta);
  return rho;
}

#define SIN_ONE_DEGREE	0.0174524064373f
#define COS_ONE_DEGREE	0.999847695156f
#define SQRT_2 1.41421356237f

void View::HandleKey (int key, int scancode, int action, int mods)
{
  cs237::vec3f camPos = cs237::toFloat(this->Camera().position());
  cs237::vec3f camDir = this->Camera().direction();
  cs237::vec3d camDird = cs237::toDouble(this->Camera().direction());
  cs237::vec3d camRight = cs237::__detail::cross(camDird,cs237::toDouble(this->Camera().up()));

  switch (key) {
  case GLFW_KEY_U:
      //right the camera
      this->CameraC().setUp(cs237::vec3f(0,1,0));
    break;
  case GLFW_KEY_1:
      //move to the origin
      this->CameraC().move(cs237::vec3d(0));
    break;
  case GLFW_KEY_ESCAPE:
      shouldMove = false;
      turningCamera = false;
    break;
  case GLFW_KEY_Q:
  	if (mods == 0)
  	    glfwSetWindowShouldClose (this->_window, true);
        std::cout<<"renderCount "<<renderCount<<"\n";
  	break;
  case GLFW_KEY_W: // toggle wireframe mode
    glClearColor (1.0f,1.0f,1.0f, 1.0f);
  	this->_wireframe = true;
    this->_flat = !this->_flat;
    this->_lighting = false;
    this->_horizon = false;
    this->_fog = false;
    getUniforms();
  	break;
  case GLFW_KEY_D: // toggle diffuse color + texture mode
    glClearColor (0.0f,0.0f,0.0f, 1.0f);
    this->_lighting = true;
    this->_wireframe = false;
    this->_horizon = false;
    //this->_skybox = true;
    this->_fog = false;
    getUniforms();
  break;
  case GLFW_KEY_S: // toggle skybox
    glClearColor (0.0f,0.0f,0.0f,1.0f);
    this->_skybox = !_skybox;
    skybox->loadSkyBox(this);
    //getUniforms();
  break; 
  case GLFW_KEY_R: // toggle reppy mode
    this->_reppy = !_reppy;   
    grass->loadGrass(this); 
    skybox->loadSkyBox(this);
    //getUniforms();
  break; 
  case GLFW_KEY_V: // toggle vegetation
    this->_grass = !_grass;
    this->_skybox = true;
    //getUniforms();
  break; 
  case GLFW_KEY_T: // toggle time based lighting
    this->_sun = !_sun;
    this->_lighting = true;
    //getUniforms();
  break; 
  case GLFW_KEY_G: // toggle time based lighting
    this->_gui = !_gui;
    //getUniforms();
  break;  
  case GLFW_KEY_X: // toggle horizon mode
    glClearColor (0.0f,0.0f,0.0f, 1.0f);
    this->_lighting = true;
    this->_wireframe = false;
    this->_horizon = true;
    this->_fog = false;
    this->curvatureValue = 0.00001;
    this->isSpherical = false;
    getUniforms();
  break;
    //Horizon control keys
  case GLFW_KEY_COMMA: 
    this->curvatureValue -= 0.000002; break;
  case GLFW_KEY_PERIOD: 
    this->curvatureValue += 0.000002; break;
  case GLFW_KEY_SLASH: 
    this->isSpherical = !this->isSpherical; break;
  case GLFW_KEY_F: // toggle fog mode
    this->_fog = !this->_fog;
    if (this->_fog) glClearColor (map->FogColor()[0],map->FogColor()[1],map->FogColor()[2], 1.0f);
    else glClearColor (0.0f,0.0f,0.0f, 1.0f);
    getUniforms();
  break;
  case GLFW_KEY_EQUAL:
  	  // decrease error tolerance
      if (this->_errorLimit > 0.5)
  		  this->_errorLimit /= SQRT_2;
    break;
  case GLFW_KEY_KP_ADD:  // keypad '+'
  	if (mods == 0) {
  	  // decrease error tolerance
  	  if (this->_errorLimit > 0.5)
  		  this->_errorLimit /= SQRT_2;
  	}
    break;
  case GLFW_KEY_MINUS:
  	if (mods == 0) {
  	  // increase error tolerance
  	    this->_errorLimit *= SQRT_2;
  	}
  	break;
  case GLFW_KEY_KP_SUBTRACT:  // keypad '-'
  	if (mods == 0) {
  	  // increase error tolerance
  	    this->_errorLimit *= SQRT_2;
  	} break;
  case GLFW_KEY_UP: { 
    cs237::vec3d amount = cs237::vec3d(5.0,5.0,5.0);
      if (mods == GLFW_MOD_SHIFT) amount = amount * cs237::vec3d(10.0,10.0,10.0);
      if (mods == GLFW_MOD_ALT)   amount = amount * cs237::vec3d(500.0,500.0,500.0);
    cs237::vec3d pos = this->CameraC().position() + (amount * camDird);
    this->CameraC().move(pos);
    } break;
  case GLFW_KEY_DOWN: { 
    cs237::vec3d amount = -cs237::vec3d(5.0,5.0,5.0);
      if (mods == GLFW_MOD_SHIFT) amount = amount * cs237::vec3d(10.0,10.0,10.0);
      if (mods == GLFW_MOD_ALT)   amount = amount * cs237::vec3d(500.0,500.0,500.0);
    cs237::vec3d pos = this->CameraC().position() + (amount * camDird);
    this->CameraC().move(pos);    
    } break;
  case GLFW_KEY_RIGHT: { 
    cs237::vec3d amount = cs237::vec3d(5.0,5.0,5.0);
      if (mods == GLFW_MOD_SHIFT) amount = amount * cs237::vec3d(10.0,10.0,10.0);
      if (mods == GLFW_MOD_ALT)   amount = amount * cs237::vec3d(500.0,500.0,500.0);
    cs237::vec3d pos = this->CameraC().position() + (amount * camRight);
    this->CameraC().move(pos);    
    } break;
  case GLFW_KEY_LEFT: {
    cs237::vec3d amount = cs237::vec3d(5.0,5.0,5.0);
      if (mods == GLFW_MOD_SHIFT) amount = amount * cs237::vec3d(10.0,10.0,10.0);
      if (mods == GLFW_MOD_ALT)   amount = amount * cs237::vec3d(500.0,500.0,500.0);
    cs237::vec3d pos = this->CameraC().position() + (amount * -camRight);
    this->CameraC().move(pos);   
    } break;
  case GLFW_KEY_SPACE: { //space goes up no matter what direction you're facing
    cs237::vec3d pos = cs237::vec3d(0.0,5.0,0.0);
      if (mods == GLFW_MOD_SHIFT) pos = pos * (double)10;
      if (mods == GLFW_MOD_ALT)   pos = pos * (double)100;
    this->CameraC().move(cs237::vec3d(this->Camera().position() + pos));
  } break;
  case GLFW_KEY_C: {
    cs237::vec3d pos = cs237::vec3d(0.0,-5.0,0.0);
      if (mods == GLFW_MOD_SHIFT) pos = pos * (double)10;
      if (mods == GLFW_MOD_ALT)   pos = pos * (double)100;
    this->CameraC().move(cs237::vec3d(this->Camera().position() + pos));
  } break;
  case GLFW_KEY_J: {
    cs237::mat4f rotation = cs237::rotate(cs237::mat4x4f(1.0),5.0f, -cs237::toFloat(camRight) );
    cs237::vec3f dir = cs237::vec3f( rotation * cs237::vec4f(camDir,1.0f));
    cs237::vec3f up = cs237::vec3f(rotation * cs237::vec4f(this->Camera().up(),1.0f));
    this->CameraC().look(dir,up);
  } break;
  case GLFW_KEY_K: {
    cs237::mat4f rotation = cs237::rotate(cs237::mat4x4f(1.0),5.0f, cs237::toFloat(camRight));
    cs237::vec3f dir = cs237::vec3f( rotation * cs237::vec4f(camDir,1.0f));
    cs237::vec3f up = cs237::vec3f( rotation * cs237::vec4f(this->Camera().up(),1.0f));
    this->CameraC().look(dir,up);
  } break;
  case GLFW_KEY_H: {
    cs237::mat4f rotation = cs237::rotate(cs237::mat4x4f(1.0),5.0f,cs237::vec3f(0.0,camPos.y,0.0));
    cs237::vec3f dir = cs237::vec3f( rotation * cs237::vec4f(camDir,1.0f));
    cs237::vec3f up = cs237::vec3f( rotation * cs237::vec4f(this->Camera().up(),1.0f));
    this->CameraC().look(dir,up);
  } break;
  case GLFW_KEY_L: {
    cs237::mat4f rotation = cs237::rotate(cs237::mat4x4f(1.0),5.0f,cs237::vec3f(0.0,-camPos.y,0.0));
    cs237::vec3f dir = cs237::vec3f( rotation * cs237::vec4f(camDir,1.0f));
    cs237::vec3f up = cs237::vec3f( rotation * cs237::vec4f(this->Camera().up(),1.0f));
    this->CameraC().look(dir,up);
  } break;
  default: // ignore all other keys
  
    break;
    return;
  }

  //std::cout << "after handleKey: " << this->_cam << std::endl;

}

void View::HandleMouseEnter (bool entered)
{
}

void View::HandleMouseMove (double y, double x)
{
  if (!oldMouseX){
    //if this is the first frame, there is no delta movement so we just save and wait for the next frame
    this->oldMouseX = x;
    this->oldMouseY = y;
    return;
  }
  float deltaX = (float) oldMouseX - x;
  float deltaY = (float) oldMouseY - y;

  this->oldMouseX = x;
  this->oldMouseY = y;

  if (leftMouseDown){
      if (!guiMode())
      {
        cs237::vec3f camRight = cs237::__detail::cross(Camera().direction(),this->Camera().up());
        cs237::mat4f rotation = cs237::rotate( cs237::mat4x4f(1.0), -deltaY*.20f, cs237::vec3f(0,1,0) );
        rotation *= cs237::rotate( cs237::mat4x4f(1.0), -deltaX*.20f, camRight );
        cs237::vec3f dir = cs237::vec3f( rotation * cs237::vec4f(CameraC().direction().normalize(),0.0f));
        cs237::vec3f up = cs237::vec3f( rotation * cs237::vec4f(CameraC().up(),0.0f));
        this->CameraC().look( dir, up );
      }
  }
}

void View::HandleMouseButton (int button, int action, int mods)
{
  double x,y;
  glfwGetCursorPos(Window(), &x, &y);
  switch (button)
  {
    case (GLFW_MOUSE_BUTTON_1):
      if (action == GLFW_PRESS)
      {
        leftMouseDown = true;
        if (guiMode())
        {
          if (x < wid / 2){
            if (y < ht/2){
              //std::cout<<"UL\n";
              if (x < wid /16.0)
                this->grassID = 3;
              else
                this->grassID = 0;
            }
            else {
              this->_sun = false;
              if (y > (3.0*ht)/4.0)
              {
                night = 1.0f;
                SunDirection = cs237::vec3f(-0.440225,0.252297,0.861713);
              }
              else
              {
                night = 0.0f;
                SunDirection = cs237::vec3f(-0.440225,-0.717862,-0.539328);


              }      
              float twilight = pow(SunDirection.y,2.0);
              this->night = cs237::clamp(SunDirection.z);
              SunIntensity = cs237::color3f(
                          (1.0-twilight)*(0.5) + twilight*(0.9) + twilight*twilight*(0.9),
                          (1.0-twilight)*(0.5) + twilight*(0.7),
                          (1.0-twilight)*(0.3) + twilight*(0.0) + twilight*twilight*(0.3));
              AmbientIntensity = cs237::color3f(
                          0.3 + night*0.05,
                          0.3 + night*0.05,
                          0.3 + night*0.25);
            }
          }
          else{
            if (y < ht/2){
              this->grassID = 1;
            }
            else {
              this->grassID = 2;
            }
          }
          grass->loadGrass(this);
        }
      }
      else if (action == GLFW_RELEASE)
        leftMouseDown = false;
      break;
    case (GLFW_MOUSE_BUTTON_2):
      if (action == GLFW_PRESS)
      {
        rightMouseDown = true;
        // translate the cursor position into the correct range
        x = (2.0f * x) / wid - 1.0f;
        y = 1.0f - (2.0f * y) / ht;
        // clip space location
        cs237::vec4f ray_clip = cs237::vec4f(x,y,-1,1);
        // into eye position using the inverse projection matrix
        cs237::vec4f ray_eye = (Camera().projTransform()).inverse() * ray_clip;
        // get rid of the z and w values which are incorrect at this point
        ray_eye = cs237::vec4f(ray_eye.x,ray_eye.y,-1,0);
        // get world space coordinates by multiplying by the inverse view transform
        cs237::vec3f ray_wor = cs237::vec3f((Camera().viewTransform(CameraC().position())).inverse() * ray_eye);
        // normalize the vector so it is the direction from the camera to the point you cliced in world space
        ray_wor = ray_wor.normalize();
        // pick out useful attributes
        cs237::vec3f camDir = this->Camera().direction();
        cs237::vec3f camRight = cs237::__detail::cross(camDir,this->Camera().up());
        cs237::vec3f camPosf = cs237::toFloat(Camera().position());
        // choose an elevation for the intersection plane. I choose the average height of the terrian. ymmv
        float avgElevation = .5*(map->MaxElevation() + map->MinElevation());
        // declare the intersection plane using three points - the camera position, and two points at a right angle to the camera position
        //    this could cause trouble if FOV goes too high
        // translate each point down from where the camera is to the avg height of the terrain
        cs237::vec3f p0 = camPosf; 
          p0.y = avgElevation;
        cs237::vec3f p2 = camPosf + (float)CameraC().far() * camRight; 
          p2.y = avgElevation;
        cs237::vec3f p1 = camPosf - (float)CameraC().far() * Camera().direction(); 
          p1.y = avgElevation;
        GPlane *basePlane = new GPlane(p0,p1,p2);
        // see if there is an intersection at all
        float t = (cs237::__detail::dot(basePlane->normal, p0-camPosf)) / (cs237::__detail::dot(basePlane->normal, ray_wor));
        if (t > 0)
        { 
          // if there is an intersection, solve for it.
          cs237::vec3f intersection = camPosf + (ray_wor * t);
          cs237::vec3f ntrsect2cam = ray_wor.normalize();
          cs237::vec3f ntrsect2camG = -cs237::vec3f(ray_wor.x, intersection.y, ray_wor.z).normalize();
          float hypLen = atan( acos(cs237::__detail::dot(ntrsect2cam, ntrsect2camG) / (ntrsect2cam.length() * ntrsect2camG.length())) );
          intersection = camPosf + (ray_wor * (t-hypLen));
          //RIGHT HERE change the y value to be on top of the terrain... somehow
          if (mods == GLFW_MOD_SHIFT)
          {
              this->_shiftDown = true;
              //intersection.y = avgElevation;
              grass->loadGrass(this);
              grass->pushVertices(intersection);
          }
          else
          {
            if (camPosf.y > map->MaxElevation()) intersection.y = map->MaxElevation();
            smoothMove(cs237::toDouble(intersection),Camera().position());
            if (map->Width()*map->hScale() < 100000){
              // smooth look is bugged on gcanyon...
              smoothLook(cs237::toFloat(this->moveDir));
            }
          }

        }

      }
      else if (action == GLFW_RELEASE)
        rightMouseDown = false;
      break;
  }
}

void View::smoothMove(cs237::vec3d moveTo,cs237::vec3d moveFrom)
{
  this->moveTo = moveTo;
  this->moveFrom = moveFrom;
  this->moveDir = moveTo - moveFrom;
  this->shouldMove = true;
}

void View::smoothLook(cs237::vec3f finalDirection)
{
  this->smoothLookTo = cs237::__detail::cross(CameraC().up(),finalDirection);
  this->smoothLookFrom = CameraC().direction();
  this->turningCamera = true;
}

void View::Animate ()
{
    double now = glfwGetTime();
    double dt = now - this->_lastStep;
    if (dt >= TIME_STEP) {
      this->_lastStep = now;
      /* ANIMATE STUFF */
      //make nights half as long as days
      if (sunMode())
      {
        float nightSpeedFactor = (night==0) ? .5 : 1.0;
        float timeFactor = cos(fmod(now,(double)M_PI/4.0)) * nightSpeedFactor;
        cs237::mat4f sunRotMat = cs237::rotateX(timeFactor);
        SunDirection = cs237::vec3f(sunRotMat * cs237::vec4f(SunDirection,1.0));
      
        float twilight = pow(SunDirection.y,2.0);
        this->night = cs237::clamp(SunDirection.z);
        SunIntensity = cs237::color3f(
                          (1.0-twilight)*(0.5) + twilight*(0.9) + twilight*twilight*(0.9),
                          (1.0-twilight)*(0.5) + twilight*(0.7),
                          (1.0-twilight)*(0.3) + twilight*(0.0) + twilight*twilight*(0.3));
        AmbientIntensity = cs237::color3f(
                          0.3 + night*0.05,
                          0.3 + night*0.05,
                          0.3 + night*0.25);
      }
      cs237::vec3d camPos = CameraC().position();
      cs237::vec3f camDir = CameraC().direction();
      cs237::vec3f camPosf = cs237::toFloat(CameraC().position());
      if (turningCamera)
      {
        float smoothLookDot = cs237::__detail::dot(smoothLookTo,camDir);
        float smoothLookAngle = smoothLookDot * 0.005;
        cs237::mat4f smoothRotationMat = cs237::rotate(cs237::mat4x4f(1.0),smoothLookAngle,cs237::__detail::cross(smoothLookTo,smoothLookFrom));
        if (std::abs(smoothLookDot) < 10.0){
          this->CameraC().look(smoothLookTo);
          turningCamera = false;
        }
        this->CameraC().look(smoothRotationMat * cs237::vec4f(camDir,1.0));
      }
      if (shouldMove)
      {
        this->moveDir = moveTo - moveFrom;
        if (turningCamera)
          moveDir *= cs237::vec3d(.001,.001,.001);
        else
          moveDir *= cs237::vec3d(.1,.1,.1);
        cs237::vec3d newCamPos = camPos + moveDir;
        if ( (newCamPos - moveTo).y < 0.0 || newCamPos.y < map->MinElevation()){
          this->CameraC().move(moveTo);
          shouldMove = false;
        }
        this->CameraC().move(newCamPos);
      }
    }

}

void View::Resize (int wid, int ht)
{
    glfwGetFramebufferSize (this->_window, &this->_fbWid, &this->_fbHt);
    glViewport(0, 0 , this->_fbWid, this->_fbHt);
    this->wid = wid;
    this->ht = ht;

  // default error limit is 1%
    this->_errorLimit = float(this->_fbHt) / 100.0f;

    this->_cam.setViewport (this->_fbWid, this->_fbHt);

}

/***** Local utility functions *****/

static void Error (int err, const char *msg)
{
    std::cerr << "[GLFW ERROR " << err << "] " << msg << "\n" << std::endl;
}

static GLFWwindow *InitGLFW (int wid, int ht, const char *title)
{
    glfwSetErrorCallback (Error);

    glfwInit ();

  // Check the GLFW version
    {
	int major;
	glfwGetVersion (&major, NULL, NULL);
	if (major < 3) {
	    std::cerr << "CS237 requires GLFW 3.0 or later\n" << std::endl;
	    exit (EXIT_FAILURE);
	}
    }

    glfwWindowHint (GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(wid, ht, title, NULL, NULL);
    if (window == nullptr) {
	exit (EXIT_FAILURE);
    }

    glfwMakeContextCurrent (window);

  // Check the OpenGL version
    {
	GLint major, minor;
	glGetIntegerv (GL_MAJOR_VERSION, &major);
	glGetIntegerv (GL_MINOR_VERSION, &minor);
	if ((major < 4) || ((major == 4) && (minor < 1))) {
	    std::cerr << "CS237 requires OpenGL 4.1 or later; found " << major << "." << minor << std::endl;
	    exit (EXIT_FAILURE);
	}
    }

    return window;
}

class Cell * View::FindCameraCell()
{
  cs237::vec3d camPos = this->CameraC().position();
  cs237::vec3d cellSize = this->map->CellSize();
  uint32_t col = static_cast<uint32_t>(camPos.x / cellSize.x);
  uint32_t row = static_cast<uint32_t>(camPos.z / cellSize.z);
  Cell * cameraCell = this->map->Cell((row), (col));
  if (cameraCell == nullptr)
  {
    isSimulatedCell = true;
    cameraCell = this->lastVisitedCell;
    cameraCell->setRow(row);
    cameraCell->setCol(col);
  }
  else{
    this->lastVisitedCell = cameraCell;
  }
  return cameraCell;
}

// **** RENDERING *** //
void View::Render ()
{
    
    if (! this->_isVis)
      return;  

    Map *m = this->map;
    //first pass - find visible tiles
    for(int i = 0; i < m->nRows(); i++) {
        for(int j = 0; j < m->nCols(); j++) {
            Cell *cell = m->Cell(i%m->nRows(), j%m->nCols());
            cell->setRowCol(i,j);
            Tile &tile = cell->Tile(0);
            FindVisibleTiles(tile);
        }
    }

    // second pass - render visible tiles
    double now = glfwGetTime();
    float dt = float(now - this->_lastFrameTime);
    this->_lastFrameTime = now;

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    getUniforms();

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0xffff);

    // find out where we are so we can copy cell information into the surrounding area
    //Cell *cameraCell = FindCameraCell();
    //RenderVisibleTiles(cameraCell->Tile(0)); 

    for(int i = 0; i < m->nRows(); i++) {
        for(int j = 0; j < m->nCols(); j++) {
            Cell *cell = m->Cell(i%m->nRows(), j%m->nCols());
            cell->setRowCol(i,j);
            Tile &tile = cell->Tile(0);
            RenderVisibleTiles(tile);
        }
    }

    glDisable(GL_PRIMITIVE_RESTART);

    // ******* RENDER THE SPEED EFFECTS *********
    if (shouldMove){
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      _shaderSpeed->Use();
      cs237::setUniform(_shaderSpeed->UniformLocation("resolution"), cs237::vec2f(wid,ht) * (this->_fbWid > 1024 ? 2.0f : 1.0f));
      cs237::setUniform(_shaderSpeed->UniformLocation("globalTime"), (float)glfwGetTime());
      cs237::setUniform(_shaderSpeed->UniformLocation("isTurning"), turningCamera);

      GLuint id_speed, vaoId_speed;
      CS237_CHECK( glGenVertexArrays (1, &vaoId_speed));
      CS237_CHECK( glGenBuffers (1, &id_speed));
      CS237_CHECK( glBindVertexArray (vaoId_speed));
      glBindVertexArray(vaoId_speed);

      glDrawArrays(GL_TRIANGLES, 0, 6);
      glDisable(GL_BLEND);
    }

    // ******* RENDER THE SKYBOX *********
    if (skyboxMode()){
      glDepthMask(GL_FALSE);
      glDepthFunc(GL_LEQUAL);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          
      _shaderS->Use();
      cs237::setUniform(_shaderS->UniformLocation("projection"),
                        this->Camera().projTransform());
      cs237::mat3x3f mv = (cs237::mat3x3f)this->Camera().viewTransform();
      cs237::mat4x4f mview = cs237::mat4x4f(
                                cs237::vec4f(mv[0], 0),
                                cs237::vec4f(mv[1], 0),
                                cs237::vec4f(mv[2], 0),
                                cs237::vec4f(0,0,0,1)); 
      cs237::setUniform(this->_shaderS->UniformLocation("view"), mview);
      this->skybox->Bind();
      glActiveTexture(GL_TEXTURE0);
      cs237::setUniform(this->_shaderS->UniformLocation("skybox"), 0);
      this->skybox->Draw();
      glBindVertexArray(0);
      glDepthMask(GL_TRUE);
    }

    //**GUI
    if (guiMode())
    {
      GLuint id_GUI, vaoId_GUI;
      _shaderGUI->Use();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      cs237::image2d *UIimage = new cs237::image2d("../data/UI.png", true);
      cs237::texture2D *UItexture = new cs237::texture2D(GL_TEXTURE_2D, UIimage);
      UItexture->Bind();
        UItexture->Parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        UItexture->Parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        UItexture->Parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        UItexture->Parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      //glActiveTexture(GL_TEXTURE0);
      cs237::setUniform(this->_shaderGUI->UniformLocation("tex"), 0);
      CS237_CHECK( glGenVertexArrays (1, &(vaoId_GUI)));
      CS237_CHECK( glGenBuffers (1, &id_GUI));
      CS237_CHECK( glBindVertexArray (vaoId_GUI));
      glBindVertexArray(vaoId_GUI);

      glDrawArrays(GL_TRIANGLES, 0, 6);
      //glBindTexture(GL_TEXTURE_2D, 0);
    }
    // ******* RENDER THE GRASS / VEGITATION *********
    if(grassMode()) {
      _shaderG->Use();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      cs237::setUniform(this->_shaderG->UniformLocation("scaleY"), .5f*(map->MaxElevation() + map->MinElevation()));
      cs237::setUniform(this->_shaderG->UniformLocation("scaleX"), map->Width()/2.0f);
      cs237::setUniform(this->_shaderG->UniformLocation("projection"),
                          this->Camera().projTransform());
      cs237::setUniform(this->_shaderG->UniformLocation("view"), this->Camera().viewTransform(this->Camera().position() ));

      this->grass->Bind();
      glActiveTexture(GL_TEXTURE1);
      cs237::setUniform(this->_shaderG->UniformLocation("grassTex"), 0);
      this->grass->Draw(this->_shaderG,this);
        glDisable(GL_BLEND);

      //glBindTexture(GL_TEXTURE_2D, 0);
    }


    glfwSwapBuffers (this->_window);
}

void View::FindVisibleTiles (class Tile &tile) {
    float rho = ScreenSpaceError(tile);

    // create a new frustum if necessary
    if (this->gfrustum == nullptr || this->CameraC().hasMoved){
      this->CameraC().hasMoved = false;
      this->gfrustum = new GFrustum((this->CameraC()));
    }
    // calculate inside/outside/intersect of this tile
    int cullResult = this->gfrustum->boxInFrustum(tile.BBox());
    // if we're inside
    if(cullResult > 0) 
    {
      // fix for gcanyon issue: don't keep digging deeper if next texture layer doesn't exist
      float depth = fmin(tile.LOD(),fmin(tile.cell()->NormTQT()->Depth(),tile.cell()->ColorTQT()->Depth()));
      if (rho <= ErrorLimit() || tile.NumChildren() == 0 || (wireframeMode() == false && depth >= tile.cell()->NormTQT()->Depth()-1)){
          tile.isVisible = LEAF;
          tile.renderDepth = depth;
          if (tile.tileVAO == nullptr) tile.tileVAO = this->VAOCache()->Acquire();
          tile.tileVAO->Load(tile.Chunk());
      }
      else 
        for(int k = 0; k < tile.NumChildren(); k++) {
            tile.isVisible = STEM;
            if (tile.tileVAO != nullptr){
              this->VAOCache()->Release(tile.tileVAO);
            }
            tile.tileVAO = nullptr;
            FindVisibleTiles(*(tile.Child(k)));
        }
    }
    else
      tile.isVisible = CULLED;
}

void View::RenderVisibleTiles(class Tile &tile)
{
  //setStaticUniforms()
  if (tile.isVisible == STEM)
  {
    for(int k = 0; k < tile.NumChildren(); k++)
      RenderVisibleTiles(*(tile.Child(k)));
  }
  else if (tile.isVisible == LEAF)
  {
    cs237::mat4x4f modelMat = cs237::scale(cs237::vec3f(tile.cell()->hScale(),tile.cell()->vScale(),tile.cell()->hScale()));
    modelMat[3] = cs237::vec4f(cs237::toFloat(this->map->NWCellCorner(tile.cell()->Row(),tile.cell()->Col())),1.0);
    tile.modelMat = modelMat;
    tile.modelViewMat = this->Camera().viewTransform(this->Camera().position()) * modelMat;

    if (tile.colorTex == nullptr && wireframeMode() == false && !this->CameraC().hasMoved){
      // calculate the actual row / col depending on depth
      int texRow= calcRow(tile._id);
      int texCol= calcCol(tile._id);

      // pull out the textures
      tile.colorTex = this->TxtCache()->Make(tile.cell()->ColorTQT(), tile.renderDepth, texRow, texCol);
      tile.normalTex = this->TxtCache()->Make(tile.cell()->NormTQT(), tile.renderDepth, texRow, texCol);
      texCount++;
    }
    // set all the uniforms we'll need
    setUniforms(this->Camera().projTransform(), &tile);
    renderCount++;
    tile.tileVAO->Render();
  }
  //if tile.isVisible == CULLED, don't even worry about it
}

int View::calcRow(uint32_t id){
  if (id<1) return id;
  uint32_t index = QTree::ChildIndex(id);
  int additive = (int) (index == 2 || index == 3);
  return (int) 2 * calcRow(QTree::Parent(id)) + additive;
}

int View::calcCol(uint32_t id){
  if (id<1) return id;
  uint32_t index = QTree::ChildIndex(id);
  int additive = (int) (index == 1 || index == 2);
  return (int) 2 * calcCol(QTree::Parent(id)) + additive;
}

void View::getUniforms() {

  if (wireframeMode()) {
    _shaderInUse = _shader;
    _shaderInUse->Use();
    this->modelViewMat = this->_shader->UniformLocation("modelViewMat");
    this->projection = this->_shader->UniformLocation("projection");
    this->Vcolor = this->_shader->UniformLocation("color");
    this->tau = this->_shader->UniformLocation("tau");
    this->vertexError = this->_shader->UniformLocation("vertexError");
  } 
  else if (lightingMode() || horizonMode()) {
    if (horizonMode())
    {
      _shaderInUse = _shaderH;
      _shaderInUse->Use();
      this->isSphericalLoc = this->_shaderInUse->UniformLocation("isSpherical");
      this->CurvatureLoc = this->_shaderInUse->UniformLocation("Curvature");
    }
    else if (lightingMode())
    {
      _shaderInUse = _shaderL;
      _shaderInUse->Use();
    }
    this->modelViewMat = this->_shaderInUse->UniformLocation("modelViewMat");
    this->projection = this->_shaderInUse->UniformLocation("projection");
    this->colorTexture = this->_shaderInUse->UniformLocation("colorTexture");
    this->normalTexture = this->_shaderInUse->UniformLocation("normalTexture");
    this->tileWidth = this->_shaderInUse->UniformLocation("tileWidth");
    this->direction = this->_shaderInUse->UniformLocation("direction");
    this->ambient = this->_shaderInUse->UniformLocation("ambient");
    this->intensity = this->_shaderInUse->UniformLocation("intensity");
    this->NWCorner = this->_shaderInUse->UniformLocation("NWCorner");
    this->hasFog = this->_shaderInUse->UniformLocation("hasFog");
    this->fogFactors = this->_shaderInUse->UniformLocation("fogFactors");
    this->camPos = this->_shaderInUse->UniformLocation("camPos");
    this->modelMatLoc = this->_shaderInUse->UniformLocation("modelMat");
    this->tau = this->_shaderInUse->UniformLocation("tau");
    this->vertexError = this->_shaderInUse->UniformLocation("vertexError");
  }

  CS237_CHECK( glEnable (GL_DEPTH_TEST));
}

void View::setUniforms(cs237::mat4x4f projection, const class Tile *tile)
{
  float rho = ScreenSpaceError(*tile);

  cs237::setUniform(this->modelViewMat,tile->modelViewMat);
  cs237::setUniform(this->projection,projection);

  if (wireframeMode()) {
    if (flatshadingMode())
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    cs237::setUniform(this->Vcolor, MeshColor[tile->LOD()]);
    cs237::setUniform(this->tau, ErrorLimit());
    cs237::setUniform(this->vertexError, rho);
    
  }
  else if (lightingMode() || horizonMode())
  {
    if(horizonMode()){
      // horizon specific ones?
      cs237::setUniform(this->CurvatureLoc,(float) curvatureValue );
      cs237::setUniform(this->isSphericalLoc,isSpherical);
    }
    else if(lightingMode()) {
      // lighting specific ones?
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    cs237::setUniform(this->tileWidth,(float) tile->Width());
    cs237::setUniform(this->direction,this->SunDirection);
    cs237::setUniform(this->ambient,this->AmbientIntensity);
    cs237::setUniform(this->intensity,this->SunIntensity);
    cs237::setUniform(this->NWCorner,cs237::vec3f(tile->NWCol(),0.0,tile->NWRow()));
    cs237::setUniform(this->tileWidth, (float) tile->Width());
    if (tile->colorTex) tile->colorTex->Use(0);
    if (tile->normalTex) tile->normalTex->Use(1);
    cs237::setUniform(this->colorTexture, 0);
    cs237::setUniform(this->normalTexture, 1);
    cs237::setUniform(this->hasFog,map->hasFog() && fogMode() ? GL_TRUE : GL_FALSE);
    cs237::setUniform(this->fogFactors, cs237::color4f(map->FogColor(),map->FogDensity()));
    cs237::setUniform(this->camPos, cs237::toFloat(this->Camera().position()));
    cs237::setUniform(this->modelMatLoc, tile->modelMat);
    cs237::setUniform(this->tau, ErrorLimit());
    cs237::setUniform(this->vertexError, rho);
    tile->colorTex->Release();
    tile->normalTex->Release();
  }
}









