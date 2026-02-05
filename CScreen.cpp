#include "CScreen.h"
#include <windows.h>
#include <string.h>

bool CScreen::IsPlatformWin32NT()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	OSVERSIONINFO osvi = { sizeof(OSVERSIONINFO) };
	GetVersionEx(&osvi);
	// 単純判定（古い互換性用、十分）
	return (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT);
}

WORD CScreen::MakeColorAtrb(unsigned uText, unsigned uBack)
{
	// uText / uBack が INVALID_COLOR の場合は既存の色を使う
	unsigned text = (uText == INVALID_COLOR) ? m_wForeColor : uText;
	unsigned back = (uBack == INVALID_COLOR) ? m_wBackColor : uBack;

	auto mapToAttr = [](unsigned v, bool isBack) -> WORD {
		WORD attr = 0;
		const WORD R = isBack ? BACKGROUND_RED : FOREGROUND_RED;
		const WORD G = isBack ? BACKGROUND_GREEN : FOREGROUND_GREEN;
		const WORD B = isBack ? BACKGROUND_BLUE : FOREGROUND_BLUE;
		const WORD I = isBack ? BACKGROUND_INTENSITY : FOREGROUND_INTENSITY;

		bool high = (v >= 8);
		unsigned base = v % 8;

		if (base & 1) attr |= R;
		if (base & 2) attr |= G;
		if (base & 4) attr |= B;
		if (high) attr |= I;
		return attr;
		};

	WORD fg = mapToAttr(text, false);
	WORD bg = mapToAttr(back, true);
	return (WORD)(fg | bg);
}

void CScreen::MakeColor(unsigned uText, unsigned uBack)
{
	WORD w = MakeColorAtrb(uText, uBack);
	SetConsoleTextAttribute(m_hCOut, w);
	m_wForeColor = (uText == INVALID_COLOR) ? m_wForeColor : (WORD)uText;
	m_wBackColor = (uBack == INVALID_COLOR) ? m_wBackColor : (WORD)uBack;
}

void CScreen::PutErr()
{
	// 最小実装: 何もしない
}

HWND CScreen::GetHwnd()
{
	// GetConsoleWindow があれば利用
	return ::GetConsoleWindow();
}

void CScreen::CleanupSound()
{
	// 音処理は使用しない（最小実装）
}

CScreen::CScreen()
{
	m_hCOut = GetStdHandle(STD_OUTPUT_HANDLE);
	m_hCIn = GetStdHandle(STD_INPUT_HANDLE);
	m_bWinNT = IsPlatformWin32NT();
	m_wForeColor = WHITE;
	m_wBackColor = BLACK;
	m_ptBase.x = 0;
	m_ptBase.y = 0;
	memset(m_dwKeyMap, 0, sizeof(m_dwKeyMap));
	m_crdMouse.X = m_crdMouse.Y = 0;
	// カーソル初期化
	m_ccinfo.bVisible = true;
	m_ccinfo.dwSize = INITIAL_CURSOR_HEIGHT;
	SetConsoleCursorInfo(m_hCOut, &m_ccinfo);
	// 初期カラー反映
	MakeColor(m_wForeColor, m_wBackColor);
}

CScreen::~CScreen()
{
	CleanupSound();
}

void CScreen::Cursor(bool bVisible, DWORD dwSize)
{
	CONSOLE_CURSOR_INFO c;
	if (!GetConsoleCursorInfo(m_hCOut, &c)) return;
	c.bVisible = bVisible ? TRUE : FALSE;
	if (dwSize != 0) c.dwSize = (DWORD)dwSize;
	SetConsoleCursorInfo(m_hCOut, &c);
}

void CScreen::Locate(int nXpos, int nYpos)
{
	COORD coord;
	coord.X = (SHORT)(nXpos - m_ptBase.x);
	coord.Y = (SHORT)(nYpos - m_ptBase.y);
	SetConsoleCursorPosition(m_hCOut, coord);
}

void CScreen::LocateBase(int nXpos, int nYpos)
{
	m_ptBase.x = (SHORT)nXpos;
	m_ptBase.y = (SHORT)nYpos;
}

void CScreen::Color(unsigned uText, unsigned uBack)
{
	MakeColor(uText, uBack);
}

void CScreen::BackColor(unsigned uBack)
{
	MakeColor(INVALID_COLOR, uBack);
}

void CScreen::Clear(unsigned uText, unsigned uBack)
{
	// コンソール全体をスペースで塗りつぶす（簡易実装）
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD written;
	if (!GetConsoleScreenBufferInfo(m_hCOut, &csbi)) return;
	COORD origin = { 0, 0 };
	DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacterA(m_hCOut, ' ', cells, origin, &written);
	WORD attr = MakeColorAtrb(uText == INVALID_COLOR ? m_wForeColor : uText,
		uBack == INVALID_COLOR ? m_wBackColor : uBack);
	FillConsoleOutputAttribute(m_hCOut, attr, cells, origin, &written);
	SetConsoleCursorPosition(m_hCOut, origin);
	MakeColor(uText == INVALID_COLOR ? m_wForeColor : uText,
		uBack == INVALID_COLOR ? m_wBackColor : uBack);
}

int CScreen::Inp(UINT uPort)
{
	// キーボード：仮想キーコードで GetAsyncKeyState を使用
	// 特殊ポート (マウス/ジョイ) は未実装で 0 を返す
	if (uPort >= PJ1_XPOS) {
		// ジョイスティック等は未実装（必要なら拡張）
		return 0;
	}

	SHORT state = GetAsyncKeyState((int)uPort);
	return (state & 0x8000) ? 1 : 0;
}

bool CScreen::FullScreen(bool bFull)
{
	// 最小実装: 無視して現在状態を false で返す
	return false;
}

void CScreen::InpClear()
{
	FlushConsoleInputBuffer(m_hCIn);
}

int CScreen::OpenWave(LPCTSTR pszPath) { return 0; }
bool CScreen::PlayWave(int nWaveHandle, bool bLoop) { return false; }
bool CScreen::SetWaveVolume(int nWaveHandle, DWORD dwPercent) { return false; }
bool CScreen::SetWavePan(int nWaveHandle, DWORD dwPan) { return false; }
bool CScreen::StopWave(int nWaveHandle) { return false; }
bool CScreen::CloseWave(int nWaveHandle) { return false; }
bool CScreen::IsPlayingWave(int nWaveHandle) { return false; }

int CScreen::OpenMIDI(LPCTSTR pszPath) { return 0; }
bool CScreen::PlayMIDI(int nMIDIHandle, bool bLoop) { return false; }
bool CScreen::SetMIDIVolume(int nMIDIHandle, DWORD dwPercent) { return false; }
bool CScreen::StopMIDI(int nMIDIHandle) { return false; }
bool CScreen::CloseMIDI(int nMIDIHandle) { return false; }
bool CScreen::IsPlayingMIDI(int nMIDIHandle) { return false; }

int CScreen::OpenMP3(LPCTSTR pszPath) { return 0; }
bool CScreen::PlayMP3(int nMP3Handle, bool bLoop) { return false; }
bool CScreen::SetMP3Volume(int nMP3Handle, DWORD dwPercent) { return false; }
bool CScreen::StopMP3(int nMP3Handle) { return false; }
bool CScreen::CloseMP3(int nMP3Handle) { return false; }
bool CScreen::IsPlayingMP3(int nMP3Handle) { return false; }

// グローバルオブジェクト定義
CScreen __scr;