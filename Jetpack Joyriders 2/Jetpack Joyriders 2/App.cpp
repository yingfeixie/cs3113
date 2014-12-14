#include"App.h"
#include <sstream>
float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
	float retVal = dstMin + ((value - srcMin) / (srcMax - srcMin) * (dstMax - dstMin));
	if (retVal < dstMin) {
		retVal = dstMin;
	}
	if (retVal > dstMax) {
		retVal = dstMax;
	}
	return retVal;
}

float easeOutElastic(float from, float to, float time) {
	float p = 0.3f;
	float s = p / 4.0f;
	float diff = (to - from);
	return from + diff + (diff*pow(2.0f, -10.0f*time) * sin((time - s)*(2 * PI) / p));
}

float easeIn(float from, float to, float time) {
	float tVal = time*time*time*time*time;
	return (1.0f - tVal)*from + tVal*to;
}

float easeOut(float from, float to, float time) {
	float oneMinusT = 1.0f - time;
	float tVal = 1.0f - (oneMinusT * oneMinusT * oneMinusT *
		oneMinusT * oneMinusT);
	return (1.0f - tVal)*from + tVal*to;
}

float easeInOut(float from, float to, float time) {
	float tVal;
	if (time > 0.5) {
		float oneMinusT = 1.0f - ((0.5f - time)*-2.0f);
		tVal = 1.0f - ((oneMinusT * oneMinusT * oneMinusT * oneMinusT *
			oneMinusT) * 0.5f);
	}
	else {
		time *= 2.0;
		tVal = (time*time*time*time*time) / 2.0;
	}
	return (1.0f - tVal)*from + tVal*to;
}

void App::checkCollision(){

	for (int i = 0; i < Entities.size(); i++){
		for (int y = 0; y < Entities.size(); y++){
			if (i != y){
				if (Entities[i]->checkCollision(*Entities[y]) && Entities[y]->checkCollision(*Entities[i])){
					cout << "hello world" << endl;
				}
			}
		}
	}
};

void App::screenShake(){
	screenShakeValue += FIXED_TIMESTEP;
	glTranslatef(0.0f, sin(screenShakeValue * 25)* 0.05, 0.0f);
	if (screenShakeValue > 1) {
		for (int i = 0; i < 20; i++)
			bullets[i].collideBot = false;
		screenShakeValue = 0.0f;
	}
}

GLuint LoadTexture(const char* image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);

	return textureID;
}

App::App(){
	Init();
}

App::~App(){
	//Mix_FreeChunk(shoot);
	Mix_FreeMusic(music);
	SDL_Quit();
}

bool App::ProcessEvents(){
	//while (SDL_PollEvent(&EVENT)) {
	//	if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {
	//		done = true;;
	//	}		////HOW TO DEBUG EVERYTHING !
	//	//else if (EVENT.type == SDL_MOUSEMOTION){
	//	//	float unitX = (((float)EVENT.motion.x / 800.0f)*2.66f) - 1.33f;
	//	//	float unitY = (((float)(600 - EVENT.motion.y) / 600.0f) * 2.0f) - 1.0f;
	//	//	player.x = unitX;
	//	//	player.y = unitY;
	//	//	/*myglTranslate(unitX, unitY, player);*/
	//	//}
	//	//else if (EVENT.type == SDL_MOUSEBUTTONDOWN){
	//	//	//worldToTileCoordinates(player.x, player.y, player.gridX, player.gridY);
	//	//	checkCollision();

	//	//	cout << "This is x value: " << player.x << " and y value: " << player.y << endl;
	//	//	/*cout << "This is the gridX value: " << player.gridX << "and the gridY value: " << player.gridY << endl;*/
	//	//}

	//}
	//cant be polling all the events, otherwise the input will not be registered on different states, making them unresponsive to inputs
	return done;
}

