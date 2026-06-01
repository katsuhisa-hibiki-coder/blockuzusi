#define DX_NON_USING_UNICODE_FUNCTION

#pragma warning(push)
#pragma warning(disable: 4828 4010)
#include "DxLib.h"
#pragma warning(pop)

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
    "             ",
    "             ",
    "             "
};
//バーの初期値
int berX = 5;
int berY = 14;

float ballX = 5;
float ballY = 13;

int ballx = 1;
int bally = -1;
bool start = false;

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
        //ボールの動き
        if (start == true)
        {
            ballX += ballx;
            ballY += bally;
            WaitTimer(80);

            if (ballX <= -3 || ballX > WIDTH +1 ) {
                ballx = -ballx;
            }
            if (ballY <= -1) {
                bally = -bally;
            }
            if (ballY > HEIGHT){
                // ボール位置リセット
                 ballX = 5;
                 ballY = 13;

                // 移動方向リセット
                ballx = 1;
                bally = -1;

                // 停止状態に戻す
                start = false;
            }
        }

        // 上キーで開始
        if (CheckHitKey(KEY_INPUT_UP))
        {
            start = true;
        }

        // 左移動
        if (CheckHitKey(KEY_INPUT_LEFT)&& berX > -3){
            berX--;
            WaitTimer(80);
        }

        // 右移動
        if (CheckHitKey(KEY_INPUT_RIGHT)&& berX < WIDTH){
            berX++;
            WaitTimer(80);
        }
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
                DrawCircle(
                    offsetX + ballX * cellSize + cellSize / 2,
                    offsetY + ballY * cellSize + cellSize / 2,
                    5,
                    GetColor(255, 255, 255),
                    TRUE
                );

                // バー当たり判定
                if ((int)ballY == berY - 1)
                {
                    if (ballX >= berX && ballX <= berX + 2)
                    {
                        bally = -bally;
                    }
                }
                // ブロック当たり判定
                int bx = (int)ballX;
                int by = (int)ballY;

                // 範囲内か確認
                if (bx >= 0 && bx < WIDTH && by >= 0 && by < HEIGHT) {
                    // ブロックがあるか
                    if (board[by][bx] == 'B'|board[by][bx] == 'G'){
                        // ブロック消す
                        board[by][bx] = ' ';

                        // 跳ね返る
                        bally = -bally;
                    }
                }
            }
        }
        //バー描画
        for (int i = 0; i < 3; i++) {
            int left = offsetX + (berX + i) * cellSize;
            int top = offsetY + berY * cellSize;
            int right = left + cellSize - 2;
            int bottom = top + cellSize - 2;

            DrawBox(
                left,
                top + 10,
                right,
                bottom - 10,
                GetColor(0, 255, 255),
                TRUE
            );
        }
        ScreenFlip();
    }

    DxLib_End();
    return 0;
}