#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include "Model.hpp"

class Entity
{
public: 
    Entity(glm::vec3 const& position, Model const&);

    virtual void draw(glm::mat4 const& projectView);
private:
    glm::mat4 transform_;
    Model const& model_;
};
#endif /* ENTITY_H */