void App::Init(){

	SDL_Event EVENT;
	currentResolutionX = 800;
	currentResolutionY = 600;
	aspect = (float)currentResolutionX / currentResolutionY;
	SDL_Init(SDL_INIT_VIDEO);//Initializes SDL
	displayWindow = SDL_CreateWindow("App", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, currentResolutionX, currentResolutionY, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?
	glViewport(0, 0, currentResolutionX, currentResolutionY);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-aspect, aspect, -1, 1, -1, 1);//The ratio of resolutions

	timeLeftOver = 0.0f;
	delay = 0.0f;
	SpriteSheetTextureID = LoadTexture("SpriteSheet.png");
	timer = 0.0f;
	timer2 = 0.0f;
	//SpriteSheetTextureID = NULL;
	animationTime = 0;
	fadeframes = 1.0;
	elapsed = 0;
	screenShakeValue = 0.0f;
	gravity_y = -.009f;
	//Player
	player.textureID = LoadTexture("Dragon2.png");
	player.spriteCountX = 2;
	player.spriteCountY = 1;
	player.index = 8;
	player.x = -.85f;
	player.y = -.65;
	player.width = .4;
	player.height = .4;
	player.scale_x = -1;
	player.rotation = 0;
	Entities.push_back(&player);
	//Floor
	for (int i = 0; i < 40; i++){
		Ast[i].textureID = LoadTexture("floor.png");
		Ast[i].spriteCountX = 1;
		Ast[i].spriteCountY = 1;
		Ast[i].index = 0;
		Ast[i].height = .3;
		Ast[i].width = .3;
		Ast[i].scale_y = 1.5;
		Ast[i].scale_x = 1.2;
		Ast[i].x = ((float)i) / 9.9 - 1.75;
		Ast[i].y = -.90;
		Ast[i].rotation = 0;
		Ast[i].velocity_x = -0.005;
		Ast[i].velocity_y = 0;
		floor.push_back(&Ast[i]);
	}
	//Player animation stuff
	for (int i = 0; i < 2; i++){
		paIndex1[i] = i;
	}
	numFrames = 4;
	currentindex = 0;
	//Particle Emitter Stuff
	ParticleEmitter temp(25);
	playerParticles = temp;
	playerParticles.position.x = -.85f;
	//Wandering background stuff
	for (int i = 0; i < 4; i++){
		Snakes[i].textureID = LoadTexture("Knight2.0.png");
		Snakes[i].spriteCountX = 3;
		Snakes[i].spriteCountY = 1;
		Snakes[i].index = 0;
		Snakes[i].height = .15;
		Snakes[i].width = .15;
		Snakes[i].y = -.825 -.025*i ;
		Snakes[i].x = 5;
		Snakes[i].rotation = 0;
		Snakes[i].velocity_x = -0.005 + (-.003*RANDOM_NUMBER);
		Snakes[i].velocity_y = .05;
		if (i % 2 == 0){
			Snakes[i].scale_x = -1;
		}
		else{
			Snakes[i].scale_x = 1;
		}
		Entities.push_back(&Snakes[i]);
	}
	//Background animation for Snakes
	for (int i = 0; i < 3; i++){
		saIndex[i] = i ;
	}
	snakescurrentindex = 0;
	//Bullets
	totalbullets = 8;
	SpriteSheetTextureID = LoadTexture("Arrows.png");
	for (int i = 0; i < totalbullets; i++){
		bullets[i].textureID = SpriteSheetTextureID;
		bullets[i].spriteCountX = 6;
		bullets[i].spriteCountY = 1;
		bullets[i].index = 0;
		bullets[i].height = .15;
		bullets[i].width = .15;
		bullets[i].x = -1.5;
		bullets[i].velocity_x = -0.04;
		bullets[i].set_x = -RANDOM_NUMBER * 200;
		bullets[i].acceleration_x = 0;
		bullets[i].rotation = 0;
		bullets[i].visible = false;
		Entities.push_back(&bullets[i]);
	}
	SpriteSheetTextureID = LoadTexture("Signal.png");
	for (int i = 0; i < totalbullets; i++){
		bulletindicators[i].textureID = SpriteSheetTextureID;
		bulletindicators[i].spriteCountX = 2;
		bulletindicators[i].spriteCountY = 1;
		bulletindicators[i].index = 1;
		bulletindicators[i].height = .075;
		bulletindicators[i].width = .075;
		bulletindicators[i].set_y = -1+2*RANDOM_NUMBER;
		bulletindicators[i].x = aspect - 0.06;
		bulletindicators[i].rotation = 0;
		bulletindicators[i].collideTop = false;
		bulletmech.push_back(&bulletindicators[i]);
	}

	//Coins
	SpriteSheetTextureID = LoadTexture("Coin.png");
	for (int i = 0; i < 5; i++){
		Coins[i].textureID = SpriteSheetTextureID;
		Coins[i].spriteCountX = 4;
		Coins[i].spriteCountY = 1;
		Coins[i].index = 0;
		Coins[i].height = .125;
		Coins[i].width = .2;
		Coins[i].x = -1.5;
		Coins[i].velocity_x = -0.01;
		Coins[i].set_x = -RANDOM_NUMBER * 100;
		Coins[i].acceleration_x = 0;
		Coins[i].rotation = 0;
		Coins[i].visible = true;
		Entities.push_back(&Coins[i]);
	}
	//if directly set state = STATE_GAME_LEVEL, the game runs fine.
	state = STATE_MAIN_MENU;
	//state = STATE_GAME_LEVEL;
	font = LoadTexture("font1.png");
	menuColors = vector<float>(2, 0.0);
	settingColors = vector<float>(5, 0.0);
	flicker = 0.0f;
	menuColorIndex = 0;
	resolutionIndex = 0;
	fullscreen = false;
	settingIndex = 0;
	done = false;
	flickerSign = 1;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("music.mp3");
	Mix_PlayMusic(music, -1);
	musicVolume = 10;
	Mix_VolumeMusic(musicVolume);
	perlinValue = 0;
	coinsCollected = 0;

	//Background
	Background[0].textureID = LoadTexture("DarkCloudy.png");
	Background[0].x = 0;
	Background[0].y = 0;
	Background[0].spriteCountX = 1;
	Background[0].spriteCountY = 1;
	Background[0].index = 0;
	Background[0].height = 2;
	Background[0].width = 2.66;
	for (int i = 1; i < 4; i++){
		Background[i].textureID = LoadTexture("Hills.png");
		Background[i].x = 0 + 2.657*(i-1);
		Background[i].y = -.5;
		Background[i].spriteCountX = 1;
		Background[i].spriteCountY = 1;
		Background[i].index = 0;
		Background[i].height = 1;
		Background[i].width = 2.66;
		Background[i].set_x = -.0075f;
	}
	
	

}

