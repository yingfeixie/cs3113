
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
SDL_Window* displayWindow;
int globalResX = 800;
int globalResY= 600;

typedef struct {
	float x;
	float y;

	float r;
	float g;
	float b;
} Vertex2D;

class Entity {
public:

	void draw();
	float width = 0.05;
	float height = 0.5;
	float x = 1.33 - width/2;
	float y = 0;
	float rotation = 0;

	int textID;


	float speed =1;
		float  directionX, directionY;
};

void setup(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33f, 1.33f, -1.0f, 1.0f, -1.0f, 1.0f);

}

bool processEvents(){
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return true;
		}
		else if (event.type == SDL_KEYDOWN) {
			return event.key.keysym.scancode;
		}
	}
}
void update(Entity& p1, Entity& p2, Entity& ball,float elapsed){
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
				p1.y += p1.speed*elapsed*100;
			}
		}
	}
}


void cleanup(){
	SDL_Quit();
}

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
	surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}

void DrawSprite( float x, float y, float rotation, float l, float w) {
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	GLfloat quad[] = { -w/2, l/2, -w/2, -l/2, w/2, -l/2, w/2, l/2 };
	//GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	//GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	//glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	//glDisable(GL_TEXTURE_2D);
}

void render(Entity& p1, Entity& p2, Entity& ball){
	glMatrixMode(GL_MODELVIEW);
	DrawSprite(-p1.x, p1.y, p1.rotation, p1.height, p1.width);
	
	DrawSprite(p2.x, p2.y, p2.rotation, p2.height, p2.width);

}

int main(int argc, char *argv[])
{
	Entity paddle1, paddle2, ball;
	setup();
	bool done = false;
	float lastTick = 0;
	while (!done){ 
		done = processEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastTick;
		lastTick = ticks;
		update(paddle1, paddle2, ball, elapsed);
		render(paddle1, paddle2, ball);
		
		
		//render(paddle1,paddle2,ball);
		
		SDL_GL_SwapWindow(displayWindow);
	}
	cleanup();
	return 0;
}