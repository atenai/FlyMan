#include "CScreen.h"
#include<stdio.h>
#include<stdlib.h>


//変数
int g_nCountFPS;
int n;
int m;
float qitiao=0.5;
int jump=6;//ジャンプスピード
int fall=5;//落ちるスピード
int frame;
int frame1;
int frame2;
int frame3;
int stop;
int flag=0;
int fps=0;
int jump1=0;
int endleft=1;//地面左範囲設定
int endright=166;//地面右範囲を広げる
float speed=0.6;//移動スピード
float gra=0.2;

//構造体
typedef struct
{
	float fPosX0;
	float fPosY0;
	float fPosX;
	float fPosY;
}character;

//プロトタイプ宣言
void Init(character *pPlayer);
int Update(character* pPlayer);
void Draw(character* pPlayer);
void DispFPS(void);
void Uninit(void);

//メイン関数
void main(void)
{
	while(1){
		COLOR(WHITE,BLACK);
	
	character aplayer;
	

	frame=0;
	frame1=0;
	frame2=0;
	frame3=0;
	stop=1;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	int nCountFrame;
	timeBeginPeriod(1);
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	nCountFrame = 0;
	//ゲーム初期化処理（自分で作る）
	Init(&aplayer);
	CUROFF();

	do {
		dwCurrentTime= timeGetTime();
		if((dwCurrentTime - dwFPSLastTime) > 500){
			g_nCountFPS = nCountFrame * 1000 / (dwCurrentTime - dwFPSLastTime);
			dwFPSLastTime = dwCurrentTime;
			nCountFrame = 0;
		}

		if((dwCurrentTime - dwExecLastTime) >= (1000/60)){
			
			dwExecLastTime = dwCurrentTime;
			//ステータス更新処理
			if(Update(&aplayer)==1){
				break;
			}
			//画面描画処理
			Draw(&aplayer);
			//DEBUGモードでFPS値を表示する
#ifdef _DEBUG
			DispFPS();
#endif
			
			nCountFrame ++;
		}
	}while(!INP(KEY_ESC));
	CURON();
	//ゲーム初終了処理（自分で作る）
	Uninit();
	timeEndPeriod(1);
	}
	
}



void Init(character* pPlayer)
{

	pPlayer->fPosX=1;//ゲーム開始時のキャラクターの最初の位置
	pPlayer->fPosY=22;//ゲーム開始時のキャラクターの最初の位置
}