void App::fadeIn() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLfloat quad[] = { -2, 2, -2, -2, 2, -2, 2, 2 };
	GLfloat color[] = { 0.0, 0.0, 0.0, fadeframes, 0.0, 0.0, 0.0, fadeframes, 0.0, 0.0, 0.0, fadeframes, 0.0, 0.0, 0.0, fadeframes };

	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, color);
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_COLOR_ARRAY);
}

void App::FixedUpdate(){
	for (int i = 0; i < floor.size(); i++){
		if (floor[i]->x < -2){
			floor[i]->x = 2.04;
		}
		floor[i]->x += floor[i]->velocity_x*FIXED_TIMESTEP;
	}

	for (int i = 1; i < Entities.size(); i++){
		Entities[i]->velocity_x += Entities[i]->acceleration_x*FIXED_TIMESTEP;
		Entities[i]->x += Entities[i]->velocity_x*FIXED_TIMESTEP;
		if (Entities[i]->checkCollision(player) && player.checkCollision(*Entities[i])){
			Entities[i]->collideLeft = true;
		}
	}
	for (int i = 0; i < 8; i++){
		if (player.checkCollision(bullets[i]) && bullets[i].checkCollision(player)){
			player.collideLeft = true;
		}
	}

	player.velocity_y += player.acceleration_y*FIXED_TIMESTEP;
	player.velocity_y += gravity_y*FIXED_TIMESTEP;
	player.y += player.velocity_y*FIXED_TIMESTEP;
	player.acceleration_y = 0;
	if (player.y < -.8){
		player.y = -.8;
		player.collideBot = true;
		player.velocity_y = 0;
	}
	if (player.y > .95){
		player.y = .95;
		player.collideTop = true;
		player.velocity_y = 0;
	}
	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_UP]){
		player.acceleration_y = 0.9f*FIXED_TIMESTEP;// Computers too laggy to run it as it's suppose to.
	}
	if (player.collideLeft){
		//cout << "you're dead!" << endl;
	}

	//Coin motion
	for (int i = 0; i < 5; i++){
		Coins[i].y += sin(elapsed*5)/200*FIXED_TIMESTEP;
		Coins[i].rotation += FIXED_TIMESTEP * 2;
		//Coin Score
		if (player.checkCollision(Coins[i]) && Coins[i].checkCollision(player)){
			if (Coins[i].visible){
				coinsCollected++;
			}
			Coins[i].visible = false;
			

		}
	}



	player.velocity_y = lerp(player.velocity_y, 0.0f, FIXED_TIMESTEP*0.5f);
}

