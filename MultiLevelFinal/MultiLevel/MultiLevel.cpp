//Nick Zheng, Anthony Wong
//Final Project

#include "App.h"

int main(int argc, char *argv[])
{
	App app;
	while (!app.updateAndRender()) {}
	return 0;
}