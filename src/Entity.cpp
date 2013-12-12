#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Entity.hpp"

Entity::Entity(glm::vec3 const& pos, Model const& m)
    : model_(m)
{ 
    transform_ = glm::translate(glm::mat4(1.0), pos);
}

void Entity::draw(glm::mat4 const& projectView)
{
    model_.draw(projectView*transform_); 
}
