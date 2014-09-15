//Nick Zheng
//Assignment 1

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

void drawSprite(GLint texture, float x, float y, float rotation){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

// The blending is making everything blend into the background to the point where barely anything is visible.
// I'm not sure how to fix it.

//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

GLuint loadTexture(const char *image_path) {
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

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;
	

	SDL_Event event;

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		glMatrixMode(GL_MODELVIEW);
		glClearColor(0.6f, 0.4f, 0.2f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLuint goldStar = loadTexture("starGold.png");
	
		float lastFrameTicks = 0.0f;
		float ticks = (float) SDL_GetTicks() / 1000.0f;
		float elasped = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		float starAngle = 0;
		starAngle += elasped;
		drawSprite(goldStar, 0.5, 0.0, starAngle);

		//polygon
		//Draw shape
		GLfloat polygon[] = {0.3f, 0.1f, -0.2f, -0.2f, 0.2f, -0.2f, -0.3f, 0.1f, 0.0f, 0.3f};
		glVertexPointer(2, GL_FLOAT, 0, polygon);
		glEnableClientState(GL_VERTEX_ARRAY);
		
		//Transformation
		glLoadIdentity();
		glTranslated(0.5, 0.0, 0.0);

		//COLORS!
		GLfloat polygonColors[] = {2.0, 1.0, 7.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.5, 0.5, 0.5};
		glColorPointer(4, GL_FLOAT, 0, polygonColors);
		glEnableClientState(GL_COLOR_ARRAY);
	
		glDrawArrays(GL_POLYGON, 0, 5);
		glDisable(GL_TEXTURE_2D);

		//square
		GLfloat quad[] = {-0.1f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f};
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);
		glLoadIdentity();
		glTranslated(1.2, 0.9, 0.0);
		GLfloat quadColors[] = {1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0};
		glColorPointer(4, GL_FLOAT, 0, quadColors);
		glEnableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
