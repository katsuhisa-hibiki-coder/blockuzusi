#define DX_NON_USING_UNICODE_FUNCTION

#pragma warning(push)
#pragma warning(disable: 4828 4010)
#include "DxLib.h"
#pragma warning(pop)

struct Block {
    int x, y;
    int is_active;
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
    "             ",
    "             ",
    "             "
};

float berX = 5 * 32.0f;
int berY = 14;

float ballX, ballY;
float vx = 3.0f;
float vy = -4.0f;
const float BALL_R = 6.0f;

bool start = false;
bool is_gameover = false;

void resetBoard() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 1 && x >= 1 && x <= 11)
                board[y][x] = 'B';
            else if ((y == 2 || y == 3) && x >= 1 && x <= 11)
                board[y][x] = 'G';
            else
                board[y][x] = ' ';
        }
    }
}

void resetBall()
{
    int cellSize = 32;
    int offsetX = (640 - WIDTH * cellSize) / 2;
    int offsetY = (480 - HEIGHT * cellSize) / 2;

    ballX = (float)(offsetX + 6 * cellSize + cellSize / 2);
    ballY = (float)(offsetY + 13 * cellSize + cellSize / 2);

    vx = 3.0f;
    vy = -4.0f;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(640, 480, 32);
    SetMainWindowText(L"ブロック崩し");

    if (DxLib_Init() == -1) {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);
    resetBall();

    // パワーアップアイテム
    float itemX = 0.0f, itemY = 0.0f;
    bool itemActive = false;
    int itemTimer = 0;

    // スコア管理
    int score = 0;

    // ライフ管理
    int life = 3;

    // キーの1つ前の状態を記憶
    bool prevKeyA = false;
    bool prevKeyL = false;
    bool prevKeyS = false;

    while (ProcessMessage() == 0 &&
        CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();
        int cellSize = 32;

        int offsetX = (640 - WIDTH * cellSize) / 2;
        int offsetY = (480 - HEIGHT * cellSize) / 2;

        float LEFT_WALL = 0.0f;
        float RIGHT_WALL = 640.0f;
        float TOP_WALL = (float)offsetY;
        float BOTTOM_WALL = (float)(offsetY + HEIGHT * cellSize);

        // スコア管理（Aキーでテスト加算）
        bool currentKeyA = CheckHitKey(KEY_INPUT_A);
        if (currentKeyA && !prevKeyA) {
            score += 10;
        }
        prevKeyA = currentKeyA;


        // ライフ管理（Lキーでテスト減少）
        bool currentKeyL = CheckHitKey(KEY_INPUT_L);
        if (currentKeyL && !prevKeyL) {
            if (life > 0) {
                life--;
            }
        }
        prevKeyL = currentKeyL;

        SetDrawScreen(DX_SCREEN_BACK);

        // パワーアップアイテム（Sキーでテスト落下）
        bool currentKeyS = CheckHitKey(KEY_INPUT_S);
        if (currentKeyS && !prevKeyS && !itemActive)
        {
            itemActive = true;
            itemX = (float)(offsetX + 6 * cellSize + cellSize / 2);
            itemY = (float)offsetY;
        }
        prevKeyS = currentKeyS;


        if (itemActive)
        {
            itemY += 3.0f;
            if (itemY > 480) itemActive = false;

            float barY = (float)(offsetY + berY * cellSize);
            float barLeft = offsetX + berX;
            float barRight = offsetX + berX + 3 * cellSize;

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
        if (is_gameover == false) {

            // ボールの動き
            if (start == true) {
                ballX += vx;
                ballY += vy;
            }
        }
        if (start == false)
        {
            ballX = (float)(offsetX + berX + 1.5f * cellSize);
            ballY = (float)(offsetY + berY * cellSize + 10 - BALL_R);
        }

        // パワーアップアイテムのバー描画
        for (int i = 0; i < 3; i++) {
            int left = (int)(offsetX + berX + i * cellSize);
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

        // パワーアップアイテム描画
        if (itemActive)
        {
            DrawCircle((int)itemX, (int)itemY, 8, GetColor(255, 50, 150), TRUE);
            DrawCircle((int)itemX, (int)itemY, 6, GetColor(255, 255, 255), TRUE);
        }

        // スコア管理
        DrawFormatString(20, 20, GetColor(255, 255, 255), L"SCORE: %d", score);

        // ライフ管理
        DrawFormatString(20, 45, GetColor(255, 255, 255), L"LIFE: %d", life);
        DrawFormatString(20, 70,GetColor(255, 255, 255),L"berX=%.1f", berX);

        // 左右の壁で反射
        if (ballX - BALL_R < LEFT_WALL) {
            ballX = LEFT_WALL + BALL_R;
            vx = -vx;
        }
        if (ballX + BALL_R > RIGHT_WALL) {
            ballX = RIGHT_WALL - BALL_R;
            vx = -vx;
        }

        // 上の壁で反射
        if (ballY - BALL_R < TOP_WALL) {
            ballY = TOP_WALL + BALL_R;
            vy = -vy;
        }

        // 下に落ちたらゲームオーバー
        if (ballY + BALL_R > BOTTOM_WALL) {
            life--; 
            if (life > 0) {
                start = false;
                resetBall();
            }
            else {
                is_gameover = true;
            }
        }

        // : バーとの衝突
        float barLeft = offsetX + berX;
        float barRight = offsetX + berX + 3 * 32.0f;
        float barTop = (float)(offsetY + berY * cellSize + 10);
        float barBot = (float)(offsetY + berY * cellSize + cellSize - 10);

        if (ballX + BALL_R > barLeft && ballX - BALL_R < barRight && ballY + BALL_R > barTop && ballY - BALL_R < barBot && vy > 0)
        {
            vy = -vy;
            ballY = barTop - BALL_R;
        }

        //  ブロックとの衝突
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (board[y][x] == ' ') continue;

                float bkLeft = (float)(offsetX + x * cellSize);
                float bkRight = (float)(offsetX + (x + 1) * cellSize);
                float bkTop = (float)(offsetY + y * cellSize);
                float bkBot = (float)(offsetY + (y + 1) * cellSize);

                if (ballX + BALL_R > bkLeft && ballX - BALL_R < bkRight && ballY + BALL_R > bkTop && ballY - BALL_R < bkBot)
                {
                    board[y][x] = ' ';
                    vy = -vy;
                    score += 10;
                }
            }
        }

        // 上キーで開始
        if (CheckHitKey(KEY_INPUT_UP)) start = true;

        // バー移動
        if (CheckHitKey(KEY_INPUT_LEFT) && berX > -offsetX)berX -= 4.0f;
        if (CheckHitKey(KEY_INPUT_RIGHT) && berX < 640 - offsetX - 3 * cellSize)berX += 4.0f;

        // 描画
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                char c = board[y][x];

                int left = offsetX + x * cellSize;
                int top = offsetY + y * cellSize;
                int right = left + cellSize - 2;
                int bottom = top + cellSize - 2;

                if (c == 'B') {
                    DrawBox(left, top, right, bottom, GetColor(255, 0, 255), TRUE);
                }
                else if (c == 'G') {
                    DrawBox(left, top, right, bottom, GetColor(0, 255, 0), TRUE);
                }
            }

          
        }
        // バー描画
        for (int i = 0; i < 3; i++) {
            int left = (int)(offsetX + berX + i * cellSize);
            int top = offsetY + berY * cellSize;
            int right = left + cellSize - 2;
            int bottom = top + cellSize - 2;

            int extend = (itemTimer > 0) ? 16 : 0;
            unsigned int barColor = (itemTimer > 0)
                ? GetColor(255, 200, 220)
                : GetColor(0, 255, 255);

            int finalLeft = left - (i == 0 ? extend : 0);
            int finalRight = right + (i == 2 ? extend : 0);

            DrawBox(finalLeft, top + 10,
                finalRight, bottom - 10,
                barColor, TRUE);
        }

        //  ボール描画
        DrawCircle((int)ballX, (int)ballY, (int)BALL_R, GetColor(255, 255, 0), TRUE);

        // ゲームオーバー画面
        if (is_gameover == true) {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
            DrawBox(0, 0, 640, 480, GetColor(0, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            SetFontSize(64);
            DrawString(160, 200, L"GAMEOVER", GetColor(255, 0, 0));
            SetFontSize(16);

            // リretry処理（Rキーでリトライ）
            if (CheckHitKey(KEY_INPUT_R)) {
                is_gameover = false;
                start = false;
                berX = 5 * 32.0f;
                resetBall();
                resetBoard();
            }
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}