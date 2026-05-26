#define DX_NON_USING_UNICODE_FUNCTION

#pragma warning(push)
#pragma warning(disable: 4828 4010)
#include "DxLib.h"
#pragma warning(pop)

//構造体"Block"
struct Block {
    int x, y;       //ブロックの座標
    int is_active;  //ブロックの状態(1：表示 0：非表示)
};

#define WIDTH 13
#define HEIGHT 15

    char board[HEIGHT][WIDTH + 1] = {
        "             ",
        " BBBBBBBBBBB ",
        " GGGGGGGGGGG ",
        " GGGGGGGGGGG ",
        "             ",
        "             ",
        "             ",
        "             ",
        "             ",
        "             ",
        "             ",
        "             ",
        "      O      ",
        "             ",
        "     ===     "
    };

    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
    {
        ChangeWindowMode(TRUE);
        SetGraphMode(640, 480, 32);
        SetMainWindowText(L"ブロック崩し");

        if (DxLib_Init() == -1)
        {
            return -1;
        }

        SetDrawScreen(DX_SCREEN_BACK);

        while (ProcessMessage() == 0 &&
            CheckHitKey(KEY_INPUT_ESCAPE) == 0)
        {
            ClearDrawScreen();

            // マスサイズ
            int cellSize = 32;

            // 画面中央配置
            int offsetX = (640 - WIDTH * cellSize) / 2;
            int offsetY = (480 - HEIGHT * cellSize) / 2;

            // 描画
            for (int y = 0; y < HEIGHT; y++)
            {
                for (int x = 0; x < WIDTH; x++)
                {
                    char c = board[y][x];

                    int left = offsetX + x * cellSize;
                    int top = offsetY + y * cellSize;
                    int right = left + cellSize - 2;
                    int bottom = top + cellSize - 2;

                    // 紫ブロック
                    if (c == 'B')
                    {
                        DrawBox(
                            left,
                            top,
                            right,
                            bottom,
                            GetColor(255, 0, 255),
                            TRUE
                        );
                    }

                    // 緑ブロック
                    else if (c == 'G')
                    {
                        DrawBox(
                            left,
                            top,
                            right,
                            bottom,
                            GetColor(0, 255, 0),
                            TRUE
                        );
                    }

                    // ボール
                    else if (c == 'O')
                    {
                        DrawCircle(
                            left + cellSize / 2,
                            top + cellSize / 2,
                            5,
                            GetColor(255, 255, 255),
                            TRUE
                        );
                    }

                    // バー
                    else if (c == '=')
                    {
                        DrawBox(
                            left,
                            top + 10,
                            right,
                            bottom - 10,
                            GetColor(0, 255, 255),
                            TRUE
                        );
                    }
                }
            }

            ScreenFlip();
        }

        DxLib_End();
        return 0;
    }