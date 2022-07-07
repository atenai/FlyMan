#include "CScreen.h"
#include<stdio.h>
#include<stdlib.h>


//�ϐ�
int g_nCountFPS;
int n;
int m;
float qitiao=0.5;
int jump=6;//�W�����v�X�s�[�h
int fall=5;//������X�s�[�h
int frame;
int frame1;
int frame2;
int frame3;
int stop;
int flag=0;
int fps=0;
int jump1=0;
int endleft=1;//�n�ʍ��͈͐ݒ�
int endright=166;//�n�ʉE�͈͂��L����
float speed=0.6;//�ړ��X�s�[�h
float gra=0.2;

//�\����
typedef struct
{
	float fPosX0;
	float fPosY0;
	float fPosX;
	float fPosY;
}character;

//�v���g�^�C�v�錾
void Init(character *pPlayer);
int Update(character* pPlayer);
void Draw(character* pPlayer);
void DispFPS(void);
void Uninit(void);

//���C���֐�
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
	//�Q�[�������������i�����ō��j
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
			//�X�e�[�^�X�X�V����
			if(Update(&aplayer)==1){
				break;
			}
			//��ʕ`�揈��
			Draw(&aplayer);
			//DEBUG���[�h��FPS�l��\������
#ifdef _DEBUG
			DispFPS();
#endif
			
			nCountFrame ++;
		}
	}while(!INP(KEY_ESC));
	CURON();
	//�Q�[�����I�������i�����ō��j
	Uninit();
	timeEndPeriod(1);
	}
	
}



void Init(character* pPlayer)
{

	pPlayer->fPosX=1;//�Q�[���J�n���̃L�����N�^�[�̍ŏ��̈ʒu
	pPlayer->fPosY=22;//�Q�[���J�n���̃L�����N�^�[�̍ŏ��̈ʒu
}



