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

float berX = 5 * 55.0f;
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(640, 480, 32);
    SetMainWindowText(L"ブロック崩し");

    if (DxLib_Init() == -1){
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    int cellSize = 32;

    int offsetX = (640 - WIDTH * cellSize) / 2;
    int offsetY = (480 - HEIGHT * cellSize) / 2;

    float LEFT_WALL = 0.0f;    
    float RIGHT_WALL = 640.0f;  
    float TOP_WALL = (float)offsetY;
    float BOTTOM_WALL = (float)(offsetY + HEIGHT * cellSize);

    ballX = (float)(offsetX + 6 * cellSize + cellSize / 2);
    ballY = (float)(offsetY + 13 * cellSize + cellSize / 2);

    
    auto resetBall = [&]() {
        ballX = (float)(offsetX + 6 * cellSize + cellSize / 2);
        ballY = (float)(offsetY + 13 * cellSize + cellSize / 2);
        vx = 3.0f;
        vy = -4.0f;
        };

    while (ProcessMessage() == 0 &&CheckHitKey(KEY_INPUT_ESCAPE) == 0){
        ClearDrawScreen();

        if (is_gameover == false) {

            // ボールの動き
            if (start == true)
            {
                ballX += vx;
                ballY += vy;

                // 左右の壁で反射
                if (ballX - BALL_R < LEFT_WALL) {
                    ballX = LEFT_WALL + BALL_R;
                    vx = -vx;
                }
                if (ballX + BALL_R > RIGHT_WALL) {
                    ballX = RIGHT_WALL  - BALL_R;
                    vx = -vx;
                }

                // 上の壁で反射
                if (ballY - BALL_R < TOP_WALL) {
                    ballY = TOP_WALL + BALL_R;
                    vy = -vy;
                }

                // 下に落ちたらゲームオーバー
                if (ballY - BALL_R > BOTTOM_WALL) {
                    is_gameover = true;
                }

                // : バーとの衝突
                float barLeft = berX;
                float barRight = berX + 3 * 32.0f;
                float barTop = (float)(offsetY + berY * cellSize + 10);
                float barBot = (float)(offsetY + berY * cellSize + cellSize - 10);

                if  (ballX + BALL_R > barLeft &&ballX - BALL_R < barRight &&ballY + BALL_R > barTop && ballY - BALL_R < barBot && vy > 0)
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

                        if (ballX + BALL_R > bkLeft &&ballX - BALL_R < bkRight &&ballY + BALL_R > bkTop &&ballY - BALL_R < bkBot)
                        {
                            board[y][x] = ' ';
                            vy = -vy;
                        }
                    }
                }
            }

            // 上キーで開始
            if (CheckHitKey(KEY_INPUT_UP)) start = true;

            // バー移動
            if (CheckHitKey(KEY_INPUT_LEFT) && berX > 0) { berX -= 4.0f; }
            if (CheckHitKey(KEY_INPUT_RIGHT) && berX < 640 - 3 * 32.0f) { berX += 4.0f; }

            // 描画
            for (int y = 0; y < HEIGHT; y++){
                for (int x = 0; x < WIDTH; x++){
                    char c = board[y][x];

                    int left = offsetX + x * cellSize;
                    int top = offsetY + y * cellSize;
                    int right = left + cellSize - 2;
                    int bottom = top + cellSize - 2;

                    if (c == 'B'){
                        DrawBox(left, top, right, bottom, GetColor(255, 0, 255), TRUE);
                    }
                    else if (c == 'G'){
                        DrawBox(left, top, right, bottom, GetColor(0, 255, 0), TRUE);
                    }
                }

                // バー描画
                for (int i = 0; i < 3; i++) {
                    int left = (int)(berX + i * 32);
                    int top = offsetY + berY * cellSize;
                    int right = left + 32 - 2;
                    int bottom = top + 32 - 2;
                    DrawBox(left, top + 10, right, bottom - 10, GetColor(0, 255, 255), TRUE);
                }
            }

            //  ボール描画
            DrawCircle((int)ballX, (int)ballY, (int)BALL_R, GetColor(255, 255, 0), TRUE);

        } 

        // ゲームオーバー画面
        if (is_gameover == true) {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
            DrawBox(0, 0, 640, 480, GetColor(0, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            SetFontSize(64);
            DrawString(160, 200, L"GAMEOVER", GetColor(255, 0, 0));
            SetFontSize(16);

            // リトライ処理（Rキーでリトライ）
            if (CheckHitKey(KEY_INPUT_R)) {
                is_gameover = false;
                start = false;
                berX = 5 * 55.0f;
                resetBall();   
                resetBoard();  
            }
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}