/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "animated_game_object.h"
#include "overlay_object.h"
#include <math.h>
#include "text_renderer.h"
#include "string.h"

// Game-related State data
SpriteRenderer  *Renderer;
GameObject      *Player;
OverlayObject	*Overlay;
TextRenderer    *Text;
//BackgroundRenderer Background;

Game::Game(GLuint width, GLuint height)
	:	State(GAME_MENU),
		Keys(),
		screen_width(static_cast<GLfloat>(width)),
		screen_height(static_cast<GLfloat>(height))
{
	Screen.init(0.0f, 0.0f, screen_width, screen_height);
}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Overlay;
	delete Text;
}


Boundary Game::CalculatePlayerCameraBox(Boundary &input_FoV) {
    Boundary new_PlayerCameraBox;
    new_PlayerCameraBox.init(
        input_FoV.x,
        input_FoV.y + (1 - player_box_height_percent) * input_FoV.height / 2.0f,
        input_FoV.width, // give player full FoV width access
        player_box_height_percent * input_FoV.height // scaled height
    );
    return new_PlayerCameraBox;
}


// updates based on percent_size and current FoV
void Game::UpdatePlayerCameraBox(){

	PlayerCameraBox.init(
		FoV.x,
		FoV.y + (1.0f - player_box_height_percent) * FoV.height / 2.0f,
        FoV.width, // give full width access
		player_box_height_percent* FoV.height // scaled height
    );
}

// Collision detection
GLboolean CheckCollision(GameObject &one, GameObject &two);
GLboolean CheckCollision(GameObject &one, CrowdCharacter* &two);
Direction VectorDirection(glm::vec2 closest);

