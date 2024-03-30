#include <stdlib.h>
#include "Screen.h"
#include "Snake.h"
#include "Unit.h"

#define UNIT_SIZE 20
//FOOD CONST
const Uint8 Food::RED = 0xFF;
const Uint8 Food::GREEN = 0x00;
const Uint8 Food::BLUE = 0x00;
const unsigned int Food::WIDTH = UNIT_SIZE;
const int Food::VALUE = 1;

//WALL CONST
const Uint8 Wall::RED = 0x80;
const Uint8 Wall::GREEN = 0x80;
const Uint8 Wall::BLUE = 0x80;
const unsigned int Wall::WIDTH = UNIT_SIZE;

//SECTION CONST
const unsigned int Section::SECTION_WIDTH = UNIT_SIZE;

//UNIT 
Unit::Unit(int x, int y): x(x), y(y) {}

//FOOD
Food::Food() : Unit (0, 0) {
	x = (int) ((Screen::WIDTH / WIDTH - 2.0) * (1.0 * rand() / RAND_MAX) + 1) * WIDTH;
	y = (int) ((Screen::HEIGHT / WIDTH - 4.0) * (1.0 * rand() / RAND_MAX) + 1) * WIDTH;
}

void Food::draw(Screen &screen) {
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < WIDTH; j++)
			screen.setPixel((int) x + i, (int) y + j, Food::RED, Food::GREEN, Food::BLUE);
}
void Food::drawSpecial(Screen &screen) {
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < WIDTH; j++)
			screen.setPixel((int) x + i, (int) y + j, 0xFF, 0xFF, 0x00);
}
//WALL
Wall::Wall(int x, int y): Unit(x, y){}

void Wall::draw(Screen & screen) {
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < WIDTH; j++)
			screen.setPixel((int) x + i, (int) y + j, Wall::RED, Wall::GREEN, Wall::BLUE);
}

//SECTION
Section::Section() : Unit (0, 0){}

Section::Section(int x, int y): Unit(x, y) {}

void Section::draw(Screen & screen) {
	for (int i = 0; i < SECTION_WIDTH; i++)
		for (int j = 0; j < SECTION_WIDTH; j++)
			screen.setPixel((int) x + i, (int) y + j, Snake::RED, Snake::GREEN, Snake::BLUE);
}
void Section::drawHead(Screen & screen) {
	for (int i = 0; i < SECTION_WIDTH; i++)
		for (int j = 0; j < SECTION_WIDTH; j++)
			screen.setPixel((int) x + i, (int) y + j, 0x35, 0x8A, 0x5E);
}
void Section::move(int direction) {
	switch (direction) {
		case Snake::Direction::UP:
			y -= SECTION_WIDTH;
			break;
		case Snake::Direction::DOWN:
			y += SECTION_WIDTH;
			break;
		case Snake::Direction::LEFT:
			x -= SECTION_WIDTH;
			break;
		case Snake::Direction::RIGHT:
			x += SECTION_WIDTH;
			break;
	}
}
bool Section::collide(int other_x, int other_y) {
	return this->x == other_x && this->y == other_y;
}
int Section::DirectionType(Section & other) {
	if (other.x - x == 0) {
		if(other.y - y < 0)
			return Snake::Direction::UP;
		else
			return Snake::Direction::DOWN;
	}
	else {
		if(other.x - x > 0)
			return Snake::Direction::RIGHT;
		else
			return Snake::Direction::LEFT;
	}
}