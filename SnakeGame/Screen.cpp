#include "Screen.h"
#include "SDL2/SDL.h"
#include <iostream>
#include <cstring>
#include <sstream>

typedef const unsigned int cui;
cui Screen::WIDTH = 800;
cui Screen::HEIGHT = 600;
Screen::Screen(): m_window(NULL), m_renderer(NULL), m_texture(NULL),
	m_mainBuffer(NULL), m_sansFont(NULL), m_textSurface(NULL),
	m_textTexture(NULL) {}

bool Screen::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Could not initialize SDL graphics" << std::endl;
		return false;
	}
	TTF_Init();
	m_sansFont = TTF_OpenFont("OpenSans-Bold.ttf", 20);
	if (!m_sansFont) 
		SDL_Log("Error. Could not load font");
	m_window = SDL_CreateWindow("Snake Game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 
		SDL_WINDOW_SHOWN);
	if (!m_window) {
		SDL_Log("Could not create the window. ");
		SDL_Log("%s", SDL_GetError());
		TTF_Quit();
		SDL_Quit();
		return false;
	}
	m_renderer = SDL_CreateRenderer(m_window, -1,
		SDL_RENDERER_PRESENTVSYNC);
	if (!m_renderer) {
		SDL_Log("Could not create the renderer. ");
		SDL_Log("%s", SDL_GetError());
		SDL_DestroyWindow(m_window);
		TTF_Quit();
		SDL_Quit();
		return false;
	}
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);
	if (!m_texture) {
		SDL_Log("Could not create the texture. ");
		SDL_Log("%s", SDL_GetError());
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		TTF_Quit();
		SDL_Quit();
		return false;
	}
	m_mainBuffer = new Uint32[WIDTH * HEIGHT];
	clear();
	return true;
}

int Screen::Events() {
	SDL_Event event;
	int action = -1;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				action = Action::QUIT;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_LEFT:
						action = Action::MOVE_LEFT;
						break;
					case SDLK_RIGHT:
						action = Action::MOVE_RIGHT;
						break;
					case SDLK_DOWN:
						action = Action::MOVE_DOWN;
						break;
					case SDLK_UP:
						action = Action::MOVE_UP;
						break;
					case SDLK_RETURN:
						action = Action::PAUSE;
						break;
				}
				break;
		}
	}
	return action;
}

void Screen::update(int score, int lives, bool isGameOver) {
	SDL_UpdateTexture(m_texture, NULL, m_mainBuffer, WIDTH * sizeof(Uint32));
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	if (!isGameOver)
		drawText(score, lives);
	else 
		drawGameOver();
	SDL_RenderPresent(m_renderer);
}

void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
		Uint32 color = (red << 24) + (green << 16)  + (blue << 8) + 0xFF;
		m_mainBuffer[y * WIDTH + x] = color;
	}
}

void Screen::clear() {
	memset(m_mainBuffer, 0, WIDTH * HEIGHT * sizeof(Uint32));
}

void Screen::freeText() {
	TTF_CloseFont(m_sansFont);
	SDL_FreeSurface(m_textSurface);
	SDL_DestroyTexture(m_textTexture);
}

void Screen::close() {
	delete[] m_mainBuffer;
	freeText();
	SDL_DestroyTexture(m_texture);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	TTF_Quit();
	SDL_Quit();
}

void Screen::drawGameOver() {
	std::string text = "Game over!";
	m_textSurface = TTF_RenderText_Solid(m_sansFont, text.c_str(), {0xFF, 0xFF, 0xFF});
	m_textTexture = SDL_CreateTextureFromSurface(m_renderer, m_textSurface);
	SDL_Rect rectangle = {200, 200, 400, 200};
	SDL_RenderCopy(m_renderer, m_textTexture, NULL, &rectangle);
}
void Screen::drawText(int score, int lives) {
	std::string text = "Score: " + std::to_string(score) + " - " 
									+ "Lives left: " + std::to_string(lives);
	m_textSurface = TTF_RenderText_Solid(m_sansFont, text.c_str(), {0xFF, 0xFF, 0xFF});
	m_textTexture = SDL_CreateTextureFromSurface(m_renderer, m_textSurface);
	SDL_Rect rectangle = {250, 565, 320, 30};
	SDL_RenderCopy(m_renderer, m_textTexture, NULL, &rectangle);
}

