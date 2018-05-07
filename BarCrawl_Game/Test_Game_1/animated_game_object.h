#pragma once
#include "game_object.h"
#include <vector>

class AnimatedGameObject :
	public GameObject
{

	using GameObject::Draw;

public:

	AnimatedGameObject();
	~AnimatedGameObject();
	AnimatedGameObject(glm::vec2 pos, glm::vec2 size, std::vector<Texture2D> &sprite_array, GLfloat delta_t, GLboolean animate_movement, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	AnimatedGameObject(glm::vec2 pos, glm::vec2 size, GLfloat rotation, std::vector<Texture2D> &sprite_array, GLfloat delta_t, GLboolean animate_movement, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	void Draw(SpriteRenderer &renderer);

private:

	GLboolean animate_movement;
	GLfloat delta_t;
	std::vector<Texture2D> sprite_array;
	std::vector<Texture2D>::iterator sprite_iterator;
	glm::vec2 previous_position;

	GLfloat calculate_angle(glm::vec2 delta_vector);

};

