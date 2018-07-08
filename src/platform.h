/*******************************************************************************
Platform layer - Main header
*******************************************************************************/
#pragma once
#include <cstdint>
#include "p_types.h"
namespace platform
{
// Clear background.
void Clear();
// Draw an ellipse.
void Ellipse(int left,int top,int right,int bottom);
// Get mouse position.
int MouseX();
int MouseY();
// Retrieve the number of milliseconds since system start.
long GetTicks();
// Check if a key is currently pressed.
bool KeyPressed(int virtual_key_code);
bool KeyPressedAny();
int KeyLast();
// Draw text.
void Text(const wchar_t*,int x,int y);
// Draw text in rectangle with alignment.
void TextAlign(const wchar_t*,int left,int top,int right,int bottom,int align);
// Set text color. Color format 0xaabbggrr. Alpha is ignored.
void TextColor(uint32_t background,uint32_t foreground);
// Set text font and pixel height.
bool TextFont(const wchar_t* face_name,int size);
// Set main window title.
void WindowTitleSet(const wchar_t*);
}