void drawText(int fontTexture, string text, float size, float spacing, float r, float g, float b, float a, float x, float y) {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size, ((size + spacing) * i) +
			(-0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), 0.5f
			* size });
		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);//gotta disable
	glPopMatrix();
}

void App::UpdateandRender(){
	//i guess the problem relates to where elapsed is updated and used.i tried putting this block in updateGameLEVEL but didnt work
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	/*float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEP) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEP;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;*/
	//lastFrameTicks = ticks was not used, dont know why???
	
	Update();

	Render();

	player.resetCollisions();
}

void App::Update(){
	switch (state){
	case STATE_MAIN_MENU:
		updateMainMenu();
		break;
	case STATE_SETTING:
		updateSetting();
		break;
	case STATE_GAME_LEVEL:
		updateGameLevel();
		break;
	case STATE_GAME_OVER:
		updateGameOver();
	}
}

void App::updateMainMenu(){
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
			return;
		}
		if (EVENT.type == SDL_KEYDOWN) {
			
			if (EVENT.key.keysym.scancode == SDL_SCANCODE_DOWN) {
				menuColorIndex++;
				if (menuColorIndex > 1) {
					menuColorIndex = 1;
				}
			}
			else if (EVENT.key.keysym.scancode == SDL_SCANCODE_UP){
				menuColorIndex--;
				if (menuColorIndex < 0) {
					menuColorIndex = 0;
				}
			}
			else if (EVENT.key.keysym.scancode == SDL_SCANCODE_RETURN) {

				if (menuColorIndex == 0){
					state = STATE_GAME_LEVEL;
				}
				else {
					state = STATE_SETTING;
				}
			}
		}
	}
	perlinValue += 0.001;
	flicker += flickerSign *0.001;
	if (flicker > 1 || flicker < 0) {
		flickerSign *= -1;
	}
	for (int i = 0; i < menuColors.size(); i++){
		if (i == menuColorIndex) {
			menuColors[i] = flicker;
		}
		else menuColors[i] = 1.0f;
	}

};

