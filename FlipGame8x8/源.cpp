/******************************
 * FlipGame8x8
 * 编译环境：VS 2019
 * 作者：that boy，2018/12/22
 * 最后修改：2020/2/14
 ******************************/

#include "Bulb.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	IMAGE IMG;
	MOUSEMSG Msg = { 0 };

	loadimage(&IMG, TEXT("IMAGE"), TEXT("BK"), 640, 640);

	LED8::SetBright(IMG);
	LED8::SetBackground(BLACK);
	LED8::SetDark(DARKGRAY);
	LED8::Create();

	// 按下鼠标中键关闭或者使用关闭按钮
	for (; Msg.uMsg != WM_MBUTTONUP;)
	{
		Msg = { NULL };
		while (MouseHit())
		{
			Msg = GetMouseMsg();

			switch (Msg.uMsg)
			{
			case WM_LBUTTONDOWN:
			{
				auto x = (Msg.y - LED8::GapHeight) / (LED8::GapHeight + LED8::BulbHeight);
				auto y = (Msg.x - LED8::GapWidth) / (LED8::GapWidth + LED8::BulbWidth);
				LED8::Board ^= (DWORD64)1 << ((x << 3) | y);
				if (x > 0)
					LED8::Board ^= (DWORD64)1 << (((x - 1) << 3) | y);
				if (x < 7)
					LED8::Board ^= (DWORD64)1 << (((x + 1) << 3) | y);
				if (y > 0)
					LED8::Board ^= (DWORD64)1 << ((x << 3) | (y - 1));
				if (y < 7)
					LED8::Board ^= (DWORD64)1 << ((x << 3) | (y + 1));
				break;
			}
			default:
				break;
			}
		}
		Sleep(10);
		LED8::Update();
	}
	LED8::Destory();
}