void Game::Init()
{	
	// Load shaders
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
	Shader sprite_shader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(sprite_shader);

	// initialize camera
	FoV.init(0.0f, 0.0f, screen_width, screen_height);
	UpdatePlayerCameraBox();
	UpdateCamera();

	// setup starting position for player in center screen
	starting_camera_position = glm::vec2(screen_width / 2.0f - PLAYER_SIZE.x / 2.0f, screen_height / 2.0f - PLAYER_SIZE.y / 2.0f);
	
	// Load Old Textures
	ResourceManager::LoadTexture("textures/awesomeface.png",	GL_TRUE,	"face");
	ResourceManager::LoadTexture("textures/block.png",			GL_FALSE,	"block");
	ResourceManager::LoadTexture("textures/block_solid.png",	GL_FALSE,	"block_solid");
    ResourceManager::LoadTexture("textures/background.jpg",		GL_FALSE,	"background");

	//// Load New Textures //// 	
    // Level 1, 2, 3 street background. TODO: maybe update background textures for level2.  
	// NOTE: keep "levelX_background" format (used while rendering)
    ResourceManager::LoadTexture("textures/cobblestone_street.png",			GL_TRUE, "level1_background"); 
    ResourceManager::LoadTexture("textures/cobblestone_circle_street.png",	GL_TRUE, "level2_background"); 
	ResourceManager::LoadTexture("textures/rainbow_road.png",				GL_TRUE, "level3_background"); 

	//Player
	ResourceManager::LoadTexture("textures/player/player_1.png", GL_TRUE, "player1");
	ResourceManager::LoadTexture("textures/player/player_2.png", GL_TRUE, "player2");
	ResourceManager::LoadTexture("textures/player/player_3.png", GL_TRUE, "player3");

	//Crowd
	ResourceManager::LoadTexture("textures/crowd/male1/male1_1.png", GL_TRUE, "male1_1");
	ResourceManager::LoadTexture("textures/crowd/male1/male1_2.png", GL_TRUE, "male1_2");
	ResourceManager::LoadTexture("textures/crowd/male1/male1_3.png", GL_TRUE, "male1_3");

	ResourceManager::LoadTexture("textures/crowd/male2/male2_1.png", GL_TRUE, "male2_1");
	ResourceManager::LoadTexture("textures/crowd/male2/male2_2.png", GL_TRUE, "male2_2");
	ResourceManager::LoadTexture("textures/crowd/male2/male2_3.png", GL_TRUE, "male2_3");

	ResourceManager::LoadTexture("textures/crowd/male3/male3_1.png", GL_TRUE, "male3_1");
	ResourceManager::LoadTexture("textures/crowd/male3/male3_2.png", GL_TRUE, "male3_2");
	ResourceManager::LoadTexture("textures/crowd/male3/male3_3.png", GL_TRUE, "male3_3");

	ResourceManager::LoadTexture("textures/crowd/male4/male4_1.png", GL_TRUE, "male4_1");
	ResourceManager::LoadTexture("textures/crowd/male4/male4_2.png", GL_TRUE, "male4_2");
	ResourceManager::LoadTexture("textures/crowd/male4/male4_3.png", GL_TRUE, "male4_3");

	ResourceManager::LoadTexture("textures/crowd/coppa/coppa_1.png", GL_TRUE, "coppa_1");
	ResourceManager::LoadTexture("textures/crowd/coppa/coppa_2.png", GL_TRUE, "coppa_2");
	ResourceManager::LoadTexture("textures/crowd/coppa/coppa_3.png", GL_TRUE, "coppa_3");

	//Cars
	ResourceManager::LoadTexture("textures/car_red.png",		GL_TRUE,	"red_car");
	ResourceManager::LoadTexture("textures/car_blue.png", 		GL_TRUE,	"blue_car");
	//Street
	ResourceManager::LoadTexture("textures/street.png",			GL_TRUE,	"street");
	ResourceManager::LoadTexture("textures/finish_line.png",	GL_TRUE,	"finish_line");
	//Objects
	ResourceManager::LoadTexture("textures/beer_stand.png",		GL_TRUE,	"beer_stand");
	ResourceManager::LoadTexture("textures/bread_stand.png",	GL_TRUE,	"bread_stand");
	//Overlay
	ResourceManager::LoadTexture("textures/beer.png",			GL_TRUE,	"beer_sprite");
	ResourceManager::LoadTexture("textures/blood.png",			GL_TRUE,	"blood_sprite");
	ResourceManager::LoadTexture("textures/money.png",			GL_TRUE,	"money_sprite");
	//Boundaries
	ResourceManager::LoadTexture("textures/clear.png",			GL_TRUE,	"clear");

	// Setup Text & Load some starting font for now (feel free to change it in functions or wherever)
	Text = new TextRenderer(camera_projection);
	Text->Load("fonts/Roboto-Light.ttf", 32); // CHANGE FONT HERE (font, textsize)

	// Load levels. // TODO change to load level 2 and 3 text files once created
	GameLevel one;   one.Load("levels/one.lvl", screen_height, player_box_height_percent);
	GameLevel two;	 two.Load("levels/two.lvl", screen_height, player_box_height_percent);
	GameLevel three; three.Load("levels/three.lvl", screen_height, player_box_height_percent);
		//GameLevel test; test.Load("levels/test.lvl", screen_width, screen_height);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
		//this->Levels.push_back(test);
	this->NUMLEVELS = 3;
	this->Level = 0; // index at 0 for first level
	level_end = Levels[Level].GetLevelEnd(); // initialize level_end to be first level's end
		
	// zoom camera boundary
	//ZoomCameraBound.bottom = screen_height;
	//ZoomCameraBound.top = level_end + screen_height;
	//ZoomCameraBound.bottom = level_end + screen_height;
	ZoomCameraBound.bottom = screen_height;
	ZoomCameraBound.top = -1 * level_end;
	ZoomCameraBound.left = 0.0f;
	ZoomCameraBound.right = screen_width;

	// Assemble sprite arrays
	std::vector<Texture2D> player_textures{ ResourceManager::GetTexture("player1"), ResourceManager::GetTexture("player2"),  ResourceManager::GetTexture("player1"), ResourceManager::GetTexture("player3")};
	Player = new AnimatedGameObject(starting_camera_position, PLAYER_SIZE, player_textures, basePlayerVelocity / 4000.0f, true);
}

