#pragma warning(push)
#pragma warning(disable: 4828 4010)
#include "DxLib.h"
#pragma warning(pop)

// ===================
// ブロックの初期設定
// ===================

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

//構造体"Block"
struct Block {
    int x, y;       //ブロックの座標
    int is_active;  //ブロックの状態(1：表示 0：非表示)
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

    //ブロック配色配列
    unsigned int block_colors[BLOCK_ROWS] = {
        GetColor(255, 0, 0),
        GetColor(255, 255, 0),
        GetColor(0, 255, 0),
        GetColor(0, 255, 255),
        GetColor(0, 0, 255)
    };

    struct Block blocks[BLOCK_ROWS][BLOCK_COLS];

    //インスタンスの初期化
    for (int row = 0; row < BLOCK_ROWS; row++) {
        for (int col = 0; col < BLOCK_COLS; col++) {
            //初期座標の計算
            blocks[row][col].x = SCREEN_MARGIN_X + col * (BLOCK_WIDTH + BLOCK_GAP_X);
            blocks[row][col].y = SCREEN_MARGIN_Y + row * (BLOCK_HEIGHT + BLOCK_GAP_Y);

            //初期状態
            blocks[row][col].is_active = 1;
        }
    }

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        DrawString(10, 10, L"Hello, DXLib!", GetColor(255, 255, 255));

        for (int row = 0; row < BLOCK_ROWS; row++) {
            for (int col = 0; col < BLOCK_COLS; col++) {
                if (blocks[row][col].is_active == 1) {

                    int bx = blocks[row][col].x;
                    int by = blocks[row][col].y;
                    int color = block_colors[row];

                    DrawBox(bx, by, bx + BLOCK_WIDTH, by + BLOCK_HEIGHT, color, TRUE);
                }
            }
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
