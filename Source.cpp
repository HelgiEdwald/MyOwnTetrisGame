#include <iostream>
#include <Windows.h>
using namespace std;

wstring tetromino[7];

int nFieldWidth = 12;
int nFieldHeight = 18;	

unsigned char* pField = nullptr;

int nScreenWidth = 80;	//Console Screen Size X (columns)
int nScreenHeight = 30;	//Console Screen Size Y (rows)

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

	for (int x = 0; x < nFieldWidth; x++) // Board Boundary
		for (int y = 0; y < nFieldHeight; y++)
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	//Gemini helped here to resize the console
	// --- ADD THIS BLOCK TO RESIZE THE CONSOLE ---
	COORD coordBufSize = { (SHORT)nScreenWidth, (SHORT)nScreenHeight };
	SMALL_RECT rectWindow = { 0, 0, (SHORT)(nScreenWidth - 1), (SHORT)(nScreenHeight - 1) };

	// 1. Temporarily shrink the window to 1x1 to prevent sizing conflicts
	SMALL_RECT minWindow = { 0, 0, 0, 0 };
	SetConsoleWindowInfo(hConsole, TRUE, &minWindow);

	// 2. Set the internal buffer size
	SetConsoleScreenBufferSize(hConsole, coordBufSize);

	// 3. Set the physical window size to match
	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);
	// --------------------------------------------


	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	bool bGameOver = false;

	while (!bGameOver)
	{
		// Draw Field
		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];


		//Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 },&dwBytesWritten);
	}



	return 0;
}