void Game::Update(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		//Calculate Beer Remaining and BAC
		bac += bacIncreaseConst * dt;

		if (bac > bacMax * 1.01f) {
			bac = bacMax * 1.01f;
		}

		beerLevel -= beerDrinkRate * dt;

		// Drunk Effect
		variablePlayerVelocity = (basePlayerVelocity / 2 * sin(drunkVaryRate * GameTime) + basePlayerVelocity / 4 * sin(2 * drunkVaryRate * GameTime)) * (bac / bacMax);
		drunkZoomAmount = (baseZoomAmount / 2 * sin(drunkZoomVaryRate * GameTime) + baseZoomAmount / 4 * sin(2 * drunkZoomVaryRate * GameTime)) * (bac / bacMax);
		drunkMoveAmount = (baseDrunkMove / 2 * sin(drunkMoveVaryRate * GameTime) + baseDrunkMove / 4 * sin(2 * drunkMoveVaryRate * GameTime)) * (bac / bacMax);
		
		//// for debug without drunk effect use these:
		//variablePlayerVelocity = basePlayerVelocity;
		//drunkZoomAmount = 0.0f;
		//drunkMoveAmount = 0.0f;

		ZoomCamera(drunkZoomAmount);

		//Stats
		//aveBac += bac / dt;
		//bacprev = bac;
		beerDrank += beerDrinkRate * dt;

		// update camera position
		this->UpdateCamera();

		// Check loss condition
		if ((beerLevel <= 0.0f) || (bac >= bacMax)) // Did you run out of beer? get too drunk?
		{
			this->State = GAME_LOSE;

			//this->ResetAll();
			//this->ResetLevel();
			//this->ResetPlayer();
		}

		for (CrowdCharacter* &c_char : this->Levels[this->Level].Crowd)
		{
			GLboolean char_collision = GL_FALSE;
			glm::vec2 next_move = c_char->NextMove(dt);
			c_char->AddMove(next_move);

			GLboolean collision_p = CheckCollision(*Player, c_char);
			if (collision_p)
			{
				char_collision = GL_TRUE;
				if (c_char->IsCoppa)
					this->State = GAME_ARRESTED;
				else
					bac += bacIncreaseConst;
			}
			for (GameObject &object : this->Levels[this->Level].Objects)
			{
				GLboolean collision_o = CheckCollision(object, c_char);
				if (collision_o && !object.IsBeerStand && !object.IsBreadStand)
				{
					char_collision = GL_TRUE;
				}
			}
			if (char_collision)
			{
				c_char->SubMove(next_move);
			}
		}

		FinalScoreAnimationCounter = 0;
		GameTime += dt;
	}
	else if (this->State == GAME_WIN)
	{
		if (FinalScoreAnimationCounter < FinalScoreAnimationTime)
		{
			FinalScoreAnimationCounter += dt;
			if (FinalScoreAnimationCounter > FinalScoreAnimationTime)
			{
				FinalScoreAnimationCounter = FinalScoreAnimationTime;
			}
		}
	}
}

// this function creates the orthoganal projection for our "camera" and updates that matrix in the shader
void Game::UpdateCamera() {
	camera_projection = glm::ortho(
		FoV.left,
		FoV.right,
		FoV.bottom,
		FoV.top,
		-1.0f, 1.0f);	// znear, zfar are for where to clip layers along z axis.
						// (just put some boundaries to include 0 since we only have 1 layer at the moment)
	ResourceManager::GetShader("sprite").SetMatrix4("camera_projection", camera_projection);
}


// also updates world_y now. stops vertical progression such that 0 < world_y < world_end
void Game::PanCamera(GLfloat amount, Direction dir) {
	// correct movement amount based on if you extend the camera FoV past the World
	GLfloat corrected_amount = amount;

	switch (dir) {
	case UP:
		if (level_progress + amount > level_end)
			corrected_amount = abs(level_end - level_progress);
		level_progress += corrected_amount; // increment world_y by corrected amount (could be 0 if on edge already)
		break;
	case DOWN:
		if (level_progress - amount < 0.0f)
			corrected_amount = level_progress;
		level_progress -= corrected_amount; // decrement world_y by corrected amount (could be 0 if on edge already)
		break;
	case LEFT:
		if (FoV.left - amount < ZoomCameraBound.left)
			//corrected_amount = abs(ZoomCameraBound.left - FoV.left);
			corrected_amount = 0;
		break;
	case RIGHT:
		if (FoV.right + amount > ZoomCameraBound.right)
			//corrected_amount = abs(ZoomCameraBound.right - FoV.right);
			corrected_amount = 0;
		break;
	}

	// translate the camera field of view along with the player camera box
	FoV.move(corrected_amount, dir);
	PlayerCameraBox.move(corrected_amount, dir);

}

