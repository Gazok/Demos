#ifdef _MSC_VER
// disable warning "glutCreateMenu_ATEXIT_HACK' : unreferenced local function has been removed"
#pragma warning( disable : 4505 )
#endif

//{{{Includes
//Standard Library
#include <exception>
#include <stdexcept>
#include <cmath>
#include <sstream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <iostream>
#endif

//SFML and GL
#include <GL/glew.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Additional
#include "Consts.hpp"
#include "Shader.hpp" 
#include "Model.hpp" 

#include "Entity.hpp" 
//}}}

//{{{#defines
#define PI 3.141592653f
#define DEG2RAD 0.017453292519943295
#define RAD2DEG 57.29577951308232 
//}}}

//{{{Global Variables 
//Window information
sf::RenderWindow window;

//Uniform Variables
glm::mat4 projection;

sf::Vector2i lastMousePos;

//Camera state
float angleX = 0;
float angleY = 0;
glm::mat4 camera_rot = glm::mat4(1.0);
glm::mat4 camera_trans = glm::mat4(1.0);
//}}}

//{{{Global Constants
const float fieldOfView = 1.0471975511965976f;
const float nearPlane = 0.1f;
const float farPlane = 100.f;
//}}}

//{{{Function Declarations
//Initialization functions
void init();
void initGL(); 

//Main Loop functions
void mainLoop();
void handleEvents(bool&);
void flipDisplay();
void mouse(sf::Vector2i const& pos);
void keyboard();
void resize(int w, int h);

//Helper functions
inline void message(char const*);
void setProjection(float fov, float aspect, float near, float far); 
//}}}

//{{{Initialization


//{{{int main()
int main()
{
    try
    {
        init(); 
        initGL();
    }
    catch(std::exception& e)
    {
        message(e.what());
        exit(EXIT_FAILURE);
    }

    mainLoop();

    window.close();

    return 0;
}
//}}}

//{{{void init()
void init()
{
    sf::ContextSettings glSettings;

    glSettings.majorVersion      = constants::glVersionMajor;
    glSettings.minorVersion      = constants::glVersionMinor;
    glSettings.antialiasingLevel = constants::glAntiAliasing;

    sf::VideoMode vMode = sf::VideoMode::getDesktopMode();

    vMode.width  = constants::windowWidth;
    vMode.height = constants::windowHeight; 

    window.create(vMode,
                  constants::windowName, 
                  sf::Style::Default, 
                  glSettings);

    window.setFramerateLimit(60);

    window.setMouseCursorVisible(false);

    //Check that graphics card supports GL version
    if(window.getSettings().majorVersion != glSettings.majorVersion ||
       window.getSettings().minorVersion != glSettings.minorVersion)
    {
        throw std::runtime_error("Could not initialize OpenGL 3.3 context");
    }

    if(glewInit() != GLEW_OK)
    {
        throw std::runtime_error("Could not initialise GLEW");
    }

    //Create the initial perspective projection
    const float aspect = static_cast<float>(constants::windowWidth) / 
                         static_cast<float>(constants::windowHeight);

    setProjection(fieldOfView, aspect, nearPlane, farPlane);
}
//}}}

//{{{void initGL()
void initGL()
{
    glClearColor(1.f,1.f,1.f,1.f);
    glEnable(GL_SAMPLE_SHADING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SAMPLE_SHADING);

    glPrimitiveRestartIndex(constants::primitiveRestartIndex); 
}//}}}


//}}}

//{{{ Main Loop


//{{{void mainLoop()
void mainLoop()
{
    bool loop = true;

    Shader s;
    try
    {
        s.loadFromFile("shaders/default.vert","shaders/default.frag");
    }
    catch(std::exception& e)
    {
        message(e.what());
        exit(EXIT_FAILURE);
    }

    Model m(s);

    Entity ent1(glm::vec3{0.f,  0.f, -5.f}, m);
    Entity ent2(glm::vec3{70.f, 0.f,  0.f}, m);
    Entity ent3(glm::vec3{0.f,  0.f,  5.f}, m);

    while(loop)
    {
        handleEvents(loop);
        keyboard();

        //reset position of mouse (hopefully)
        const int w2 = window.getSize().x / 2;
        const int h2 = window.getSize().y / 2;
        sf::Mouse::setPosition({w2, h2}, window);
        lastMousePos = {w2, h2}; 

        ent1.draw(projection*camera_rot*camera_trans);
        ent2.draw(projection*camera_rot*camera_trans);
        ent3.draw(projection*camera_rot*camera_trans);

        flipDisplay();
    }
} //}}}

//{{{ Event Handling


//{{{void handleEvents(bool& loop)
void handleEvents(bool& loop)
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            loop = false;
            break;
        case sf::Event::MouseMoved:
            mouse({event.mouseMove.x,
                   event.mouseMove.y});
            break;
        case sf::Event::Resized:
            resize(event.size.width, event.size.height);
        default:
            break;
        } 
    }
}//}}}

//{{{void mouse(sf::Vector2i const& pos)
void mouse(sf::Vector2i const& pos)
{
    using namespace glm;

    const float rot = 0.1f;

    sf::Vector2i delta = pos - lastMousePos;

    angleY += rot*delta.x;
    angleX -= rot*delta.y;

    if     (angleX < -90.f) angleX = -90.f;
    else if(angleX >  90.f) angleX =  90.f;
    
    camera_rot = rotate(mat4(1.0), angleX, vec3(1.f, 0.f, 0.f));
    camera_rot = rotate(camera_rot, angleY, vec3(0.f, 1.f, 0.f));

    lastMousePos = pos;
}
//}}}


//{{{void keyboard()
void keyboard()
{
    using namespace glm;

    int xMove = 0, zMove = 0;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) zMove += 1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) zMove -= 1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) xMove += 1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) xMove -= 1;

    if(xMove != 0 || zMove != 0)
    {
        const float speed = 0.01f;
        const float aY = angleY*DEG2RAD; //Rotation about Y axis
        const float aX = angleX*DEG2RAD; //Rotation about X axis

        const float sinX = sin(aX);
        const float sinY = sin(aY);
        const float cosX = cos(aX);
        const float cosY = cos(aY);

        float actualX = speed*(-zMove*cosX*sinY + xMove*cosY); //"left/right"
        float actualZ = speed*( zMove*cosX*cosY + xMove*sinY); //"forward/back"
        float actualY = speed*( zMove*sinX);                   //"up/down"

        camera_trans = translate(camera_trans, vec3(actualX, actualY, actualZ));
    }
}
//}}}

//{{{void resize(int w, int h)
void resize(int w, int h)
{
    glViewport(0,0,w,h);

    setProjection(fieldOfView,
                  static_cast<float>(w) /static_cast<float>(h),
                  nearPlane, farPlane); 
}
//}}}


//}}}

//{{{void draw()
void flipDisplay()
{ 
    window.display();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
} //}}}


//}}}

//{{{Helper Functions


//{{{inline void message(char const* msg)
inline void message(char const* msg)
{
#ifdef _WINDOWS_
    LPCTSTR caption = "Message";
	MessageBox( NULL,
		msg,
		caption,
        MB_OK | MB_ICONQUESTION);
#else
    std::cout << msg << std::endl;
#endif
}
//}}}

//{{{void setProjection(float fov, float aspect, float near, float far)
void setProjection(float fov, float aspect, float near, float far)
{ 
    const float tangent = tan(fov/2.f);
    const float height  = near * tangent;
    const float width   = height * aspect;

    projection = glm::frustum(-width, width, -height, height, near, far);
} //}}}


//}}}
