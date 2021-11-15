#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <filesystem>
#include "BASSPlayer.h"

void OpenFile() {
	std::thread th([]() {

			std::mutex mu;
			mu.lock();
			std::cout << "Âûבונטעו פאיכ:\n";
			std::filesystem::path currPath = std::filesystem::current_path();
			for (auto& file : currPath) {
				if (std::filesystem::is_regular_file(file) && file.extension() == "mp3")
					std::cout << file.filename() << "\n";
			}
			int a;
			std::cin >> a;
			mu.unlock();
		
		});
	th.join();
	/*for (std::filesystem::path::iterator itr = currPath.begin(); itr != currPath.end(); itr) {
		if()
	}*/
}

void PrintPlayer(std::vector<Button*>& buttons, COORD& start, COORD& finish) {
	std::cout << "\t\t\t\t\tMyPlayer\n";

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(h, &csbi);

	COORD pos{ 0,0 };
	start.X = csbi.dwCursorPosition.X;
	start.Y = csbi.dwCursorPosition.Y;
	pos = finish = start;

	pos.Y++;
	pos.X += 5;
	SetConsoleCursorPosition(h, pos);

	for (auto& b : buttons) {
		b->showButton();
		pos = b->getBegin();
		pos.X = b->getEnd().X + 5;
		SetConsoleCursorPosition(h, pos);
	}

	finish.X = pos.X;
	pos = buttons.back()->getEnd();
	SetConsoleCursorPosition(h, pos);
	std::cout << "\n";
}


int main() {
	setlocale(LC_ALL, "ru");

	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS);
	SetConsoleMode(hIn, ENABLE_MOUSE_INPUT);
	HWND hwnd = GetConsoleWindow();
	INPUT_RECORD iRec;
	DWORD events;

	BASS_Init(-1, 48000, BASS_DEVICE_3D, 0, NULL);

	Player* p = new BASSPlayer;
	bool work{ true };
	std::vector<Button*>buttons;

	buttons.push_back(new Button("Play", [&]() {p->play(); }));
	buttons.push_back(new Button("Open", [&]() {OpenFile(); }));
	buttons.push_back(new Button("Pause", [&]() {p->pause(); }));
	buttons.push_back(new Button("Stop", [&]() {p->stop(); }));
	buttons.push_back(new Button("Exit", [&work]() {work = false; }));

	COORD start, finish;
	PrintPlayer(buttons, start, finish);

	std::thread timer([&]() {
		while (work) {
			int length = finish.X - start.X;
			COORD curPos = start;
			curPos.X += 5;

			while (curPos.X < finish.X - 5)
			{
				SetConsoleCursorPosition(hOut, start);
				if (p->getCurrentTime() != -1 || p->getFullTime() != -1)
					std::cout << p->getCurrentTime() / 60 << ":" << p->getCurrentTime() % 60;
				for (int i = start.X; i < finish.X - 5; i++)
				{
					std::cout << " ";
				}
				if (p->getCurrentTime() != -1 || p->getFullTime() != -1) {
					std::cout << (p->getFullTime() - p->getCurrentTime()) / 60 << ":" << (p->getFullTime() - p->getCurrentTime()) % 60;
				}
				SetConsoleCursorPosition(hOut, curPos);
				std::cout << "[]";
				if (p->getCurrentTime() == -1 || p->getFullTime() == -1)
					curPos.X = 5;
				else
					curPos.X = 5 + (length - 10) * (static_cast<double>(p->getCurrentTime()) / p->getFullTime());
				Sleep(500);
			}
		}
		});
	timer.detach();

	while (work) {
		ReadConsoleInput(hIn, &iRec, 1, &events);
		if (iRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			for (auto& b : buttons) {
				if (iRec.Event.MouseEvent.dwMousePosition.X >= b->getBegin().X && iRec.Event.MouseEvent.dwMousePosition.X <= b->getEnd().X
					&& iRec.Event.MouseEvent.dwMousePosition.Y >= b->getBegin().Y && iRec.Event.MouseEvent.dwMousePosition.Y <= b->getEnd().Y) {
					b->doSomething();
					break;
				}
			}
		}
	}

	SetConsoleCursorPosition(hOut, buttons.back()->getEnd());
	std::cout << "\n";


	return 0;
}