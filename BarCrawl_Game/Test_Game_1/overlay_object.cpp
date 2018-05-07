#include "overlay_object.h"
#include "resource_manager.h"
#include "game_object.h"
#include "game.h"

OverlayObject::OverlayObject()
	: GameObject() { }


void OverlayObject::Render(SpriteRenderer &renderer, Boundary &screen, Boundary &world, GLfloat beerR, GLfloat bacR, GLfloat bacMax, GLfloat money, GLfloat StartingMoney)
{
	Texture2D	BarSprite		= ResourceManager::GetTexture("block");
	Texture2D	BarFullSprite	= ResourceManager::GetTexture("block");
	Texture2D	BeerSprite		= ResourceManager::GetTexture("beer_sprite");
	Texture2D	BloodSprite		= ResourceManager::GetTexture("blood_sprite");
	Texture2D	MoneySprite		= ResourceManager::GetTexture("money_sprite");

	GLfloat x_aspect = screen.width / world.width;
	GLfloat y_aspect = screen.height / world.height;

	const GLfloat barFullWidth = 24;
	const GLfloat barFullHeight = 204;
	const GLfloat barWidth = 20;
	const GLfloat barHeight = 200;
	const GLfloat barSeperation = 15;
	const GLfloat beerspriteWidth = 30;
	const GLfloat beerspriteHeight = 28;
	const GLfloat bloodspriteWidth = 20;
	const GLfloat bloodspriteHeight = 25;
	const GLfloat moneyspriteWidth = 35;
	const GLfloat moneyspriteHeight = 30;

	glm::vec2 BeerBar_Size		= glm::vec2(barWidth * x_aspect, barHeight * beerR / 100.0f * y_aspect);
	glm::vec2 BeerBar_Pos		= glm::vec2(screen.right - (barSeperation + barWidth)*x_aspect, screen.top + (barSeperation * y_aspect - BeerBar_Size.y  + barHeight * y_aspect));
	glm::vec2 BeerBarFull_Size	= glm::vec2(barFullWidth * x_aspect, barFullHeight * y_aspect);
	glm::vec2 BeerBarFull_Pos	= glm::vec2(BeerBar_Pos.x - (barFullWidth - barWidth) / 2.0f * x_aspect, screen.top + (barSeperation - ((barFullHeight - barHeight) / 2.0f)) * y_aspect);
	glm::vec2 BeerSprite_Size	= glm::vec2(beerspriteWidth * x_aspect, beerspriteHeight * y_aspect);
	glm::vec2 BeerSprite_Pos	= glm::vec2(BeerBar_Pos.x + BeerBar_Size.x / 2.0f - BeerSprite_Size.x / 2.0f, screen.top + (barHeight + (0.5f * barSeperation) + (moneyspriteHeight / 2.0f))* y_aspect);

	glm::vec2 BACBar_Size		= glm::vec2(barWidth * x_aspect, barHeight * bacR / bacMax * y_aspect);
	glm::vec2 BACBar_Pos		= glm::vec2(screen.right - 2*(barSeperation + barWidth)*x_aspect, screen.top + (barSeperation * y_aspect - BACBar_Size.y + barHeight * y_aspect));
	glm::vec2 BACBarFull_Size	= glm::vec2(barFullWidth * x_aspect, barFullHeight * y_aspect);
	glm::vec2 BACBarFull_Pos	= glm::vec2(BACBar_Pos.x - (barFullWidth - barWidth) / 2.0f * x_aspect, screen.top + (barSeperation - ((barFullHeight - barHeight) / 2.0f)) * y_aspect);
	glm::vec2 BACSprite_Size	= glm::vec2(bloodspriteWidth * x_aspect, bloodspriteHeight * y_aspect);
	glm::vec2 BACSprite_Pos		= glm::vec2(BACBar_Pos.x + BACBar_Size.x / 2.0f - BACSprite_Size.x / 2.0f, screen.top + (barHeight + (0.5f * barSeperation) + (moneyspriteHeight / 2.0f))* y_aspect);

	glm::vec2 MoneyBar_Size		= glm::vec2(barWidth * x_aspect, barHeight * money / StartingMoney * y_aspect);
	glm::vec2 MoneyBar_Pos		= glm::vec2(screen.right - 3*(barSeperation + barWidth)*x_aspect, screen.top + (barSeperation * y_aspect - MoneyBar_Size.y + barHeight * y_aspect));
	glm::vec2 MoneyBarFull_Size = glm::vec2(barFullWidth * x_aspect, barFullHeight * y_aspect);
	glm::vec2 MoneyBarFull_Pos	= glm::vec2(MoneyBar_Pos.x - (barFullWidth - barWidth) / 2.0f * x_aspect, screen.top + (barSeperation - ((barFullHeight - barHeight) / 2.0f)) * y_aspect);
	glm::vec2 MoneySprite_Size	= glm::vec2(moneyspriteWidth * x_aspect, moneyspriteHeight * y_aspect);
	glm::vec2 MoneySprite_Pos	= glm::vec2(MoneyBar_Pos.x + MoneyBar_Size.x / 2.0f - MoneySprite_Size.x / 2.0f, screen.top + (barHeight + (0.5f * barSeperation) + (moneyspriteHeight / 2.0f))* y_aspect);


	//Draw boxes
	renderer.DrawSprite(BarFullSprite, BeerBarFull_Pos, BeerBarFull_Size, 0, glm::vec3(1.0f, 1.0f, 1.0f));
	renderer.DrawSprite(BarFullSprite, BACBarFull_Pos, BACBarFull_Size, 0, glm::vec3(1.0f, 1.0f, 1.0f));
	renderer.DrawSprite(BarFullSprite, MoneyBarFull_Pos, MoneyBarFull_Size, 0, glm::vec3(1.0f, 1.0f, 1.0f));

	// Draw status Bars
	renderer.DrawSprite(BarSprite, BeerBar_Pos, BeerBar_Size, 0, glm::vec3(0.0f, 0.0f, 1.0f));
	renderer.DrawSprite(BarSprite, BACBar_Pos, BACBar_Size, 0, glm::vec3(1.0f, 0.0f, 0.0f));
	renderer.DrawSprite(BarSprite, MoneyBar_Pos, MoneyBar_Size, 0, glm::vec3(0.0f, 1.0f, 0.0f));

	// Draw Label Sprites
	renderer.DrawSprite(BeerSprite, BeerSprite_Pos, BeerSprite_Size, 0, glm::vec3(1.0f, 1.0f, 1.0f));
	renderer.DrawSprite(BloodSprite, BACSprite_Pos, BACSprite_Size, 0, glm::vec3(1.0f, 1.0f, 1.0f));
	renderer.DrawSprite(MoneySprite, MoneySprite_Pos, MoneySprite_Size, 0, glm::vec3(1.0f, 1.0f, 1.0f));
}
