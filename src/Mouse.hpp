#ifndef MOUSE_H
#define MOUSE_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include "vmath.hpp"

class Mouse
{
public:
    static sf::Vector2i delta();

    static sf::Vector2i const& getPosition();
    static sf::Vector2i getPosition(sf::Window const& rel);
    static sf::Vector2i const& getLastPosition();
    static sf::Vector2i getLastPosition(sf::Window const& rel);

    static void setPosition(sf::Vector2i const&);
    static void setPosition(sf::Vector2i const&, sf::Window const& rel);
    static void setLastPosition(sf::Vector2i const&);
    static void setLastPosition(sf::Vector2i const&, sf::Window const& rel);

    static void update();
private:
    static sf::Vector2i pos_;
    static sf::Vector2i lastPos_;
};

#endif /* MOUSE_H */
