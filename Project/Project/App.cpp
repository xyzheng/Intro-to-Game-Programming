#include "App.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define TILE_SIZE 0.05f
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8
float timeLeftOver = 0.0f;
float lastFrameTicks;

using namespace std;

enum GameState {STATE_MAIN_MENU, STATE_INSTR_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};

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
}

//constructor
App::App() {
	init();

	keys = SDL_GetKeyboardState(NULL);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	state = STATE_MAIN_MENU;

	font = loadTexture("spritesheet_font.png");
	characters = loadTexture("arne_sprites.png");

	done = false;
	gravity = -9.8f;

	glMatrixMode(GL_MODELVIEW);
}

//destructor
App::~App(){
	/*
	Mix_FreeChunk(gemSound);
	Mix_FreeChunk(jumpSound);
	Mix_FreeChunk(dieSound);
	Mix_FreeChunk(winSound);
	Mix_FreeMusic(music);
	*/
	SDL_Quit();
}

//initializer
void App::init(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
}

void App::reset(){
//	delete player;

}

//render level
void App::makeLevel(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, characters);
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
//GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a, float x, float y
void App::renderMainMenu(){
	drawText(font, "SANCTUARY", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.3f, 0.7f);
	drawText(font, "Collect all gems", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.8f, 0.5f);	
	drawText(font, "Arrow keys to move", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.9f, 0.3f);
	drawText(font, "Space to jump", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.8f, 0.1f);
	drawText(font, "Reach the end", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -0.1f);
	drawText(font, "Press SPACE to start", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.9f, -0.3f);
}

void App::renderInstrMenu(){
	drawText(font, "INSTRUCTIONS", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.7f);
}

void App::renderGameLevel(){
	glLoadIdentity();
	glTranslatef(-TILE_SIZE * mapWidth/4, TILE_SIZE * mapHeight/4, 0.0f);
	string c = to_string(levelData[25][11]);
//	drawText(font, c, 0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	makeLevel();
}

//renders game over
void App::renderGameOver(){
	drawText(font, "GAME OVER", 0.2f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.7f);
	drawText(font, "Press Enter for main menu", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.2f, 0.3f);
}

//open file and read
void App::read(){
	ifstream ifs;
	ifs.open("map.txt");
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

//place the entities
void App::placeEntity(string& type, float placeX, float placeY){
	if(type == "Player"){
//		Entity(unsigned int spritesheet, float x, float y, float xD, float yD, float rotate, float u, 
//	float v, float width, float height, float mass, bool dynamic);
		player = new Entity(characters, placeX, placeY, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f, 0.05f, 1.0f, true);
	}

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

void App::updateGameLevel(){
}

void App::update(){
	SDL_Event event;

	if(state == STATE_GAME_LEVEL){
		updateGameLevel();
	}

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		/*
		//jump
		if(state == STATE_GAME_LEVEL && event.key.keysym.scancode == SDL_SCANCODE_SPACE && player.collidedBot == true){
			player.yVelo = 3.0f;
			player.collidedBot = false;
			Mix_PlayChannel(-1, jumpSound, 0);
		}
		*/

		if(state == STATE_MAIN_MENU){
			if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
				state = STATE_GAME_LEVEL;
				reset();
			}
		}

		if(state == STATE_MAIN_MENU){
			if(event.key.keysym.scancode == SDL_SCANCODE_I){
				state = STATE_INSTR_MENU;
			}
		}

		if(state == STATE_INSTR_MENU){
			if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
				state = STATE_GAME_LEVEL;
				reset();
			}
		}

		if(state == STATE_GAME_LEVEL){
			if(event.key.keysym.scancode == SDL_SCANCODE_R){
				state = STATE_GAME_OVER;
			}
		}

		if(state == STATE_GAME_OVER){
			if(event.key.keysym.scancode == SDL_SCANCODE_RETURN){
				state = STATE_MAIN_MENU;
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