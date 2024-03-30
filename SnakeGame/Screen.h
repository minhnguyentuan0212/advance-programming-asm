#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <string>

#ifndef SCREEN_H
#define SCREEN_H

typedef const unsigned int cui;

class Screen {
public:
	enum Action { QUIT, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, PAUSE };
	static cui WIDTH;
	static cui HEIGHT;
private:
	SDL_Window * m_window;
	SDL_Renderer * m_renderer;
	SDL_Texture * m_texture;
	SDL_Surface * m_textSurface;
	SDL_Texture * m_textTexture;
	TTF_Font * m_sansFont;
	Uint32 * m_mainBuffer;
public:
	Screen();
	bool init();
	int Events();
	void update(int lives, int score, bool isGameOver);
	void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
	void clear();
	void close();
	void drawGameOver();

private:
	void freeText();
	void drawText(int score, int lives);
};


#endif // SCREEN_H

