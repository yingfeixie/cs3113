
#include <SpaceInvader.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

void drawText(int fontTexture, string text, float size, float spacing, float r, float g, float b, float a,float x, float y) {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
}

GLuint loadTexture(const char* imagePath) {
	SDL_Surface* surface = IMG_Load(imagePath);
	GLuint textId;
	glGenTextures(1, &textId);
	glBindTexture(GL_TEXTURE_2D, textId);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textId;
}

SpaceInvader::SpaceInvader() : bullets(MAX_BULLETS) {
	init();
	done = false;
	score = 0;
	bulletIndex = 0;

	life = 3;
	lastFrameTicks = 0.0f;
}

SpaceInvader::~SpaceInvader() {
	SDL_Quit();
}


void SpaceInvader::init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33f, 1.33f, -1.0f, 1.0f, -1.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);

	state = STATE_MAIN_MENU;

	texture = loadTexture("sheet.png");
	//SheetSprite texture(textureId, 211.0f / 1024.0f, 941.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f);
	Entity player(99.0f / 1024.0f, 75.0f / 1024.0f, 0.0f, -1.0f + 75.0f / 1024.0f, texture);//playership1blue
	player.u = 211.0f / 1024.0f;
	player.v = 941.0f / 1024.0f;
	player.speed = 0.5;
	entities.push_back(player);
	textText = loadTexture("font1.png");


	bgtexture = loadTexture("bg.PNG");
	Entity bg(2.66f, 2.0f, 0, 0, bgtexture);
	bg.u = 0.0;
	bg.v = 0.0;
	bg.speed = 0.5;
	background = bg;
	



	for (int i = -2; i <= 2; ++i) {//enemyblack1
		Entity mob(93.0f / 1024.0f, 84.0f / 1024.0f, i / 2.5f, 0.80f, texture);
		mob.u = 423.0f / 1024.0f;
		mob.v = 728.0f / 1024.0f;
		mob.directionX = -1.0f;
		mob.directionY = -1.0f;
		entities.push_back(mob);
	}
	for (int i = -2; i <= 2; ++i) {//enemyBlye2
		Entity mob(104.0f / 1024.0f, 84.0f / 1024.0f, i / 2.5f, 0.60f, texture);
		mob.u = 143.0f / 1024.0f;
		mob.v = 293.0f / 1024.0f;
		mob.directionX = -1.0f;
		mob.directionY = -1.0f;
		entities.push_back(mob);
	}
	for (int i = -2; i <= 2; ++i) {//enemygreen3
		Entity mob(103.0f / 1024.0f, 84.0f / 1024.0f, i / 2.5f, 0.40f, texture);
		mob.u = 224.0f / 1024.0f;
		mob.v = 496.0f / 1024.0f;
		mob.directionX = -1.0f;
		mob.directionY = -1.0f;
		entities.push_back(mob);
	}
	for (int i = -2; i <= 2; ++i) {//enemyred4
		Entity mob(82.0f / 1024.0f, 84.0f / 1024.0f, i / 2.5f, 0.20f, texture);
		mob.u = 520.0f / 1024.0f;
		mob.v = 577.0f / 1024.0f;
		mob.directionX = -1.0f;
		mob.directionY = -1.0f;
		entities.push_back(mob);
	}

	

}

void SpaceInvader::shoot(const Entity& shooter){
	Entity bullet(19.0f / 1024.0f, 30.0f / 1024.0f, shooter.x, shooter.y + shooter.height / 2 + 0.01, texture);
	bullet.u = 810.0f / 1024.0f;
	bullet.v = 837.0f / 1024.0f;
	if (&shooter == &entities.front()) bullet.directionY = 1.0f;
	else { bullet.directionY = -1.0f; bullet.y = shooter.y - shooter.height; }
	
	bullets[bulletIndex] = bullet;
	++bulletIndex;
	if (bulletIndex > MAX_BULLETS - 1) bulletIndex = 0;
}
bool SpaceInvader::updateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	update(elapsed);
	render(elapsed);
	return done;
}

void SpaceInvader::updateMainMenu() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)	{
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) state = STATE_GAME_LEVEL;
	}
}