void Game::ZoomCamera(GLfloat amount) {

	//constant to scale top and bottom zoom by
	GLfloat top_bottom_constant = FoV.height;
	//constant to scale left and right zoom by
	GLfloat left_right_constant = FoV.width;
	//constant to scale top zoom by so camera zooms in on player
	GLfloat top_player_centerzoom_constant = (Player->Position.y + (Player->Size.y / 2.0f)) - FoV.top;
	//constant to scale bottom zoom by so camera zooms in on player
	GLfloat bottom_player_centerzoom_constant = FoV.bottom - (Player->Position.y + (Player->Size.y / 2.0f));
	//constant to scale left zoom by so camera zooms in on player
	GLfloat left_player_centerzoom_constant = (Player->Position.x + (Player->Size.x / 2.0f)) - FoV.left;
	//constant to scale right zoom by so camera zooms in on player
	GLfloat right_player_centerzoom_constant = FoV.right - (Player->Position.x + (Player->Size.x / 2.0f));

	//calculate amount to zoom by for each side
	GLfloat top_zoom = amount * top_bottom_constant * (top_player_centerzoom_constant / (top_player_centerzoom_constant + bottom_player_centerzoom_constant));
	GLfloat bottom_zoom = amount * top_bottom_constant * (bottom_player_centerzoom_constant / (top_player_centerzoom_constant + bottom_player_centerzoom_constant));
	GLfloat left_zoom = amount * left_right_constant * (left_player_centerzoom_constant / (left_player_centerzoom_constant + right_player_centerzoom_constant));
	GLfloat right_zoom = amount * left_right_constant * (right_player_centerzoom_constant / (left_player_centerzoom_constant + right_player_centerzoom_constant));

	//create new variables to temporarily hold the new zoom and camera box boundaries while we test their values to see if they're valid
	Boundary new_FoV;
	/*Boundary new_PlayerCameraBox;*/

    new_FoV.init(
        FoV.left + left_zoom, // upper left x
        FoV.top + top_zoom,  // upper left y
        (FoV.right - right_zoom) - (FoV.left + left_zoom), // width
        (FoV.bottom - bottom_zoom) - (FoV.top + top_zoom) // height    
    );

	//if the camera is at the top edge of the world and we have room on the bottom to zoom extra
	if ((new_FoV.top < ZoomCameraBound.top) && ((new_FoV.bottom - top_zoom) < ZoomCameraBound.bottom)) {

		new_FoV.top = ZoomCameraBound.top;
		new_FoV.y = ZoomCameraBound.top;

	}
	//if the camera is at the bottom edge of the world and we have room on the top to zoom extra
	if (((new_FoV.top + bottom_zoom) > ZoomCameraBound.top) && (new_FoV.bottom > ZoomCameraBound.bottom)) {

		new_FoV.top += bottom_zoom;
		new_FoV.y += bottom_zoom;
		new_FoV.bottom += bottom_zoom;

	}
	//if the camera is at the left edge of the world and we have room on the right to zoom extra
	if ((new_FoV.left < ZoomCameraBound.left) && ((new_FoV.right - left_zoom) < ZoomCameraBound.right)) {

		new_FoV.left = ZoomCameraBound.left;
		new_FoV.x = ZoomCameraBound.left;

	}
	//if the camera is at the right edge of the world and we have room on the left to zoom extra
	if (((new_FoV.left + right_zoom) > ZoomCameraBound.left) && (new_FoV.right > ZoomCameraBound.right)) {

		new_FoV.left += right_zoom;
		new_FoV.x += right_zoom;
		new_FoV.right += right_zoom;

	}
	//if we've are completely zoomed out in the top/bottom or left/right direction, don't allow any more zooming to preserve aspect ratio
	if (((new_FoV.left < ZoomCameraBound.left) || (new_FoV.right > ZoomCameraBound.right)) || ((new_FoV.top < ZoomCameraBound.top) || (new_FoV.bottom > ZoomCameraBound.bottom))) {
		new_FoV = FoV;
	}

	//assign new FoV and cameraBox
	FoV = new_FoV;
	UpdatePlayerCameraBox(); // updates PlayerCameraBox based off the newly set FoV
}