void App::updateSetting(){
	int numDisplay = SDL_GetNumDisplayModes(0);
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
			return;
		}
		if (EVENT.type == SDL_KEYDOWN) {
			if (EVENT.key.keysym.scancode == SDL_SCANCODE_DOWN) {
				settingIndex++;
				if (settingIndex > 4) settingIndex = 4;
			}
			else if (EVENT.key.keysym.scancode == SDL_SCANCODE_UP) {
				settingIndex--;
				if (settingIndex < 0) settingIndex = 0;
			}
			else if (EVENT.key.keysym.scancode == SDL_SCANCODE_LEFT) {
				if (settingIndex == 0) {
					resolutionIndex++;
					if (resolutionIndex > numDisplay - 1) resolutionIndex = numDisplay - 1;
				}
				else if (settingIndex == 1){
					musicVolume--;
					Mix_VolumeMusic(musicVolume);
				}
				else {
					fullscreen = true;
				}
			}
			else if (EVENT.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
				if (settingIndex == 0) {
					resolutionIndex--;
					if (resolutionIndex < 0) resolutionIndex = 0;
				}
				else if (settingIndex == 1) {
					musicVolume++;
					Mix_VolumeMusic(musicVolume);
				}
				else {
					fullscreen = false;
				}
			}
			else if (EVENT.key.keysym.scancode == SDL_SCANCODE_RETURN ){
				if (settingIndex == 3) {
					SDL_DisplayMode mode;
					SDL_GetDisplayMode(0, resolutionIndex, &mode);
					if (fullscreen) {
						SDL_SetWindowDisplayMode(displayWindow, &mode);
						SDL_SetWindowFullscreen(displayWindow, 0);
						SDL_SetWindowFullscreen(displayWindow, SDL_WINDOW_FULLSCREEN);
						SDL_ShowCursor(0);

					}
					else {
						SDL_SetWindowFullscreen(displayWindow, 0);
						SDL_SetWindowSize(displayWindow, mode.w, mode.h);
						SDL_ShowCursor(1);
					}
					cout << mode.w << ' ' << mode.h << endl;
					currentResolutionX = mode.w;
					currentResolutionY = mode.h;
					aspect = (float)currentResolutionX / currentResolutionY;
					glViewport(0, 0, currentResolutionX, currentResolutionY);
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();//have to set identity before setting ortho or the new ortho is set based on previous ortho.
					glOrtho(-aspect , aspect, -1.0f, 1.0f, -1.0f, 1.0f);					
				}
				else if (settingIndex == 4) {
					state = STATE_MAIN_MENU;
				}
			}
		}
	}
	flicker += flickerSign *0.001;
	if (flicker > 1 || flicker < 0) {
		flickerSign *= -1;
	}
	for (int i = 0; i < settingColors.size(); i++){
		if (i == settingIndex) {
			settingColors[i] = flicker;
		}
		else settingColors[i] = 1.0f;
	}
};