int Update(character* pPlayer)
{

	pPlayer->fPosX0=pPlayer->fPosX;//前回のキャラクターが表示されなくする
	pPlayer->fPosY0=pPlayer->fPosY;//ジャンプ設定
	


	
	

	//右移動
	if(INP(PK_D)){
		pPlayer->fPosX=pPlayer->fPosX+speed;
		if(pPlayer->fPosX>endright){
			pPlayer->fPosX=endright;
		}
	}

	//左移動
	if(INP(PK_A)){
		pPlayer->fPosX=pPlayer->fPosX-speed;
		if(pPlayer->fPosX<endleft){
			pPlayer->fPosX=endleft;
		}
	}

	//ジャンプ
	if(flag==1){
		pPlayer->fPosY=pPlayer->fPosY-qitiao;
		fps=fps+1;
	}

	//ジャンプ操作キー
		if(INP(PK_W)&&(fall==0)&&(jump1==0)){
		flag=1;
	}

		//ジャンプ上限
	if(fps==20){
		flag=0;
		fps=0;	
	}




	pPlayer->fPosY=pPlayer->fPosY+ gra;
	
		if(pPlayer->fPosY>42)
		{
			pPlayer->fPosY=42;//一番下の地面
			CLS(RED,BLACK);
			LOCATE(44,15);
			printf("ゲームオーバー\n");
			system("pause");
			return 1;
			
		}

			if((pPlayer->fPosY>20)&&(pPlayer->fPosY<22)&&(pPlayer->fPosX<=10)&&(fall==1))//空中地面の設定1
		{
			pPlayer->fPosY=21;
		}

		if((pPlayer->fPosY>16)&&(pPlayer->fPosY<18)&&(pPlayer->fPosX<=49)&&(pPlayer->fPosX>=39)&&(fall==1))//空中地面の設定2
		{
			pPlayer->fPosY=17;
		}

		if((pPlayer->fPosY>10)&&(pPlayer->fPosY<12)&&(pPlayer->fPosX<=40)&&(pPlayer->fPosX>=30)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=11;//キャラクターの上っている位置3
		}

		if((pPlayer->fPosY>14)&&(pPlayer->fPosY<16)&&(pPlayer->fPosX<=34)&&(pPlayer->fPosX>=24)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=15;//キャラクターの上っている位置4
		}

		if((pPlayer->fPosY>17)&&(pPlayer->fPosY<19)&&(pPlayer->fPosX<=20)&&(pPlayer->fPosX>=10)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=18;//キャラクターの上っている位置5
		}

				if((pPlayer->fPosY>18)&&(pPlayer->fPosY<20)&&(pPlayer->fPosX<=60)&&(pPlayer->fPosX>=50)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=19;//キャラクターの上っている位置6
		}

		if((pPlayer->fPosY>8)&&(pPlayer->fPosY<10)&&(pPlayer->fPosX<=55)&&(pPlayer->fPosX>=45)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=9;//キャラクターの上っている位置7
		}

		if((pPlayer->fPosY>6)&&(pPlayer->fPosY<8)&&(pPlayer->fPosX<=75)&&(pPlayer->fPosX>=65)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=7;//キャラクターの上っている位置8
		}

		if((pPlayer->fPosY>6)&&(pPlayer->fPosY<8)&&(pPlayer->fPosX<=94)&&(pPlayer->fPosX>=84)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=7;//キャラクターの上っている位置9
		}
		
			if((pPlayer->fPosY>34)&&(pPlayer->fPosY<36)&&(pPlayer->fPosX<=22)&&(fall==1))//空中地面の設定10
		{
			pPlayer->fPosY=35;
		}

			if((pPlayer->fPosY>33)&&(pPlayer->fPosY<34)&&(pPlayer->fPosX<=4)&&(fall==1))//バグリセット
		{
			CLS();
			LOCATE(44,15);
			printf("ゲームスタート");	
		}

		if((pPlayer->fPosY>34)&&(pPlayer->fPosY<36)&&(pPlayer->fPosX<=56)&&(pPlayer->fPosX>=34)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=35;//キャラクターの上っている位置11
		}

				if((pPlayer->fPosY>15)&&(pPlayer->fPosY<17)&&(pPlayer->fPosX<=166)&&(pPlayer->fPosX>=156)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=16;//キャラクターの上っている位置12/男たちの楽園
			CLS(BLUE);
			LOCATE(44,15);
			printf("ゲームクリアッー♂♂");
			
		   WAIT(5000);
			return 1;
		}

		if((pPlayer->fPosY>30)&&(pPlayer->fPosY<32)&&(pPlayer->fPosX<=67)&&(pPlayer->fPosX>=57)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=31;//キャラクターの上っている位置13
		}

		if((pPlayer->fPosY>27)&&(pPlayer->fPosY<29)&&(pPlayer->fPosX<=77)&&(pPlayer->fPosX>=67)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=28;//キャラクターの上っている位置14
		}
		if((pPlayer->fPosY>23)&&(pPlayer->fPosY<25)&&(pPlayer->fPosX<=88)&&(pPlayer->fPosX>=78)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=24;//キャラクターの上っている位置15
		}
		if((pPlayer->fPosY>19)&&(pPlayer->fPosY<21)&&(pPlayer->fPosX<=77)&&(pPlayer->fPosX>=67)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=20;//キャラクターの上っている位置16
		}
		if((pPlayer->fPosY>25)&&(pPlayer->fPosY<27)&&(pPlayer->fPosX<=34)&&(pPlayer->fPosX>=24)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=26;//キャラクターの上っている位置17
		}
		if((pPlayer->fPosY>10)&&(pPlayer->fPosY<12)&&(pPlayer->fPosX<=120)&&(pPlayer->fPosX>=110)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=11;//キャラクターの上っている位置18
		}
		if((pPlayer->fPosY>18)&&(pPlayer->fPosY<20)&&(pPlayer->fPosX<=142)&&(pPlayer->fPosX>=132)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=19;//キャラクターの上っている位置19
		}
		if((pPlayer->fPosY>21)&&(pPlayer->fPosY<23)&&(pPlayer->fPosX<=122)&&(pPlayer->fPosX>=112)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=22;//キャラクターの上っている位置20
		}
		if((pPlayer->fPosY>23)&&(pPlayer->fPosY<25)&&(pPlayer->fPosX<=19)&&(pPlayer->fPosX>=9)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=24;//キャラクターの上っている位置21
		}
		if((pPlayer->fPosY>29)&&(pPlayer->fPosY<31)&&(pPlayer->fPosX<=21)&&(pPlayer->fPosX>=11)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=30;//キャラクターの上っている位置22
		}
		if((pPlayer->fPosY>32)&&(pPlayer->fPosY<34)&&(pPlayer->fPosX<=137)&&(pPlayer->fPosX>=127)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=33;//キャラクターの上っている位置23
		}
		if((pPlayer->fPosY>27)&&(pPlayer->fPosY<29)&&(pPlayer->fPosX<=150)&&(pPlayer->fPosX>=140)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=28;//キャラクターの上っている位置24
		}
		if((pPlayer->fPosY>22)&&(pPlayer->fPosY<24)&&(pPlayer->fPosX<=159)&&(pPlayer->fPosX>=149)&&(fall==1))//空中地面の設定(x=1ブロック2マス)
		{
			pPlayer->fPosY=23;//キャラクターの上っている位置25
		}







	//ジャンプ設定
	if((pPlayer->fPosY>pPlayer->fPosY0))
	{
		fall=1;
	}
	if((pPlayer->fPosY==pPlayer->fPosY0))
	{
		fall=0;
		jump1=0;
	}
	if(pPlayer->fPosY<pPlayer->fPosY0)
	{
		jump1=1;
	}
}






