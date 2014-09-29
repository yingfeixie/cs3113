
#include <SpaceInvader.h>

int main(int argc, char *argv[])
{
	SpaceInvader app;
	while (!app.updateAndRender()){}
	return 0;
}