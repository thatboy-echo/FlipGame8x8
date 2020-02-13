/*************************************************
 * FlipGame8x8
 * 编译环境：VC++ 2017
 * 作者：that boy，2018/12/22
 * 最后修改：2018/12/27
 * E-mail：zhuguangfeng.thatboy@gmail.com
 *************************************************/

#include "Bulb.h"

HWND const& LED8::Hwnd = LED8::_Hwnd;	// 句柄
DWORD* const& LED8::Buff = LED8::_Buff;	// 显存
HDC const& LED8::Hdc = LED8::_Hdc;		// HDC

int const& LED8::WndWidth = LED8::_WndWidth;
int const& LED8::WndHeight = LED8::_WndHeight;

int LED8::BulbWidth = 70;
int LED8::BulbHeight = 70;
int LED8::GapWidth = 10;
int LED8::GapHeight = 10;

IMAGE LED8::BackgroundImage;
IMAGE LED8::BrightImage;
IMAGE LED8::DarkImage;

IMAGE LED8::ActiveBackgroundImage;
IMAGE LED8::BrightImagePiece[64];
IMAGE LED8::DarkImagePiece[64];

HWND LED8::_Hwnd;		// 句柄
DWORD64 LED8::Board;	// 画板
DWORD* LED8::_Buff;		// 显存
HDC LED8::_Hdc;			// HDC
int LED8::_WndWidth;
int LED8::_WndHeight;

const IMAGE* LED8::SplitImage(const IMAGE& Src, IMAGE* Dst, size_t NumX, size_t NumY, size_t Width, size_t Height, bool AutoSize)
{
	if (Width == 0 || Height == 0 || (AutoSize && Width * NumX > (size_t)Src.getwidth() && Height* NumY > (size_t)Src.getheight()))
	{
		Width = Src.getwidth() / NumX;
		Height = Src.getheight() / NumY;
	}
	SetWorkingImage((IMAGE*)(&Src));
	for (size_t i = 0; i < NumX * NumY; ++i)
		getimage(Dst + i, (i % NumX) * Width, (i / NumX) * Height, Width, Height);
	SetWorkingImage();
	return Dst;
}

const IMAGE& LED8::SetBright(COLORREF col)
{
	BrightImage.Resize(BulbWidth * 8, BulbHeight * 8);
	SetWorkingImage(&BrightImage);
	setbkcolor(col);
	cleardevice();
	SetWorkingImage();
	SplitImage(BrightImage, BrightImagePiece, 8, 8, BulbWidth, BulbHeight);
	return BrightImage;
}

const IMAGE& LED8::SetDark(COLORREF col)
{
	DarkImage.Resize(BulbWidth * 8, BulbHeight * 8);
	SetWorkingImage(&DarkImage);
	setbkcolor(col);
	cleardevice();
	SetWorkingImage();
	SplitImage(DarkImage, DarkImagePiece, 8, 8, BulbWidth, BulbHeight);
	return DarkImage;
}

const IMAGE& LED8::SetBackground(COLORREF col)
{
	BackgroundImage.Resize(_WndWidth, _WndHeight);
	SetWorkingImage(&BackgroundImage);
	setbkcolor(col);
	cleardevice();
	SetWorkingImage();
	ActiveBackgroundImage = BackgroundImage;
	return BackgroundImage;
}

const IMAGE& LED8::SetBright(const IMAGE& img)
{
	BrightImage = img;
	SplitImage(BrightImage, BrightImagePiece, 8, 8, BulbWidth, BulbHeight);
	return BrightImage;
}

const IMAGE& LED8::SetDark(const IMAGE& img)
{
	DarkImage = img;
	SplitImage(DarkImage, DarkImagePiece, 8, 8, BulbWidth, BulbHeight);
	return DarkImage;
}

const IMAGE& LED8::SetBackground(const IMAGE& img)
{
	BackgroundImage = img;
	ActiveBackgroundImage = img;
	return BackgroundImage;
}

