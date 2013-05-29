#include <GL/glew.h>
#include "Entity.hpp"

Entity::Entity(vmath::vec3 const& pos, Model const& m)
    : model_(m)
{ 
    transform_ = vmath::translate(pos);
}

void Entity::draw(vmath::mat4 const& projectView)
{
    model_.draw(projectView*transform_); 
}
