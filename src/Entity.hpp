#ifndef ENTITY_H
#define ENTITY_H

#include "vmath.hpp"
#include "Shader.hpp"

class Entity
{
public: 
    Entity(vmath::vec3 const& position, Shader const&, GLuint vao);

    virtual void draw(vmath::mat4 const& projectView);
private:
    Shader const& shader_;
    GLuint vao_;
    vmath::mat4 transform_;
};
#endif /* ENTITY_H */