int LED8::SetBulbSize(int bulbwidth, int bulbheight)
{
	return (BulbWidth = bulbwidth) * (BulbHeight = bulbheight);
}

int LED8::GetBulbSize(int& bulbwidth, int& bulbheight)
{
	return (bulbwidth = BulbWidth) * (bulbheight = BulbHeight);
}

int LED8::SetGapSize(int gapwidth, int gapheight)
{
	return (GapWidth = gapwidth) * (GapHeight = gapheight);
}

int LED8::GetGapSize(int& gapwidth, int& gapheight)
{
	return (gapwidth = GapWidth) * (gapheight = GapHeight);
}

int LED8::ResizeWndSize(int wndwidth, int wndheight)
{
	if (wndwidth <= 0 || wndheight <= 0)
		return -1;
	::Resize(nullptr, wndwidth, wndheight);
	_WndWidth = getwidth();
	_WndHeight = getheight();
	_Hwnd = GetHWnd();
	_Hdc = GetImageHDC();
	_Buff = GetImageBuffer();
	return 0;
}

int LED8::AutoWndSize()
{
	_WndWidth = GapWidth + ((GapWidth + BulbWidth) << 3);
	_WndHeight = GapHeight + ((GapHeight + BulbHeight) << 3);
	ResizeWndSize(_WndWidth, _WndHeight);
	return 0;
}

HWND LED8::Create(DWORD64 inBoard)
{
	_WndWidth = GapWidth + ((GapWidth + BulbWidth) << 3);
	_WndHeight = GapHeight + ((GapHeight + BulbHeight) << 3);
	Board = inBoard;
	_Hwnd = initgraph(_WndWidth, _WndHeight);
	_Buff = GetImageBuffer();
	_Hdc = GetImageHDC();

	BeginBatchDraw();

	// // 设置无边框
	// ::SetWindowLong(_Hwnd, GWL_STYLE, GetWindowLong(_Hwnd, GWL_STYLE)&(~(WS_CAPTION | WS_SYSMENU | WS_SIZEBOX)));
	// // 设置透明属性
	// ::SetWindowLong(_Hwnd, GWL_EXSTYLE, (GetWindowLong(_Hwnd, GWL_EXSTYLE)&(~(WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME))) | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
	// // 设置居中靠前
	// SetWindowPos(_Hwnd, HWND_TOPMOST, (GetSystemMetrics(SM_CXSCREEN) - getwidth()) / 2,
	// 	(GetSystemMetrics(SM_CYSCREEN) - getheight()) / 2, getwidth(), getheight(), SWP_SHOWWINDOW | SWP_FRAMECHANGED | SWP_DRAWFRAME);
	// // SetLayeredWindowAttributes(_Hwnd, BackgroundColor, 0, LWA_COLORKEY);
	cleardevice();

	return _Hwnd;
}

int LED8::Destory()
{
	EndBatchDraw();
	closegraph();
	return 0;
}

DWORD64 LED8::Write(DWORD64 inBoard)
{
	Board = inBoard;
	return 0;
}

DWORD64 LED8::Write(int x, int y)
{
	Board = (DWORD64)1 << ((y & 0B0000'0111 << 3) | (x & 0B0000'0111));
	return 0;
}

DWORD64 LED8::Read(DWORD64& outBoard)
{
	outBoard = Board;
	return 0;
}

int LED8::Update(bool bFlush)
{
	cleardevice();
	for (size_t i = 0; i < 64; i++)
	{
		putimage(GapWidth + (i & 0B111) * (GapWidth + BulbWidth), GapHeight + (i >> 3)* (GapHeight + BulbHeight), Board& (DWORD64)1 << i ? BrightImagePiece + i : DarkImagePiece + i);
	}
	if (bFlush)
		FlushBatchDraw();
	return 0;
}