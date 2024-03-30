#include "Screen.h"
#include "Snake.h"
#include <iostream>

const Uint8 Snake::RED = 0x01;
const Uint8 Snake::GREEN = 0xFF;
const Uint8 Snake::BLUE = 0x08;
int Snake::INITIAL_SPEED = 1;
cui Snake::INITIAL_DIRECTION = Snake::Direction::RIGHT;
cui Snake::N_SECTS = 4;
cui Snake::INITIAL_LIVES = 3;

Snake::Snake(): speed(Snake::INITIAL_SPEED), lives(Snake::INITIAL_LIVES),
	direction(INITIAL_DIRECTION), hasUpdated(false) {
	Section * newSection = NULL;
	for (int i = 0; i < N_SECTS; i++) {
		newSection = new Section(Screen::WIDTH/2 - i*Section::SECTION_WIDTH,
			3*Section::SECTION_WIDTH);
		sections.push_back(newSection);
	}
}
Snake::~Snake() {
	for (auto section: sections)
		delete section;
}

void Snake::draw(Screen & screen) {	
	for (auto section: sections)
		section->draw(screen);
	sections[0]->drawHead(screen);
}

void Snake::updateDirection(int direction) {
	if ((this->direction == Snake::Direction::UP || this->direction == Snake::Direction::DOWN) 
	&& (direction == Snake::Direction::LEFT || direction == Snake::Direction::RIGHT)
	|| (this->direction == Snake::Direction::LEFT || this->direction == Snake::Direction::RIGHT) 
	&& (direction == Snake::Direction::UP || direction == Snake::Direction::DOWN)) 
		{
		this->direction = direction;
		hasUpdated = true;
		}
}

bool Snake::move() {
	for (int i = sections.size()-1; i > 0; i--)
		sections[i]->move(sections[i]->DirectionType(*sections[i-1]));
	sections[0]->move(direction);

	hasUpdated = false;

	if (sections[0]->x >= Screen::WIDTH || sections[0]->x < 0 ||
		sections[0]->y >= Screen::HEIGHT || sections[0]->y < 0)		
		return false;
	else
		return true;
}

void Snake::reset() {
	for(int i = N_SECTS; i < sections.size(); i++)
		delete sections[i];

	sections.erase(sections.begin() + N_SECTS, sections.end());

	for (int i = 0; i < N_SECTS; i++){
		sections[i]->x = Screen::WIDTH/2 - Section::SECTION_WIDTH*i;
		sections[i]->y = 3*Section::SECTION_WIDTH;
	}
	direction = INITIAL_DIRECTION;
}
bool Snake::collide(int other_x, int other_y) {
	return sections[0]->collide(other_x, other_y);
}

void Snake::addSection() {
	const int N = sections.size();
	int basePlacement = sections[N-2]->DirectionType(* sections[N-1]);

	int x = 0;
	int y = 0;

	switch (basePlacement) {
		case Snake::Direction::UP:
			x = sections[N-1]->x;
			y = sections[N-1]->y - Section::SECTION_WIDTH;
			break;
		case Snake::Direction::DOWN:
			x = sections[N-1]->x;
			y = sections[N-1]->y + Section::SECTION_WIDTH;
			break;
		case Snake::Direction::LEFT:
			x = sections[N-1]->x - Section::SECTION_WIDTH;
			y = sections[N-1]->y;
			break;
		case Snake::Direction::RIGHT:
			x = sections[N-1]->x + Section::SECTION_WIDTH;
			y = sections[N-1]->y;
			break;
	}
	Section * newSection = new Section(x,y);
	sections.push_back(newSection);	
}
