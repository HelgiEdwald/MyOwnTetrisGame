#include <iostream>
#include <windows.h>
using namespace std;

wstring tetromino[7];

int nFieldHeight = 12;
int nFieldWidth = 18;

unsigned char* pField = nullptr;

int nScreenWidth = 80;	//Console Screen Width X (columns)
int nScreenHeight = 30;	//Console Screen Height Y (rows)

int Rotate(int px, int py, int r)
{
	switch (r % 4)
	{
	case 0: return py * 4 + px;		//0 degrees
	case 1: return 12 + py - (px * 4);	//90 degrees
	case 2: return 15 - (py * 4) - px;	//180 degrees
	case 3: return 3 - py + (px * 4);
	}
	return 0;
}

int main()
{
	//Create assets

	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");

	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L".X..");
	tetromino[1].append(L"....");

	tetromino[2].append(L".X..");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");

	tetromino[3].append(L"....");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L"....");

	tetromino[4].append(L"..X.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"..X.");
	tetromino[4].append(L"....");

	tetromino[5].append(L"....");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"..X.");

	tetromino[6].append(L"..X.");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L".X..");
	tetromino[6].append(L".X..");

	//Create play field buffer
	pField = new unsigned char[nFieldHeight * nFieldWidth];

	//Board boundary
	for (int x = 0; x < nFieldHeight; x++)
	{
		for (int y = 0; y < nFieldWidth; y++)
		{
			bool bBoundary = (x == 0 || x == nFieldWidth - 1
				|| y == nFieldHeight - 1);

			pField[y * nFieldWidth + x] = bBoundary ? 9 : 0;
		}
	}

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

	HANDLE hConsole = CreateConsoleScreenBuffer
	(
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	bool bGameOver = false;

	while (!bGameOver)
	{
		// Draw Field
		for (int x = 0; x < nFieldWidth; x++)
		{
			for (int y = 0; y < nFieldHeight; y++)
			{
				screen[(y + 2) * nScreenWidth + (x + 2)] = 
					L" ABCDEFG=#"[pField[y * nFieldWidth + x]];
			}
		}


		//Display Frame
		WriteConsoleOutputCharacter(
			hConsole,
			screen,
			nScreenWidth * nScreenHeight,
			{ 0,0 },
			&dwBytesWritten
		);
	}



	return 0;
}