#include "App.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
float timeLeftOver = 0.0f;
float lastFrameTicks;

enum GameState {STATE_MAIN_MENU, STATE_INSTR_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};

GLuint loadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	if (!surface){return -1;}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(surface);

	return textureID;
}

//draw texts
void drawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a, float x, float y){
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;

	glLoadIdentity();
	glTranslatef(x, y, 0.0f);

	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		for(int j=0; j < 4; j++) {
			colorData.push_back(r);
			colorData.push_back(g);
			colorData.push_back(b);
			colorData.push_back(a);
		}

		vertexData.push_back(((size + spacing) * i) + (-0.5f* size)); 
		vertexData.push_back(0.5f* size);
		vertexData.push_back(((size + spacing) * i) + (-0.5f* size)); 
		vertexData.push_back(-0.5f* size);
		vertexData.push_back(((size + spacing) * i) + (0.5f* size));
		vertexData.push_back(-0.5f* size);
		vertexData.push_back(((size + spacing) * i) + (0.5f* size));
		vertexData.push_back(0.5f* size);

		texCoordData.push_back(texture_x);
		texCoordData.push_back(texture_y);
		texCoordData.push_back(texture_x);
		texCoordData.push_back(texture_y + texture_size);
		texCoordData.push_back(texture_x + texture_size);
		texCoordData.push_back(texture_y + texture_size);
		texCoordData.push_back(texture_x + texture_size);
		texCoordData.push_back(texture_y);
	}
	
	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
}

