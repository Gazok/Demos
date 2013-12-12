#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

class Model
{
public: 
    Model(Shader const& shader);
    virtual ~Model(); 

    virtual void draw(glm::mat4 const& projectView) const;
private:
    Shader const& shader_;
    GLuint ebo_;
    GLuint vbo_;
    GLuint vao_; 
};
#endif /* Model */
