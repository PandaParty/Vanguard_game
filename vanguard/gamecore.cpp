#include "gamecore.h"




// Creates the main window. Returns true on success.
bool GameCore::init(int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
		return false;
	}

	SDL_GL_LoadLibrary(nullptr); // Default OpenGL is fine.

	// Request an OpenGL 4.3 context (should be core)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	//Create window
	window = SDL_CreateWindow("Vanguard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (window == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	static SDL_GLContext maincontext = SDL_GL_CreateContext(window);
	if (maincontext == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create OpenGL context. SDL_Error: %s\n", SDL_GetError());
		return nullptr;
	}

	SDL_GL_SetSwapInterval(1);

	// initialize the keys
	key.fireUp = false;	
	key.fireLeft = false;	
	key.fireRight = false; 	
	key.fireDown = false;
	key.up = false;
	key.down = false;
	key.left = false;
	key.right = false;

	return true;
}


// Destroys the avancez library instance
void GameCore::destroy()
{
	SDL_DestroyWindow(window);

	SDL_Quit();
}


bool GameCore::update()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			return false;

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			case SDLK_q:
				return false;
				break;
			case SDLK_w:
				key.up = true;
				break; 
			case SDLK_s:
				key.down = true;
				break;
			case SDLK_d:
				key.right = true;
				break;
			case SDLK_a:
				key.left = true;
				break;
			case SDLK_LEFT:
				key.fireLeft = true;
				break;
			case SDLK_RIGHT:
				key.fireRight = true;
				break;
			case SDLK_UP:
				key.fireUp = true;
				break;
			case SDLK_DOWN:
				key.fireDown = true;
				break;
			}
		}

		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				key.up = false;
				break;
			case SDLK_s:
				key.down = false;
				break;
			case SDLK_d:
				key.right = false;
				break;
			case SDLK_a:
				key.left = false;
				break;
			case SDLK_LEFT:
				key.fireLeft = false;
				break;
			case SDLK_RIGHT:
				key.fireRight = false;
				break;
			case SDLK_UP:
				key.fireUp = false;
				break;
			case SDLK_DOWN:
				key.fireDown = false;
				break;
			}
		}

	}

	return true;
}

void GameCore::processInput()
{

}

void GameCore::render()
{
	SDL_GL_SwapWindow(window);
}

float GameCore::getElapsedTime()
{
	return SDL_GetTicks();
}

void GameCore::getKeyStatus(KeyStatus & keys)
{
	keys.fireUp = key.fireUp;
	keys.fireDown = key.fireDown;
	keys.fireLeft = key.fireLeft;
	keys.fireRight = key.fireRight;
	keys.left = key.left;
	keys.right = key.right;
	keys.up = key.up;
	keys.down = key.down;
}