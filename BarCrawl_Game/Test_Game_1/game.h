/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H
#include <vector>
#include <tuple>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_level.h"
#include "Boundary.h"


// Represents the current state of the game
enum GameState {
	GAME_INTRO,
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_ARRESTED,
	GAME_LOSE
};

// included in Boundary.h
// Represents the four possible (collision) directions
//enum Direction {
//	UP,
//	RIGHT,
//	DOWN,
//	LEFT
//};

enum TextLocation {
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	CENTER,
	UPPER_CENTER,
	LOWER_CENTER,
	BOTTOM_CENTER
};

// Defines a Collision typedef that represents collision data
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>

// Initial size of the player sprite
const glm::vec2 PLAYER_SIZE(50, 50);

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:


	// Game state
	GameState	State;
	GLboolean   Keys[1024];
	GLboolean   KeysProcessed[1024];

	// Camera and Screen Variables
	GLfloat     screen_width, screen_height;
	Boundary	Screen;
	Boundary	FoV;		        // field of view boundary for camera
	Boundary	PlayerCameraBox;    // boundary for when to move camera if player gets near edge
	GLfloat		player_box_height_percent = 0.55f; // percent of FoV height that player box is
    glm::mat4   camera_projection;
    glm::vec2	starting_camera_position; // player starting position at beginning of level center screen

	// Level Variables
	std::vector<GameLevel>	Levels;
	GLint					Level;
	GLint 					NUMLEVELS = 0;
	GLfloat					level_progress = 0.0f;		// tracks the progress in the level (vertical distance traveled)
	GLfloat					level_end;					// ending vertical distance for level

	// Zoom Variables
	Boundary	ZoomCameraBound;		 // world boundary (for zoom purposes)

    // Player Movement Variables
	GLfloat		basePlayerVelocity = 300;
	GLfloat		variablePlayerVelocity = 0;
	GLfloat		drunkVaryRate = (2.0f * 3.1415f)/10.0f; //10 sec period to rad/s
	GLfloat		baseZoomAmount = 0.02f;
	GLfloat		drunkZoomAmount = 0;
	GLfloat		drunkZoomVaryRate = (2.0f * 3.1415f) / 4.0f; //4 sec period to rad/s
	GLfloat		baseDrunkMove = 100.0f;
	GLfloat		drunkMoveAmount = 0;
	GLfloat		drunkMoveVaryRate = (2.0f * 3.1415f) / 1.0f; //6 sec period to rad/s

	// Collision detection
	GLboolean Collision_detect = 0;

	// Overlay Variables
	GLfloat		beerLevel = 100;		//beer remaining (%)
	GLfloat		bac = 0.07f;				//Blood Alcohol Content
	GLfloat		StartingMoney = 100;
	GLfloat		moneyRemaining = StartingMoney;	//Money Allowance ($)

	// Scoring Variables
	GLfloat GameTime = 0;
	GLuint FinalScore = 0;
	GLfloat FinalScoreAnimationCounter = 0;
	GLfloat FinalScoreAnimationTime = 2;
	GLfloat aveBac = 0;
	GLfloat bacprev = bac;
	GLfloat bacConst = 1000.0f;
	GLfloat beerDrank = 0;
	GLfloat beerConst = 1000.0f;
	GLfloat timeConst = 1000.0f;
	
	// Beer System Variables
	const GLfloat	beerDrinkRate = 1;			// %/second
	const GLfloat	BeerCost = 10;				// Beer price ($)

	// BAC System Variables
	const GLfloat	bacMax = 0.4f;
	const GLfloat	bacIncreaseConst = 0.002f;  // amount/second
	const GLfloat	bacReduceConst = 0.01f;		//Amount to reduce BAC when eating bread
	const GLfloat	BreadCost = 5;				//Bread Price ($)

	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	
	// Initialize game state (load all shaders/textures/levels)
	void Init();

	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
	void RenderScrollingBackground();
	void DoCollisions();
	
    // Camera Functions
    void UpdateCamera();
    void UpdatePlayerCameraBox();
    Boundary CalculatePlayerCameraBox(Boundary &input_FoV);
    void PanCamera(GLfloat amount, Direction dir);
	void ZoomCamera(GLfloat amount); //positive value zooms in, negative out
	
    // Printing Text
	void PrintText(std::string text, GLfloat x, GLfloat y, GLfloat scale = 1.0f, glm::vec3 color = glm::vec3(1.0f));  // define upper left x & y
	void PrintText(std::string text, TextLocation location, GLfloat scale = 1.0f, glm::vec3 color = glm::vec3(1.0f)); // printing text based off a string for location on screen.
                                                                                                                      // valid locations: TopLeft, TopRight, BottomLeft, BottomRight
	// color constants
	glm::vec3 red = { 1.0f, 0.0f, 0.0f };
	glm::vec3 orange = { 1.0f, 0.64f, 0.0f };
	glm::vec3 green = { 0.0f, 1.0f, 0.0f };
	glm::vec3 blue = { 0.0f, 0.0f, 1.0f };
	glm::vec3 yellow = { 1.0f, 1.0f, 0.0f };
	glm::vec3 purple = { 0.5f, 0.0f, 0.5f };

	// Reset
	void ResetAll();
	void ResetLevel();
	void ResetPlayer();
	void ResetCamera();

	void CalculateScore();
};

#endif