#pragma	once
// Platform
#define DIRECTINPUT_VERSION 0x0900
#define WIN32_EXTRA_LEAN

#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <xinput.h>
#include <ctime>

// Boost
//#include <boost/lambda/lambda.hpp>
//#include <boost/asio.hpp>
//#include <iterator>
//#include <algorithm>

#include <windows.h>
#include <time.h>

#include "input.h"
#include "display.h"
#include "game.h"
#include "surface.h"
using namespace std;

const string APPTITLE = "MyGame";
extern const int SCREENW = GetSystemMetrics(SM_CXSCREEN);
extern const int SCREENH = GetSystemMetrics(SM_CYSCREEN);
bool gameover = false;;
HWND gWindow;
Game gGame;

//macro to detect key presses
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)


LRESULT WINAPI WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            gameover = true;
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

//	Display* tempDisplay = new Display(50, 50);


		// Use UNREFERENCED_PARAMETER() to quiet compiler warnings about unused API based params.
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    //initialize window settings
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX); 
	wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinProc;
    wc.cbClsExtra	 = 0;
    wc.cbWndExtra	 = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = APPTITLE.c_str();
    wc.hIconSm       = NULL;
    RegisterClassEx(&wc);

    //create a new window
    gWindow = CreateWindow( APPTITLE.c_str(), APPTITLE.c_str(),
       WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
       SCREENW, SCREENH, NULL, NULL, hInstance, NULL);
    if (!gWindow) return false;

    //display the window
    ShowWindow(gWindow, nCmdShow);
    UpdateWindow(gWindow);
	
	//initialize the game
    if (!gGame.Game_Init(gWindow, SCREENW, SCREENH)) return false;

    // main message loop
	MSG message;
	while (!gameover)
    {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) 
	    {
		    TranslateMessage(&message);
		    DispatchMessage(&message);
	    }

        //process game loop 
        gGame.Game_Run(gWindow);

    }

	return message.wParam;
}

