//Nick Zheng
//Assignment 5 - Side Scroller

#include "App.h"

using namespace std;

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define TILE_SIZE 0.075f
#define SPRITE_COUNT_X 30
#define SPRITE_COUNT_Y 30
#define WORLD_OFFSET_X 0
#define WORLD_OFFSET_Y 0 

float timeLeftOver = 0.0f;
float lastFrameTicks;

enum GameState {STATE_MAIN_MENU, STATE_INSTR_MENU, STATE_LEVEL_ONE, STATE_LEVEL_TWO, STATE_LEVEL_THREE, STATE_GAME_OVER};

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

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
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

App::App(){
	init();

	keys = SDL_GetKeyboardState(NULL);
	done = false;

	state = STATE_MAIN_MENU;

	sheetSprite = loadTexture("spritesheet_rgba.png");
	font = loadTexture("spritesheet_font.png");

	//readLevelOne();

	gravity = -9.8f;
}

App::~App(){
	SDL_Quit();
}

void App::init(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void App::resetLevelOne(){
	readLevelOne();
	score = 0;
	playerOne = new Entity(sheetSprite, -0.1f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.05f, 1.0f, true);
	playerOne->collidedTop = false;
	playerOne->collidedBot = false;
	playerOne->collidedRight = false;
	playerOne->collidedLeft = false;
	playerOne->xVelo = 0.0f;
	playerOne->yVelo = 0.0f;
	playerOne->yAcc = 0.0f;
	playerOne->yFric = 1.0f;
	playerOne->xFric = 1.0f;
	
	playerTwo = new Entity(sheetSprite, -0.1f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.05f, 1.0f, true);
	playerTwo->collidedTop = false;
	playerTwo->collidedBot = false;
	playerTwo->collidedRight = false;
	playerTwo->collidedLeft = false;
	playerTwo->xVelo = 0.0f;
	playerTwo->yVelo = 0.0f;
	playerTwo->yAcc = 0.0f;
	playerTwo->yFric = 1.0f;
	playerTwo->xFric = 1.0f;
}

void App::resetLevelTwo(){
//	delete playerOne;
//	delete playerTwo;
	readLevelTwo();
	score = 0;
	playerOne = new Entity(sheetSprite, -0.1f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.05f, 1.0f, true);
	playerOne->collidedTop = false;
	playerOne->collidedBot = false;
	playerOne->collidedRight = false;
	playerOne->collidedLeft = false;
	playerOne->xVelo = 0.0f;
	playerOne->yVelo = 0.0f;
	playerOne->yAcc = 0.0f;
	playerOne->yFric = 1.0f;
	playerOne->xFric = 1.0f;
	
	playerTwo = new Entity(sheetSprite, -0.1f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.05f, 1.0f, true);
	playerTwo->collidedTop = false;
	playerTwo->collidedBot = false;
	playerTwo->collidedRight = false;
	playerTwo->collidedLeft = false;
	playerTwo->xVelo = 0.0f;
	playerTwo->yVelo = 0.0f;
	playerTwo->yAcc = 0.0f;
	playerTwo->yFric = 1.0f;
	playerTwo->xFric = 1.0f;
}

void App::resetLevelThree(){
//	delete playerOne;
//	delete playerTwo;
	readLevelThree();
	score = 0;
	playerOne = new Entity(sheetSprite, -0.1f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.05f, 1.0f, true);
	playerOne->collidedTop = false;
	playerOne->collidedBot = false;
	playerOne->collidedRight = false;
	playerOne->collidedLeft = false;
	playerOne->xVelo = 0.0f;
	playerOne->yVelo = 0.0f;
	playerOne->yAcc = 0.0f;
	playerOne->yFric = 1.0f;
	playerOne->xFric = 1.0f;
	
	playerTwo = new Entity(sheetSprite, -0.1f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.05f, 1.0f, true);
	playerTwo->collidedTop = false;
	playerTwo->collidedBot = false;
	playerTwo->collidedRight = false;
	playerTwo->collidedLeft = false;
	playerTwo->xVelo = 0.0f;
	playerTwo->yVelo = 0.0f;
	playerTwo->yAcc = 0.0f;
	playerTwo->yFric = 1.0f;
	playerTwo->xFric = 1.0f;
}

//place the entities
void App::placeEntity(string& type, float placeX, float placeY){

	if(type == "Spikes"){
		spikes.push_back(new Entity(sheetSprite, placeX + 0.05f, placeY + 0.075f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.075f, 0.0f, true));
	}
	if(type == "ExitTop"){
		exitTop = new Entity(sheetSprite, placeX, placeY, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.075f, 0.0f, true);
	}
	if(type == "ExitBot"){
		exitBot = new Entity(sheetSprite, placeX, placeY, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.075f, 0.0f, true);
	}
	if(type == "BlueGem"){
		blueGems.push_back(new Entity(sheetSprite, placeX + 0.4f, placeY, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.075f, 0.0f, true));
	}
	if(type == "GreenGem"){
		greenGems.push_back(new Entity(sheetSprite, placeX, placeY, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.075f, 0.0f, true));
	}
	if(type == "Key"){
		exitKeys.push_back(new Entity(sheetSprite, placeX + 0.07f, placeY - 0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.075f, 0.0f, true));
	}
}

//open level 1 file and read
void App::readLevelOne(){
	ifstream ifs;
	ifs.open("levelOne.txt");
	string line;
	while(getline(ifs, line)){
		if(line == "[header]"){
			if(!readHeader(ifs)){
				return;
			}
		}
		else if(line == "[layer]"){
			readLayerData(ifs);
		}
		else if(line == "[ObjectsLayer]"){
			readEntityData(ifs);
		}
	}
	ifs.close();
}

//open level 2 file and read
void App::readLevelTwo(){
	ifstream ifs;
	ifs.open("levelTwo.txt");
	string line;
	while(getline(ifs, line)){
		if(line == "[header]"){
			if(!readHeader(ifs)){
				return;
			}
		}
		else if(line == "[layer]"){
			readLayerData(ifs);
		}
		else if(line == "[ObjectsLayer]"){
			readEntityData(ifs);
		}
	}
	ifs.close();
}

//open level 3 file and read
void App::readLevelThree(){
	ifstream ifs;
	ifs.open("levelThree.txt");
	string line;
	while(getline(ifs, line)){
		if(line == "[header]"){
			if(!readHeader(ifs)){
				return;
			}
		}
		else if(line == "[layer]"){
			readLayerData(ifs);
		}
		else if(line == "[ObjectsLayer]"){
			readEntityData(ifs);
		}
	}
	ifs.close();
}

//reads data from text
bool App::readHeader(ifstream& stream){
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
			istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
			if (key == "width") {
			mapWidth = atoi(value.c_str());
			}
			else if (key == "height"){
				mapHeight = atoi(value.c_str());
			}
	}
		if (mapWidth == -1 || mapHeight == -1) {
		return false;
		}
		else { // allocate our map data
			levelData = new unsigned char*[mapHeight];
			for (int i = 0; i < mapHeight; ++i) {
				levelData[i] = new unsigned char[mapWidth];
			}
			return true;
		}
}

