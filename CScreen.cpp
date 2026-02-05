#include "CScreen.h"
#include <string.h>

CScreen __scr; // 実体定義

// --- ヘルパー ---
bool CScreen::IsPlatformWin32NT()
{
	OSVERSIONINFO oi = {};
	oi.dwOSVersionInfoSize = sizeof(oi);
	if (GetVersionEx(&oi) == 0) // 取得失敗時は NT と仮定
		return true;
	return (oi.dwPlatformId == VER_PLATFORM_WIN32_NT);
}

WORD CScreen::MakeColorAtrb(unsigned uText, unsigned uBack)
{
	WORD attr = 0;

	// 文字色 (簡易マッピング)
	switch (uText)
	{
	default:
	case WHITE:	attr |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
	case BLACK:	attr |= 0; break;
	case RED:	attr |= FOREGROUND_RED; break;
	case GREEN:	attr |= FOREGROUND_GREEN; break;
	case BLUE:	attr |= FOREGROUND_BLUE; break;
	case YELLOW: attr |= FOREGROUND_RED | FOREGROUND_GREEN; break;
	case PURPLE: attr |= FOREGROUND_RED | FOREGROUND_BLUE; break;
	case CYAN:	attr |= FOREGROUND_GREEN | FOREGROUND_BLUE; break;
	case GRAY:	attr |= FOREGROUND_INTENSITY; break;
	}

	// 高輝度簡易処理
	if (uText >= H_BLACK && uText <= H_WHITE)
		attr |= FOREGROUND_INTENSITY;

	// 背景色 (簡易マッピング)
	WORD back = 0;
	switch (uBack)
	{
	default:
	case BLACK: back = 0; break;
	case RED:	back = BACKGROUND_RED; break;
	case GREEN: back = BACKGROUND_GREEN; break;
	case BLUE:	back = BACKGROUND_BLUE; break;
	case YELLOW: back = BACKGROUND_RED | BACKGROUND_GREEN; break;
	case PURPLE: back = BACKGROUND_RED | BACKGROUND_BLUE; break;
	case CYAN:	back = BACKGROUND_GREEN | BACKGROUND_BLUE; break;
	case WHITE: back = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE; break;
	}
	attr |= back;
	return attr;
}

// --- コンストラクタ / デストラクタ ---
CScreen::CScreen()
{
	m_hCOut = GetStdHandle(STD_OUTPUT_HANDLE);
	m_hCIn  = GetStdHandle(STD_INPUT_HANDLE);
	m_bWinNT = IsPlatformWin32NT();
	m_ptBase.x = 0;
	m_ptBase.y = 0;
	m_wForeColor = MakeColorAtrb(WHITE, BLACK);
	m_wBackColor = 0;
	memset(m_dwKeyMap, 0, sizeof(m_dwKeyMap));
	m_crdMouse.X = m_crdMouse.Y = 0;
	// カーソル情報初期化
	ZeroMemory(&m_ccinfo, sizeof(m_ccinfo));
	m_ccinfo.bVisible = true;
	m_ccinfo.dwSize = INITIAL_CURSOR_HEIGHT;
	GetConsoleCursorInfo(m_hCOut, &m_ccinfo);
	SetConsoleTextAttribute(m_hCOut, m_wForeColor);
}

CScreen::~CScreen()
{
	CleanupSound();
}

// --- 表示/カーソル ---
void CScreen::Cursor(bool bVisible, DWORD dwSize)
{
	if (m_hCOut == INVALID_HANDLE_VALUE) return;
	if (dwSize) m_ccinfo.dwSize = (DWORD)dwSize;
	m_ccinfo.bVisible = bVisible ? TRUE : FALSE;
	SetConsoleCursorInfo(m_hCOut, &m_ccinfo);
}

void CScreen::Locate(int nXpos, int nYpos)
{
	if (m_hCOut == INVALID_HANDLE_VALUE) return;
	COORD pos;
	pos.X = (SHORT)(nXpos + m_ptBase.x);
	pos.Y = (SHORT)(nYpos + m_ptBase.y);
	SetConsoleCursorPosition(m_hCOut, pos);
}

void CScreen::LocateBase(int nXpos, int nYpos)
{
	m_ptBase.x = (SHORT)nXpos;
	m_ptBase.y = (SHORT)nYpos;
}

