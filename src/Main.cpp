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
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp> 
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

//Additional
#include "vmath.hpp"
#include "Consts.hpp"
#include "Shader.hpp" 

#include "Entity.hpp" 
//}}}

//{{{#defines
#define PI 3.141592653f
#define DEG2RAD 0.017453292519943295
#define RAD2DEG 57.29577951308232 

#define BUFFER_OFFSET(offset) ((GLvoid*)(offset))
//}}}

//{{{Global Variables
//Shader programs
Shader program;

//Buffers
GLuint ebo;
GLuint vao;
GLuint vbo;

//Window information
sf::RenderWindow window;

//Uniform Variables
vmath::mat4 projection;

//Mouse status
int lastX = 0;
int lastY = 0;

//Camera state
float angleX = 0;
float angleY = 0;
vmath::mat4 camera_rot = vmath::mat4::identity();
vmath::mat4 camera_trans = vmath::mat4::identity();
//}}}

//{{{Global Constants
//Projection settings
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
void mouse(int x, int y);
void keyboard(sf::Keyboard::Key const&);
void resize(int w, int h);
void cleanup();

//Helper functions
inline void message(char const*);
void setProjection(float fov, float aspect, float near, float far); 
//}}}

//{{{Initialization


//{{{int main()
int main()//int argc, char** argv)
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

    cleanup();
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
                         static_cast<float>(constants::windowWidth);

    setProjection(fieldOfView, aspect, nearPlane, farPlane);
}
//}}}

//{{{void initGL()
void initGL()
{
    glClearColor(1.f,1.f,1.f,1.f);
    glEnable(GL_SAMPLE_SHADING);
    glEnable(GL_DEPTH_TEST);

    glPrimitiveRestartIndex(constants::primitiveRestartIndex);

    GLfloat const vertexPositions[] = {
        -0.2f, -0.2f,  -0.2f, 1.0f,
         0.2f, -0.2f,  -0.2f, 1.0f,
        -0.2f,  0.2f,  -0.2f, 1.0f,
         0.2f,  0.2f,  -0.2f, 1.0f,
        -0.2f,  0.2f,   0.2f, 1.0f,
         0.2f,  0.2f,   0.2f, 1.0f,
        -0.2f, -0.2f,   0.2f, 1.0f,
         0.2f, -0.2f,   0.2f, 1.0f
    };

    GLfloat const vertexColours[] = {
        1.0f, 1.0f, 1.0f, 1.0f, //c0
        0.5f, 0.5f, 0.5f, 1.0f, //c1
        0.5f, 0.0f, 0.0f, 1.0f, //c2
        1.0f, 0.0f, 0.0f, 1.0f, //c3
        0.0f, 0.5f, 0.0f, 1.0f, //c4
        0.0f, 1.0f, 0.0f, 1.0f, //c5
        0.0f, 0.0f, 1.0f, 1.0f, //c6
        0.5f, 0.5f, 0.5f, 1.0f  //c7
    };

    GLushort const vertexIndices[] = {
        0, 1, 2, 3, 4, 5, 6, 7,
        constants::primitiveRestartIndex,
        2, 4, 0, 6, 1, 7, 3, 5
    };

    //Create a Vertex Array Object to store subsequent state
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao); 

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(vertexIndices),
                 vertexIndices,
                 GL_STATIC_DRAW);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertexPositions) + sizeof(vertexColours),
                 NULL,
                 GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPositions), vertexPositions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPositions), sizeof(vertexColours), vertexColours);

    //Load shaders 
    program.loadFromFile("shaders/default.vert","shaders/default.frag");
    
    //Set up attributes
    GLuint g_positionID = glGetAttribLocation(program.name(), "vertex");
    GLuint g_colourID   = glGetAttribLocation(program.name(), "vColour");

    glVertexAttribPointer(g_positionID,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          BUFFER_OFFSET(0));
    glVertexAttribPointer(g_colourID,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          BUFFER_OFFSET(sizeof(vertexPositions)));

    glEnableVertexAttribArray(g_positionID);
    glEnableVertexAttribArray(g_colourID); 

    glBindVertexArray(0);
}//}}}


//}}}

//{{{ Main Loop


//{{{void mainLoop()
void mainLoop()
{
    bool loop = true;

    Entity ent1({0.f,  0.f, -5.f}, program, vao);
    Entity ent2({70.f, 0.f,  0.f}, program, vao);
    Entity ent3({0.f,  0.f,  5.f}, program, vao);

    while(loop)
    {
        handleEvents(loop);

        //reset position of mouse (hopefully)
        const int w2 = window.getSize().x / 2;
        const int h2 = window.getSize().y / 2;
        sf::Mouse::setPosition({w2, h2}, window);
        lastX = w2;
        lastY = h2;

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
        case sf::Event::KeyPressed:
            keyboard(event.key.code);
            break;
        case sf::Event::MouseMoved:
            mouse(event.mouseMove.x, event.mouseMove.y);
            break;
        case sf::Event::Resized:
            resize(event.size.width, event.size.height);
        default:
            break;
        } 
    }
}//}}}

//{{{void mouse(int x, int y)
void mouse(int x, int y)
{
    if(lastX == 0xFF)
    {
        lastX = x;
        lastY = y;
    }
    const float rot = 0.1f;

    int xDif = lastX - x;
    int yDif = lastY - y;

    angleX += rot*xDif;
    angleY += rot*yDif;
    
    camera_rot =  vmath::rotate(angleY, 1.f, 0.f, 0.f);
    camera_rot *= vmath::rotate(angleX, 0.f, 1.f, 0.f);

    lastX = x;
    lastY = y;
}
//}}}

//{{{void keyboard(sf::Keyboard::Key const& key)
void keyboard(sf::Keyboard::Key const& key)
{
    int xMove = 0, zMove = 0;

    switch(key)
    {
    case sf::Keyboard::W: zMove += 1; break;
    case sf::Keyboard::S: zMove -= 1; break;
    case sf::Keyboard::A: xMove += 1; break;
    case sf::Keyboard::D: xMove -= 1; break;
    default: break;
    }

    if(xMove != 0 || zMove != 0)
    {
        const float speed = 0.01f;
        /*float aY = angleY*PI/180.f;
        float aX = angleX*PI/180.f;

        float actualX = speed*(zMove*sin(aY) + xMove*cos(aY));
        float actualY = speed*(zMove*sin(aX));
        float actualZ = speed*(zMove*cos(aX)*cos(aY) + xMove*sin(aY));*/

        camera_trans *= vmath::translate(xMove*speed, 0.0f, zMove*speed);
        //camera_trans *= vmath::translate(xMove*speed, 0.0f, 0.0f);

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

//{{{void cleanup()
void cleanup()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
} //}}}


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

    projection = vmath::frustum(-width, width, -height, height, near, far);
} //}}}


//}}}
