#include "p_private.h"
#include <iostream>
namespace platform
{
HINSTANCE instance;
HWND hwnd;
int mouse_x,mouse_y;
bool any_key_down;
BufferBitmap buffer;
BufferBitmap::BufferBitmap():_hbitmap(NULL),_hdc(NULL),_font(NULL),_old_bitmap(),_old_font(),_saved_dc(0)
{
}
bool BufferBitmap::Init(HWND hwnd)
{
	HDC hdc=GetDC(hwnd);
	_hbitmap=CreateCompatibleBitmap(hdc,800,600);
	_hdc=CreateCompatibleDC(hdc);
	if (_hbitmap==NULL || _hdc==NULL)
		return false;
	_saved_dc=SaveDC(_hdc);
	if (0==_saved_dc)
		return false;
	SelectObject(_hdc,_hbitmap);
	ReleaseDC(hwnd,hdc);
	return true;
}
BufferBitmap::~BufferBitmap()
{
	// When you select an object into a DC, that object becomes undeletable.
	// We have to reselect old default objects.
	RestoreDC(_hdc,_saved_dc);
	// Only then we can delete them.
	DeleteObject(_font);
	DeleteObject(_hbitmap);
	// And finally the DC.
	DeleteDC(_hdc);
}
HDC BufferBitmap::Hdc() const
{
	return _hdc;
}
bool BufferBitmap::SetDefaultFont()
{
	NONCLIENTMETRICS ncmetrics;
	ncmetrics.cbSize=sizeof(NONCLIENTMETRICS);
	if (0==SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS),&ncmetrics,0))
	{
		return false;
	}
	_font=CreateFontIndirect(&ncmetrics.lfMessageFont);
	_old_font=SelectObject(_hdc,_font);
	return NULL!=_font && !(_old_font==NULL || _old_font==HGDI_ERROR);
}
bool BufferBitmap::SetFont(int size,LPCWSTR face_name)
{
	// Delete existing font if any.
	if (_font)
	{
		SelectObject(_hdc,_old_font);
		DeleteObject(_font);
	}
	_font=CreateFont(size,0,0,0,0,FALSE,0,0,0,0,0,0,0,face_name);
	_old_font=SelectObject(_hdc,_font);
	return NULL!=_font && !(_old_font==NULL || _old_font==HGDI_ERROR);
}
}