void Game::DoCollisions()
{
	for (CrowdCharacter* &c_char : this->Levels[this->Level].Crowd)
	{
		GLboolean collision = CheckCollision(*Player, c_char);
		if (collision)
		{
			this->Collision_detect = 1;
		}
	}
	for (GameObject &object : this->Levels[this->Level].Objects)
	{
		if (!object.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, object);
			if (collision) // If collision is true
			{

				if (object.IsBeerStand && this->Keys[GLFW_KEY_F] && !this->KeysProcessed[GLFW_KEY_F])
				{
					if ((beerLevel <= 100) && (moneyRemaining >= BeerCost))//Check for enough money and low beer
					{
						beerLevel = 100;				//Refill Beer
						moneyRemaining -= BeerCost;		//Reduce Money
					}
					this->KeysProcessed[GLFW_KEY_F] = GL_TRUE;
				}

				if (object.IsBreadStand && this->Keys[GLFW_KEY_F] && !this->KeysProcessed[GLFW_KEY_F])
				{
					if ((bac >= 0) && (moneyRemaining >= BreadCost))		//check for enough money and high enough bac
					{
						bac -= bacReduceConst;			//Reduce BAC
						moneyRemaining -= BreadCost;	//Reduce Money
						if (bac < 0)					//bac cannot go below 0
						{
							bac = 0;
						}
					}
					this->KeysProcessed[GLFW_KEY_F] = GL_TRUE;
				}

				
				if (object.IsSolid)
				{
					this->Collision_detect = 1;
				}
				else if (!object.IsSolid)
				{
					if (object.IsFinishLine)		// Win Condition
					{
						this->CalculateScore();
						this->State = GAME_WIN;
					}
				}
			}
		}
	}
}

GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
	// Collision x-axis?
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// Collision y-axis?
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// Collision only if on both axes
	return collisionX && collisionY;
}

GLboolean CheckCollision(GameObject &one, CrowdCharacter* &two)
{
	// Collision x-axis?
	bool collisionX = one.Position.x + one.Size.x >= two->Position.x &&
		two->Position.x + two->Size.x >= one.Position.x;
	// Collision y-axis?
	bool collisionY = one.Position.y + one.Size.y >= two->Position.y &&
		two->Position.y + two->Size.y >= one.Position.y;
	// Collision only if on both axes
	return collisionX && collisionY;
}

// Calculates which direction a vector is facing (N,E,S or W)
Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_INTRO)
	{
		if (this->Keys[GLFW_KEY_SPACE] && !this->KeysProcessed[GLFW_KEY_SPACE]) //Start Level
		{
			this->ResetAll();			//reset on single press
			this->State = GAME_MENU;
			KeysProcessed[GLFW_KEY_SPACE] = GL_TRUE;
		}
	}
	else if (this->State == GAME_MENU)
	{
		if (this->Keys[GLFW_KEY_SPACE] && !this->KeysProcessed[GLFW_KEY_SPACE]) //Start Level
		{
			this->ResetAll();			//reset on single press
			this->State = GAME_ACTIVE;
			KeysProcessed[GLFW_KEY_SPACE] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_A] && !this->KeysProcessed[GLFW_KEY_A])			//Change level left
		{
			this->Level = ((this->Level - 1));
			if (this->Level == -1)
				this->Level = this->NUMLEVELS - 1;
			KeysProcessed[GLFW_KEY_A] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_D] && !this->KeysProcessed[GLFW_KEY_D])			//change level right
		{
			this->Level = ((this->Level + 1));
			if (this->Level >= NUMLEVELS)
				this->Level = 0;
			KeysProcessed[GLFW_KEY_D] = GL_TRUE;
		}
	}
	else if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = basePlayerVelocity + variablePlayerVelocity ; // base speed + drunk effect
		GLfloat cameraVelocity = basePlayerVelocity * dt;
		
		/// Move player and update camera field of view variable ///

		/// left
		if (this->Keys[GLFW_KEY_A]) {
			if (Player->Position.x - velocity * dt >= PlayerCameraBox.left) {
				Player->Position.x -= velocity * dt;
				Player->Position.y -= drunkMoveAmount * dt;
				this->DoCollisions();
				if (this->Collision_detect == 1) {
					this->Collision_detect = 0; //Unflag Collision
					Player->Position.x += velocity * dt; //Move player back
					Player->Position.y += drunkMoveAmount * dt;
				}
			}
			else {
				PanCamera(cameraVelocity, LEFT);
			}
		}
		/// right
		if (this->Keys[GLFW_KEY_D]) {
			if (Player->Position.x + Player->Size.x + velocity * dt<= PlayerCameraBox.right) {
				Player->Position.x += velocity * dt;
				Player->Position.y += drunkMoveAmount * dt;
				this->DoCollisions(); 
				if (this->Collision_detect == 1) {
					this->Collision_detect = 0; //Unflag Collision
					Player->Position.x -= velocity * dt; //Move player back
					Player->Position.y -= drunkMoveAmount * dt;
				}
			}
			else {
				PanCamera(cameraVelocity, RIGHT); // handles FoV collisions
			}
		}

		/// up
		if (this->Keys[GLFW_KEY_W]) {
			
			if (Player->Position.y >= PlayerCameraBox.top) {
				Player->Position.y -= velocity * dt;
				Player->Position.x -= drunkMoveAmount * dt;

				this->DoCollisions();

				if (this->Collision_detect == 1)
				{
					this->Collision_detect = 0; //Unflag Collision
					Player->Position.y += velocity * dt; //Move player back
					Player->Position.x += drunkMoveAmount * dt;
				}
			}
			else {
				PanCamera(cameraVelocity, UP); // camera, playercamerabox, and level_progress update handled in this function
			}
		}
		
		/// down
		if (this->Keys[GLFW_KEY_S]) {
			
			if (Player->Position.y + Player->Size.y <= PlayerCameraBox.bottom) {
				Player->Position.y += velocity * dt; // larger y coords are downwards
				Player->Position.x += drunkMoveAmount * dt;

				this->DoCollisions();

				if (this->Collision_detect == 1)
				{
					this->Collision_detect = 0; //Unflag Collision
					Player->Position.y -= velocity * dt; //Move player back
					Player->Position.x -= drunkMoveAmount * dt;
				}
			}
			else {
				PanCamera(cameraVelocity, DOWN);  // camera, playercamerabox, and level_progress update handled in this function
			}
		}

		// Stop
		if ((!this->Keys[GLFW_KEY_W])&&(!this->Keys[GLFW_KEY_A])&&(!this->Keys[GLFW_KEY_S])&&(!this->Keys[GLFW_KEY_D]))
		{
			this->DoCollisions();
		}

		// tweaking player velocities. For developers only.
		/*if (this->Keys[GLFW_KEY_COMMA]) {
			variablePlayerVelocity -= 0.5;
			if (variablePlayerVelocity < 0) {
				variablePlayerVelocity = 0;
			}

		}*/
		/*if (this->Keys[GLFW_KEY_PERIOD]) {
			variablePlayerVelocity += 0.5;
			if (variablePlayerVelocity > 1600) {
				variablePlayerVelocity = 1600;
			}
		}*/

