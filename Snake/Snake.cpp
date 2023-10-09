// Snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <list>
#include <thread>


using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 30;

struct sSnakeSegment
{
	int x;
	int y;
};

int main()
{

	int nFoodX = 10;
	int nFoodY = 10;
	int nScore = 0;
	int nSnakeDirection = 0;
	bool bDead = false;
	bool bKeyLeft = false, bKeyLeftOld = false;
	bool bKeyUp = false, bKeyUpOld = false;
	bool bKeyDown = false, bKeyDowntOld = false;
	bool bKeyRight = false, bKeyRightOld = false;



    // Creat Screen Buffer

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	while (1) {

		//initial snake
		list<sSnakeSegment> snake = { {60,15},{61,15},{62,15},{63,15},{64,15},{65,15},{66,15},{67,15} };

		while (!bDead) {


			//Time

			auto t1 = chrono::system_clock::now();
			while ((chrono::system_clock::now() - t1) < ((nSnakeDirection % 2 == 1) ? 120ms : 200ms)) {

				//Input

				bKeyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
				bKeyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;
				bKeyUp = (0x8000 & GetAsyncKeyState((unsigned char)('\x26'))) != 0;
				bKeyDown = (0x8000 & GetAsyncKeyState((unsigned char)('\x28'))) != 0;

				if (bKeyRight && !bKeyRightOld)
				{
					nSnakeDirection = 1;
				}

				if (bKeyLeft && !bKeyLeftOld)
				{
					nSnakeDirection = 3;
				}
				if (bKeyUp && !bKeyUpOld)
				{
					nSnakeDirection = 0;
				}
				if (bKeyDown && !bKeyDowntOld)
				{
					nSnakeDirection = 2;
				}

				bKeyRightOld = bKeyRight;
				bKeyLeftOld = bKeyLeft;
				bKeyUpOld = bKeyUp;
				bKeyDowntOld = bKeyDown;
			}

			//Movement

			switch (nSnakeDirection)
			{
			case 0:	//Up
				snake.push_front({ snake.front().x, snake.front().y - 1 });
				break;
			case 1:	//Right
				snake.push_front({ snake.front().x + 1, snake.front().y });
				break;
			case 2:	//Down
				snake.push_front({ snake.front().x, snake.front().y + 1 });
				break;
			case 3:	//Up
				snake.push_front({ snake.front().x - 1, snake.front().y });
				break;
			}
			snake.pop_back();


			//Collidions

			//food collision

			if (snake.front().x == nFoodX && snake.front().y == nFoodY) {
				nScore++;
				while (screen[nFoodY * nScreenWidth + nFoodX] != L' ')
				{
					nFoodX = rand() % nScreenWidth;
					nFoodY = (rand() % (nScreenHeight - 3)) + 3;
				}

				for (int i = 0; i < 5; i++)
					snake.push_back({ snake.back().x, snake.back().y });
			}

			//collision with itself

			for (list<sSnakeSegment>::iterator i = snake.begin(); i != snake.end(); i++) {
				if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y) {
					bDead = true;
				}
			}


			//screen border check

			if (snake.front().x < 0 || snake.front().x >= nScreenWidth)
				bDead = true;
			if (snake.front().y < 3 || snake.front().y >= nScreenHeight)
				bDead = true;


			//Show

			//Screen Clear

			// Clear Screen
			for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

			// Draw Stats & Border
			for (int i = 0; i < nScreenWidth; i++)
			{
				screen[i] = L'-';
				screen[2 * nScreenWidth + i] = L'_';
			}
			wsprintf(&screen[nScreenWidth + 5], L"---------- SCORE: %d ----------", nScore);


			// Draw Snake Body
			for (auto s : snake)
				screen[s.y * nScreenWidth + s.x] = bDead ? L'+' : L'O';

			// Draw Snake Head
			screen[snake.front().y * nScreenWidth + snake.front().x] = bDead ? L'X' : L'@';

			// Draw Food
			screen[nFoodY * nScreenWidth + nFoodX] = L'%';

			if (bDead)
				wsprintf(&screen[15 * nScreenWidth + 40], L"    You Lost!    ");

			// Display Frame
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		}
	
		while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);

	}

	return 0;

}

