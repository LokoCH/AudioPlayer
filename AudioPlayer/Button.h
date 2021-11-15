#ifndef BUTTON
#define BUTTON
#include <iostream>
#include <string>
#include <iomanip>
#include <functional>

class Button
{
public:
	Button(std::string name, std::function<void()>fn);
	void showButton();
	void doSomething();
	COORD getBegin() const;
	COORD getEnd() const;
	void setState(bool newState);
	bool getState() const;

private:
	std::string name;
	std::function<void()>fn;
	COORD begin;
	COORD end;
	bool state;
};

Button::Button(std::string name, std::function<void()>fn) :
	name{ name },
	fn{ fn }
{
	begin.X = 0;
	begin.Y = 0;
	end.X = 0;
	end.Y = 0;
}

void Button::showButton() {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(h, &csbi);
	COORD pos;
	pos.X = csbi.dwCursorPosition.X;
	pos.Y = csbi.dwCursorPosition.Y;
	begin = pos;

	int space = (8 - name.length()) / 2;
	std::cout << "**********";
	pos.Y++;
	SetConsoleCursorPosition(h, pos);
	std::cout << "*" << std::setw(space) << "" << name << std::setw(8 - space - name.length()) << "" << "*";
	pos.Y++;
	SetConsoleCursorPosition(h, pos);
	std::cout << "**********";
	GetConsoleScreenBufferInfo(h, &csbi);
	end.X = csbi.dwCursorPosition.X;
	end.Y = csbi.dwCursorPosition.Y;
}

void Button::doSomething()
{
	fn();
}

COORD Button::getBegin()const {
	return begin;
}


COORD Button::getEnd()const {
	return end;
}

void Button::setState(bool newState) {
	state = newState;
}

bool Button::getState() const {
	return state;
}


#endif //!BUTTON