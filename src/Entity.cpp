#include <GL/glew.h>
#include "Entity.hpp"

Entity::Entity(vmath::vec3 const& pos, Shader const& shader, GLuint vao)
    :shader_(shader), vao_(vao)
{ 
    transform_ = vmath::translate(pos);
}

void Entity::draw(vmath::mat4 const& projectView)
{
    Shader::bind(shader_);

    glBindVertexArray(vao_);
    glEnable(GL_PRIMITIVE_RESTART);

    //Transfer object position
    shader_.setUniform("project_view_model", 
                       projectView*transform_);

    //Draw vertices
    glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, 0);

    //Unbind
    glBindVertexArray(0); 
    glDisable(GL_PRIMITIVE_RESTART);
}
