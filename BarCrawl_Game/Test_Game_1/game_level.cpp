/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game_level.h"

#include <fstream>
#include <sstream>


void GameLevel::Load(const GLchar *file, GLfloat screen_height, GLfloat player_box_height_percent)
{
	// Clear old data
	this->Objects.clear();
	this->Crowd.clear();
	// Load from file
	GLchar tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLchar>> tileData;
	if (fstream)
	{
		while (std::getline(fstream, line)) // Read each line from level file
		{
			std::istringstream sstream(line);
			std::vector<char> row;
			while (sstream >> tileCode) // Read each character seperated by spaces
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			this->init(tileData, screen_height, player_box_height_percent);
		else
			throw std::invalid_argument("received 0 height tile map!");
	}
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
	for (GameObject &obj : this->Objects)
		if (!obj.IsClear)
			obj.Draw(renderer);
	for (CrowdCharacter* &c_char : this->Crowd)
		c_char->Draw(renderer);
}

GLboolean GameLevel::IsCompleted()
{
	for (GameObject &tile : this->Objects)
		if (!tile.IsSolid && !tile.Destroyed)
			return GL_FALSE;
	return GL_TRUE;
}

void GameLevel::init(std::vector<std::vector<GLchar>> tileData, GLfloat screen_height, GLfloat player_box_height_percent)
{
	// for now hardcode level_width to be 800 and tiles to be 100x100. and height to be based on text file
	// so 8 tiles across and as many tiles vertically as you want based on lines in the text file

	// Calculate dimensions
	GLfloat tileData_height = static_cast<GLfloat>(tileData.size());
	GLfloat tileData_width = static_cast<GLfloat>(tileData[0].size()); // Note we can index vector at [0] since this function is only called if height > 0
	GLfloat tile_width = 100, tile_height = 100; // pixel size of each individual tile = 100x100
	glm::vec2 tile_size = { tile_width, tile_height }; // fixed size for tiles (maybe change based on aspect ratio
													   // of image if looks really bad). but do in loop on individual basis. 
													   // and keep the size less than 100x100

	// set level end height (vertical distance traveled that defines end of level)
	level_end = tile_height * tileData_height;
	
	// camera coords
	GLfloat bottom_level = screen_height;
	GLfloat top_level = bottom_level - level_end; 
	
	
	// setup defaults
	glm::vec2 pos = { 0.0f, 0.0f };
	glm::vec2 size = tile_size; // change if you think aspect ratio gets too fucked while loading. keep less than tile_size though.
	GLfloat rot = 0.0f * DEG_TO_RADS; // change input rotation per texture underneath if needed	
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };


	// Initialize level tiles based on tileData
	GLfloat x_pos, y_pos; // x y coordinates of the tile. note for y, up is negative direction
	y_pos = bottom_level - tile_height; // start off ready for first tile position (we're placing upper left corner)
	for (auto it = tileData.rbegin(); it != tileData.rend(); ++it) // load rows backwards from order put into structure
	{
		std::vector<GLchar> row = *it;
		// reset tile x position to 0
		x_pos = 0.0f;
		for (auto item : row) // dereference iterator
		{
			pos = { x_pos, y_pos };
			
			/// beer stand
			if (item == 'B') {
				size = glm::vec2(100.0f ,100.0f / 172.0f * 145.0f); 
				GameObject obj(pos, size, rot, ResourceManager::GetTexture("beer_stand"), color);
				obj.IsSolid = GL_FALSE; // make stands walkable for closer interaction
				obj.IsBeerStand = GL_TRUE;
				this->Objects.push_back(obj);
			}
			/// bread stand
			else if (item == 'b') {
				size = glm::vec2(81,100);
				GameObject obj(pos, size, rot, ResourceManager::GetTexture("bread_stand"), color);
				obj.IsSolid = GL_FALSE; // make stands walkable for closer interaction
				obj.IsBreadStand = GL_TRUE;
				this->Objects.push_back(obj);
			}
			/// red car 
			else if (item == 'C') {
				size = glm::vec2(75, 100);
				GameObject obj(pos, size, rot, ResourceManager::GetTexture("red_car"), color);
				obj.IsSolid = GL_TRUE;
				this->Objects.push_back(obj);
			}
			/// blue car
			else if (item == 'c') {
				size = glm::vec2(78, 100);
				GameObject obj(pos, size, rot, ResourceManager::GetTexture("blue_car"), color);
				obj.IsSolid = GL_TRUE;
				this->Objects.push_back(obj);
			}
			
			/// po po
			else if (item == 'P') {
				size = glm::vec2(50, 50);
				std::vector<Texture2D> coppa_textures{ ResourceManager::GetTexture("coppa_1"), ResourceManager::GetTexture("coppa_2"),  ResourceManager::GetTexture("coppa_1"), ResourceManager::GetTexture("coppa_3") };
				CrowdCharacter *coppa;
				coppa = new CrowdCharacter(pos, size, 0.0f, coppa_textures);
				coppa->IsCoppa = GL_TRUE;
				this->Crowd.push_back(coppa);
			}
			/// male1
			else if (item == '1') {
				size = glm::vec2(50, 50);
				std::vector<Texture2D> male1_textures{ ResourceManager::GetTexture("male1_1"), ResourceManager::GetTexture("male1_2"),  ResourceManager::GetTexture("male1_1"), ResourceManager::GetTexture("male1_3") };
				CrowdCharacter *male1;
				male1 = new CrowdCharacter(pos, size, 0.0f, male1_textures);
				this->Crowd.push_back(male1);
			}
			/// male2
			else if (item == '2') {
				size = glm::vec2(50, 50);
				std::vector<Texture2D> male2_textures{ ResourceManager::GetTexture("male2_1"), ResourceManager::GetTexture("male2_2"),  ResourceManager::GetTexture("male2_1"), ResourceManager::GetTexture("male2_3") };
				CrowdCharacter *male2;
				male2 = new CrowdCharacter(pos, size, 0.0f, male2_textures);
				this->Crowd.push_back(male2);
			}
			/// male3
			else if (item == '3') {
				size = glm::vec2(50, 50);
				std::vector<Texture2D> male3_textures{ ResourceManager::GetTexture("male3_1"), ResourceManager::GetTexture("male3_2"),  ResourceManager::GetTexture("male3_1"), ResourceManager::GetTexture("male3_3") };
				CrowdCharacter *male3;
				male3 = new CrowdCharacter(pos, size, 0.0f, male3_textures);
				this->Crowd.push_back(male3);
			}
			/// male4
			else if (item == '4') {
				size = glm::vec2(50, 50);
				std::vector<Texture2D> male4_textures{ ResourceManager::GetTexture("male4_1"), ResourceManager::GetTexture("male4_2"),  ResourceManager::GetTexture("male4_1"), ResourceManager::GetTexture("male4_3") };
				CrowdCharacter *male4;
				male4 = new CrowdCharacter(pos, size, 0.0f, male4_textures);
				this->Crowd.push_back(male4);
			}
			
			// increment x to next tile position
			x_pos += tile_width; 
		}
		// increment tile y value (negative is up direction)
		y_pos -= tile_height;
	}

	// add finish line last
	GLfloat finishline_y = top_level - screen_height * (player_box_height_percent + (1-player_box_height_percent)/2.0f );
	pos = { 0.0f, finishline_y };// since level_progress tracks bottom of screen. we still have a whole screen_height to travel
													// let's place it 75% up to make the player just barely cross finish line before hitting 100%					
	size = glm::vec2(800, 20);
	rot = 0.0f;
	color = { 1.0f, 1.0f, 1.0f };

	GameObject obj(pos, size, rot, ResourceManager::GetTexture("finish_line"), color);
	obj.IsSolid = GL_FALSE;
	obj.IsFinishLine = GL_TRUE;
	this->Objects.push_back(obj);


	// for reference on how to add a 1.2x tile size invisible boundary
	/*size = glm::vec2(100, 100);
	size2.x = size.x * 1.2f;
	size2.y = size.y * 1.2f;
	pos2.x = pos.x - (size2.x - size.x) / 2.0f;
	pos2.y = pos.y - (size2.y - size.y) / 2.0f;
	GameObject obj2(pos2, size2, rot, ResourceManager::GetTexture("clear"), glm::vec3(1.0f, 1.0f, 1.0f));
	obj2.IsSolid = GL_FALSE;
	obj2.IsClear = GL_TRUE;
	obj2.IsBreadStand = GL_TRUE;
	this->Objects.push_back(obj2);*/

}

GLfloat GameLevel::GetLevelEnd() {
	return this->level_end;
}