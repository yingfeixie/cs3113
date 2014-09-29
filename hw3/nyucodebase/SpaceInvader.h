#pragma once 

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>

#define MAX_BULLETS 50
enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
class Entity {
public:
	Entity(float w, float h, float initx, float inity, GLuint textureId);
	Entity(){}
	float width;
	float height;
	float x;
	float y;
	float rotation;
	float speed;
	float  directionX;
	float  directionY;
	bool visible;
	GLuint texture;
	float u;
	float v;
	float scale;

	void draw(float scale = 1.0f);
	void reset();
};


class SpaceInvader {
public:
	SpaceInvader();
	~SpaceInvader();

	void init();
	bool updateAndRender();
	void SpaceInvader::shoot(const Entity& shooter);
	void render(float elapsed);
	void update(float  elapsed);
	void updateMainMenu();
	void updateGameLevel(float elapsed);
	void updateGameOver();
	void renderMainMenu();
	void renderGameLevel();
	void renderGameOver();
	GLuint texture;
	GLuint bgtexture;
	GLuint textText;
	Entity background;
	void reset();
	int state;
	void drawBg();
private:
	std::vector<Entity> entities;
	std::vector<Entity> bullets;
	int bulletIndex;
	bool done;
	int score;
	int life;
	float lastFrameTicks;
	SDL_Window* displayWindow;

};