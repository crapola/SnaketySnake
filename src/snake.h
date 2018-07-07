/*******************************************************************************
Snake game
*******************************************************************************/
#pragma once
#include <array>
namespace snake
{
struct Cell
{
	enum Type: unsigned char
	{
		EMPTY=0,
		// [1,100] is snake range.
		FRUIT=101,
		WALL=102
	} type=Type::EMPTY;
	bool IsSnakeRange();
};
struct Point
{
	int x,y;
};
struct Player
{
	Point position;
	Point direction;
	unsigned char length;
};
enum class Event
{
	NONE,
	FRUIT_SPAWN,
	FRUIT_EAT,
	FRUIT_EAT_MAX,
	COLLIDE_SELF,
	COLLIDE_WALL,
	INPUT_UP,
	INPUT_DOWN,
	INPUT_LEFT,
	INPUT_RIGHT
};
class Board
{
public:
	static constexpr size_t kWidth=20,kHeight=15;
	Board();
	Cell& At(size_t x,size_t y);
	void Reset();
	void SpawnFruit();
	Event Update(const Event& input);
private:
	void Clear();
	size_t CountEmptyCells() const;
	void DrawWalls();
	bool InBounds(size_t x,size_t y) const;
	std::array<std::array<Cell,kWidth>,kHeight> _cells;
	Player _player;
};
}