if (this->Keys[GLFW_KEY_SPACE] && !this->KeysProcessed[GLFW_KEY_SPACE]) //Single Press Detection
{
	this->ResetAll();			//reset on single press
	//this->State = GAME_MENU;
	KeysProcessed[GLFW_KEY_SPACE] = GL_TRUE;
}
	}
	else if (this->State == GAME_ARRESTED)
	{
		if (this->Keys[GLFW_KEY_SPACE] && !this->KeysProcessed[GLFW_KEY_SPACE]) //Single Press Detection
		{
			this->ResetAll();			//reset on single press
			this->State = GAME_MENU;
			KeysProcessed[GLFW_KEY_SPACE] = GL_TRUE;
		}
	}
	else if (this->State == GAME_LOSE)
	{
		if (this->Keys[GLFW_KEY_SPACE] && !this->KeysProcessed[GLFW_KEY_SPACE]) //Single Press Detection
		{
			this->ResetAll();			//reset on single press
			this->State = GAME_MENU;
			KeysProcessed[GLFW_KEY_SPACE] = GL_TRUE;
		}
	}
	else if (this->State == GAME_WIN)
	{
		if (this->Keys[GLFW_KEY_SPACE] && !this->KeysProcessed[GLFW_KEY_SPACE]) //Single Press Detection
		{
			this->ResetAll();			//reset on single press
			this->State = GAME_MENU;
			KeysProcessed[GLFW_KEY_SPACE] = GL_TRUE;
		}
	}

	// for developers not supposed to be accessible to player
	/*if (this->Keys[GLFW_KEY_EQUAL]) {
		ZoomCamera(static_cast<GLfloat>(1) * dt);
		this->UpdateCamera();
	}

	if (this->Keys[GLFW_KEY_MINUS]) {
		ZoomCamera(-static_cast<GLfloat>(1) * dt);
		this->UpdateCamera();
	}*/

	// menu button
	if (this->Keys[GLFW_KEY_BACKSPACE] && !this->KeysProcessed[GLFW_KEY_BACKSPACE]) //Start Level
	{
		this->ResetAll();			//reset on single press
		this->State = GAME_MENU;
		KeysProcessed[GLFW_KEY_BACKSPACE] = GL_TRUE;
	}

}

