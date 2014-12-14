//Nick Zheng
//Assignment 5 - Side Scroller

#include "App.h"

int main(int argc, char *argv[])
{
	App app;
	while (!app.updateAndRender()) {}
	return 0;
}