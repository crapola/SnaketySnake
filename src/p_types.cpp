#include "p_types.h"
#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include "p_private.h"
#include <iostream>
namespace platform
{
Bitmap::Bitmap():_width(0),_height(0),_handle(nullptr),_hdc(nullptr),_old(nullptr)
{
}
Bitmap::~Bitmap()
{
	if (_handle==nullptr)
		return;
	if (!Destroy())
		std::wcout<<L"Bad Bitmap destruction.\n";
}
void Bitmap::Blit(int x,int y)
{
	HDC hdc=static_cast<HDC>(_hdc);
	BitBlt(platform::buffer.Hdc(),x,y,_width,_height,hdc,0,0,SRCCOPY);
}
bool Bitmap::Load(const wchar_t* path)
{
	if (_handle!=nullptr)
		Destroy();
	HANDLE handle=LoadImage(NULL,path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	HDC hdc=CreateCompatibleDC(platform::buffer.Hdc());
	_old=SelectObject(hdc,handle);
	_hdc=static_cast<void*>(hdc);
	_handle=static_cast<void*>(handle);
	// Store dimensions.
	BITMAP bmp;
	GetObject(handle,sizeof(bmp),&bmp);
	_width=bmp.bmWidth;
	_height=bmp.bmHeight;
	// Return.
	return _handle!=NULL && _hdc!=NULL;
}
bool Bitmap::Destroy()
{
	HDC hdc=static_cast<HDC>(_hdc);
	HGDIOBJ err_so=SelectObject(hdc,_old);
	BOOL err_dc=DeleteDC(hdc);
	BOOL err_obj=DeleteObject(_handle);
	_handle=_hdc=_old=nullptr;
	return (err_so!=NULL && err_so!=HGDI_ERROR) && err_dc!=0 && err_obj!=0;
}
// =============================================================================
Sample::Sample():_data(nullptr)
{
}
Sample::~Sample()
{
	if (_data)
		free(_data);
}
bool Sample::Load(const wchar_t* path)
{
	HANDLE file=CreateFile(path,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (file==INVALID_HANDLE_VALUE)
	{
		std::wcout<<L"CreateFile failed ("<<GetLastError()<<").\n";
		return false;
	}
	DWORD file_size=GetFileSize(file,NULL);
	_data=malloc(file_size);
	if (ReadFile(file,_data,file_size,NULL,NULL)==FALSE)
	{
		std::wcout<<L"ReadFile failed ("<<GetLastError()<<").\n";
		return false;
	}
	if (FALSE==CloseHandle(file))
	{
		std::wcout<<L"CloseHandle failed!\n";
	}
	return true;
}
void Sample::Play()
{
	PlaySound(static_cast<LPCWSTR>(_data),NULL,SND_MEMORY|SND_ASYNC);
}
}
