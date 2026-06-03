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
bool is_gameover = false;

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

        if (is_gameover == false) {

            //ボールの動き
            if (start == true)
            {
                ballX += ballx;
                ballY += bally;
                WaitTimer(80);

                if (ballX <= -3 || ballX > WIDTH + 1) {
                    ballx = -ballx;
                }
                if (ballY <= -1) {
                    bally = -bally;
                }
                // ボールが画面下部へ落ちた時の処理
                if (ballY > HEIGHT) {
                    // ボール位置リセット
                    ballX = 5;
                    ballY = 13;

                    // 移動方向リセット
                    ballx = 1;
                    bally = -1;

                    // 停止状態に戻す
                    start = false;

                    // ライフを1つ減らす
                    if (life > 0) {
                        life--;
                    }
                    // ライフが0の場合ゲームオーバーフラグを立てる
                    if (life <= 0) {
                        is_gameover = true;
                    }
                }
            }

            // 上キーで開始
            if (CheckHitKey(KEY_INPUT_UP))
            {
                start = true;
            }

            // 左移動
            if (CheckHitKey(KEY_INPUT_LEFT) && berX > -3) {
                berX--;
                WaitTimer(80);
            }

            // 右移動
            if (CheckHitKey(KEY_INPUT_RIGHT) && berX < WIDTH) {
                berX++;
                WaitTimer(80);
            }

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
                if (life <= 0) {
                    is_gameover = true;
                }
            }
            prevKeyL = currentKeyL;
        }

        // マスサイズ
        int cellSize = 32;
        
        // 画面中央配置
        int offsetX = (640 - WIDTH * cellSize) / 2;
        int offsetY = (480 - HEIGHT * cellSize) / 2;

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
            float barLeft = (float)(offsetX + berX * cellSize);
            float barRight = (float)(offsetX + (berX + 3) * cellSize);

            // パワーアップ中のバー拡張計算
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
                // ボール（文字としてのOがあれば描画する保険）
                else if (c == '0')
                {
                    DrawCircle(offsetX + ballX * cellSize + cellSize / 2, offsetY + ballY * cellSize + cellSize / 2, 5, GetColor(255, 255, 255), TRUE);
                }
            }
        }

        // ボールの描画（座標変数から描画）
        DrawCircle(
            offsetX + (int)(ballX * cellSize) + cellSize / 2,
            offsetY + (int)(ballY * cellSize) + cellSize / 2,
            5,
            GetColor(255, 255, 255),
            TRUE
        );

        // バー当たり判定
        if ((int)ballY == berY - 1)
        {
            int range = (itemTimer > 0) ? 4 : 2;
            if (ballX >= berX && ballX <= berX + 2)
            {
                bally = -bally;   
            }           
        }
                    
        // ブロック当たり判定
        int bx = (int)ballX;
        int by = (int)ballY;

        if (bx >= 0 && bx <= WIDTH && by >= 0 && by <= HEIGHT) {
            if (board[by][bx] == 'B' || board[by][bx] == 'G') {
                board[by][bx] = ' ';
                bally = -bally;
                score += 10; // ブロックを壊したらスコア加算
            }
        }

        // バー描画
        for (int i = 0; i < 3; i++) {
            int left = offsetX + (berX + i) * cellSize;
            int top = offsetY + berY * cellSize;
            int right = left + cellSize - 2;
            int bottom = top + cellSize - 2;

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

        // スコア・ライフ表示
        DrawFormatString(20, 20, GetColor(255, 255, 255), L"SCORE: %d", score);
        DrawFormatString(20, 45, GetColor(255, 255, 255), L"LIFE: %d", life);


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
                berX = 5;

                ballX = 5;
                ballY = 13;
                ballx = 1;
                bally = -1;

                score = 0;
                life = 3;
                itemActive = false;
                itemTimer = 0;

                for (int y = 0; y < HEIGHT; y++) {
                    for (int x = 0; x < WIDTH; x++) {
                        if (y == 1 && x >= 1 && x <= 11) {
                            board[y][x] = 'B';
                        }
                        else if ((y == 2 || y == 3) && x >= 1 && x <= 11) {
                            board[y][x] = 'G';
                        }
                        else {
                            board[y][x] = ' ';
                        }
                    }
                }
            }
        }

        // ゲームオーバー処理テスト（Gキーで開始）
        if (CheckHitKey(KEY_INPUT_G))
        {
            is_gameover = true;
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}