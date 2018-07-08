#include "platform.h"
#include "p_private.h"
namespace platform
{
void Clear()
{
	RECT r{0,0,800,600};
	FillRect(platform::buffer.Hdc(),&r,reinterpret_cast<HBRUSH>(COLOR_WINDOWTEXT));
}
void Ellipse(int left,int top,int right,int bottom)
{
	HDC dc=platform::buffer.Hdc();
	Ellipse(dc,left,top,right,bottom);
}
int MouseX()
{
	return mouse_x;
}
int MouseY()
{
	return mouse_y;
}
long GetTicks()
{
	return GetTickCount();
}
bool KeyPressed(int vkey)
{
	return GetAsyncKeyState(vkey);
}
bool KeyPressedAny()
{
	return platform::any_key_down;
}
int KeyLast()
{
	int returned=platform::last_key;
	platform::last_key=0;
	return returned;
}
void Text(const wchar_t* text,int x,int y)
{
	/*
	Using GetTextExtentPoint to get correct size is slower than simply
	using a large enough rectangle.
	...
	SIZE s{0,0};
	GetTextExtentPoint(platform::buffer.Hdc(),text,20,&s);
	RECT r{x,y,x+s.cx,y+s.cy};
	*/
	RECT r{x,y,800,600};
	DrawText(platform::buffer.Hdc(),text,-1,&r,0);
}
void TextAlign(const wchar_t* text,int left,int top,int right,int bottom,int align)
{
	RECT r{left,top,right,bottom};
	DrawText(platform::buffer.Hdc(),text,-1,&r,align);
}
void TextColor(uint32_t bg,uint32_t fg)
{
	SetBkColor(platform::buffer.Hdc(),bg);
	SetTextColor(platform::buffer.Hdc(),fg);
}
bool TextFont(const wchar_t* face_name,int size)
{
	return platform::buffer.SetFont(size,face_name);
}
void WindowTitleSet(const wchar_t* title)
{
	SetWindowText(hwnd,title);
}
/*
void* ImageLoad(const wchar_t* path)
{
	HANDLE h=LoadImage(NULL,path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	return h;
}
bool ImageDestroy(void* image)
{
	return TRUE==DeleteObject(image);
}
void Blit(void* image,int x,int y)
{
	HDC window_dc=platform::buffer.Hdc();
	{
		HDC bmp_dc=CreateCompatibleDC(window_dc); // Expensive!
		{
			auto old=SelectObject(bmp_dc,image);
			{
				BITMAP bm;
				GetObject(image,sizeof(bm),&bm);
				BitBlt(window_dc,x,y,bm.bmWidth,bm.bmHeight,bmp_dc,0,0,SRCCOPY);
			}
			SelectObject(bmp_dc,old);
		}
		DeleteDC(bmp_dc);
	}
	//ReleaseDC(hwnd,window_dc);
}
void SoundPlay(const wchar_t* path)
{
	PlaySound(path,NULL,SND_FILENAME|SND_ASYNC);
}
*/
}
