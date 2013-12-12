#ifdef _MSC_VER
// disable warning "glutCreateMenu_ATEXIT_HACK' : unreferenced local function has been removed"
#pragma warning( disable : 4505 )
#endif

//{{{Includes
//Standard Library
#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <sstream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
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
sf::RenderWindow g_window;

// Uniform Variables
glm::mat4 g_projection;

// Mouse State
sf::Vector2i g_lastMousePos;
bool g_locked = false;

// Camera state
float g_angleX = 0;
float g_angleY = 0;
glm::mat4 g_camera_rot = glm::mat4(1.0);
glm::mat4 g_camera_trans = glm::mat4(1.0);
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
void toggleMouseLock();
void lockMouse();
void unlockMouse();
void updateMouse();
void resize(int w, int h);

//Helper functions
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
        std::cout << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    mainLoop();

    g_window.close();

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

    g_window.create(vMode,
                  constants::windowName,
                  sf::Style::Default,
                  glSettings);

    //g_window.setFramerateLimit(60);
    //
    lockMouse();

    //Check that graphics card supports GL version
    if(g_window.getSettings().majorVersion != glSettings.majorVersion ||
       g_window.getSettings().minorVersion != glSettings.minorVersion)
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
        std::cout << e.what() << std::endl;
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

        updateMouse();

        const glm::mat4 transform = g_projection*g_camera_rot*g_camera_trans;

        ent1.draw(transform);
        ent2.draw(transform);
        ent3.draw(transform);

        flipDisplay();
    }
} //}}}

//{{{ Event Handling


//{{{void handleEvents(bool& loop)
void handleEvents(bool& loop)
{
    using Key = sf::Keyboard::Key;

    sf::Event event;
    while(g_window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            loop = false;
            break;

        case sf::Event::KeyPressed:
            if (event.key.code == Key::F5 or event.key.code == Key::Escape)
            {
                toggleMouseLock();
            }
            if ((event.key.code == Key::F4 and event.key.alt) or
                (event.key.code == Key::W and event.key.control))
            {
                loop = false;
            }
            break;
        case sf::Event::MouseMoved:
            mouse({event.mouseMove.x,
                   event.mouseMove.y});
            break;
        case sf::Event::Resized:
            resize(event.size.width, event.size.height);
            break;
        case sf::Event::LostFocus:
            unlockMouse();
            break;
        case sf::Event::GainedFocus:
            lockMouse();
            break;
        default:
            break;
        }
    }
}//}}}

//{{{void mouse(sf::Vector2i const& pos)
void mouse(sf::Vector2i const& pos)
{
    using namespace glm;
    if (not g_locked) return;

    const float rot = 0.1f;

    sf::Vector2i delta = pos - g_lastMousePos;

    g_angleY += rot*delta.x;
    g_angleX -= rot*delta.y;

    if     (g_angleX < -90.f) g_angleX = -90.f;
    else if(g_angleX >  90.f) g_angleX =  90.f;

    g_camera_rot = rotate(mat4(1.0), g_angleX, vec3(1.f, 0.f, 0.f));
    g_camera_rot = rotate(g_camera_rot, g_angleY, vec3(0.f, 1.f, 0.f));

    g_lastMousePos = pos;
}
//}}}


//{{{void keyboard()
void keyboard()
{
    using namespace glm;
    using kbd = sf::Keyboard;
    using Key = sf::Keyboard::Key;

    int xMove = 0, zMove = 0;

    if(kbd::isKeyPressed(Key::W)) zMove += 1;
    if(kbd::isKeyPressed(Key::S)) zMove -= 1;
    if(kbd::isKeyPressed(Key::A)) xMove += 1;
    if(kbd::isKeyPressed(Key::D)) xMove -= 1;

    if(xMove != 0 || zMove != 0)
    {
        const float speed = 0.01f;
        const float aY = g_angleY*DEG2RAD; //Rotation about Y axis
        const float aX = g_angleX*DEG2RAD; //Rotation about X axis

        const float sinX = sin(aX);
        const float sinY = sin(aY);
        const float cosX = cos(aX);
        const float cosY = cos(aY);

        float actualX = speed*(-zMove*cosX*sinY + xMove*cosY); //"left/right"
        float actualZ = speed*( zMove*cosX*cosY + xMove*sinY); //"forward/back"
        float actualY = speed*( zMove*sinX);                   //"up/down"

        g_camera_trans = translate(g_camera_trans, vec3(actualX, actualY, actualZ));
    }
}
//}}}

void toggleMouseLock()
{
    g_locked = !g_locked;
    g_window.setMouseCursorVisible(!g_locked);

}

void lockMouse()
{
    if (not g_locked) toggleMouseLock();
}

void unlockMouse()
{
    if (g_locked) toggleMouseLock();
}

void updateMouse()
{
    if (g_locked)
    {
        //reset position of mouse
        const int w2 = g_window.getSize().x / 2;
        const int h2 = g_window.getSize().y / 2;
        sf::Mouse::setPosition({w2, h2}, g_window);
        g_lastMousePos = {w2, h2};
    }
}

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
    g_window.display();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
} //}}}


//}}}

//{{{Helper Functions


//{{{void setProjection(float fov, float aspect, float near, float far)
void setProjection(float fov, float aspect, float near, float far)
{
    const float tangent = tan(fov/2.f);
    const float height  = near * tangent;
    const float width   = height * aspect;

    g_projection = glm::frustum(-width, width, -height, height, near, far);
} //}}}


//}}}
