/*
    Snake
    A simple snake game for the VEX brain
    Author: poyu39
*/
#include <queue>
#include <stack>

#include "vex.h"

using namespace vex;

brain Brain;

controller Controller1 = controller(primary);

void pre_game();
int screen_update();
void len_bar();
int draw_snake();
int gen_point();

// global variables
std::queue<int> snake_x;
std::queue<int> snake_y;
int snakeLength = 5;
int len = 5;
int len_apply = 0;
int dir = 3;
int apple_m = 0;

int main() {
	pre_game();
	task s = task(screen_update);
	task d = task(draw_snake);
	task a = task(gen_point);
	while(true) {
		if (Controller1.ButtonUp.pressing() && dir != 2) {
			dir = 0;
		} else if (Controller1.ButtonRight.pressing() && dir != 3) {
			dir = 1;
		} else if (Controller1.ButtonDown.pressing() && dir != 0) {
			dir = 2;
		} else if (Controller1.ButtonLeft.pressing() && dir != 1) {
			dir = 3;
		}
	}
}

void pre_game() {
	Brain.Screen.setPenColor(color::white);
	Brain.Screen.printAt(170, 150, "Snake generating...");
	Brain.Screen.setPenColor(color::black);
	std::stack<int> pre_x;
	std::stack<int> pre_y;
	for (int i = 0; i < snakeLength; i++) {
		pre_x.push(220 + i * 10);
		pre_y.push(100);
		Brain.Screen.drawRectangle(pre_x.top(), pre_y.top(), 10, 10, color::yellow);
		wait(500, msec);
	}
	for (int i = 0; i < snakeLength; i++) {
		snake_x.push(pre_x.top());
		snake_y.push(pre_y.top());
		pre_x.pop();
		pre_y.pop();
	}
	Brain.Screen.setPenColor(color::white);
	Brain.Screen.printAt(170, 170, "Press A to start");
	waitUntil(Controller1.ButtonA.pressing());
	Brain.Screen.setPenColor(color::black);
	Brain.Screen.printAt(170, 150, "                     ");
	Brain.Screen.printAt(170, 170, "                ");
}

int screen_update() {
	while(true) {
		len_bar();
		wait(10, msec);
		Brain.Screen.printAt(320, 20, "          ");
	}
	return 0;
}

void len_bar() {
	Brain.Screen.printAt(320, 20, "[len: %d]", len);
}

int draw_snake() {
	Brain.Screen.setPenColor(color::black);
	while (true) {
		int temp_x = snake_x.back();
		int temp_y = snake_y.back();
		switch (dir) {
			case 0:
				temp_y -= 10;
				break;
			case 1:
				temp_x += 10;
				break;
			case 2:
				temp_y += 10;
				break;
			case 3:
				temp_x -= 10;
				break;
			default:
				break;
		}
		if (temp_x > 480) {
			temp_x -= 480;
		} else if (temp_x < 0) {
			temp_x += 480;
		} else if (temp_y > 240) {
			temp_y -= 240;
		} else if (temp_y < 0) {
			temp_y += 240;
		}
		snake_x.push(temp_x);
		snake_y.push(temp_y);
		Brain.Screen.drawRectangle(snake_x.back(), snake_y.back(), 10, 10, color::yellow);
		Brain.Screen.drawRectangle(snake_x.front(), snake_y.front(), 10, 10, color::black);
		if (len_apply == 0) {
			snake_x.pop();
			snake_y.pop();
		} else {
			len_apply--;
		}
		int check_x = snake_x.back();
		int check_y = snake_y.back();
		for (int i = 0; i < len - 1; i++) {
			if (check_x == snake_x.front() && check_y == snake_y.front()) {
				Brain.Screen.setPenColor(color::white);
				Brain.Screen.printAt(170, 150, "Game Over");
				Brain.Screen.printAt(170, 170, "Length: %d", len);
				waitUntil(Controller1.ButtonB.pressing());
			}
			snake_x.push(snake_x.front());
			snake_y.push(snake_y.front());
			snake_x.pop();
			snake_y.pop();
		}
		snake_x.push(snake_x.front());
		snake_y.push(snake_y.front());
		snake_x.pop();
		snake_y.pop();
		wait(100, msec);
		Brain.Screen.render(true);
	}
	return 0;
}

int gen_point() {
	Brain.Screen.setPenColor(color::black);
	while (true) {
		apple_m = int(rand() % 5);
		int apple_x[apple_m];
		int apple_y[apple_m];
		for (int i = 0; i < apple_m; i++) {
			apple_x[i] = rand() % 470;
			apple_y[i] = rand() % 260;
			Brain.Screen.drawCircle(apple_x[i], apple_y[i], 5, color::red);
		}
		while(apple_m != 0) {
			for (int i = 0; i < apple_m; i++) {
				if (abs(snake_x.back() - apple_x[i]) < 10 && abs(snake_y.back() - apple_y[i]) < 10) {
					Brain.Screen.drawCircle(apple_x[i], apple_y[i], 5, color::black);
					apple_x[i] = -1;
					apple_y[i] = -1;
					apple_m--;
					len++;
					len_apply++;
				}
			}
			this_thread::sleep_for(10);
		}
	}
	return 0;
}