void Game::RenderScrollingBackground() {
	
	// TODO: maybe fix way level texture is loaded later to not be dependent on resource name?
	Texture2D current_level_background = ResourceManager::GetTexture("level"+std::to_string(Level+1)+"_background"); // loads texture based on name currently
	float scaled_width = screen_width;
	float scaled_height = (screen_width / current_level_background.Width) * current_level_background.Height;

	float mod_height = scaled_height / 2.0f;
	GLuint mod_ctr = 0;
	float mod_tracker = level_progress;
	while (mod_tracker > mod_height) {
		mod_tracker -= mod_height;
		++mod_ctr;
	}
	float move_amount = scaled_height; //+ 25; //add 25 here see breaks where tiles change for debugging (only affects background rendering)
	float init_position = level_progress - (scaled_height - screen_height);
	float render_position = init_position - move_amount * mod_ctr;
	float render_position2 = render_position - move_amount;

	// draw 2 background tiles and move them as the level_progress moves
	// for infinite scrolling in range: 0 < level_progress < level_end
	Renderer->DrawSprite(current_level_background, glm::vec2(0, render_position), glm::vec2(scaled_width, scaled_height));
	Renderer->DrawSprite(current_level_background, glm::vec2(0, render_position2), glm::vec2(scaled_width, scaled_height));
}

void Game::Render()
{
	if (this->State == GAME_INTRO)
	{
		this->ResetCamera();
		this->UpdateCamera();
		// Draw Text
		PrintText("BAR CRAWL", CENTER, 2.5f, purple);
		PrintText("PRESS 'SPACE' TO PLAY", LOWER_CENTER, 1.5f, green);
	}
	if (this->State == GAME_MENU) {
		this->ResetCamera();
		this->UpdateCamera();
		
		// Draw Text
		PrintText("LEVEL SELECT", UPPER_CENTER, 1.5f, purple);
		PrintText("PRESS 'A' and 'D' TO SELECT LEVEL", CENTER, 1.5f, blue);
		PrintText("PRESS 'SPACE' TO PLAY", LOWER_CENTER, 1.5f, green);

		if (this->Level == 0)
		{
			PrintText("LEVEL 1", BOTTOM_CENTER, 1.0f, green);
		}
		else if (this->Level == 1)
		{
			PrintText("LEVEL 2", BOTTOM_CENTER, 1.0f, yellow);
		}
		else if (this->Level == 2)
		{
			PrintText("LEVEL 3", BOTTOM_CENTER, 1.0f, red);
		}
	}
	else if (this->State == GAME_ACTIVE) {
		// Draw scrolling background
		RenderScrollingBackground();
		// Draw level
		Levels[Level].Draw(*Renderer);
		// Draw player
		Player->Draw(*Renderer);
		// Draw overlay
		Overlay->Render(*Renderer, FoV, Screen, beerLevel, bac, bacMax, moneyRemaining, StartingMoney);
		
		//Draw Text
		PrintText("Time: " + std::to_string((GLuint)GameTime), TOP_LEFT, 600*sqrt(FoV.width*FoV.height)/(static_cast<GLfloat>(screen_width*screen_height))); //600 is scaling factor, feel free to adjust as needed
		PrintText("Progress: " + std::to_string(level_progress / level_end * 100.0f) + "%", BOTTOM_LEFT, 600 * sqrt(FoV.width*FoV.height) / (static_cast<GLfloat>(screen_width*screen_height))); //600 is scaling factor, feel free to adjust as needed
	}
	else if (this->State == GAME_WIN) 
	{
		this->ResetCamera();
		this->UpdateCamera();
		PrintText("YOU WIN!!", UPPER_CENTER);
		PrintText("SCORE: " + std::to_string((GLuint)(this->FinalScore * (this->FinalScoreAnimationCounter / this->FinalScoreAnimationTime))), CENTER);

		PrintText(std::to_string(this->FinalScore), BOTTOM_CENTER);
		PrintText("PRESS 'SPACE' TO RETURN TO MENU", LOWER_CENTER);
	}
	else if (this->State == GAME_ARRESTED) {
		this->ResetCamera();
		this->UpdateCamera();
		// Draw level
		Levels[Level].Draw(*Renderer);
		// Draw player
		Player->Draw(*Renderer);

		//Draw 
		PrintText("ARRESTED FOR", UPPER_CENTER, 2.5f, orange);
		PrintText("PUBLIC", CENTER, 2.5f, red);
		PrintText("INTOXICATION", LOWER_CENTER, 2.5f, red);
		PrintText("PRESS 'SPACE' TO RETURN TO MENU", BOTTOM_CENTER, 1.0f, green);
	}
	else if (this->State == GAME_LOSE)
	{
		this->ResetCamera();
		this->UpdateCamera();
		if (this->bac >= this->bacMax)
			PrintText("PASSED OUT!", UPPER_CENTER);
		else if (this->beerLevel <= 0.0f)
			PrintText("OUT OF BEER!", UPPER_CENTER);
		//add more lose conditions here
		
		PrintText("YOU LOSE!!", LOWER_CENTER);
		PrintText("PRESS 'SPACE' TO RETURN TO MENU", BOTTOM_CENTER);
	}
	
}

