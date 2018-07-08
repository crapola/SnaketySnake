// Private header.
#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <windowsx.h>
#include "p_nocopy.h"
namespace platform
{
//extern HINSTANCE instance;
extern HWND hwnd; // Main window.
extern int mouse_x,mouse_y;
extern bool any_key_down;
extern int last_key;
// Main window back buffer, where we draw everything.
class BufferBitmap
{
	NOCOPY(BufferBitmap)
public:
	BufferBitmap();
	bool Init(HWND hwnd);
	~BufferBitmap();
	HDC Hdc() const;
	bool SetDefaultFont();
	bool SetFont(int size,LPCWSTR face_name);
private:
	HBITMAP _hbitmap;
	HDC _hdc;
	HFONT _font;
	HGDIOBJ _old_bitmap,_old_font;
	int _saved_dc;
};
extern BufferBitmap buffer;
}
