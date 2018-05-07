#pragma once
//#include "game_object.h"

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"



class MoveCmd
{
public:
	// Constructors
	MoveCmd();
	MoveCmd(GLfloat pause_dur, GLfloat mov_deg, GLfloat mov_spd, GLfloat mov_dur);

	// Attributes
	GLfloat PauseDuration = 0.0f;
	GLfloat MoveDegrees   = 0.0f;
	GLfloat MoveSpeed     = 0.0f;
	GLfloat MoveDuration  = 0.0f;
};

class CrowdCharacter //: public GameObject
{
	//using GameObject::Draw;
public:
	// Constructors
	CrowdCharacter(glm::vec2 pos, glm::vec2 size, GLfloat rotation, std::vector<Texture2D> &sprite_array);
	CrowdCharacter(glm::vec2 pos, glm::vec2 size, GLfloat rotation, std::vector<Texture2D> &sprite_array, std::vector<MoveCmd> &mov_seq);

	//Attributes
	glm::vec2   Position, Size;
	GLfloat     Rotation;
	GLboolean IsCoppa = GL_FALSE;

	std::vector<Texture2D> SpriteArray;
	GLint SArrIndex = 0;
	Texture2D CurrentSprite;

	std::vector<MoveCmd> MoveSequence;
	GLint MSeqIndex = 0;
	GLfloat PauseTotal = 0.0f;
	GLfloat MTimeTotal = 0.0f;

	GLfloat LastDraw = 0.0f;
	
	glm::vec2 PreviousPosition;

	// Methods
	//void MoveChar(GLfloat &dt);
	GLfloat CalculateAngle(glm::vec2 delta_vector);
	void Draw(SpriteRenderer &renderer);
	void UpdateMovement(GLfloat dt);

	glm::vec2 NextMove(GLfloat dt);
	void AddMove(glm::vec2 next_move);
	void SubMove(glm::vec2 next_move);
};

