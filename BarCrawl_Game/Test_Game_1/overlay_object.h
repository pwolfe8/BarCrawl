#ifndef OVERLAYOBJECT_H
#define OVERLAYOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "Boundary.h"

class OverlayObject : public GameObject
{
public:
	// Constructor(s)
	OverlayObject();
    ~OverlayObject() {};

	void Render(
        SpriteRenderer &renderer, 
        Boundary &screen,
        Boundary &world,
        GLfloat beerR,
        GLfloat bacR,
        GLfloat bacMax,
        GLfloat money,
        GLfloat StartingMoney
    );

};

#endif