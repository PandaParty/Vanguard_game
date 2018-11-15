#include "gamecore.h"
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <streambuf>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::default_random_engine GameCore::generator;
std::uniform_real_distribution<float> GameCore::distribution;
// Creates the main window. Returns true on success.
bool GameCore::init(int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
		return false;
	}

	SDL_GL_LoadLibrary(nullptr);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	window = SDL_CreateWindow("Vanguard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
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
	glewInit();
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


// Destroys the library instance
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

	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

//returns the elapsed game time in milliseconds
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

GLuint GameCore::loadShaderProgram(const std::string &vertexShader, const std::string &fragmentShader)
{
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::ifstream vs_file(vertexShader);
	std::string vs_src((std::istreambuf_iterator<char>(vs_file)), std::istreambuf_iterator<char>());

	std::ifstream fs_file(fragmentShader);
	std::string fs_src((std::istreambuf_iterator<char>(fs_file)), std::istreambuf_iterator<char>());

	const char *vs = vs_src.c_str();
	const char *fs = fs_src.c_str();

	glShaderSource(vShader, 1, &vs, nullptr);
	glShaderSource(fShader, 1, &fs, nullptr);

	glCompileShader(vShader);
	int compileOk = 0;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileOk);
	if (!compileOk)
	{
		//SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load shader file: %s\n", vs_file);
		return 0;
	}

	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compileOk);
	if (!compileOk)
	{
		//SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load shader file: %s\n", fs_file);
		return 0;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fShader);
	glDeleteShader(fShader);
	glAttachShader(shaderProgram, vShader);
	glDeleteShader(vShader);

	if (!linkShaderProgram(shaderProgram)) return 0;

	return shaderProgram;
}

bool GameCore::linkShaderProgram(GLuint shaderProgram)
{
	glLinkProgram(shaderProgram);
	GLint linkOk = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkOk);
	if (!linkOk)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to link shader program\n");
		return false;
	}
	return true;
}

void GameCore::setUniform(GLuint shaderProgram, const char *name, const float value)
{
	glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

void GameCore::setUniform(GLuint shaderProgram, const char *name, const glm::vec2 value)
{
	glUniform2fv(glGetUniformLocation(shaderProgram, name), 1, &value.x);
}

void GameCore::setUniform(GLuint shaderProgram, const char *name, const glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, &value.x);
}

void GameCore::setUniform(GLuint shaderProgram, const char *name, const glm::mat4 &value)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, false, &value[0].x);
}

GLuint GameCore::LoadTexture(const char* spriteFile, TextureFormat format, TextureFiltering filtering)
{
	int w, h, comp;
	unsigned char* image = stbi_load(spriteFile, &w, &h, &comp, STBI_rgb_alpha);

	GLint internalFormat;
	switch (format)
	{
	case GRAY:
		internalFormat = GL_RED;
		break;
	case RGB:
		internalFormat = GL_RGB;
		break;
	case RGBA:
	default:
		internalFormat = GL_RGBA;
	}

	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, internalFormat, GL_UNSIGNED_BYTE, image);
	free(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	switch (filtering)
	{
	case NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case BILINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case TRILINEAR:
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case ANISOTROPIC_1:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
		break;
	case ANISOTROPIC_2:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.0f);
		break;
	case ANISOTROPIC_4:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
		break;
	case ANISOTROPIC_8:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
		break;
	case ANISOTROPIC_16:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
		break;
	}

	return texture;
}

float GameCore::randf() {
	
	return distribution(generator);
}