void App::updateGameLevel(){

	//Update() is changed to updateGameLevel(),definition not changed. but not working properly\
			have to figure out where to put fixedupdate in .
	//do a poll event to check quit and input.
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;;
		}
	}

	/*float ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;*/
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEP) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEP;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;
	//lastFrameTicks = ticks was not used, dont know why???


	actualElapsed = elapsed - delay;
	timer += actualElapsed;
	timer2 += actualElapsed;
	//Walking animation		
	if (timer > .2) {
			currentindex++;
			timer = 0.0;
			if (currentindex > numFrames - 1) {
				currentindex = 0;
			}
		}
	player.index = paIndex1[currentindex];

	if (!player.collideBot){
		playerParticles.position.y = player.y -.05;
		playerParticles.position.x = player.x - 0.02;
	}
	//Snake movement, death, and respawn.
	for (int i = 0; i < 6; i++){
		if ((Snakes[i].x - player.x) < .6 && player.collideBot){
			Snakes[i].velocity_x = -.003 - (0.001*actualElapsed);
			Snakes[i].scale_x = 1;
		}
		if (timer2 > .2) {
			snakescurrentindex++;
			timer2 = 0.0;
			if (snakescurrentindex > numFrames - 1) {
				snakescurrentindex = 0;
			}
		}
		Snakes[i].index = saIndex[snakescurrentindex];

		if (Snakes[i].x < -1.5 || (Snakes[i].y < -1.2)){
			Snakes[i].x = 4 + RANDOM_NUMBER;
			Snakes[i].y = -.825f - .12*RANDOM_NUMBER;
			Snakes[i].rotation = 0;
			Snakes[i].velocity_y = .05;
			if (RANDOM_NUMBER * 2 > 1){
				Snakes[i].scale_x = -1;
				Snakes[i].velocity_x = -0.005 + (-.003*RANDOM_NUMBER);
			}
			else
			{
				Snakes[i].velocity_x = -0.005 + (-.001*RANDOM_NUMBER);
			}
			Snakes[i].resetCollisions();
			Snakes[i].scale_y = 1;
		}
		if (Snakes[i].collideLeft){
			Snakes[i].velocity_x = .002;
			Snakes[i].y += Snakes[i].velocity_y*FIXED_TIMESTEP;
			Snakes[i].velocity_y += gravity_y*FIXED_TIMESTEP;
			Snakes[i].rotation += 10 * FIXED_TIMESTEP;
		}
	}
	//Speed increase
	for (int i = 1; i < Entities.size(); i++){
		Entities[i]->velocity_x += (-.0002*actualElapsed);
	}
	for (int i = 0; i < floor.size(); i++){
		floor[i]->velocity_x += (-.0001*actualElapsed);
	}
	//Particles
	playerParticles.Update(timeLeftOver);
	//Bullets
	for (int i = 0; i < totalbullets; i++){
		if (bulletindicators[i].collideTop){
			bullettimers[i] += actualElapsed;
			if (bullettimers[i] <= 2.5){
				float animationValue = mapValue(bullettimers[i], 0, 1750, 0.0f, 1.0);
				bulletindicators[i].y = bulletindicators[i].set_y;
				bulletindicators[i].set_y = lerp(bulletindicators[i].y, player.y, animationValue);
			}
			else
			{
				bullettimers[i] = 0;
				bullets[i].x = 2.5;
				bullets[i].y = bulletindicators[i].set_y;
				bulletindicators[i].collideTop = false;
				bullets[i].velocity_x = -0.04;
				bullets[i].visible = true;
				bulletindicators[i].index = 1;
			}
		}
		if (bullets[i].x < bullets[i].set_x){
			bulletindicators[i].collideTop = true;
		}
	}
	//Added Bullet Animations
	for (int i = 0; i < totalbullets; i++){


		//Stupid fix for unknown issue.
		if (!bulletindicators[i].collideTop){
			bulletindicators[i].visible = false;
		}
		else{
			bulletindicators[i].visible = true;
		}
		//Animations
		if (bullets[i].visible){
			bulletanimationtimers[i] += actualElapsed;
			if (bulletanimationtimers[i] > .035) {
				bulletindex[i]++;
				bulletanimationtimers[i] = 0.0;
			}
			if (bulletindex[i] > 5){
				bulletindex[i] = 0;
			}
			bullets[i].index = bulletindex[i];
		}
		if (bulletindicators[i].collideTop){
			indanimationtimer[i] += actualElapsed;
			if (indanimationtimer[i] > 2.3) {
				bulletindicators[i].index = 0;
				indanimationtimer[i] = 0.0;
			}

		}
	}
	//Coin Animation and Movement
	for (int i = 0; i < 5; i++){
		cointimer[i] += actualElapsed;
		if (cointimer[i] > .2){
			cointimer[i] = 0;
			Coins[i].index++;
			if (Coins[i].index > 4){
				Coins[i].index = 0;
			}
		}
		if (Coins[i].x < Coins[i].set_x){
			Coins[i].x = 2;
			Coins[i].y = -0.75f + RANDOM_NUMBER*1.5f;
			Coins[i].visible = true;
		}

	}
	//Background move
	for (int i = 1; i < 4; i++){

		Background[i].x += Background[i].set_x*FIXED_TIMESTEP;

		if (Background[i].x < -3.99){
			Background[i].x = 5.32;
		}
	}

	delay = elapsed;
}

void App::updateGameOver(){};

void App::Render() {
	switch (state){
	case STATE_MAIN_MENU:
		renderMainMenu();
		break;
	case STATE_SETTING:
		renderSetting();
		break;
	case STATE_GAME_LEVEL:
		renderGameLevel();
		break;
	case STATE_GAME_OVER:
		renderGameOver();
	}


}

