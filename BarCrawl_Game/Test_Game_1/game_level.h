/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "crowd_character.h"
#include "sprite_renderer.h"
#include "resource_manager.h"


/// GameLevel holds all Tiles as part of a level and 
/// hosts functionality to Load/render levels from text files.

// Example format for level text files:
//    8 chars space-separated per line
//    unlimited height
//    player starts at last line in file and works their way up to top

// - - - - - - - -
// c - - - - 1 - -
// - - C - - - P -
// c b c - - B - -
// - C C - - - - -

// Legend:
// - => empty space
// B => beer stand
// b => bread stand
// C => red car
// c => blue car
// P => policeman
// 1 => male 1
// 2 => male 2
// 3 => male 3
// 4 => male 4

class GameLevel
{
public:
	
	// Empty default constructor & destructor
	GameLevel()  {};
	~GameLevel() {};

	// Level state
	std::vector<GameObject>     Objects;
	std::vector<CrowdCharacter*>  Crowd;
	
	// Loads level from file
	void Load(const GLchar *file, GLfloat screen_height, GLfloat player_box_height_percent);
	// Render level
	void Draw(SpriteRenderer &renderer);
	// Check if the level is completed (all non-solid tiles are destroyed)
	GLboolean IsCompleted();
	GLfloat GetLevelEnd();

private:
	// Initialize level from tile data
	void init(std::vector<std::vector<GLchar>> tileData, GLfloat screen_height, GLfloat player_box_height_percent);
	
	// conversion constant for going from degrees to radians
	const GLfloat DEG_TO_RADS = 3.14159f / 180.0f;

	// level end variable (vertical distance necessary to be traveled to complete level)
	GLfloat level_end;
};

#endif