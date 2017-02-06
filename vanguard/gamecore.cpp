#include "gamecore.h"




// Creates the main window. Returns true on success.
bool GameCore::init(int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	window = SDL_CreateWindow("Vanguard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	
	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	// initialize the keys
	key.fireUp = false;	
	key.fireLeft = false;	
	key.fireRight = false; 	
	key.fireDown = false;
	key.up = false;
	key.down = false;
	key.left = false;
	key.right = false;

	//Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Clear screen
	SDL_RenderClear(renderer);

	return true;
}


// Destroys the avancez library instance
void GameCore::destroy()
{
	SDL_DestroyRenderer(renderer);
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

	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);

	return true;
}

void GameCore::processInput()
{

}

void GameCore::render()
{

}


Sprite * GameCore::createSprite(const char * path)
{
	SDL_Surface* surf = SDL_LoadBMP(path);
	if (surf == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Create texture from surface pixels
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(surf);

	Sprite * sprite = new Sprite(renderer, texture);

	return sprite;
}

void GameCore::drawText(int x, int y, const char * msg)
{
	SDL_Color black = { 0, 0, 0 };

	SDL_Surface* surf = TTF_RenderText_Solid(font, msg, black);

	SDL_Texture* msg_texture = SDL_CreateTextureFromSurface(renderer, surf);

	int w = 0;
	int h = 0;
	SDL_QueryTexture(msg_texture, NULL, NULL, &w, &h);
	SDL_Rect dst_rect = { x, y, w, h };

	SDL_RenderCopy(renderer, msg_texture, NULL, &dst_rect);

	SDL_DestroyTexture(msg_texture);
	SDL_FreeSurface(surf);
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


Sprite::Sprite(SDL_Renderer * renderer, SDL_Texture * texture)
{
	this->renderer = renderer;
	this->texture = texture;
}


void Sprite::draw(int x, int y)
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));

	//Render texture to screen
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}


void Sprite::destroy()
{
	SDL_DestroyTexture(texture);
}

