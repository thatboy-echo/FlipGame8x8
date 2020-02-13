#pragma once
#ifndef BULB_H
#define BULB_H

#include <EasyX.h>

class LED8
{
private:
	static int _WndWidth;	// 窗口宽度
	static int _WndHeight;	// 窗口高度

	static HWND _Hwnd;		// 句柄
	static DWORD* _Buff;	// 显存
	static HDC _Hdc;		// HDC

	static IMAGE BackgroundImage;
	static IMAGE BrightImage;
	static IMAGE DarkImage;

	static IMAGE ActiveBackgroundImage;
	static IMAGE BrightImagePiece[64];
	static IMAGE DarkImagePiece[64];
public:
	LED8() = delete;

	// 常量引用,供访问使用，避免函数调用

	static HWND const& Hwnd;	// 句柄
	static DWORD* const& Buff;	// 显存
	static HDC const& Hdc;		// HDC

	static int const& WndWidth;
	static int const& WndHeight;

	// 可直接设置变量
	static int BulbWidth;	// 灯泡宽度
	static int BulbHeight;	// 灯泡高度
	static int GapWidth;	// 间隙宽度
	static int GapHeight;	// 间隙高度

	static DWORD64 Board;	// 画板

	static const IMAGE* SplitImage(const IMAGE&, IMAGE*, size_t, size_t, size_t = 0, size_t = 0, bool = true);

	static const IMAGE& SetBright(COLORREF);
	static const IMAGE& SetDark(COLORREF);
	static const IMAGE& SetBackground(COLORREF);

	static const IMAGE& SetBright(const IMAGE&);
	static const IMAGE& SetDark(const IMAGE&);
	static const IMAGE& SetBackground(const IMAGE&);

	static int SetBulbSize(int, int);	// 设置灯泡大小
	static int GetBulbSize(int&, int&);	// 获得灯泡大小
	static int SetGapSize(int, int);	// 设置间隙大小
	static int GetGapSize(int&, int&);	// 获得间隙大小

	static int ResizeWndSize(int, int);	// 调整窗口大小
	static int AutoWndSize();			// 自适应窗口大小

	static HWND Create(DWORD64 = 0);
	static int Destory();
	static DWORD64 Write(DWORD64);
	static DWORD64 Write(int, int);
	static DWORD64 Read(DWORD64&);
	static int Update(bool = true);
};

#endif // !LED_H