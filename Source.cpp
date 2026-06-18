#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>

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

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
	for (int px = 0; px < 4; px++)
	{
		for (int py = 0; py < 4; py++)
		{
			//Get index into piece
			int pi = Rotate(px, py, nRotation);

			//Get index into field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
						return false;	//fail on first hit
				}
			}
		}
	}

	return true;
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

	// Board Boundary
	for (int x = 0; x < nFieldWidth; x++)
	{
		for (int y = 0; y < nFieldHeight; y++)
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


	//Gemini helped here to resize the console
	// --------------------------------------------
	COORD coordBufSize = { (SHORT)nScreenWidth, (SHORT)nScreenHeight };

	SMALL_RECT rectWindow =
	{
		0, 0, (SHORT)(nScreenWidth - 1),
		(SHORT)(nScreenHeight - 1)
	};

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

	//Game Logic Variables
	bool bGameOver = false;
	int nCurrentPiece = 1;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;

	bool bKey[4];

	while (!bGameOver)
	{

		// GAME TIMING ======================
		this_thread::sleep_for(50ms);

		// INPUT ============================
		for (int k = 0; k < 4; k++)
		{
			bool bKeyState = 
				(0x8000 & 
					GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k])));

			bKey[k] = bKeyState != 0;
			
		}

		// GAME LOGIC =======================
		if (bKey[0])
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation,
				nCurrentX + 1, nCurrentY))
			{
				nCurrentX = nCurrentX + 1;
			}
		}

		if (bKey[1])
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation,
							 nCurrentX - 1, nCurrentY))
			{
				nCurrentX = nCurrentX - 1;
			}
		}

		// RENDER OUTPUT ====================


		// Draw Field
		for (int x = 0; x < nFieldWidth; x++)
		{
			for (int y = 0; y < nFieldHeight; y++)
			{
				screen[(y + 2) * nScreenWidth + (x + 2)] =
					L" ABCDEFG=#"[pField[y * nFieldWidth + x]];
			}
		}

		//Draw Current Piece
		for (int px = 0; px < 4; px++)
		{
			for (int py = 0; py < 4; py++)
			{
				int nIndex = Rotate(px, py, nCurrentRotation);
				if (tetromino[nCurrentPiece][nIndex] == L'X')
				{
					int nScreenPos = (nCurrentY + py + 2) * nScreenWidth
									+ (nCurrentX + px + 2);
					screen[nScreenPos] = nCurrentPiece + 65;
				}
			}
		}

		//Display Frame
		WriteConsoleOutputCharacter
		(
			hConsole,
			screen,
			nScreenWidth * nScreenHeight,
			{ 0,0 },
			&dwBytesWritten
		);
	}

	return 0;
}