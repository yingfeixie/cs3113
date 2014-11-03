#include "Astroid.h"

int main(int argc, char *argv[])
{
	Astroid app;
	while (!app.updateAndRender()){}
	return 0;
}