void Draw(character* pPlayer)
{
	LOCATE(1,42);//(x,y);
	COLOR(RED);
	printf("▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲");//51個 一番下の地面
	LOCATE(1,22);//地面表示1
	COLOR(GRAY);
	printf("■■■■■");
	LOCATE(10,19);//地面表示5
	printf("■■■■■");
	LOCATE(39,18);//地面表示2
	printf("■■■■■");
	LOCATE(30,12);//地面表示3
	printf("■■■■■");
	LOCATE(24,16);//地面表示4
	printf("■■■■■");
	LOCATE(50,20);//地面表示6
	printf("■■■■■");
	LOCATE(45,10);//地面表示7
	printf("■■■■■");
	LOCATE(65,8);//地面表示8
	printf("■■■■■");
	LOCATE(84,8);//地面表示9
	printf("■■■■■");
	LOCATE(1,36);//地面表示10
	printf("■■■■■■■■■■■");
	LOCATE(34,36);//地面表示11
	printf("■■■■■■■■■■■");
	LOCATE(57,32);//地面表示13
	printf("■■■■■");
	LOCATE(67,29);//地面表示14
	printf("■■■■■");
	LOCATE(78,25);//地面表示15
	printf("■■■■■");
	LOCATE(67,21);//地面表示16
	printf("■■■■■");
	LOCATE(24,27);//地面表示17
	printf("■■■■■");
	LOCATE(110,12);//地面表示18
	printf("■■■■■");
	LOCATE(132,20);//地面表示19
	printf("■■■■■");
	LOCATE(112,23);//地面表示20
	printf("■■■■■");
	LOCATE(9,25);//地面表示21
	printf("■■■■■");
	LOCATE(11,31);//地面表示22
	printf("■■■■■");
	LOCATE(127,34);//地面表示23
	printf("■■■■■");
	LOCATE(140,29);//地面表示24
	printf("■■■■■");
	LOCATE(149,24);//地面表示25
	printf("■■■■■");
	LOCATE(156,17);//地面表示12/男たちの楽園
	COLOR(BLUE);
	printf("男たちの楽園");



	LOCATE(pPlayer->fPosX0,pPlayer->fPosY0);
	printf("  ");//キャラクター増幅を消す
	LOCATE(pPlayer->fPosX,pPlayer->fPosY);
	printf("男");//キャラクターの見た目

}




//フレームレート
void DispFPS(void)
{
	LOCATE(2,4);//フレームレート表示場所
	COLOR(WHITE);
	printf("FPS : %d",g_nCountFPS);//フレームレート表示
}

void Uninit(void)
{
	
}

