#ifndef CONSTS_H
#define CONSTS_H

namespace constants
{
    //OpenGL Settings
    const unsigned int primitiveRestartIndex = 0xFFFF;
    const unsigned int glVersionMajor = 3;
    const unsigned int glVersionMinor = 3;
    const unsigned int glAntiAliasing = 4;

    //Initial Window Settings
    char const*        windowName     = "GL Test App";
    const int          windowWidth    = 640;
    const int          windowHeight   = 480;
}

#endif /* CONSTS_H */
