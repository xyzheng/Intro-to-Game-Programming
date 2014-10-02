#include "App.h"

int main(int argc, char *argv[])
{
	App anApp;
	while(!anApp.updateAndRender()){}
	return 0;
}