void App::renderMainMenu(){
	glClear(GL_COLOR_BUFFER_BIT);
	string text = "Insert game name here";
	float size = (float)currentResolutionX / currentResolutionY * 2 / 40;
	float space = 0.0f;
	float x = 0 - (float)text.size()*size / 2;
	float y = 1.0 / 4;
	float per = noise1(perlinValue)*0.1 ; 
	drawText(font, text, size, space, .5, .7, 1, 1, x, 1 - 2 * y+per);

	animationAValue = mapValue(elapsed, 0.0f, 5.0f, 0.0f, 1.0f);

	text = "Start";
	x = 0 - (float)text.size()*size / 2;
	float bounce = easeOutElastic(-aspect, x, animationAValue);
	drawText(font, text, size, space, menuColors[0], menuColors[0], menuColors[0], menuColors[0], bounce, 0 - y);
	text = "Setting";
	x = 0 - (float)text.size()*size / 2;
	bounce = easeOutElastic(aspect, x, animationAValue);
	drawText(font, text, size, space, menuColors[1], menuColors[1], menuColors[1], menuColors[1], bounce, 0 - 2 * y);
	SDL_GL_SwapWindow(displayWindow);

};

void App::renderSetting(){
	glClear(GL_COLOR_BUFFER_BIT);
	string text;
	SDL_DisplayMode mode;
	SDL_GetDisplayMode(0, resolutionIndex, &mode);
	stringstream s;
	s << mode.w << 'x' << mode.h;
	text = s.str().c_str();
	float size = (float)currentResolutionX / currentResolutionY * 2 / 40;
	float space = 0.0f;
	float x = 0 - (float)text.size()*size / 2;
	float y = 2.0 / 6;
	drawText(font, text, size, space, settingColors[0], settingColors[0], settingColors[0], 1, x, 1 - y);

	stringstream t;
	t << "Music Volume: " << musicVolume;
	text = t.str().c_str();
	x = 0 - (float)text.size()*size / 2;
	drawText(font, text, size, space, settingColors[1], settingColors[1], settingColors[1], 1, x, 1 - 2 * y);

	if (fullscreen) text = "Full screen";
	else text = "Windowed";
	x = 0 - (float)text.size()*size / 2;
	drawText(font, text, size, space, settingColors[2], settingColors[2], settingColors[2], 1, x, 1 - 3 * y);

	text = "Confirm";
	x = 0 - (float)text.size()*size / 2;
	drawText(font, text, size, space, settingColors[3], settingColors[3], settingColors[3], 1, x, 1 - 4 * y);

	text = "Back";
	x = 0 - (float)text.size()*size / 2;
	drawText(font, text, size, space, settingColors[4], settingColors[4], settingColors[4], 1, x, 1 - 5 * y);

	SDL_GL_SwapWindow(displayWindow);
};

void App::renderGameLevel(){
	//render changed to renderGameLEvel
	glClearColor(55.0f / 255.0f, 84.0f / 255.0f, 229.0f / 255.0f, 1.0f);//Determines default coloring
	glClear(GL_COLOR_BUFFER_BIT);//Makes background default color

	//fadeIn();
	for (int i = 0; i < 4; i++){
		Background[i].Render();
	}
	for (int i = 0; i < floor.size(); i++){
		floor[i]->Render();
	}
	if (!player.collideBot){
		playerParticles.Render();
	}

	for (int i = 0; i < bulletmech.size(); i++){
		bulletmech[i]->Render();
	}

	for (int i = 0; i < Entities.size(); i++){
		Entities[i]->Render();
	}

	stringstream s;
	s << "Coins: " << coinsCollected;
	string text = s.str().c_str();
	float size = 0.1;
	float space = 0;
	float x = -aspect;

	drawText(font, text, size, space, 1, 1, 1,1 , x+0.1, 1-size/2);

	SDL_GL_SwapWindow(displayWindow);
}

void App::renderGameOver(){};

