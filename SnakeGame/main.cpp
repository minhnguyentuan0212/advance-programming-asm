#include <iostream>
#include <cstring>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include "Snake.h"
#include "Screen.h"
#include "Unit.h"
#include "SDL2/SDL.h"

using namespace std;
static int count = 0;
#define BONUS_SCORE 5
#define HOLD_ON_TIME 1500
bool holdOn(Screen & screen, int hold_on_time) {
	int startTime = SDL_GetTicks();
	bool quit = false;
	while (SDL_GetTicks() - startTime < hold_on_time && !quit) {
		if(screen.Events() == Screen::Action::QUIT)
			quit = true;
	}
	return quit;
}

bool pauseGame(Screen & screen, bool & pause) {
	int startTime = SDL_GetTicks();
	bool quit = false;
	while (!quit && pause) {
		int action = screen.Events();
		switch (action) {
			case Screen::Action::QUIT:
				quit = true;
				break;
			case Screen::Action::PAUSE:
				pause = false;
				break;
		}

	}
	return quit;
}

void reset(Snake & snake, Food & food, bool & starting) {
	snake.lives--;
	snake.reset();
	food = Food();
	starting = true;
	count = 0;
	Snake::INITIAL_SPEED = 1;
}
int main(int argc, char ** argv) {
	//Declaration
	srand(time(NULL));
	Screen screen;
	Snake snake;
	Food food;
	//Create walls
	std::vector<Wall*> walls;
	int N_HORIZONTAL = Screen::WIDTH / Wall::WIDTH;
	int N_VERTICAL = Screen::HEIGHT / Wall::WIDTH - 2;

	for (int i = 0; i < N_HORIZONTAL; i++) {
		Wall * upperWall = new Wall(i * Wall::WIDTH, 0);
		Wall * lowerWall = new Wall(i * Wall::WIDTH, 
			Screen::HEIGHT - 3 * Wall::WIDTH);
		walls.push_back(upperWall);
		walls.push_back(lowerWall);
	}
	for (int i = 1; i < N_VERTICAL - 1; i++) {
		Wall * leftmostWall = new Wall(0, i * Wall::WIDTH);
		Wall * rightmostWall = new Wall(Screen::WIDTH - Wall::WIDTH,
			i * Wall::WIDTH);
		walls.push_back(leftmostWall);
		walls.push_back(rightmostWall);
	}

	//Initialization
	int score = 0;
	if (!screen.init()) {
		SDL_Log("Error screen Initialization");
		return -1;
	}

	bool quit = false;
	bool starting = true;
	bool pause = false;
	while (!quit && snake.lives > 0) {
		screen.clear();
		snake.draw(screen);
		if (count != 5) food.draw(screen);
		else food.drawSpecial(screen);
		for (auto wall: walls)
			wall->draw(screen);
		screen.update(score, snake.lives, false);

		if (starting) {
			quit = holdOn(screen, HOLD_ON_TIME);
			starting = false;
		}
		switch (screen.Events()) {
			case Screen::Action::QUIT:
				quit = true;
				break;
			case Screen::Action::PAUSE:
				pause = true;
				break;
			case Screen::Action::MOVE_UP:
				if(!snake.hasUpdated)
					snake.updateDirection(Snake::Direction::UP);
				break;
			case Screen::Action::MOVE_DOWN:
				if(!snake.hasUpdated) {
					snake.updateDirection(Snake::Direction::DOWN);
				}
				break;
			case Screen::Action::MOVE_LEFT:
				if(!snake.hasUpdated)
					snake.updateDirection(Snake::Direction::LEFT);
				break;
			case Screen::Action::MOVE_RIGHT:
				if(!snake.hasUpdated)
					snake.updateDirection(Snake::Direction::RIGHT);
				break;
		}

		if (pause)
			quit = pauseGame(screen, pause);
		int elapsed = SDL_GetTicks();
		if (elapsed/10 % 6 == 0) {
			if (!snake.move())
				reset(snake, food, starting);
			else {
				if (snake.collide(food.x, food.y)) {
					count++;
					food = Food();
					if (count == 6) { score += BONUS_SCORE * Food::VALUE; count = 0; Snake::INITIAL_SPEED++;}
					else score += Food::VALUE;
					snake.addSection();
				}
				for (auto wall: walls)
					if (snake.collide(wall->x, wall->y))
						reset(snake, food, starting);
				for (int i = 1; i < snake.sections.size(); i++)
					if (snake.collide(snake.sections[i]->x, snake.sections[i]->y))
						reset(snake, food, starting);
			}
		}

		if (!snake.lives) {
			screen.clear();			
			screen.update(score, snake.lives, true);
			holdOn(screen, HOLD_ON_TIME);
		}
	}

	for (auto wall: walls)
		delete wall;
	walls.clear();	
	screen.close();

	return 0;
}
