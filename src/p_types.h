/*******************************************************************************
Platform layer - Types
*******************************************************************************/
#pragma once
#include "p_nocopy.h"
namespace platform
{
class Bitmap
{
	NOCOPY(Bitmap)
public:
	Bitmap();
	~Bitmap();
	void Blit(int x,int y);
	bool Load(const wchar_t* path);
private:
	bool Destroy();
	int _width,_height;
	void* _handle;
	void* _hdc;
	void* _old;
};
class Sample
{
	NOCOPY(Sample)
public:
	Sample();
	~Sample();
	bool Load(const wchar_t* path);
	void Play();
private:
	void* _data;
};
}
