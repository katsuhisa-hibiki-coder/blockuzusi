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
    "      O      ",
    "             ",
    "             "
};

int berX = 5;
int berY = 14;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(640, 480, 32);
    SetMainWindowText(L"ブロック崩し");

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    // パワーアップアイテム
    float itemX = 0.0f, itemY = 0.0f;
    bool itemActive = false;
    int itemTimer = 0;

    // スコア管理
    int score = 0;

    while (ProcessMessage() == 0 &&
        CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        // 左移動
        if (CheckHitKey(KEY_INPUT_LEFT) && berX > -3) {
            berX--;
            WaitTimer(100);
        }

        // 右移動
        if (CheckHitKey(KEY_INPUT_RIGHT) && berX < WIDTH) {
            berX++;
            WaitTimer(100);
        }

        // スコア管理（Aキーでテスト加算）
        if (CheckHitKey(KEY_INPUT_A)) {
            score += 10;
            WaitTimer(100);
        }

        // マスサイズ
        int cellSize = 32;

        // 画面中央配置
        int offsetX = (640 - WIDTH * cellSize) / 2;
        int offsetY = (480 - HEIGHT * cellSize) / 2;

        SetDrawScreen(DX_SCREEN_BACK);

        // パワーアップアイテム（Sキーでテスト落下）
        if (CheckHitKey(KEY_INPUT_S) && !itemActive)
        {
            itemActive = true;
            itemX = (float)(offsetX + 6 * cellSize + cellSize / 2);
            itemY = (float)offsetY;
        }

        if (itemActive)
        {
            itemY += 3.0f;
            if (itemY > 480) itemActive = false;

            float barY = (float)(offsetY + berY * cellSize);
            float barLeft = (float)(offsetX + berX * cellSize);
            float barRight = (float)(offsetX + (berX + 3) * cellSize);

            // パワーアップアイテム
            if (itemTimer > 0)
            {
                barLeft -= 16.0f;
                barRight += 16.0f;
            }

            if (itemY >= barY && itemY <= barY + cellSize && itemX >= barLeft && itemX <= barRight)
            {
                itemActive = false;
                itemTimer = 300;
            }
        }

        if (itemTimer > 0)
        {
            itemTimer--;
        }

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
                    DrawBox(left, top, right, bottom, GetColor(255, 0, 255), TRUE);
                }
                // 緑ブロック
                else if (c == 'G')
                {
                    DrawBox(left, top, right, bottom, GetColor(0, 255, 0), TRUE);
                }
                // ボール
                else if (c == 'O')
                {
                    DrawCircle(left + cellSize / 2, top + cellSize / 2, 5, GetColor(255, 255, 255), TRUE);
                }
            }
        }

        // バー描画
        for (int i = 0; i < 3; i++) {
            int left = offsetX + (berX + i) * cellSize;
            int top = offsetY + berY * cellSize;
            int right = left + cellSize - 2;
            int bottom = top + cellSize - 2;

            // パワーアップアイテム
            int extend = (itemTimer > 0) ? 16 : 0;
            unsigned int barColor = (itemTimer > 0) ? GetColor(255, 200, 220) : GetColor(0, 255, 255);

            int finalLeft = left - (i == 0 ? extend : 0);
            int finalRight = right + (i == 2 ? extend : 0);

            DrawBox(finalLeft, top + 10, finalRight, bottom - 10, barColor, TRUE);
        }

        // パワーアップアイテム
        if (itemActive)
        {
            DrawCircle((int)itemX, (int)itemY, 8, GetColor(255, 50, 150), TRUE);
            DrawCircle((int)itemX, (int)itemY, 6, GetColor(255, 255, 255), TRUE);
        }

        // スコア管理
        DrawFormatString(20, 20, GetColor(255, 255, 255), L"SCORE: %d", score);

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}