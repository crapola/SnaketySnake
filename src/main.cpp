/*******************************************************************************
Snakety Snake
*******************************************************************************/
#include <iostream>
#include <sstream>
#include "platform.h"
#include "highscores.h"
#include "snake.h"
// Application data.
platform::Bitmap bitmaps[4];
platform::Sample samples[2];
snake::Board board;
HighScores high_scores;
int lives;
int score;
int timer;
enum GameState
{
	MENU,
	GAME,
	GAMEOVER,
	ENTRY
} state;
std::wstring name;
void GameReset()
{
	lives=3;
	score=0;
	board.Reset();
}
void DrawTopTen(int x,int y)
{
	std::wstringstream table_string;
	table_string<<L"High Scores\n";
	for (size_t i=0; i<high_scores.kNumEntries; i++)
	{
		HighScores::Entry e=high_scores.Get(i);
		std::wstringstream ss;
		e.second.resize(high_scores.kNameSize,L' ');
		ss<<(i<9?L" ":L"")<<(i+1)<<L". "<<e.second<<e.first<<L'\n';
		table_string<<ss.str();
	}
	platform::Text(table_string.str().c_str(),x,y);
}
// =============================================================================
// Setup(),Update() and Exit() are called from WinMain.
// =============================================================================
bool Setup()
{
	platform::WindowTitleSet(L"Snakety Snake");
	// Load assets.
	bool ok=true;
	ok&=bitmaps[0].Load(L"assets/block.bmp");
	ok&=bitmaps[1].Load(L"assets/grass.bmp");
	ok&=bitmaps[2].Load(L"assets/fruit.bmp");
	ok&=bitmaps[3].Load(L"assets/snake.bmp");
	ok&=samples[0].Load(L"assets/zup.wav");
	ok&=samples[1].Load(L"assets/quack.wav");
	if (!ok)
		return false;
	platform::TextColor(0x00000000,0x00FFFFFF);
	if (!high_scores.Load())
		high_scores.Populate();
	state=MENU;
	return true;
}
bool Update()
{
	if (platform::KeyPressed(0x1B)) // ESC
		return false;
	switch (state)
	{
	case MENU:
		goto menu;
		break;
	case GAME:
		goto game;
		break;
	case GAMEOVER:
		goto gameover;
		break;
	case ENTRY:
		goto entry;
		break;
	default:
		break;
	}
menu:
	platform::TextFont(L"Times New Roman",32);
	platform::TextColor(0,0x00FFFFFF);
	platform::TextAlign(L"Snakety Snake",0,16,800,50,1);
	platform::TextFont(L"Arial",16);
	platform::TextColor(0,0x00AAAAAA);
	platform::Text(L"Use arrow keys to control the snake.\nPress any key to start, or ESC to exit.",128,48);
	platform::TextFont(L"Courier New",16);
	DrawTopTen(128,240);
	if (platform::KeyPressedAny())
	{
		state=GAME;
		timer=0;
		platform::Clear();
		platform::TextFont(L"Times New Roman",32);
		platform::TextColor(0,0x00FFFFFF);
		GameReset();
	}
	return true;
game:
	// Input.
	static snake::Event input_event=snake::Event::NONE;// Static so we don't need to keep pressed.
	input_event=platform::KeyPressed(0x26)?snake::Event::INPUT_UP:
				platform::KeyPressed(0x28)?snake::Event::INPUT_DOWN:
				platform::KeyPressed(0x25)?snake::Event::INPUT_LEFT:
				platform::KeyPressed(0x27)?snake::Event::INPUT_RIGHT:
				input_event;
	// Update.
	snake::Event event;
	if (timer++==11)
	{
		event=board.Update(input_event);
		timer=0;
	}
	// React to events.
	switch (event)
	{
	case snake::Event::COLLIDE_SELF:
	case snake::Event::COLLIDE_WALL:
		lives--;
		samples[1].Play();
		if (lives<0)
		{
			state=GAMEOVER;
			platform::Text(L"Game Over",400-64,300-16);
			timer=0;
			return true;
		}
		board.Reset();
		break;
	case snake::Event::FRUIT_EAT:
		samples[0].Play();
		score+=1;
		break;
	case snake::Event::FRUIT_EAT_MAX:
		samples[0].Play();
		score+=10;
		break;
	default:
		break;
	}
	// Draw board.
	for (size_t y=0; y<board.kHeight; y++)
		for (size_t x=0; x<board.kWidth; x++)
		{
			size_t pic;
			switch (board.At(x,y).type)
			{
			case snake::Cell::EMPTY:
				pic=1;
				break;
			case snake::Cell::FRUIT:
				pic=2;
				break;
			default:
				if (board.At(x,y).IsSnakeRange())
					pic=3;
				else
					pic=0;
				break;
			}
			bitmaps[pic].Blit(80+x*32,32+y*32);
		}
	{
		// Draw score and lives.
		std::wstringstream ss;
		ss<<L"Score: "<<score;
		platform::Text(ss.str().c_str(),80,0);
		ss.str(std::wstring()); // Best way to clear a stringstream.
		ss<<L"Tries left: "<<lives;
		platform::Text(ss.str().c_str(),592,0);
	}
	return true;
gameover:
	if (timer++==100)
	{
		timer=0;
		name.clear();
		if (high_scores.Qualifies(score))
		{
			state=ENTRY;
			platform::TextFont(L"Arial",16);
		}
		else
		{
			platform::Clear();
			state=MENU;
		}
	}
	return true;
entry:
	int key=platform::KeyPressedLast();
	std::wstring name_view;
	if (name.length()<high_scores.kNameSize && ((key>=65 && key<=90) || key==32))
	{
		name.push_back(key);
		platform::Text(name.c_str(),128,256);
	}
	if (key==0x08 && !name.empty())
	{
		name.pop_back();
	}
	if (key==0x0D)
	{
		platform::KeyboardClear();
		high_scores.Insert({score,name.c_str()});
		state=MENU;
		platform::Clear();
		return true;
	}
	platform::Clear();
	platform::Text(L"Enter your name and press ENTER:",128,240);
	name_view=name;
	if (platform::GetTicks()%1000>250) // Blink.
		name_view.push_back(L'<');
	platform::Text(name_view.c_str(),128,256);
	return true;
}
void Exit()
{
	high_scores.Save();
	std::wcout<<L"Bye!\n";
}
