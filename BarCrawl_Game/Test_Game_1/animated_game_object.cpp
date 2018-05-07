#include "animated_game_object.h"

#include <GLFW/glfw3.h>
#include "resource_manager.h"

#define _USE_MATH_DEFINES
#include <math.h>



GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


AnimatedGameObject::AnimatedGameObject()
{
}


AnimatedGameObject::~AnimatedGameObject()
{
}

AnimatedGameObject::AnimatedGameObject(glm::vec2 pos, glm::vec2 size, std::vector<Texture2D> &sprite_array, GLfloat delta_t, GLboolean animate_movement, glm::vec3 color, glm::vec2 velocity):
	GameObject(pos, size, sprite_array.at(0), color, velocity), previous_position{ pos }, delta_t{ delta_t }, sprite_array{ sprite_array }, sprite_iterator{ this->sprite_array.begin() }, animate_movement{ animate_movement } {
}

AnimatedGameObject::AnimatedGameObject(glm::vec2 pos, glm::vec2 size, GLfloat rotation, std::vector<Texture2D> &sprite_array, GLfloat delta_t, GLboolean animate_movement, glm::vec3 color, glm::vec2 velocity) :
	GameObject(pos, size, rotation, sprite_array.at(0), color, velocity), previous_position{ pos }, delta_t{ delta_t }, sprite_array{ sprite_array }, sprite_iterator{ this->sprite_array.begin() }, animate_movement{ animate_movement } {
}

void AnimatedGameObject::Draw(SpriteRenderer &renderer)
{

	GLfloat currentFrame = GLfloat(glfwGetTime());
	deltaTime = currentFrame - lastFrame;

	if (deltaTime > delta_t) {

		lastFrame = currentFrame;

		if (animate_movement) {

			if (this->Position != previous_position) {

				glm::vec2 delta_vector = this->Position - previous_position;


				this->Rotation = calculate_angle(delta_vector);

				previous_position = this->Position;

				if (sprite_iterator == sprite_array.end()) {

					sprite_iterator = sprite_array.begin();
				}

				this->Sprite = *sprite_iterator++;

			}
			else {
				this->Sprite = sprite_array[0];
			}

		}
		else {

			if (sprite_iterator == sprite_array.end()) {

				sprite_iterator = sprite_array.begin();
			}

			this->Sprite = *sprite_iterator++;

		}

	}

	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

GLfloat AnimatedGameObject::calculate_angle(glm::vec2 delta_vector)
{
	glm::vec2 dv = delta_vector;
	GLfloat rotation_angle = 0.0f;

	if (dv.y < 0) {
		rotation_angle = -atan(dv.x / dv.y);
	}
	else if (dv.y > 0) {
		rotation_angle = static_cast<GLfloat>(M_PI - atan(dv.x / dv.y));
	}
	else if ((dv.y == 0) && (dv.x > 0)) {
		rotation_angle = static_cast<GLfloat>(M_PI / 2.0f);
	}
	else if ((dv.y == 0) && (dv.x < 0)) {
		rotation_angle = static_cast<GLfloat>(-M_PI / 2.0f);
	}
	else {
		rotation_angle = 0.0f;
	}


	return rotation_angle;
}
