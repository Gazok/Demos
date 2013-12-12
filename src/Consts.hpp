#ifndef CONSTS_H
#define CONSTS_H

namespace constants
{
    //OpenGL Settings
    constexpr unsigned int primitiveRestartIndex = 0xFFFF;
    constexpr unsigned int glVersionMajor = 3;
    constexpr unsigned int glVersionMinor = 3;
    constexpr unsigned int glAntiAliasing = 4;

    //Initial Window Settings
    const char *const  windowName     = "GL Test App";
    constexpr int          windowWidth    = 640;
    constexpr int          windowHeight   = 480;
} 

#endif /* CONSTS_H */
