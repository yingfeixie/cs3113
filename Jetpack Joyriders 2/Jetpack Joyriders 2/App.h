#pragma once
#include"MatrixEntities.h"
#include"ParticleSystem.h"
#include <SDL_mixer.h>
#define FIXED_TIMESTEP 0.016667
#define MAX_TIMESTEP 6
#include "PerlinNoise.h"


class App{
public:
	App();
	~App();

	SDL_Event EVENT;

	void Init();
	bool ProcessEvents();
	void UpdateandRender();
	void FixedUpdate();
	void Render();
	void Update();

	void updateMainMenu();
	void updateSetting();
	void updateGameLevel();
	void updateGameOver();
	void renderMainMenu();
	void renderSetting();
	void renderGameLevel();
	void renderGameOver();
	vector<float> menuColors;
	vector<float> settingColors;
	float flicker;
	int menuColorIndex;
	int settingIndex;
	int resolutionIndex;
	int gameOverfIndex;
	vector<float> gameOverColors;
	float p1hitCooldown;
	float p2hitCooldown;
	void fadeOut();
	float frameAlpha;


	int currentResolutionX;
	int currentResolutionY;
	bool fullscreen;
	int flickerSign;
	bool done;
	enum GameState { STATE_MAIN_MENU,STATE_SETTING, STATE_GAME_LEVEL, STATE_GAME_OVER };
	int state;
	Mix_Music *music;
	float perlinValue;
	float aspect;
	float animationAValue;
	float musicVolume;
	GLuint font;
	float lastFrameTicks;
	float elapsed, delay, actualElapsed, timer, timer2,timer3;
	GLuint hillID;
	void drawBg(GLuint id);
	float bgv;
	float bgu;
	float fgu;
	float fgv;

	float uniSpawnCounter;
	bool spawnUni;
	GLuint SpriteSheetTextureID;

	vector<Entity*> Entities;
	vector<Entity*> floor;

	Entity player;
	Entity player2;

	int player1hp;
	int player2hp;
	bool pause;
	bool invincible;
	float invincibleTimer;
	float invincibleStart;
	GLuint mainScreen;

	int paIndex1[4];
	int numFrames;
	int currentindex;

	ParticleEmitter playerParticles;
	ParticleEmitter player2Particles;

	Entity Ast[40];
	Entity Snakes[4];
	Entity Background[4];
	Entity fireSprite;
	Entity Unicorns[4];
	float unitimer[4];
	Mix_Chunk *coinsound, *hitsound,*kdsound; 

	int saIndex[4];
	int snakescurrentindex;

	float gravity_y;

	int totalbullets;
	vector<Entity*> bulletmech;
	Entity bulletindicators[16];
	Entity bullets[16];
	float bullettimers[16];
	float bulletanimationtimers[16];
	float indanimationtimer[16];
	unsigned int bulletindex[16];

	Entity Coins[10];
	float cointimer[10];
	int coinsCollected;
	int coinsCollected2;

	const Uint8* keys;
	SDL_Window* displayWindow;

	float timeLeftOver;

	void checkCollision();
	void fadeIn();
	float fadeframes;

	//float perlinValue;
	void screenShake();
	float screenShakeValue;

	float animationTime;
	float animationStart;
	float animationEnd;


};