bool App::readLayerData(ifstream& stream){
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
					for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val;
					}
					else {
						levelData[y][x] = 0;
					}
					}
			}
		}
	}
	return true;
}

bool App::readEntityData(ifstream& stream){
	string line;
	string type;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
			float placeX = atoi(xPosition.c_str()) / 16 * TILE_SIZE;
			float placeY = atoi(yPosition.c_str()) / 16 * -TILE_SIZE;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

//renders the states
void App::render(){
	glClearColor(162.0f/255.0f, 208.0f/255.0f, 89.0f/255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
//	glLoadIdentity();

	switch(state){
	case STATE_MAIN_MENU:
		renderMainMenu();
		break;
	case STATE_INSTR_MENU:
		renderInstrMenu();
		break;
	case STATE_LEVEL_ONE:
		renderLevelOne();
		break;
	case STATE_LEVEL_TWO:
		renderLevelTwo();
		break;
//	case STATE_LEVEL_THREE:
//		renderLevelThree();
//		break;
	case STATE_GAME_OVER:
		renderGameOver();
		break;
	}
	SDL_GL_SwapWindow(displayWindow);
}

//renders main menu
//GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a, float x, float y
void App::renderMainMenu(){
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	drawText(font, "SANCTUARY", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.4f, 0.7f);
	drawText(font, "I for instructions", 0.08f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, -0.3f, 0.5f);	
	drawText(font, "Escape to quit", 0.08f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, -0.3f, 0.3f);
	glPopMatrix();
}

void App::renderInstrMenu(){
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	drawText(font, "INSTRUCTIONS", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.6f, 0.7f);
	drawText(font, "Collect all gems", 0.08f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, -0.3f, 0.5f);	
	drawText(font, "Arrow keys to move for player 1", 0.08f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.3f);
	drawText(font, "WAD keys to move for player 2", 0.08f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.1f);
	drawText(font, "Collect corresponding gems & key and get to the end", 0.08f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, -0.8f, -0.1f);
	drawText(font, "Press 1, 2, 3 for levels", 0.08f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, -0.4f, -0.3f);
	glPopMatrix();
}

void App::renderLevelOne(){
	glClearColor(162.0f/255.0f, 208.0f/255.0f, 89.0f/255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	float translateX = 0.0f;
		if (playerOne->x > 1.33)
			translateX = -playerOne->x;
		else
			translateX = -1.33f;
		if (playerOne->x > 6.66)
			translateX = -6.66f;

	glTranslatef(translateX + 0.32f, 1.4f, 0.0f);

	playerOne->draw(sheetSprite, 27, 30, 30);
	playerTwo->draw(sheetSprite, 57, 30, 30);
	for(size_t i=0; i<spikes.size(); i++){
		spikes[i]->draw(sheetSprite, 575, 30, 30);
	}
	exitTop->draw(sheetSprite, 137, 30, 30);
	exitBot->draw(sheetSprite, 138, 30, 30);	
	for(size_t i=0; i<blueGems.size(); i++){
		if(blueGems[i]->visible){
			blueGems[i]->draw(sheetSprite, 288, 30, 30);
		}
	}
	for(size_t i=0; i<greenGems.size(); i++){
		if(greenGems[i]->visible){
			greenGems[i]->draw(sheetSprite, 286, 30, 30);
		}
	}
	for(size_t i=0; i<exitKeys.size(); i++){
		if(exitKeys[i]->visible){
			exitKeys[i]->draw(sheetSprite, 409, 30, 30);
		}
	}
	makeLevel();
//	string c = to_string(exitKeys[0]->y);
//	drawText(font, c, 0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.2f);
//	string d = to_string(exitKeys[0]->x);
//	drawText(font, d, 0.2f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.5f);

	glPopMatrix();
}

void App::renderLevelTwo(){
	glClearColor(162.0f/255.0f, 208.0f/255.0f, 89.0f/255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	float translateX = 0.0f;
		if (playerOne->x > 1.33)
			translateX = -playerOne->x;
		else
			translateX = -1.33f;
		if (playerOne->x > 6.66)
			translateX = -6.66f;

	glTranslatef(translateX + 0.32f, 1.4f, 0.0f);
//	playerOne->draw(sheetSprite, 27, 30, 30);
//	playerTwo->draw(sheetSprite, 57, 30, 30);


	makeLevel();
//	string c = to_string(exitKeys[0]->y);
//	drawText(font, c, 0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.2f);
//	string d = to_string(exitKeys[0]->x);
//	drawText(font, d, 0.2f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.5f);

	glPopMatrix();
}

//renders game over
void App::renderGameOver(){
	glLoadIdentity();
	drawText(font, "GAME OVER", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.7f);
	drawText(font, "Score:" + to_string(score), 0.08f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.3f, 0.5f);
	drawText(font, "Press Enter for main menu", 0.08f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, -0.4f, 0.3f);
}

//render level
void App::makeLevel(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sheetSprite);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	vector<float> vertexData;
	vector<float> texCoordData;
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (levelData[y][x]) {
				float u = (float)(((int)levelData[y][x]-1) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]-1) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.push_back(TILE_SIZE * x);
				vertexData.push_back(-TILE_SIZE * y);
				vertexData.push_back(TILE_SIZE * x);
				vertexData.push_back((-TILE_SIZE * y) - TILE_SIZE);
				vertexData.push_back((TILE_SIZE * x) + TILE_SIZE);
				vertexData.push_back((-TILE_SIZE * y) - TILE_SIZE);
				vertexData.push_back((TILE_SIZE * x) + TILE_SIZE);
				vertexData.push_back(-TILE_SIZE * y);

				texCoordData.push_back(u);
				texCoordData.push_back(v);
				texCoordData.push_back(u);
				texCoordData.push_back(v + spriteHeight);
				texCoordData.push_back(u + spriteWidth);
				texCoordData.push_back(v + spriteHeight);
				texCoordData.push_back(u + spriteWidth);
				texCoordData.push_back(v);
			}
		}
	}
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, vertexData.size()/2);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
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

