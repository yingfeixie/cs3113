
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
SDL_Window* displayWindow;

typedef struct {
	float x;
	float y;

	float r;
	float g;
	float b;
} Vertex2D;

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

void DrawSprite(GLint texture, float x, float y, float rotation) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0,quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;

	SDL_Event event;

	glViewport(0, 0, 800, 600);
	glClearColor(0.4f, 0.2f, 0.4f, 0.0f);
	
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33f, 1.33f, -1.0f, 1.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	GLuint textId1, textId2, textId3;
	textId1 = LoadTexture("1.png");
	textId2 = LoadTexture("2.png");
	textId3 = LoadTexture("3.png");

	GLfloat s1[] = { -1.33f, 0.5f, -1.33f, -0.5f, -0.42f, -0.5f, -0.42f, 0.5f };
	GLfloat color1[] = { 1.0f, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.1, 1.0, 1.0, 1.0 };
	glColorPointer(3, GL_FLOAT, 0, color1);
	
	GLfloat s2[] = { -0.44f, 0.5f, -0.44f, -0.5f, 0.44f, -0.5f, 0.44f, 0.5f };
	GLfloat s3[] = { 0.44f, 0.5f, 0.44f, -0.5f, 1.33f, -0.5f, 1.33f, 0.5f };
	//GLfloat s3[] = { -0.44f, 0.5f, -0.44f, -0.5f, 0.44f, -0.5f, 0.44f, 0.5f };
	float lastFrameTicks = 0.0f;
	float angle = 0.0f;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glEnableClientState(GL_COLOR_ARRAY);
		DrawSprite(textId1, -0.88, 0, 0);
		glDisableClientState(GL_COLOR_ARRAY);
		float ticks = (float)SDL_GetTicks() / 1000.f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		angle += elapsed;
		DrawSprite(textId2, 0, 0, 0);
		
		DrawSprite(textId3, 0.88, 0, angle);
		
		SDL_GL_SwapWindow(displayWindow);
	}
	
	SDL_Quit();
	return 0;
}