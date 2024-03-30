#include "Screen.h"
#include "SDL2/SDL.h"

#ifndef UNIT_H
#define UNIT_H

struct Unit {
    int x, y;
    Unit(int x, int y);
    virtual void draw(Screen &screen) = 0;

};

struct Wall : Unit {
	static const Uint8 RED;
	static const Uint8 GREEN;
	static const Uint8 BLUE;
	static const unsigned int WIDTH;

	Wall(int x, int y);
	void draw(Screen & screen);
};

struct Food : Unit {
	static const Uint8 RED;
	static const Uint8 GREEN;
	static const Uint8 BLUE;

	static const unsigned int WIDTH;
	static const int VALUE;

	Food();
	void draw(Screen & screen);
    void drawSpecial(Screen & screen);
};

struct Section : Unit {
	static const unsigned int SECTION_WIDTH;

	Section();
	Section(int x, int y);
	void draw(Screen & screen);
    void drawHead(Screen & screen);
	void move(int direction);
	bool collide(int other_x, int other_y);
	int DirectionType(Section & other);
};

#endif // UNIT_H