void App::updateGameLevelOne(){
	if(keys[SDL_SCANCODE_RIGHT]){
		playerOne->xVelo += 2.0f * FIXED_TIMESTEP;
		playerOne->xAcc = 0.5f;
	}
	if(keys[SDL_SCANCODE_LEFT]){
		playerOne->xVelo -= 2.0f * FIXED_TIMESTEP;
		playerOne->xAcc = -0.5f;
	}
	else{
		playerOne->xAcc = 0.0f;
	}
	if(keys[SDL_SCANCODE_UP] && playerOne->collidedBot){
		playerOne->yVelo = 2.0f;
		playerOne->collidedBot = false;
	}
	if(playerOne->x < 0.0f){
		playerOne->x = 0.0f;
	}
	if(playerOne->x > 7.30){
		playerOne->x = 7.30;
	}

	//player two keys
	if(keys[SDL_SCANCODE_D]){
		playerTwo->xVelo += 2.0f * FIXED_TIMESTEP;
		playerTwo->xAcc = 0.5f;
	}
	if(keys[SDL_SCANCODE_A]){
		playerTwo->xVelo -= 2.0f * FIXED_TIMESTEP;
		playerTwo->xAcc = -0.5f;
	}
	else{
		playerTwo->xAcc = 0.0f;
	}
	if(keys[SDL_SCANCODE_W] && playerTwo->collidedBot){
		playerTwo->yVelo = 2.0f;
		playerTwo->collidedBot = false;
	}
	if(playerTwo->x < 0.0f){
		playerTwo->x = 0.0f;
	}


	//playerOne time steps
	playerOne->yVelo = lerp(playerOne->yVelo, 0.0f, FIXED_TIMESTEP * playerOne->yFric);
	playerOne->yVelo += playerOne->yAcc * FIXED_TIMESTEP;
	playerOne->y += playerOne->yVelo * FIXED_TIMESTEP;

	collideWithMapY(playerOne);
	
	playerOne->xVelo = lerp(playerOne->xVelo, 0.0f, FIXED_TIMESTEP * playerOne->xFric);
	playerOne->xVelo += playerOne->xAcc * FIXED_TIMESTEP;
	playerOne->x += playerOne->xVelo * FIXED_TIMESTEP;
	playerOne->yVelo += gravity * FIXED_TIMESTEP;

	collideWithMapX(playerOne);

	//playerTwo time steps
	playerTwo->yVelo = lerp(playerTwo->yVelo, 0.0f, FIXED_TIMESTEP * playerTwo->yFric);
	playerTwo->yVelo += playerTwo->yAcc * FIXED_TIMESTEP;
	playerTwo->y += playerTwo->yVelo * FIXED_TIMESTEP;

	collideWithMapY(playerTwo);
	
	playerTwo->xVelo = lerp(playerTwo->xVelo, 0.0f, FIXED_TIMESTEP * playerTwo->xFric);
	playerTwo->xVelo += playerTwo->xAcc * FIXED_TIMESTEP;
	playerTwo->x += playerTwo->xVelo * FIXED_TIMESTEP;
	playerTwo->yVelo += gravity * FIXED_TIMESTEP;

	collideWithMapX(playerTwo);

	for(size_t i=0; i<spikes.size(); i++){
		if(playerOne->collideX(spikes[i]) || playerOne->collideY(spikes[i]) || playerTwo->collideX(spikes[i]) || playerTwo->collideY(spikes[i])){
			state = STATE_GAME_OVER;
		}
	}

	for(size_t i=0; i<blueGems.size(); i++){
		if(playerOne->collideX(blueGems[i]) || playerOne->collideY(blueGems[i])){
			blueGems[i]->visible = false;
		//	delete blueGems[i];
		}
	}
	for(size_t i=0; i<greenGems.size(); i++){
		if(playerTwo->collideX(greenGems[i]) || playerTwo->collideY(greenGems[i])){
			greenGems[i]->visible = false;
		//	delete greenGems[i];
		}
	}
	for(size_t i=0; i<exitKeys.size(); i++){
		if(playerOne->collideX(exitKeys[i]) || playerOne->collideY(exitKeys[i]) || playerTwo->collideX(exitKeys[i]) || playerTwo->collideY(exitKeys[i])){
			exitKeys[i]->visible = false;
		//	delete exitKeys[i];
		}
	}
	if(blueGems.size() == 0 && greenGems.size() == 0 && exitKeys.size() == 0){
		if(playerOne->collideX(exitBot) || playerOne->collideY(exitBot) || playerTwo->collideX(exitBot) || playerTwo->collideY(exitBot)){
			state = STATE_GAME_OVER;
		}
	}
}