//draws the grid lines
//float x, float y, float rotation
void drawGrid(){

	GLfloat line[] = {0.750, 1.000, 0.750, -1.000}; 
	glVertexPointer(2, GL_FLOAT, 0, line);
	glEnableClientState(GL_VERTEX_ARRAY);
		
	//Transformation
//	glLoadIdentity();
//	glTranslated(x, y, rotation);

	//COLORS!
	GLfloat lineColors[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	glColorPointer(2, GL_FLOAT, 0, lineColors);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glDrawArrays(GL_LINES, 0, 2);
	glDisable(GL_TEXTURE_2D);
}

//constructor
App::App() {
	init();

	keys = SDL_GetKeyboardState(NULL);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	state = STATE_MAIN_MENU;

	font = loadTexture("spritesheet_font.png");
	characters = loadTexture("characters_1.png");
	mapSprite = loadTexture("RPGpack_sheet.png");

	done = false;
	
	glMatrixMode(GL_MODELVIEW);

}

//destructor
App::~App(){
	SDL_Quit();
}

//initializer
void App::init(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
}

//resets the game
void App::reset(){
	for(size_t i=0; i<map.size(); i++){
		delete map[i];
	}
	map.clear();

	//will be changing some tiles in the map
	//row one
	for(float i=-1.0f; i<1.0f; i+=0.125f){
	//(unsigned int spriteSheet, float x, float y, float rotate, float width, float height, bool dynamic, bool collide)
		map.push_back(new Entity(mapSprite, i, 0.875f, 0.0f, 0.125f, 0.125f));
	}
	//row two
	for(float i=-1.0f; i<1.0f; i+=0.125f){
	//(unsigned int spriteSheet, float x, float y, float rotate, float width, float height, bool dynamic, bool collide)
		map.push_back(new Entity(mapSprite, i, 0.625f, 0.0f, 0.125f, 0.125f));
	}
	//row three
	for(float i=-1.0f; i<1.0f; i+=0.125f){
	//(unsigned int spriteSheet, float x, float y, float rotate, float width, float height, bool dynamic, bool collide)
		map.push_back(new Entity(mapSprite, i, 0.375f, 0.0f, 0.125f, 0.125f));
	}
	//row four
	for(float i=-1.0f; i<1.0f; i+=0.125f){
	//(unsigned int spriteSheet, float x, float y, float rotate, float width, float height, bool dynamic, bool collide)
		map.push_back(new Entity(mapSprite, i, 0.125f, 0.0f, 0.125f, 0.125f));
	}
	//row five
	for(float i=-1.0f; i<1.0f; i+=0.125f){
	//(unsigned int spriteSheet, float x, float y, float rotate, float width, float height, bool dynamic, bool collide)
		map.push_back(new Entity(mapSprite, i, -0.125f, 0.0f, 0.125f, 0.125f));
	}
	//row six
	for(float i=-1.0f; i<1.0f; i+=0.125f){
	//(unsigned int spriteSheet, float x, float y, float rotate, float width, float height, bool dynamic, bool collide)
		map.push_back(new Entity(mapSprite, i, -0.375f, 0.0f, 0.125f, 0.125f));
	}
	//row seven
	for(float i=-1.0f; i<1.0f; i+=0.125f){
	//(unsigned int spriteSheet, float x, float y, float rotate, float width, float height, bool dynamic, bool collide)
		map.push_back(new Entity(mapSprite, i, -0.625, 0.0f, 0.125f, 0.125f));
	}
	//row eight
	for(float i=-1.0f; i<1.0f; i+=0.125f){
	//(unsigned int spriteSheet, float x, float y, float rotate, float width, float height, bool dynamic, bool collide)
		map.push_back(new Entity(mapSprite, i, -0.875f, 0.0f, 0.125f, 0.125f));
	}

	//player one units
	for(size_t i=0; i<playerOneUnits.size(); i++){
		delete playerOneUnits[i];
	}
	playerOneUnits.clear();
	playerOneUnits.push_back(new Entity(characters, -0.375f, -0.875f, 0.0f, 0.1f, 0.1f));
	playerOneUnits.push_back(new Entity(characters, -0.125f, -0.875f, 0.0f, 0.1f, 0.1f));
	playerOneUnits.push_back(new Entity(characters, 0.125f, -0.875f, 0.0f, 0.1f, 0.1f));
	playerOneUnits.push_back(new Entity(characters, 0.375f, -0.875f, 0.0f, 0.1f, 0.1f));

	//player two units
	for(size_t i=0; i<playerTwoUnits.size(); i++){
		delete playerTwoUnits[i];
	}
	playerTwoUnits.clear();
	playerTwoUnits.push_back(new Entity(characters, -0.375f, 0.875f, 0.0f, 0.1f, 0.1f));
	playerTwoUnits.push_back(new Entity(characters, -0.125f, 0.875f, 0.0f, 0.1f, 0.1f));
	playerTwoUnits.push_back(new Entity(characters, 0.125f, 0.875f, 0.0f, 0.1f, 0.1f));
	playerTwoUnits.push_back(new Entity(characters, 0.375f, 0.875f, 0.0f, 0.1f, 0.1f));
}

//renders the states
void App::render(){
	glClearColor(162.0f/255.0f, 208.0f/255.0f, 89.0f/255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	switch(state){
	case STATE_MAIN_MENU:
		renderMainMenu();
		break;
	case STATE_INSTR_MENU:
		renderInstrMenu();
		break;
	case STATE_GAME_LEVEL:
		renderGameLevel();
		break;
	case STATE_GAME_OVER:
		renderGameOver();
		break;
	}
	SDL_GL_SwapWindow(displayWindow);
}

//renders main menu
void App::renderMainMenu(){
	drawText(font, "TITLE", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.2f, 0.7f);
	drawText(font, "Press I For Instructions", 0.1f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.5f);	
	drawText(font, "Press Space to Start", 0.1f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, -0.6f, 0.3f);
}

//renders instruction menu
void App::renderInstrMenu(){
	drawText(font, "INSTRUCTIONS", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.7f);
}

//renders game level
void App::renderGameLevel(){
	//render map
	for(size_t i=0; i<map.size(); i++){
		//(int spriteTexture, int index, int spriteCountX, int spriteCountY)
		map[i]->draw(mapSprite, 46, 21, 13);
	}

	//render player one units
	playerOneUnits[0]->draw(characters, 86, 12, 8);
	playerOneUnits[1]->draw(characters, 92, 12, 8);
	playerOneUnits[2]->draw(characters, 92, 12, 8);
	playerOneUnits[3]->draw(characters, 86, 12, 8);

	//render player two units
	playerTwoUnits[0]->draw(characters, 50, 12, 8);
	playerTwoUnits[1]->draw(characters, 56, 12, 8);
	playerTwoUnits[2]->draw(characters, 56, 12, 8);
	playerTwoUnits[3]->draw(characters, 50, 12, 8);

	//draw grid
	drawGrid();

}

//renders game over
void App::renderGameOver(){
	drawText(font, "GAME OVER", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.7f);
}

//updates game level
void App::updateGameLevel(){
	//stuff goes here
}

//general update function
void App::update(){
	SDL_Event event;

	if(state == STATE_GAME_LEVEL){
		updateGameLevel();
	}

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		//menu to game
		if(state == STATE_MAIN_MENU){
			if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
				state = STATE_GAME_LEVEL;
				reset();
			}
			//closes game 
			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				SDL_Quit();
			}
		}
		//menu to instructions
		if(state == STATE_MAIN_MENU){
			if(event.key.keysym.scancode == SDL_SCANCODE_I){
				state = STATE_INSTR_MENU;
			}
			//closes game 
			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				SDL_Quit();
			}
		}
		//instructions to game
		if(state == STATE_INSTR_MENU){
			if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
				state = STATE_GAME_LEVEL;
				reset();
			}
			//closes game 
			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				SDL_Quit();
			}
		}
		//instructions to main menu
		if(state == STATE_INSTR_MENU){
			if(event.key.keysym.scancode == SDL_SCANCODE_RETURN){
				state = STATE_MAIN_MENU;
			}
			//closes game 
			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				SDL_Quit();
			}
		}
		//game to game over
		if(state == STATE_GAME_LEVEL){
			if(event.key.keysym.scancode == SDL_SCANCODE_R) {
				state = STATE_GAME_OVER;
			}
			//closes game 
			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				SDL_Quit();
			}
		}
		//game over to main menu
		if(state == STATE_GAME_OVER){
			if(event.key.keysym.scancode == SDL_SCANCODE_RETURN){
				state = STATE_MAIN_MENU;
			}
			//closes game 
			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				SDL_Quit();
			}
		}
	}
}

bool App::updateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}

	while (fixedElapsed >= FIXED_TIMESTEP){
		fixedElapsed -= FIXED_TIMESTEP;
		update();
		render();
		elapsed -= FIXED_TIMESTEP;
	}
	timeLeftOver = fixedElapsed;

	return done;
}