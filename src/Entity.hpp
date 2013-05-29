#ifndef ENTITY_H
#define ENTITY_H

#include "vmath.hpp"
#include "Shader.hpp"

class Entity
{
public: 
    Entity(vmath::vec3 const& position);

    virtual void draw(vmath::mat4 const& projectView);

    static void cleanup();
private:
    //Shader const& shader_;
    vmath::mat4 transform_;

    static Shader shader_;
    static GLuint ebo_;
    static GLuint vbo_;
    static GLuint vao_;
    static bool loaded_; 
    static void load(); 
};
#endif /* ENTITY_H */
