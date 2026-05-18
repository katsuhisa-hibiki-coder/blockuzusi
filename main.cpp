#define DX_NON_USING_UNICODE_FUNCTION

#pragma warning(push)
#pragma warning(disable: 4828 4010)
#include "DxLib.h"
#pragma warning(pop)

#define WIDTH 13
#define HEIGHT 15

char board[HEIGHT][WIDTH + 1] = {
    "############",
    "#          #",
    "# ######## #",
    "# ######## #",
    "# ######## #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#    O     #",
    "#    ===   #",
    "############"
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(640, 480, 32);
    SetMainWindowText(L"DXLib Project");

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        DrawString(10, 10, L"Hello, DXLib!", GetColor(255, 255, 255));

        int cellSize = 20;
        int offsetX = 100;
        int offsetY = 50;

        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                char c = board[y][x];

                wchar_t str[2] = { (wchar_t)c, L'\0' };

                DrawString(
                    offsetX + x * cellSize,
                    offsetY + y * cellSize,
                    str,
                    GetColor(255, 255, 255)
                );
            }
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
