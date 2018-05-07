#include "crowd_character.h"

#include <GLFW/glfw3.h>
#include "resource_manager.h"

#define _USE_MATH_DEFINES
#include <math.h>


const GLfloat RefreshRate = 0.1f;

const GLfloat SpriteCycleRate = 0.5f;

// conversion constant for going from degrees to radians
const GLfloat DEG_TO_RADS = 3.14159f / 180.0f;

std::vector<MoveCmd> StdMoveSeq{
	MoveCmd(2.0f,   0.0f, 0.01f, 2.0f),
	MoveCmd(2.0f,  90.0f, 0.01f, 2.0f),
	MoveCmd(2.0f, 180.0f, 0.01f, 2.0f), 
	MoveCmd(2.0f, 270.0f, 0.01f, 2.0f)};


MoveCmd::MoveCmd()
	: PauseDuration(2.0f), MoveDegrees(0.0f), MoveSpeed(0.01f), MoveDuration(2.0f) { }

MoveCmd::MoveCmd(GLfloat pause_dur, GLfloat mov_deg, GLfloat mov_spd, GLfloat mov_dur)
	: PauseDuration(pause_dur), MoveDegrees(mov_deg), MoveSpeed(mov_spd), MoveDuration(mov_dur) { }


CrowdCharacter::CrowdCharacter(glm::vec2 pos, glm::vec2 size, GLfloat rotation, std::vector<Texture2D> &sprite_array)
	: Position(pos), Size(size), Rotation(rotation), SpriteArray(sprite_array), MoveSequence(StdMoveSeq) { }

CrowdCharacter::CrowdCharacter(glm::vec2 pos, glm::vec2 size, GLfloat rotation, std::vector<Texture2D> &sprite_array, std::vector<MoveCmd> &mov_seq)
	: Position(pos), Size(size), Rotation(rotation), SpriteArray(sprite_array), MoveSequence(mov_seq) { }


void CrowdCharacter::Draw(SpriteRenderer &renderer)
{
	GLfloat curr_time = GLfloat(glfwGetTime());
	GLfloat delta_time = curr_time - LastDraw;

	if (delta_time > RefreshRate) {
		LastDraw = curr_time;

		if (Position != PreviousPosition) {
			glm::vec2 delta_vector = Position - PreviousPosition;
			Rotation = CalculateAngle(delta_vector);
			PreviousPosition = Position;

			SArrIndex++;
			if (SArrIndex == 3) {	SArrIndex = 0; }
			
			CurrentSprite = SpriteArray[SArrIndex];
		}
	    else {
		    CurrentSprite = SpriteArray[0];
	    }
	}

	renderer.DrawSprite(CurrentSprite, Position, Size, Rotation, glm::vec3(1.0f, 1.0f, 1.0f));

}

GLfloat CrowdCharacter::CalculateAngle(glm::vec2 delta_vector)
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

void CrowdCharacter::UpdateMovement(GLfloat dt)
{
	MoveCmd CurrMoveCmd = MoveSequence[MSeqIndex];
	if (PauseTotal < CurrMoveCmd.PauseDuration)
	{
		PauseTotal += dt;
	}
	else if (MTimeTotal < CurrMoveCmd.MoveDuration)
	{
		Position.x += cos(CurrMoveCmd.MoveDegrees * DEG_TO_RADS) * (dt / CurrMoveCmd.MoveSpeed);
		Position.y += sin(CurrMoveCmd.MoveDegrees * DEG_TO_RADS) * (dt / CurrMoveCmd.MoveSpeed);
		MTimeTotal += dt;
	}
	else
	{
		PauseTotal = 0.0f;
		MTimeTotal = 0.0f;
		MSeqIndex++;

		if (MSeqIndex == MoveSequence.size())
		{
			MSeqIndex = 0;
		}
	}
}

glm::vec2 CrowdCharacter::NextMove(GLfloat dt)
{
	GLfloat dx = 0.0f;
	GLfloat dy = 0.0f;

	MoveCmd CurrMoveCmd = MoveSequence[MSeqIndex];
	if (PauseTotal < CurrMoveCmd.PauseDuration)
	{
		PauseTotal += dt;
	}
	else if (MTimeTotal < CurrMoveCmd.MoveDuration)
	{
		dx = cos(CurrMoveCmd.MoveDegrees * DEG_TO_RADS) * (dt / CurrMoveCmd.MoveSpeed);
		dy = sin(CurrMoveCmd.MoveDegrees * DEG_TO_RADS) * (dt / CurrMoveCmd.MoveSpeed);
		MTimeTotal += dt;
	}
	else
	{
		PauseTotal = 0.0f;
		MTimeTotal = 0.0f;
		MSeqIndex++;

		if (MSeqIndex == MoveSequence.size())
		{
			MSeqIndex = 0;
		}
	}
	glm::vec2 next_move(dx, dy);
	return (next_move);
}

void CrowdCharacter::AddMove(glm::vec2 next_move)
{
	Position.x += next_move[0];
	Position.y += next_move[1];
}

void CrowdCharacter::SubMove(glm::vec2 next_move)
{
	Position.x -= next_move[0];
	Position.y -= next_move[1];
}