// --- 色 / 画面 ---
void CScreen::Color(unsigned uText, unsigned uBack)
{
	m_wForeColor = MakeColorAtrb(uText, uBack);
	SetConsoleTextAttribute(m_hCOut, m_wForeColor);
}

void CScreen::BackColor(unsigned uBack)
{
	WORD attr = MakeColorAtrb(INVALID_COLOR, uBack);
	// MakeColorAtrb で文字色がデフォルトになるので、そのままセット
	SetConsoleTextAttribute(m_hCOut, attr);
}

void CScreen::Clear(unsigned uText, unsigned uBack)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD written = 0;
	if (!GetConsoleScreenBufferInfo(m_hCOut, &csbi)) return;

	COORD home = { 0, 0 };
	DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacterA(m_hCOut, ' ', cells, home, &written);

	WORD attr = (uText == INVALID_COLOR && uBack == INVALID_COLOR) ? m_wForeColor : MakeColorAtrb(uText == INVALID_COLOR ? WHITE : uText, uBack == INVALID_COLOR ? BLACK : uBack);
	FillConsoleOutputAttribute(m_hCOut, attr, cells, home, &written);
	SetConsoleCursorPosition(m_hCOut, home);
}

// --- 入力 / 状態 ---
int CScreen::Inp(UINT uPort)
{
	// 仮実装: 仮想キーでの判定を行う (GetAsyncKeyState)
	SHORT s = 0;
	// ゲームパッド等の特殊ポートは未対応 -> 0 を返す
	if (uPort >= PJ1_XPOS && uPort <= PJ_BTNS) {
		return 0;
	}
	// GetAsyncKeyState は短絡的に扱う
	s = GetAsyncKeyState((int)uPort);
	return (s & 0x8000) ? 1 : 0;
}

void CScreen::InpClear()
{
	FlushConsoleInputBuffer(m_hCIn);
}

// --- 全画面 (未完全) ---
bool CScreen::FullScreen(bool /*bFull*/)
{
	// 簡易実装: サポートせず false を返す
	return false;
}

// --- サウンド系 (スタブ実装) ---
void CScreen::CleanupSound()
{
	// スタブ: リソース解放が必要ならここに実装
}

int CScreen::OpenWave(LPCTSTR /*pszPath*/) { return 0; }
bool CScreen::PlayWave(int /*nWaveHandle*/, bool /*bLoop*/) { return false; }
bool CScreen::SetWaveVolume(int /*nWaveHandle*/, DWORD /*dwPercent*/) { return false; }
bool CScreen::SetWavePan(int /*nWaveHandle*/, DWORD /*dwPan*/) { return false; }
bool CScreen::StopWave(int /*nWaveHandle*/) { return false; }
bool CScreen::CloseWave(int /*nWaveHandle*/) { return false; }
bool CScreen::IsPlayingWave(int /*nWaveHandle*/) { return false; }

int CScreen::OpenMIDI(LPCTSTR /*pszPath*/) { return 0; }
bool CScreen::PlayMIDI(int /*nMIDIHandle*/, bool /*bLoop*/) { return false; }
bool CScreen::SetMIDIVolume(int /*nMIDIHandle*/, DWORD /*dwPercent*/) { return false; }
bool CScreen::StopMIDI(int /*nMIDIHandle*/) { return false; }
bool CScreen::CloseMIDI(int /*nMIDIHandle*/) { return false; }
bool CScreen::IsPlayingMIDI(int /*nMIDIHandle*/) { return false; }

int CScreen::OpenMP3(LPCTSTR /*pszPath*/) { return 0; }
bool CScreen::PlayMP3(int /*nMP3Handle*/, bool /*bLoop*/) { return false; }
bool CScreen::SetMP3Volume(int /*nMP3Handle*/, DWORD /*dwPercent*/) { return false; }
bool CScreen::StopMP3(int /*nMP3Handle*/) { return false; }
bool CScreen::CloseMP3(int /*nMP3Handle*/) { return false; }
bool CScreen::IsPlayingMP3(int /*nMP3Handle*/) { return false; }

// --- 未使用の内部関数 ---
void CScreen::PutErr() {}
HWND CScreen::GetHwnd() { return GetConsoleWindow(); }
void CScreen::MakeColor(unsigned /*uText*/, unsigned /*uBack*/) {}