void App::updateGameLevelTwo(){
	if(keys[SDL_SCANCODE_RIGHT]){
		playerOne->xVelo += 2.0f * FIXED_TIMESTEP;
		playerOne->xAcc = 0.5f;
	}
	if(keys[SDL_SCANCODE_LEFT]){
		playerOne->xVelo -= 2.0f * FIXED_TIMESTEP;
		playerOne->xAcc = -0.5f;
	}
	else{
		playerOne->xAcc = 0.0f;
	}
	if(keys[SDL_SCANCODE_UP] && playerOne->collidedBot){
		playerOne->yVelo = 2.0f;
		playerOne->collidedBot = false;
	}
	if(playerOne->x < 0.0f){
		playerOne->x = 0.0f;
	}
	if(playerOne->x > 7.30){
		playerOne->x = 7.30;
	}

	//player two keys
	if(keys[SDL_SCANCODE_D]){
		playerTwo->xVelo += 2.0f * FIXED_TIMESTEP;
		playerTwo->xAcc = 0.5f;
	}
	if(keys[SDL_SCANCODE_A]){
		playerTwo->xVelo -= 2.0f * FIXED_TIMESTEP;
		playerTwo->xAcc = -0.5f;
	}
	else{
		playerTwo->xAcc = 0.0f;
	}
	if(keys[SDL_SCANCODE_W] && playerTwo->collidedBot){
		playerTwo->yVelo = 2.0f;
		playerTwo->collidedBot = false;
	}
	if(playerTwo->x < 0.0f){
		playerTwo->x = 0.0f;
	}


	//playerOne time steps
	playerOne->yVelo = lerp(playerOne->yVelo, 0.0f, FIXED_TIMESTEP * playerOne->yFric);
	playerOne->yVelo += playerOne->yAcc * FIXED_TIMESTEP;
	playerOne->y += playerOne->yVelo * FIXED_TIMESTEP;

	collideWithMapY(playerOne);
	
	playerOne->xVelo = lerp(playerOne->xVelo, 0.0f, FIXED_TIMESTEP * playerOne->xFric);
	playerOne->xVelo += playerOne->xAcc * FIXED_TIMESTEP;
	playerOne->x += playerOne->xVelo * FIXED_TIMESTEP;
//	playerOne->yVelo += gravity * FIXED_TIMESTEP;

	collideWithMapX(playerOne);

	//playerTwo time steps
	playerTwo->yVelo = lerp(playerTwo->yVelo, 0.0f, FIXED_TIMESTEP * playerTwo->yFric);
	playerTwo->yVelo += playerTwo->yAcc * FIXED_TIMESTEP;
	playerTwo->y += playerTwo->yVelo * FIXED_TIMESTEP;

	collideWithMapY(playerTwo);
	
	playerTwo->xVelo = lerp(playerTwo->xVelo, 0.0f, FIXED_TIMESTEP * playerTwo->xFric);
	playerTwo->xVelo += playerTwo->xAcc * FIXED_TIMESTEP;
	playerTwo->x += playerTwo->xVelo * FIXED_TIMESTEP;
//	playerTwo->yVelo += gravity * FIXED_TIMESTEP;

	collideWithMapX(playerTwo);
	
}

