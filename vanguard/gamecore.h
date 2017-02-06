#ifndef __GAME_CORE__
#define __GAME_CORE__

#include "SDL.h"
#include "SDL_ttf.h"

class Sprite
{
	SDL_Renderer * renderer;
	SDL_Texture * texture;

public:

	Sprite(SDL_Renderer * renderer, SDL_Texture * texture);

	// Destroys the sprite instance
	void destroy();

	// Draw the sprite at the given position.
	// Valid coordinates are between (0,0) (upper left) and (width-32, height-32) (lower right).
	// (All sprites are 32*32 pixels, clipping is not supported)
	void draw(int x, int y);
};


class GameCore
{
public:
	void destroy();

	// Creates the main window. Returns true on success.
	bool init(int width, int height);

	// Clears the screen and draws all sprites and texts which have been drawn
	// since the last update call.
	// If update returns false, the application should terminate.
	bool update();
	void processInput();
	void render();

	// Create a sprite given a string.
	Sprite* createSprite(const char* name);

	// Draws the given text.
	void drawText(int x, int y, const char* msg);

	// Returns the total time spent in the game, in milliseconds.
	float getElapsedTime();

	struct KeyStatus
	{
		bool fireUp;
		bool fireDown;
		bool fireLeft;
		bool fireRight;
		bool left;
		bool right;
		bool up;
		bool down;
	};

	// Returns the keyboard status. If a flag is set, the corresponding key is being held down.
	void getKeyStatus(KeyStatus& keys);

private:
	SDL_Window * window;
	SDL_Renderer * renderer;

	TTF_Font* font;

	KeyStatus key;
};





#endif __GAME_CORE__
