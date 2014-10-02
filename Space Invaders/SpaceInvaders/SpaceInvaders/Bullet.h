#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Bullet{
public:
	Bullet();
	Bullet(GLuint textureID, float u, float v, float width, float height, float x, float y, float scale, bool onScreen);

	void draw();
	void render();
	
	GLuint textureID;
	float u;
	float v;
	float width;
	float height;
	float x;
	float y;
	float scale;
	float speed;
	float timer;
	bool onScreen;
};