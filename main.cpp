#pragma warning(push)
#pragma warning(disable: 4828 4010)
#include "DxLib.h"
#pragma warning(pop)

// ===============
// ブロックの設定
// ===============

//ブロックの数 (行数＊列数)
#define BLOCK_ROWS 5
#define BLOCK_COLS 8

//ブロックのサイズ (幅と高さ：ピクセル)
#define BLOCK_WIDTH 60
#define BLOCK_HEIGHT 20

//マージン (画面端からブロックまで：ピクセル)
#define SCREEN_MARGIN_X 40
#define SCREEN_MARGIN_Y 50

//ギャップ (ブロック同士の隙間)
#define BLOCK_GAP_X 5
#define BLOCK_GAP_Y 5

//ブロックの配色
#define COLOR_RED GetColor(255, 0, 0)
#define COLOR_YELLOW GetColor(255, 255, 0)
#define COLOR_GREEN GetColor(0, 255, 0)
#define COLOR_WHITEBLUE GetColor(0, 255, 255)
#define COLOR_BLUE GetColor(0, 0, 255)

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

    int block_colors[BLOCK_ROWS] = {
        COLOR_RED,
        COLOR_YELLOW,
        COLOR_GREEN,
        COLOR_WHITEBLUE,
        COLOR_BLUE
    };

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        DrawString(10, 10, L"Hello, DXLib!", GetColor(255, 255, 255));

        for (int row = 0; row < BLOCK_ROWS; row++) {
            for (int col = 0; col < BLOCK_COLS; col++) {
                int block_x = SCREEN_MARGIN_X + col * (BLOCK_WIDTH + BLOCK_GAP_X);
                int block_y = SCREEN_MARGIN_Y + row * (BLOCK_HEIGHT + BLOCK_GAP_Y);

                int color = block_colors[row];
                DrawBox(block_x, block_y, block_x + BLOCK_WIDTH, block_y + BLOCK_HEIGHT, color);
            }
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
