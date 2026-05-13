#include "CScreen.h"
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>


// グローバル変数（必要なもののみ）
int g_currentFPS;
float g_jumpStep = 0.5f;
int g_fallState = 5; // 落下状態フラグ (0/1 を使う)
int g_isJumping = 0;
int g_jumpFrameCounter = 0;
int g_jumpAscending = 0;
int g_groundLeftBound = 1;   // 地面左範囲設定
int g_groundRightBound = 166; // 地面右範囲
float g_moveSpeed = 0.6f;    // 移動スピード
float g_gravity = 0.2f;

// 構造体
typedef struct
{
	float fPrevPosX;
	float fPrevPosY;
	float fPosX;
	float fPosY;
} PlayerCharacter;

// プロトタイプ宣言（名前を説明的に）
void InitializePlayer(PlayerCharacter* pPlayer);
int UpdatePlayer(PlayerCharacter* pPlayer);
void RenderScene(PlayerCharacter* pPlayer);
void DisplayFPS(void);
void CleanupGame(void);

// メイン関数
int main(void)
{
	// コンソールウィンドウの取得
	HWND hWnd = GetConsoleWindow();
	// ウィンドウの位置とサイズを設定 (コンソールウィンドウ, X座標, Y座標, 横幅, 高さ, 再描画フラグ)
	MoveWindow(hWnd, 0, 0, 1920, 1080, TRUE);


	while (1) {
		COLOR(WHITE, BLACK);

		PlayerCharacter player;

		DWORD dwExecLastTime;
		DWORD dwFPSLastTime;
		DWORD dwCurrentTime;
		int nCountFrame;
		timeBeginPeriod(1);
		dwExecLastTime = dwFPSLastTime = timeGetTime();
		nCountFrame = 0;
		// ゲーム初期化処理
		InitializePlayer(&player);
		CUROFF();

		do {
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) > 500) {
				g_currentFPS = nCountFrame * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				nCountFrame = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60)) {

				dwExecLastTime = dwCurrentTime;
				// ステータス更新処理
				if (UpdatePlayer(&player) == 1) {
					break;
				}
				// 画面描画処理
				RenderScene(&player);

#ifdef _DEBUG
				DisplayFPS();// DEBUGモードでFPS値を表示する
#endif

				nCountFrame++;
			}
		} while (!INP(KEY_ESC));
		CURON();
		// ゲーム終了処理
		CleanupGame();
		timeEndPeriod(1);
	}

	return 0;
}

// 初期化処理
void InitializePlayer(PlayerCharacter* pPlayer)
{
	pPlayer->fPosX = 1;  // ゲーム開始時のキャラクターの最初の位置
	pPlayer->fPosY = 22; // ゲーム開始時のキャラクターの最初の位置
}