int Update(character* pPlayer)
{

	pPlayer->fPosX0=pPlayer->fPosX;//�O��̃L�����N�^�[���\������Ȃ�����
	pPlayer->fPosY0=pPlayer->fPosY;//�W�����v�ݒ�
	


	
	

	//�E�ړ�
	if(INP(PK_D)){
		pPlayer->fPosX=pPlayer->fPosX+speed;
		if(pPlayer->fPosX>endright){
			pPlayer->fPosX=endright;
		}
	}

	//���ړ�
	if(INP(PK_A)){
		pPlayer->fPosX=pPlayer->fPosX-speed;
		if(pPlayer->fPosX<endleft){
			pPlayer->fPosX=endleft;
		}
	}

	//�W�����v
	if(flag==1){
		pPlayer->fPosY=pPlayer->fPosY-qitiao;
		fps=fps+1;
	}

	//�W�����v����L�[
		if(INP(PK_W)&&(fall==0)&&(jump1==0)){
		flag=1;
	}

		//�W�����v���
	if(fps==20){
		flag=0;
		fps=0;	
	}




	pPlayer->fPosY=pPlayer->fPosY+ gra;
	
		if(pPlayer->fPosY>42)
		{
			pPlayer->fPosY=42;//��ԉ��̒n��
			CLS(RED,BLACK);
			LOCATE(44,15);
			printf("�Q�[���I�[�o�[\n");
			system("pause");
			return 1;
			
		}

			if((pPlayer->fPosY>20)&&(pPlayer->fPosY<22)&&(pPlayer->fPosX<=10)&&(fall==1))//�󒆒n�ʂ̐ݒ�1
		{
			pPlayer->fPosY=21;
		}

		if((pPlayer->fPosY>16)&&(pPlayer->fPosY<18)&&(pPlayer->fPosX<=49)&&(pPlayer->fPosX>=39)&&(fall==1))//�󒆒n�ʂ̐ݒ�2
		{
			pPlayer->fPosY=17;
		}

		if((pPlayer->fPosY>10)&&(pPlayer->fPosY<12)&&(pPlayer->fPosX<=40)&&(pPlayer->fPosX>=30)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=11;//�L�����N�^�[�̏���Ă���ʒu3
		}

		if((pPlayer->fPosY>14)&&(pPlayer->fPosY<16)&&(pPlayer->fPosX<=34)&&(pPlayer->fPosX>=24)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=15;//�L�����N�^�[�̏���Ă���ʒu4
		}

		if((pPlayer->fPosY>17)&&(pPlayer->fPosY<19)&&(pPlayer->fPosX<=20)&&(pPlayer->fPosX>=10)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=18;//�L�����N�^�[�̏���Ă���ʒu5
		}

				if((pPlayer->fPosY>18)&&(pPlayer->fPosY<20)&&(pPlayer->fPosX<=60)&&(pPlayer->fPosX>=50)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=19;//�L�����N�^�[�̏���Ă���ʒu6
		}

		if((pPlayer->fPosY>8)&&(pPlayer->fPosY<10)&&(pPlayer->fPosX<=55)&&(pPlayer->fPosX>=45)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=9;//�L�����N�^�[�̏���Ă���ʒu7
		}

		if((pPlayer->fPosY>6)&&(pPlayer->fPosY<8)&&(pPlayer->fPosX<=75)&&(pPlayer->fPosX>=65)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=7;//�L�����N�^�[�̏���Ă���ʒu8
		}

		if((pPlayer->fPosY>6)&&(pPlayer->fPosY<8)&&(pPlayer->fPosX<=94)&&(pPlayer->fPosX>=84)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=7;//�L�����N�^�[�̏���Ă���ʒu9
		}
		
			if((pPlayer->fPosY>34)&&(pPlayer->fPosY<36)&&(pPlayer->fPosX<=22)&&(fall==1))//�󒆒n�ʂ̐ݒ�10
		{
			pPlayer->fPosY=35;
		}

			if((pPlayer->fPosY>33)&&(pPlayer->fPosY<34)&&(pPlayer->fPosX<=4)&&(fall==1))//�o�O���Z�b�g
		{
			CLS();
			LOCATE(44,15);
			printf("�Q�[���X�^�[�g");	
		}

		if((pPlayer->fPosY>34)&&(pPlayer->fPosY<36)&&(pPlayer->fPosX<=56)&&(pPlayer->fPosX>=34)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=35;//�L�����N�^�[�̏���Ă���ʒu11
		}

				if((pPlayer->fPosY>15)&&(pPlayer->fPosY<17)&&(pPlayer->fPosX<=166)&&(pPlayer->fPosX>=156)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=16;//�L�����N�^�[�̏���Ă���ʒu12/�j�����̊y��
			CLS(BLUE);
			LOCATE(44,15);
			printf("�Q�[���N���A�b�[����");
			
		   WAIT(5000);
			return 1;
		}

		if((pPlayer->fPosY>30)&&(pPlayer->fPosY<32)&&(pPlayer->fPosX<=67)&&(pPlayer->fPosX>=57)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=31;//�L�����N�^�[�̏���Ă���ʒu13
		}

		if((pPlayer->fPosY>27)&&(pPlayer->fPosY<29)&&(pPlayer->fPosX<=77)&&(pPlayer->fPosX>=67)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=28;//�L�����N�^�[�̏���Ă���ʒu14
		}
		if((pPlayer->fPosY>23)&&(pPlayer->fPosY<25)&&(pPlayer->fPosX<=88)&&(pPlayer->fPosX>=78)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=24;//�L�����N�^�[�̏���Ă���ʒu15
		}
		if((pPlayer->fPosY>19)&&(pPlayer->fPosY<21)&&(pPlayer->fPosX<=77)&&(pPlayer->fPosX>=67)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=20;//�L�����N�^�[�̏���Ă���ʒu16
		}
		if((pPlayer->fPosY>25)&&(pPlayer->fPosY<27)&&(pPlayer->fPosX<=34)&&(pPlayer->fPosX>=24)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=26;//�L�����N�^�[�̏���Ă���ʒu17
		}
		if((pPlayer->fPosY>10)&&(pPlayer->fPosY<12)&&(pPlayer->fPosX<=120)&&(pPlayer->fPosX>=110)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=11;//�L�����N�^�[�̏���Ă���ʒu18
		}
		if((pPlayer->fPosY>18)&&(pPlayer->fPosY<20)&&(pPlayer->fPosX<=142)&&(pPlayer->fPosX>=132)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=19;//�L�����N�^�[�̏���Ă���ʒu19
		}
		if((pPlayer->fPosY>21)&&(pPlayer->fPosY<23)&&(pPlayer->fPosX<=122)&&(pPlayer->fPosX>=112)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=22;//�L�����N�^�[�̏���Ă���ʒu20
		}
		if((pPlayer->fPosY>23)&&(pPlayer->fPosY<25)&&(pPlayer->fPosX<=19)&&(pPlayer->fPosX>=9)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=24;//�L�����N�^�[�̏���Ă���ʒu21
		}
		if((pPlayer->fPosY>29)&&(pPlayer->fPosY<31)&&(pPlayer->fPosX<=21)&&(pPlayer->fPosX>=11)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=30;//�L�����N�^�[�̏���Ă���ʒu22
		}
		if((pPlayer->fPosY>32)&&(pPlayer->fPosY<34)&&(pPlayer->fPosX<=137)&&(pPlayer->fPosX>=127)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=33;//�L�����N�^�[�̏���Ă���ʒu23
		}
		if((pPlayer->fPosY>27)&&(pPlayer->fPosY<29)&&(pPlayer->fPosX<=150)&&(pPlayer->fPosX>=140)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=28;//�L�����N�^�[�̏���Ă���ʒu24
		}
		if((pPlayer->fPosY>22)&&(pPlayer->fPosY<24)&&(pPlayer->fPosX<=159)&&(pPlayer->fPosX>=149)&&(fall==1))//�󒆒n�ʂ̐ݒ�(x=1�u���b�N2�}�X)
		{
			pPlayer->fPosY=23;//�L�����N�^�[�̏���Ă���ʒu25
		}







	//�W�����v�ݒ�
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
	printf("������������������������������������������������������������������������������������������������������������������������������������������������������������������������");//51�� ��ԉ��̒n��
	LOCATE(1,22);//�n�ʕ\��1
	COLOR(GRAY);
	printf("����������");
	LOCATE(10,19);//�n�ʕ\��5
	printf("����������");
	LOCATE(39,18);//�n�ʕ\��2
	printf("����������");
	LOCATE(30,12);//�n�ʕ\��3
	printf("����������");
	LOCATE(24,16);//�n�ʕ\��4
	printf("����������");
	LOCATE(50,20);//�n�ʕ\��6
	printf("����������");
	LOCATE(45,10);//�n�ʕ\��7
	printf("����������");
	LOCATE(65,8);//�n�ʕ\��8
	printf("����������");
	LOCATE(84,8);//�n�ʕ\��9
	printf("����������");
	LOCATE(1,36);//�n�ʕ\��10
	printf("����������������������");
	LOCATE(34,36);//�n�ʕ\��11
	printf("����������������������");
	LOCATE(57,32);//�n�ʕ\��13
	printf("����������");
	LOCATE(67,29);//�n�ʕ\��14
	printf("����������");
	LOCATE(78,25);//�n�ʕ\��15
	printf("����������");
	LOCATE(67,21);//�n�ʕ\��16
	printf("����������");
	LOCATE(24,27);//�n�ʕ\��17
	printf("����������");
	LOCATE(110,12);//�n�ʕ\��18
	printf("����������");
	LOCATE(132,20);//�n�ʕ\��19
	printf("����������");
	LOCATE(112,23);//�n�ʕ\��20
	printf("����������");
	LOCATE(9,25);//�n�ʕ\��21
	printf("����������");
	LOCATE(11,31);//�n�ʕ\��22
	printf("����������");
	LOCATE(127,34);//�n�ʕ\��23
	printf("����������");
	LOCATE(140,29);//�n�ʕ\��24
	printf("����������");
	LOCATE(149,24);//�n�ʕ\��25
	printf("����������");
	LOCATE(156,17);//�n�ʕ\��12/�j�����̊y��
	COLOR(BLUE);
	printf("�j�����̊y��");



	LOCATE(pPlayer->fPosX0,pPlayer->fPosY0);
	printf("  ");//�L�����N�^�[����������
	LOCATE(pPlayer->fPosX,pPlayer->fPosY);
	printf("�j");//�L�����N�^�[�̌�����

}




//�t���[�����[�g
void DispFPS(void)
{
	LOCATE(2,4);//�t���[�����[�g�\���ꏊ
	COLOR(WHITE);
	printf("FPS : %d",g_nCountFPS);//�t���[�����[�g�\��
}

void Uninit(void)
{
	
}