// Print Text using raw x, y, scale.
void Game::PrintText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color ) {
	// call text class to render text with text shader/camera projection
	Text->RenderText(text, camera_projection, x, y, scale, color);
	// switch back to sprite shader to stop camera from going crazy
	ResourceManager::GetShader("sprite").Use();
}

void Game::PrintText(std::string text, TextLocation location, GLfloat scale, glm::vec3 color) {

	// might play with aspect ratio later to fix text while zooming, for now we'll call it a feature
	/*GLfloat aspect_ratio = Width/float(Height);
	scale = 0.7 * aspect_ratio;*/

	glm::vec2 string_size = Text->GetStringSize(text, scale);

	// calculate offsets (change as needed)
	GLfloat top_offset = FoV.height * .03f;
	GLfloat bottom_offset = FoV.height * .03f + string_size.y;
	GLfloat left_offset = FoV.width * .01f;
	GLfloat right_offset = FoV.width * .01f + string_size.x;

	GLfloat top = FoV.top + top_offset;
	GLfloat bottom = FoV.bottom - bottom_offset;
	GLfloat left = FoV.left + left_offset;
	GLfloat right = FoV.right - right_offset;

	GLfloat center_x = FoV.left + FoV.width / 2.0f - string_size.x / 2.0f;
	GLfloat center_y = FoV.top + FoV.height / 2.0f - string_size.y / 2.0f;

	GLfloat newline_offset = 2.0f*string_size.y; // offset between lines of text

	// default top left
	GLfloat x = left;
	GLfloat y = top;
	switch (location) {
		case TOP_LEFT:
			y = top;
			x = left;
			break;
		case TOP_RIGHT:
			y = top;
			x = right;
			break;
		case BOTTOM_LEFT:
			y = bottom;
			x = left;
			break;
		case BOTTOM_RIGHT:
			y = bottom;
			x = right;
			break;
		case CENTER:
			y = center_y;
			x = center_x;
			break;
		case UPPER_CENTER:
			y = center_y - newline_offset;
			x = center_x;
			break;
		case LOWER_CENTER:
			y = center_y + newline_offset;
			x = center_x;
			break;
		case BOTTOM_CENTER:
			y = bottom;
			x = center_x;
			break;
	}
	
	// call text class to render text with text shader/camera projection
	Text->RenderText(text, camera_projection, x, y, scale, color);
	// switch back to sprite shader to stop camera from going crazy
	ResourceManager::GetShader("sprite").Use(); 
}

void Game::ResetAll()
{
	GameTime = 0;
	beerLevel = 100;		
	bac = 0;				
	moneyRemaining = 100;	

	ResetLevel();
	ResetPlayer();
	ResetCamera();

	this->Render();
}


void Game::ResetLevel()
{
	if (this->Level == 0)
		this->Levels[0].Load("levels/one.lvl", screen_height, player_box_height_percent);
	else if (this->Level == 1)
		this->Levels[1].Load("levels/two.lvl", screen_height, player_box_height_percent);
	else if (this->Level == 2)
		this->Levels[2].Load("levels/three.lvl", screen_height, player_box_height_percent);

	level_end = Levels[Level].GetLevelEnd();
	ZoomCameraBound.top = -1 * level_end;
}

void Game::ResetPlayer()
{
	// Reset player
	Player->Size = PLAYER_SIZE;
	Player->Position = starting_camera_position;
	level_progress = 0.0f;
	// TODO: reset stats too?
}

// resets camera to starting position
void Game::ResetCamera() {
	FoV.init(0.0f, 0.0f, screen_width, screen_height);
	UpdatePlayerCameraBox();
}

void Game::CalculateScore()
{
	this->FinalScore = static_cast<GLuint>(this->beerDrank * this->beerConst + this->bac * this->bacConst + this->GameTime * this->timeConst);
}