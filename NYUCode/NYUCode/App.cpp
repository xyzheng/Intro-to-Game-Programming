//Nick Zheng
//Assignment 6 - Asteroids
#include "App.h"

enum GameState{STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define MAX_ASTEROIDS 20

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

App::App(){
	init();

	keys = SDL_GetKeyboardState(NULL);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);

	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	state = STATE_GAME_LEVEL;

	sheetSprite = loadTexture("sheet.png");
	font = loadTexture("spritesheet_font.png");

	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	hitCount = 3;

	done = false;

	glMatrixMode(GL_MODELVIEW);

	reset();
}

App::~App(){
	SDL_Quit();
}

void App::init(){

	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

}

void App::reset(){
	for(size_t i=0; i<asteroids.size(); i++){
		delete asteroids[i];
	}
	asteroids.clear();

	//(unsigned int textureID, float u, float v, float width, float height, float xScale, float yScale);
	player = Entity(sheetSprite, 211.0f/1024.0f, 941.0f/1024.0f, 99.0f/1024.0f, 75.0f/1024.0f, 1.0f, 1.0f);
	bullet = Entity(sheetSprite, 856.0f/1024.0f, 421.0f/1024.0f, 9.0f/1024.0f, 54.0f/1024.0f, 1.0f, 1.0f);

	player.x = 0.0f;
	player.y = 0.0f;

	bullet.visible = false;

	//need asteroids
	for(int i=0; i<MAX_ASTEROIDS; i++){
		asteroids.push_back(new Entity(sheetSprite, 406.0f/1024.0f, 262.0f/1024.0f, 28.0f/1024.0f, 28.0f/1024.0f, 5.0f, 5.0f));
		asteroids[i]->x = (float)rand() / (float)RAND_MAX*1.33f;
		asteroids[i]->y = (float)rand() / (float)RAND_MAX*1.0f;

		asteroids[i]->generateRandom();
	}
}

//renders the states
void App::render(){
	glClearColor(162.0f/255.0f, 208.0f/255.0f, 89.0f/255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	renderGameLevel();
	
	//switch(state){
	//case STATE_MAIN_MENU:
	//	renderMainMenu();
	//	break;
	//case STATE_GAME_LEVEL:
	//	renderGameLevel();
	//	break;
	//case STATE_GAME_OVER:
	//	renderGameOver();
	//	break;
	//}
	
	SDL_GL_SwapWindow(displayWindow);
}

void App::renderMainMenu(){
	
	drawText(font, "ASTEROIDS", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.7f, 0.7f);
	drawText(font, "ARROW KEYS TO MOVE", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.5f);	
	drawText(font, "SPACE TO SHOOT", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.7f, 0.3f);
	drawText(font, "R FOR GAME OVER", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.8f, 0.1f);
	drawText(font, "Press SPACE to start", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.9f, -0.3f);
	
}

void App::renderGameLevel(){
	player.draw();
	if(bullet.visible){
		bullet.draw();
	}
	for(int i=0; i<MAX_ASTEROIDS; i++){
		asteroids[i]->draw();
	}
}

void App::renderGameOver(){
	drawText(font, "GAME OVER", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.7f);
	//drawText(font, "Score:" + to_string(score), 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.7f, 0.5f);
	drawText(font, "Press ENTER for main menu", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.2f, 0.3f);
}

void App::updateGameLevel(){
	for(int i=0; i<MAX_ASTEROIDS; i++){
		asteroids[i]->collided(&player);
	}
	for(int i=0; i<MAX_ASTEROIDS; i++){
		if(asteroids[i]->collided(&bullet)){
			bullet.visible = false;
			delete asteroids[i];
		}
	}

	//game over
/*	for(int i=0; i<MAX_ASTEROIDS; i++){
		if(collided(asteroids[i], &player)){
			hitCount -= 1;
		}
	}

	if(hitCount == 0){
		state = STATE_GAME_OVER;
	}
	*/
	player.vector = Vector(float(cos(player.rotation + M_PI/2.0f)), float(sin(player.rotation + M_PI/2.0f)), 0.0f);
	player.makeMatrix();
	bullet.makeMatrix();

	//movements of player
	if(keys[SDL_SCANCODE_RIGHT]){
		player.rotation -= 2.5f * FIXED_TIMESTEP;
	}
	else if(keys[SDL_SCANCODE_LEFT]){
		player.rotation += 2.5f * FIXED_TIMESTEP;
	}

	if(keys[SDL_SCANCODE_UP]){
		player.x += player.vector.x * FIXED_TIMESTEP;
		player.y += player.vector.y * FIXED_TIMESTEP;
	}
	else if(keys[SDL_SCANCODE_DOWN]){
		player.x -= player.vector.x * FIXED_TIMESTEP;
		player.y -= player.vector.y * FIXED_TIMESTEP;
	}

	if(keys[SDL_SCANCODE_SPACE]){
		bullet.visible = true;
		bullet.x = player.x;
		bullet.y = player.y;
		bullet.rotation = player.rotation;
	}
	if(bullet.visible){
		bullet.x += player.vector.x * FIXED_TIMESTEP * 2.0f;
		bullet.y += player.vector.y * FIXED_TIMESTEP * 2.0f;
	}
	if(bullet.x > 1.2f || bullet.x < -1.2f || bullet.y > 1.0f || bullet.y < -1.0f){
		bullet.visible = false;
	}

	//borders
	if(player.x > 1.2f){
		player.x = 1.2f;
	}
	else if(player.x < -1.2f){
		player.x = -1.2f;
	}
	if(player.y > 0.9f){
		player.y = 0.9f;
	}
	else if(player.y < -0.9f){
		player.y = -0.9f;
	}

	//asteroids movement
	for(int i=0; i<MAX_ASTEROIDS; i++){
		asteroids[i]->x += asteroids[i]->vector.x * FIXED_TIMESTEP;
		asteroids[i]->y += asteroids[i]->vector.y * FIXED_TIMESTEP;

		asteroids[i]->makeMatrix();

		//borders for asteroids
		if(asteroids[i]->x > 1.3f){
			asteroids[i]->x = -asteroids[i]->x + 0.01f;
		}
		else if(asteroids[i]->x < -1.3f){
			asteroids[i]->x = -asteroids[i]->x - 0.01f;
		}
		if(asteroids[i]->y > 1.0f){
			asteroids[i]->y = -asteroids[i]->y + 0.01f;
		}
		else if(asteroids[i]->y < -1.0f){
			asteroids[i]->y = -asteroids[i]->y - 0.01f;
		}
	}

}

void App::update(){
	SDL_Event event;
	
	updateGameLevel();	

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}

		if(state == STATE_GAME_LEVEL){
			if(event.key.keysym.scancode == SDL_SCANCODE_R){
				state = STATE_GAME_OVER;
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