void SpaceInvader::updateGameLevel(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)	{
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				shoot(entities[0]);
			}
		}
	}
	background.v -= elapsed/4;
	if (!entities.front().visible) {
		entities.front().reset();
		entities.front().visible = true;
		--life;
		--score;
		if (life == 0) {
			state = STATE_GAME_OVER;
		}
	}
	if (score == 20) state = STATE_GAME_OVER;

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_LEFT]) entities.front().directionX = -1.0f;
	else if (keys[SDL_SCANCODE_RIGHT]) entities.front().directionX = 1.0f;
	else entities.front().directionX = 0.0f;


	if ((entities.front().x - entities.front().width / 2) <= -1.33 && entities.front().directionX < 0) entities.front().directionX = 0.0f;
	else if (entities.front().x + entities.front().width / 2 >= 1.33&& entities.front().directionX > 0) entities.front().directionX = 0.0f;

	if (entities[1].x - entities[1].width / 2 <= -1.33) for (Entity& e : entities) e.directionX = 1.0f;
	else if (entities[5].x + entities[5].width / 2 >= 1.33) for (Entity& e : entities) e.directionX = -1.0f;
	for (Entity& e : entities) {
		for (Entity& b : bullets){
			if ((b.visible && e.visible) && (e.x - e.width / 2 <= b.x && b.x <= e.x + e.width / 2) &&
				(b.y + b.height / 2 >= e.y - e.height / 2 && e.y - e.height / 2 >= b.y - b.height / 2)) {
				b.visible = false;
				e.visible = false;
				++score;
			}
		}
	}


	entities.front().x += entities.front().speed* entities.front().directionX* elapsed;

	for (int i = 1; i < entities.size(); ++i) {
		entities[i].x += entities[i].speed * entities[i].directionX* elapsed;
		entities[i].y += entities[i].speed * entities[i].directionY* elapsed*0.05;
		float fpselapse = 0;
		float fpslast = lastFrameTicks;
		while (fpselapse < 1.0 / 45){
			float fpstick = (float)SDL_GetTicks() / 1000.0f;
			fpselapse += fpstick - fpslast;
			fpslast = fpstick;
		}
		int j = i;
		bool canShoot = true;
		if (entities[j].visible) {
			while (j + 5 < entities.size()) {
				if (entities[j + 5].visible)  {
					canShoot = false;
					break;
				}
				else j += 5;
			}
		}
		if (rand() % 100 >98 && (i / 16 >= 1 || canShoot) && entities[i].visible){
			shoot(entities[i]);
		}
	}
	for (Entity& b : bullets) {
		if (b.visible) {
			b.y += b.speed* b.directionY * elapsed;
		}
	}

}

void SpaceInvader::updateGameOver() {
	
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)	{
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			done = true;
		}
	}
}

void SpaceInvader::update(float elapsed){
	switch (state){
	case STATE_MAIN_MENU:
		updateMainMenu();
		break;
	case STATE_GAME_LEVEL:
		updateGameLevel(elapsed);
		break;
	case STATE_GAME_OVER:
		updateGameOver();
	}
}

void SpaceInvader::renderMainMenu(){
	glClear(GL_COLOR_BUFFER_BIT);
	drawText(textText, "press any key to start", 0.10, 0.0, 1, 1, 1, 1, -1.0, 0);
	SDL_GL_SwapWindow(displayWindow);
}

void SpaceInvader::renderGameLevel(){
	glClear(GL_COLOR_BUFFER_BIT);
	drawBg();
	stringstream ss;
	ss << "lives:" << life << "  pt:" << score;
	drawText(textText, ss.str(), 0.10, 0.0, 1, 1, 1, 1, -1.0, 0.9);
	for (Entity i : entities) { if (i.visible)i.draw(); };
	for (Entity i : bullets) { if (i.visible)i.draw(); };

	SDL_GL_SwapWindow(displayWindow);
}

void SpaceInvader::renderGameOver(){
	glClear(GL_COLOR_BUFFER_BIT);
	string text;
	if (score == 20)  text = "you won";
	else text = "you lost";
	drawText(textText, text, 0.10, 0.0, 1, 1, 1, 1, -0.4, 0);
	SDL_GL_SwapWindow(displayWindow);
}
void SpaceInvader::render(float ) {
	switch (state){
	case STATE_MAIN_MENU:
		renderMainMenu();
		break;
	case STATE_GAME_LEVEL:
		renderGameLevel();
		break;
	case STATE_GAME_OVER:
		renderGameOver();
	}

	
}

Entity::Entity(float w, float h, float initx, float inity, GLuint textureId) {
	width = w;
	height = h;
	x = initx;
	y = inity;
	texture = textureId;
	directionX = 0.0f;
	directionY = 0.0f;
	speed = 0.25f;
	rotation = 0.0f;
	visible = true;
}

void Entity::draw(float scale) {
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	GLfloat quad[] = { -width / 2, height / 2, 
					   -width / 2, -height / 2, 
					   width / 2, -height / 2, 
					   width / 2, height / 2 };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
}
void Entity::reset() {
	x = 0;
	y = -1.0f + 75.0f / 1024.0f;
}

void SpaceInvader::drawBg() {
	glLoadIdentity();


	GLfloat quad[] = { -1.33, 1,
					-1.33, -1,
					1.33, -1,
					1.33,1};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bgtexture);
	GLfloat quadUVs[] = { background.u, background.v, 
		background.u, background.v + background.height,
		background.u + background.width, background.v + background.height, 
		background.u + background.width, background.v };
	
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
}