// 更新処理
int UpdatePlayer(PlayerCharacter* pPlayer)
{
	pPlayer->fPrevPosX = pPlayer->fPosX; // 前フレームの位置保存（消去用）
	pPlayer->fPrevPosY = pPlayer->fPosY; // 前フレームの位置保存（ジャンプ判定）

	// 右移動
	if (INP(PK_D)) {
		pPlayer->fPosX = pPlayer->fPosX + g_moveSpeed;
		if (pPlayer->fPosX > g_groundRightBound) {
			pPlayer->fPosX = g_groundRightBound;
		}
	}

	// 左移動
	if (INP(PK_A)) {
		pPlayer->fPosX = pPlayer->fPosX - g_moveSpeed;
		if (pPlayer->fPosX < g_groundLeftBound) {
			pPlayer->fPosX = g_groundLeftBound;
		}
	}

	// ジャンプ中の上昇処理
	if (g_isJumping == 1) {
		pPlayer->fPosY = pPlayer->fPosY - g_jumpStep;
		g_jumpFrameCounter = g_jumpFrameCounter + 1;
	}

	// ジャンプキー入力（上昇開始）
	if (INP(PK_W) && (g_fallState == 0) && (g_jumpAscending == 0)) {
		g_isJumping = 1;
	}

	// ジャンプ上限判定
	if (g_jumpFrameCounter == 20) {
		g_isJumping = 0;
		g_jumpFrameCounter = 0;
	}

	// 重力適用
	pPlayer->fPosY = pPlayer->fPosY + g_gravity;

	// 地面判定（ゲームオーバー）
	if (pPlayer->fPosY > 42)
	{
		pPlayer->fPosY = 42; // 一番下の地面
		CLS(RED, BLACK);
		LOCATE(44, 15);
		printf("ゲームオーバー\n");
		system("pause");
		return 1;
	}

	// 空中地面の設定（各プラットフォームの当たり判定）
	if ((pPlayer->fPosY > 20) && (pPlayer->fPosY < 22) && (pPlayer->fPosX <= 10) && (g_fallState == 1))
	{
		pPlayer->fPosY = 21;
	}

	if ((pPlayer->fPosY > 16) && (pPlayer->fPosY < 18) && (pPlayer->fPosX <= 49) && (pPlayer->fPosX >= 39) && (g_fallState == 1))
	{
		pPlayer->fPosY = 17;
	}

	if ((pPlayer->fPosY > 10) && (pPlayer->fPosY < 12) && (pPlayer->fPosX <= 40) && (pPlayer->fPosX >= 30) && (g_fallState == 1))
	{
		pPlayer->fPosY = 11;
	}

	if ((pPlayer->fPosY > 14) && (pPlayer->fPosY < 16) && (pPlayer->fPosX <= 34) && (pPlayer->fPosX >= 24) && (g_fallState == 1))
	{
		pPlayer->fPosY = 15;
	}

	if ((pPlayer->fPosY > 17) && (pPlayer->fPosY < 19) && (pPlayer->fPosX <= 20) && (pPlayer->fPosX >= 10) && (g_fallState == 1))
	{
		pPlayer->fPosY = 18;
	}

	if ((pPlayer->fPosY > 18) && (pPlayer->fPosY < 20) && (pPlayer->fPosX <= 60) && (pPlayer->fPosX >= 50) && (g_fallState == 1))
	{
		pPlayer->fPosY = 19;
	}

	if ((pPlayer->fPosY > 8) && (pPlayer->fPosY < 10) && (pPlayer->fPosX <= 55) && (pPlayer->fPosX >= 45) && (g_fallState == 1))
	{
		pPlayer->fPosY = 9;
	}

	if ((pPlayer->fPosY > 6) && (pPlayer->fPosY < 8) && (pPlayer->fPosX <= 75) && (pPlayer->fPosX >= 65) && (g_fallState == 1))
	{
		pPlayer->fPosY = 7;
	}

	if ((pPlayer->fPosY > 6) && (pPlayer->fPosY < 8) && (pPlayer->fPosX <= 94) && (pPlayer->fPosX >= 84) && (g_fallState == 1))
	{
		pPlayer->fPosY = 7;
	}

	if ((pPlayer->fPosY > 34) && (pPlayer->fPosY < 36) && (pPlayer->fPosX <= 22) && (g_fallState == 1))
	{
		pPlayer->fPosY = 35;
	}

	if ((pPlayer->fPosY > 33) && (pPlayer->fPosY < 34) && (pPlayer->fPosX <= 4) && (g_fallState == 1))
	{
		CLS();
		LOCATE(44, 15);
		printf("ゲームスタート");
	}

	if ((pPlayer->fPosY > 34) && (pPlayer->fPosY < 36) && (pPlayer->fPosX <= 56) && (pPlayer->fPosX >= 34) && (g_fallState == 1))
	{
		pPlayer->fPosY = 35;
	}

	if ((pPlayer->fPosY > 15) && (pPlayer->fPosY < 17) && (pPlayer->fPosX <= 166) && (pPlayer->fPosX >= 156) && (g_fallState == 1))
	{
		pPlayer->fPosY = 16;
		CLS(BLUE);
		LOCATE(44, 15);
		printf("ゲームクリアー");

		WAIT(5000);
		return 1;
	}

	if ((pPlayer->fPosY > 30) && (pPlayer->fPosY < 32) && (pPlayer->fPosX <= 67) && (pPlayer->fPosX >= 57) && (g_fallState == 1))
	{
		pPlayer->fPosY = 31;
	}

	if ((pPlayer->fPosY > 27) && (pPlayer->fPosY < 29) && (pPlayer->fPosX <= 77) && (pPlayer->fPosX >= 67) && (g_fallState == 1))
	{
		pPlayer->fPosY = 28;
	}
	if ((pPlayer->fPosY > 23) && (pPlayer->fPosY < 25) && (pPlayer->fPosX <= 88) && (pPlayer->fPosX >= 78) && (g_fallState == 1))
	{
		pPlayer->fPosY = 24;
	}
	if ((pPlayer->fPosY > 19) && (pPlayer->fPosY < 21) && (pPlayer->fPosX <= 77) && (pPlayer->fPosX >= 67) && (g_fallState == 1))
	{
		pPlayer->fPosY = 20;
	}
	if ((pPlayer->fPosY > 25) && (pPlayer->fPosY < 27) && (pPlayer->fPosX <= 34) && (pPlayer->fPosX >= 24) && (g_fallState == 1))
	{
		pPlayer->fPosY = 26;
	}
	if ((pPlayer->fPosY > 10) && (pPlayer->fPosY < 12) && (pPlayer->fPosX <= 120) && (pPlayer->fPosX >= 110) && (g_fallState == 1))
	{
		pPlayer->fPosY = 11;
	}
	if ((pPlayer->fPosY > 18) && (pPlayer->fPosY < 20) && (pPlayer->fPosX <= 142) && (pPlayer->fPosX >= 132) && (g_fallState == 1))
	{
		pPlayer->fPosY = 19;
	}
	if ((pPlayer->fPosY > 21) && (pPlayer->fPosY < 23) && (pPlayer->fPosX <= 122) && (pPlayer->fPosX >= 112) && (g_fallState == 1))
	{
		pPlayer->fPosY = 22;
	}
	if ((pPlayer->fPosY > 23) && (pPlayer->fPosY < 25) && (pPlayer->fPosX <= 19) && (pPlayer->fPosX >= 9) && (g_fallState == 1))
	{
		pPlayer->fPosY = 24;
	}
	if ((pPlayer->fPosY > 29) && (pPlayer->fPosY < 31) && (pPlayer->fPosX <= 21) && (pPlayer->fPosX >= 11) && (g_fallState == 1))
	{
		pPlayer->fPosY = 30;
	}
	if ((pPlayer->fPosY > 32) && (pPlayer->fPosY < 34) && (pPlayer->fPosX <= 137) && (pPlayer->fPosX >= 127) && (g_fallState == 1))
	{
		pPlayer->fPosY = 33;
	}
	if ((pPlayer->fPosY > 27) && (pPlayer->fPosY < 29) && (pPlayer->fPosX <= 150) && (pPlayer->fPosX >= 140) && (g_fallState == 1))
	{
		pPlayer->fPosY = 28;
	}
	if ((pPlayer->fPosY > 22) && (pPlayer->fPosY < 24) && (pPlayer->fPosX <= 159) && (pPlayer->fPosX >= 149) && (g_fallState == 1))
	{
		pPlayer->fPosY = 23;
	}

	// ジャンプ/落下状態判定
	if ((pPlayer->fPosY > pPlayer->fPrevPosY))
	{
		g_fallState = 1;
	}
	if ((pPlayer->fPosY == pPlayer->fPrevPosY))
	{
		g_fallState = 0;
		g_jumpAscending = 0;
	}
	if (pPlayer->fPosY < pPlayer->fPrevPosY)
	{
		g_jumpAscending = 1;
	}

	return 0;
}

