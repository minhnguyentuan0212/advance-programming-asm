#include "SDL2/SDL.h"
#include <vector>
#include "Unit.h"

#ifndef SNAKE_H
#define SNAKE_H

typedef const unsigned int cui;


struct Snake {
	static const Uint8 RED;
	static const Uint8 GREEN;
	static const Uint8 BLUE;
	static cui N_SECTS;
	static cui INITIAL_LIVES;
	static cui INITIAL_DIRECTION;
	static int INITIAL_SPEED;

	enum Direction { UP, DOWN, LEFT, RIGHT };

	int speed;
	int lives;
	int direction;
	bool hasUpdated;

	std::vector<Section*> sections;

	Snake();
	~Snake();
	void draw(Screen & screen);
	void updateDirection(int direction);
	bool move();
	void reset();
	bool collide(int other_x, int other_y);
	void addSection();
};

#endif // SNAKE_H