void App::updateGameLevelThree(){
	if(keys[SDL_SCANCODE_RIGHT]){
		playerOne->xVelo += 2.0f * FIXED_TIMESTEP;
		playerOne->xAcc = 0.5f;
	}
	if(keys[SDL_SCANCODE_LEFT]){
		playerOne->xVelo -= 2.0f * FIXED_TIMESTEP;
		playerOne->xAcc = -0.5f;
	}
	else{
		playerOne->xAcc = 0.0f;
	}
	if(keys[SDL_SCANCODE_UP] && playerOne->collidedBot){
		playerOne->yVelo = 2.0f;
		playerOne->collidedBot = false;
	}
	if(playerOne->x < 0.0f){
		playerOne->x = 0.0f;
	}
	if(playerOne->x > 7.30){
		playerOne->x = 7.30;
	}

	//player two keys
	if(keys[SDL_SCANCODE_D]){
		playerTwo->xVelo += 2.0f * FIXED_TIMESTEP;
		playerTwo->xAcc = 0.5f;
	}
	if(keys[SDL_SCANCODE_A]){
		playerTwo->xVelo -= 2.0f * FIXED_TIMESTEP;
		playerTwo->xAcc = -0.5f;
	}
	else{
		playerTwo->xAcc = 0.0f;
	}
	if(keys[SDL_SCANCODE_W] && playerTwo->collidedBot){
		playerTwo->yVelo = 2.0f;
		playerTwo->collidedBot = false;
	}
	if(playerTwo->x < 0.0f){
		playerTwo->x = 0.0f;
	}


	//playerOne time steps
	playerOne->yVelo = lerp(playerOne->yVelo, 0.0f, FIXED_TIMESTEP * playerOne->yFric);
	playerOne->yVelo += playerOne->yAcc * FIXED_TIMESTEP;
	playerOne->y += playerOne->yVelo * FIXED_TIMESTEP;

	collideWithMapY(playerOne);
	
	playerOne->xVelo = lerp(playerOne->xVelo, 0.0f, FIXED_TIMESTEP * playerOne->xFric);
	playerOne->xVelo += playerOne->xAcc * FIXED_TIMESTEP;
	playerOne->x += playerOne->xVelo * FIXED_TIMESTEP;
//	playerOne->yVelo += gravity * FIXED_TIMESTEP;

	collideWithMapX(playerOne);

	//playerTwo time steps
	playerTwo->yVelo = lerp(playerTwo->yVelo, 0.0f, FIXED_TIMESTEP * playerTwo->yFric);
	playerTwo->yVelo += playerTwo->yAcc * FIXED_TIMESTEP;
	playerTwo->y += playerTwo->yVelo * FIXED_TIMESTEP;

	collideWithMapY(playerTwo);
	
	playerTwo->xVelo = lerp(playerTwo->xVelo, 0.0f, FIXED_TIMESTEP * playerTwo->xFric);
	playerTwo->xVelo += playerTwo->xAcc * FIXED_TIMESTEP;
	playerTwo->x += playerTwo->xVelo * FIXED_TIMESTEP;
//	playerTwo->yVelo += gravity * FIXED_TIMESTEP;

	collideWithMapX(playerTwo);
}

