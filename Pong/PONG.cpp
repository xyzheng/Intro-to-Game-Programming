//Nick Zheng
//Assignment #2 - PONG!

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

	GLfloat quad[] = {-0.1f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}


//borders
void drawBlocks(GLint texture, float x, float y, float rotation){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = {-3.0f, 0.1f, -3.0f, -0.1f, 3.0f, -0.1f, 3.0f, 0.1f};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

//Loads texture
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
	//Setup
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	GLint ballGrey = loadTexture("ballGrey.png");
	GLint paddleRed = loadTexture("paddleRed.png");
	GLint paddleBlu = loadTexture("paddleBlu.png");
	GLint blocks = loadTexture("element_grey_rectangle.png");

	//initializing variables
	float redPosX = 1.0;
	float redPosY = 0.0;
	float bluePosX = -1.0;
	float bluePosY = 0.0;
	float ballPosY = 0.0;
	float ballPosX = 0.0;
	int sign = 0;
	
	bool done = false;
	
	SDL_Event event;
		
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	//Keyboard Up, down, W, and S
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			if(keys[SDL_SCANCODE_UP] && redPosY < 0.8f){
				redPosY += 0.1f;
			}
			else if(keys[SDL_SCANCODE_DOWN] && redPosY > -0.8){
				redPosY -= 0.1f;
			}	
			if(keys[SDL_SCANCODE_W] && bluePosY < 0.8){
				bluePosY += 0.1f;
			}
			else if(keys[SDL_SCANCODE_S] && bluePosY > -0.8){
				bluePosY -= 0.1f;
			}

		}
	

		glClearColor(162.0f/255.0f, 208.0f/255.0f, 89.0f/255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float lastFrameTicks = 0.0f;
		float ticks = (float)SDL_GetTicks()/1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		ballPosX = elapsed;

		//The blue seems to go up and down much faster than the red, not sure why. 
		//I checked the values and they seem fine.
		drawSprite(ballGrey, ballPosX, ballPosY, 0.0);
		drawSprite(paddleRed, redPosX, redPosY, 0.0);
		drawSprite(paddleBlu, bluePosX, bluePosY, 0.0);
		drawBlocks(blocks, 0.0, 1.0, 0.0);
		drawBlocks(blocks, 0.0, -1.0, 0.0);

		//check collision
		//red paddle
/*		if(!((ballPosX - 0.1)>(redPosX + 0.1)) && !((ballPosX + 0.1)<(redPosX - 0.1)) && !((ballPosY - 0.1)>(redPosY + 0.1)) && !((ballPosY + 0.1)<(redPosY -0.1))){
			//reverse directions
			ballPosX = -0.1f*elapsed;
		}
		else{
			//reverse directions
			ballPosX = elapsed;
		}
		//blue paddle
		if(!((ballPosX - 0.1)>(bluePosY + 0.1)) && !((ballPosX + 0.1)<(bluePosX - 0.1)) && !((ballPosY - 0.1)>(bluePosY + 0.1)) && !((ballPosY + 0.1)<(redPosY - 0.1))){
			//reverse directions
			ballPosX = 0.1f*elapsed;
		}
		else{
			//reverse directions
			ballPosX = -0.1f*elapsed;
		}
		//borders
		if(ballPosY >= 0.8 || ballPosY <= -0.8){
			//reverse directions
		}
*/		
		
		//win conditions
		if(ballPosX > 1.33){
			GLint medal = loadTexture("shaded_medal8.png");
			drawSprite(medal, -0.5, 0.5, 0.0);
		}
		else if(ballPosX < -1.33){
			GLint medal = loadTexture("shaded_medal8.png");
			drawSprite(medal, 0.5, 0.5, 0.0);
		}

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
