
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include<functional>
SDL_Window* displayWindow;


typedef struct {
	float x;
	float y;

	float r;
	float g;
	float b;
} Vertex2D;

void wait(float elapsed) {
	float thistime = (float)SDL_GetTicks() / 1000.0f;
	while (elapsed <= 1 / 30) {
		float nexttime = (float)SDL_GetTicks() / 1000.0f;
		elapsed += nexttime - thistime;
	}
}
class Entity {
public:
	Entity(float w, float h, float initx, float inity, float dx= 0, float dy = 0, float speed = 25.0f) : width(w), height(h), x(initx), y(inity), directionX(dx), directionY(dy) , speed(speed) {}
	float width ;
	float height ;
	float x;
	float y;
	float rotation = 0;

	float speed;
	float  directionX;
	float  directionY;
	void draw() {
		glLoadIdentity();
		glTranslatef(x, y, 0.0);
		glRotatef(rotation, 0.0, 0.0, 1.0);
		GLfloat quad[] = { -width / 2, height / 2, -width / 2, -height / 2, width / 2, -height / 2, width / 2, height / 2 };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	void reset() {
		x = 0;
		y = 0;
	}
};

void newColor() {
	float r = (rand() % 255) / 255.0;
	float g = (rand() % 255) / 255.0;
	float b = (rand() % 255) / 255.0;
	glClearColor(r, g, b, 0.5f);
}

void setup(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	newColor();
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33f, 1.33f, -1.0f, 1.0f, -1.0f, 1.0f);

}

bool processEvents(Entity& p1, Entity& p2){
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return true;
		}
		else if (event.type == SDL_KEYDOWN) {
			/*if (event.key.keysym.scancode == SDL_SCANCODE_W) {
				p1.directionY = 1.0f;
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_S){
				p1.directionY = -1.0f;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
				p2.directionY = 1.0f;
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN){
				p2.directionY = -1.0f;
			}*/
			const Uint8 *keys = SDL_GetKeyboardState(NULL);
			if (keys[SDL_SCANCODE_W])p1.directionY = 1.0f;
			else if (keys[SDL_SCANCODE_S]) p1.directionY = -1.0f;
			else p1.directionY = 0.0f;

			if (keys[SDL_SCANCODE_UP])p2.directionY = 1.0f;
			else if (keys[SDL_SCANCODE_DOWN]) p2.directionY = -1.0f;
			else p2.directionY = 0.0f;
		}
	}
}

void update(Entity& p1, Entity& p2, Entity& ball, float elapsed){
	float p1top = p1.y + p1.height / 2;
	float p1bot = p1.y - p1.height / 2;
	float p1right = p1.x + p1.width / 2;
	float p2top = p2.y + p2.height / 2;
	float p2bot = p2.y - p2.height / 2;
	float p2right = p2.x + p2.width / 2;
	float p2left = p2.x - p2.width / 2;
	float bleft = ball.x - ball.width / 2;
	float bright = ball.x + ball.width / 2;
	float btop = ball.y + ball.height / 2;
	float bbot = ball.y - ball.height / 2;


	if (p1.directionY > 0){
		if (p1top < 1.0){
			p1.y += p1.speed * elapsed * p1.directionY;
			p1.directionY = 0.0f;
		}
	}
	else if (p1.directionY < 0){
		if (p1bot > -1.0){
			p1.y += p1.speed * elapsed * p1.directionY;
			p1.directionY = 0.0f;
		}
	}

	if (p2.directionY > 0){
		if (p2top < 1.0){
			p2.y += p2.speed * elapsed * p2.directionY;
			p2.directionY = 0.0f;
		}
	}
	else if (p2.directionY < 0){
		if (p2bot > -1.0){
			p2.y += p2.speed * elapsed * p2.directionY;
			p2.directionY = 0.0f;
		}
	}


	if (bleft <= p1right) { // checking if bleft == p1right does not work??
		if ((btop > p1top && p1top >= bbot) || ((p1top >= btop) && (bbot >= p1bot)) || (btop >= p1bot && p1bot > bbot)) {
			ball.directionX = 1.0; // ball.direction *= -1.0 also doesnt work the way it should, so im hardwireing values
		}
	}
	if (bright >= p2left) {
		if ((btop > p2top && p2top >= bbot) || ((p2top >= btop) && (bbot >= p2bot)) || (btop >= p2bot && p2bot > bbot)) {
			ball.directionX = -1.0;
		}
	}
	if ( btop  >= 1.0f  ) {

		ball.directionY = -1;
	}
	if (bbot <= -1.0f) {
		ball.directionY = 1;

	}
	ball.y += ((ball.directionY)*(ball.speed)*(elapsed));
	ball.x += ball.directionX*ball.speed *elapsed;
	if (ball.x < -1.33 || ball.x > 1.33){
		newColor();
		glClear(GL_COLOR_BUFFER_BIT);

		ball.reset();
	}
	
}



void cleanup(){
	SDL_Quit();
}

void render(Entity& p1, Entity& p2, Entity& ball){

	p1.draw();
	p2.draw();
	ball.draw();

}

int main(int argc, char *argv[])
{
	Entity paddle1(0.05f, 0.5f, -1.33 + 0.05 / 2, 0), paddle2(0.05f, 0.5f, 1.33 - 0.05 / 2, 0);
	Entity ball(0.1, 0.1, 0, 0, -1, 1, 0.5f);
	setup();
	glMatrixMode(GL_MODELVIEW);
	bool done = false;
	float lastTick = 0;
	while (!done){
		done = processEvents(paddle1,paddle2);
		glClear(GL_COLOR_BUFFER_BIT);
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastTick;
		lastTick = ticks;
		update(paddle1, paddle2, ball, elapsed);
		render(paddle1, paddle2, ball);
		wait(elapsed);//somehow makes paddle more responsive

		SDL_GL_SwapWindow(displayWindow);
	}
	cleanup();
	return 0;
}