void App::update(){
	SDL_Event event;

	if(state == STATE_LEVEL_ONE){
		updateGameLevelOne();
	}
	if(state == STATE_LEVEL_TWO){
		updateGameLevelTwo();
	}
	if(state == STATE_LEVEL_THREE){
		updateGameLevelThree();
	}

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		if(state == STATE_LEVEL_ONE || state == STATE_LEVEL_TWO || state == STATE_LEVEL_THREE){
			if(event.key.keysym.scancode == SDL_SCANCODE_R){
				state = STATE_GAME_OVER;
			}
			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				SDL_Quit();
			}
		}
		if(state == STATE_MAIN_MENU){
			if(event.key.keysym.scancode == SDL_SCANCODE_I){
				state = STATE_INSTR_MENU;
			}
			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				SDL_Quit();
			}
		}
		if(state == STATE_INSTR_MENU){
			if(event.key.keysym.scancode == SDL_SCANCODE_1){
				resetLevelOne();
				state = STATE_LEVEL_ONE;
			}
			if(event.key.keysym.scancode == SDL_SCANCODE_2){
				resetLevelTwo();
				state = STATE_LEVEL_TWO;
			}
			if(event.key.keysym.scancode == SDL_SCANCODE_3){
				resetLevelThree();
				state = STATE_LEVEL_THREE;
			}
			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				SDL_Quit();
			}
		}
		if(state == STATE_GAME_OVER){
			if(event.key.keysym.scancode == SDL_SCANCODE_RETURN){
				state = STATE_MAIN_MENU;
			}
			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				SDL_Quit();
			}
		}
	}
}

bool App::isSolid(int tile){
	switch (tile){
		//floors
	case 2:
		return true;
		break;
	case 3:
		return true;
		break;
	case 9:
		return true;
		break;
	case 33:
		return true;
		break;
	case 62:
		return true;
		break;
	case 69:
		return true;
		break;
	case 122:
		return true;
		break;
	case 129:
		return true;
		break;
	case 153:
		return true;
		break;
	case 302:
		return true;
		break;
	case 309:
		return true;
		break;
	default:
		return false;
		break;
	}
}