void RenderScene(PlayerCharacter* pPlayer)
{
	LOCATE(1, 42); //(x,y);
	COLOR(RED);
	printf("▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲");//51個 一番下の地面
	LOCATE(1, 22); // 地面表示1
	COLOR(GRAY);
	printf("■■■■■");
	LOCATE(10, 19); // 地面表示5
	printf("■■■■■");
	LOCATE(39, 18); // 地面表示2
	printf("■■■■■");
	LOCATE(30, 12); // 地面表示3
	printf("■■■■■");
	LOCATE(24, 16); // 地面表示4
	printf("■■■■■");
	LOCATE(50, 20); // 地面表示6
	printf("■■■■■");
	LOCATE(45, 10); // 地面表示7
	printf("■■■■■");
	LOCATE(65, 8); // 地面表示8
	printf("■■■■■");
	LOCATE(84, 8); // 地面表示9
	printf("■■■■■");
	LOCATE(1, 36); // 地面表示10
	printf("■■■■■■■■■■■");
	LOCATE(34, 36); // 地面表示11
	printf("■■■■■■■■■■■");
	LOCATE(57, 32); // 地面表示13
	printf("■■■■■");
	LOCATE(67, 29); // 地面表示14
	printf("■■■■■");
	LOCATE(78, 25); // 地面表示15
	printf("■■■■■");
	LOCATE(67, 21); // 地面表示16
	printf("■■■■■");
	LOCATE(24, 27); // 地面表示17
	printf("■■■■■");
	LOCATE(110, 12); // 地面表示18
	printf("■■■■■");
	LOCATE(132, 20); // 地面表示19
	printf("■■■■■");
	LOCATE(112, 23); // 地面表示20
	printf("■■■■■");
	LOCATE(9, 25); // 地面表示21
	printf("■■■■■");
	LOCATE(11, 31); // 地面表示22
	printf("■■■■■");
	LOCATE(127, 34); // 地面表示23
	printf("■■■■■");
	LOCATE(140, 29); // 地面表示24
	printf("■■■■■");
	LOCATE(149, 24); // 地面表示25
	printf("■■■■■");
	LOCATE(156, 17); // 地面表示12//ゴール
	COLOR(BLUE);
	printf("ゴール");


	LOCATE(pPlayer->fPrevPosX, pPlayer->fPrevPosY);
	printf("  "); // キャラクター消去
	LOCATE(pPlayer->fPosX, pPlayer->fPosY);
	printf("男"); // キャラクター表示

}

// フレームレート表示
void DisplayFPS(void)
{
	LOCATE(2, 4); // フレームレート表示場所
	COLOR(WHITE);
	printf("FPS : %d", g_currentFPS); // フレームレート表示
}

void CleanupGame(void)
{

}