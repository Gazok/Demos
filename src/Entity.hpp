#ifndef ENTITY_H
#define ENTITY_H

#include "vmath.hpp"
#include "Model.hpp"

class Entity
{
public: 
    Entity(vmath::vec3 const& position, Model const&);

    virtual void draw(vmath::mat4 const& projectView);
private:
    vmath::mat4 transform_;
    Model const& model_;
};
#endif /* ENTITY_H */