void App::worldToTileCoord(float worldX, float worldY, int* gridX, int* gridY){
	*gridX = (int)((worldX + (WORLD_OFFSET_X))/ TILE_SIZE);
	*gridY = (int)((-worldY + (WORLD_OFFSET_Y))/ TILE_SIZE);

}

float App::getCollideX(float x, float y){
	int gridX;
	int gridY;
	worldToTileCoord(x, y, &gridX, &gridY);
	if(gridX < 0 || gridX > 128 || gridY < 0 || gridY > 32){
		return 0.0f;
	}
	if(isSolid(levelData[gridY][gridX])){
		float xCoord = (gridX * TILE_SIZE);
		return -x + xCoord;
	}
	return 0.0f;
}

float App::getCollideY(float x, float y){
	int gridX;
	int gridY;
	worldToTileCoord(x, y, &gridX, &gridY);
 	if(gridX < 0 || gridX > 128 || gridY < 0 || gridY > 32){
		return 0.0f;
	}
	if(isSolid(levelData[gridY][gridX])){
		float yCoord = (gridY * TILE_SIZE) - (TILE_SIZE * 0.0f);
		return -y - yCoord;
	}
	return 0.0f;
}

void App::collideWithMapY(Entity* ent){
	float adjustLeft = getCollideY(ent->x - (ent->width / 2), ent->y - (ent->height / 2));
	float adjustCentre = getCollideY(ent->x, ent->y - (ent->height / 2));
	float adjustRight = getCollideY(ent->x + (ent->width / 2), ent->y - (ent->height / 2 ));

	if (adjustLeft != 0.0f || adjustCentre != 0.0f || adjustCentre != 0.0f) {
	
		float adjustMax = max(adjustLeft, adjustCentre);
		float adjust = max(adjustRight, adjustMax);
		ent->y += adjust;
		ent->yVelo = 0.0f;
		ent->collidedBot = true;
	}

	// top-side

	adjustLeft = getCollideY(ent->x - (ent->width / 2), ent->y + (ent->height / 2));
	adjustCentre = getCollideY(ent->x, ent->y - (ent->height / 2));
	adjustRight = getCollideY(ent->x + (ent->width / 2), ent->y + (ent->height / 2));	
	if (adjustLeft != 0.0f || adjustCentre != 0.0f || adjustCentre != 0.0f) {
		float adjustMax1 = max(adjustLeft, adjustCentre);
		float adjust = max(adjustMax1, adjustRight);

		ent->y += adjust - TILE_SIZE/2;
		ent->yVelo = 0.0f;
		ent->collidedTop = true;
	}
}

void App::collideWithMapX(Entity* ent){

	float adjustTop = getCollideX(ent->x + (ent->width / 2), ent->y + (ent->height / 4)); // not top right corner to avoid double collisions
	float adjustMiddle = getCollideX(ent->x + (ent->width / 2), ent->y);
	float adjustBottom = getCollideX(ent->x + (ent->width / 2), ent->y - (ent->height / 4)); // not bottom right corner to avoid collisions w/ ground

	
	if (adjustTop != 0.0f || adjustMiddle != 0.0f || adjustBottom != 0.0f) {
		float adjustMax = min(adjustTop, adjustMiddle);
		float adjust = min(adjustMax, adjustBottom );
		ent->x += adjust;
		ent->xVelo = 0.0f;
		ent->collidedRight = true;
	}
	// left-side
	adjustTop = getCollideX(ent->x - (ent->width / 2), ent->y + (ent->height / 4));
	adjustMiddle = getCollideX(ent->x - (ent->width / 2), ent->y);
	adjustBottom = getCollideX(ent->x - (ent->width / 2), ent->y - (ent->height / 4));

	//adjust = checkPointForGridCollisionX(ent->x - (ent->width / 2), ent->y);
	if (adjustTop != 0.0f || adjustMiddle != 0.0f || adjustBottom != 0.0f) {
		float adjustMax1 = min(adjustTop, adjustMiddle);
		float adjust = min(adjustMax1, adjustBottom );
		ent->x -= adjust;
		ent->x -= TILE_SIZE - 0.01f;
		ent->xVelo = 0.0f;
		ent->collidedLeft = true;
	}

}




