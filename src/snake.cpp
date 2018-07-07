#include "snake.h"
#include <cstdlib> // rand
#include <ctime> // time()
#ifndef NDEBUG
#include <iostream>
#endif
namespace snake
{
Cell::Type& operator--(Cell::Type& t)
{
	unsigned char c=t;
	c--;
	t=static_cast<Cell::Type>(c);
	return t;
}
bool Cell::IsSnakeRange()
{
	return type>0 && type<Cell::Type::FRUIT;
}
Board::Board():_cells(),_player()
{
	srand(time(0));
	Reset();
}
Cell& Board::At(size_t x,size_t y)
{
#ifndef NDEBUG
	if (!InBounds(x,y))
		throw std::out_of_range("Board::At() parameters out of bounds.");
#endif
	return _cells[y][x];
}
void Board::Reset()
{
	Clear();
	DrawWalls();
	_player.position.x=kWidth/2;
	_player.position.y=kHeight/2;
	_player.length=3;
	_player.direction.x=0;
	_player.direction.y=-1;
	SpawnFruit();
}
void Board::SpawnFruit()
{
	// Assumes no fruit on the board.
	// Free cells=board minus walls minus player.
	const int empty_cells_count=(kWidth-2)*(kHeight-2)-_player.length;
	if (empty_cells_count<=0)
		return;
	const int random_offset=rand()%empty_cells_count;
#ifndef NDEBUG
	std::cout<<"Empty cells:"<<empty_cells_count<<" Random:"<<random_offset<<"\n";
	std::cout<<"Manual count:"<<CountEmptyCells()<<"\n";
#endif
	Cell* spot=&_cells[0][0];
	// Advance to random_offset'th empty cell.
	for (int empty_found=-1; empty_found<random_offset; spot++)
	{
		if (spot->type==Cell::Type::EMPTY)
			empty_found++;
	}
	spot--;
#ifndef NDEBUG
	if (spot->type!=Cell::Type::EMPTY)
	{
		std::cout<<"("<<random_offset<<") Badly placed fruit! Was:"<<(int)spot->type<<'\n';
	}
	if (spot>=_cells.back().end())
	{
		std::cout<<spot<<" "<<"last array:"<<&_cells[kHeight-1][kWidth-1]<<" end:"<<_cells.back().end()<<'\n';
		throw (std::out_of_range("Fruit out of range."));
	}
#endif
	spot->type=Cell::Type::FRUIT;
}
Event Board::Update(const Event& input)
{
	std::array<Cell,kWidth>::iterator it=_cells.front().begin();
	for (; it!=_cells.back().end(); ++it)
	{
		if (it->IsSnakeRange())
			--(it->type);
	}
	if (input!=Event::NONE)
	{
		Point new_direction(_player.direction);
		if (_player.direction.y==0)
		{
			switch (input)
			{
			case Event::INPUT_UP:
				new_direction.y=-1;
				new_direction.x=0;
				break;
			case Event::INPUT_DOWN:
				new_direction.y=1;
				new_direction.x=0;
				break;
			default:
				break;
			}
		}
		if (_player.direction.x==0)
		{
			switch (input)
			{
			case Event::INPUT_LEFT:
				new_direction.x=-1;
				new_direction.y=0;
				break;
			case Event::INPUT_RIGHT:
				new_direction.x=1;
				new_direction.y=0;
				break;
			default:
				break;
			}
		}
		_player.direction=new_direction;
	}
	Event returned=Event::NONE;
	// Update player position.
	int new_x=_player.position.x+_player.direction.x;
	int new_y=_player.position.y+_player.direction.y;
	if (!InBounds(new_x,new_y))
		return Event::NONE;
	_player.position.x=new_x;
	_player.position.y=new_y;
	if (At(_player.position.x,_player.position.y).type==Cell::Type::WALL)
		returned=Event::COLLIDE_WALL;
	if (At(_player.position.x,_player.position.y).IsSnakeRange())
		returned=Event::COLLIDE_SELF;
	if (At(_player.position.x,_player.position.y).type==Cell::Type::FRUIT)
	{
		if (_player.length<Cell::Type::FRUIT-1)
		{
			++_player.length;
			returned=Event::FRUIT_EAT;
		}
		else
		{
			// Max lenght reached.
			returned=Event::FRUIT_EAT_MAX;
		}
		SpawnFruit();
	}
	// Write.
	At(_player.position.x,_player.position.y).type=static_cast<Cell::Type>(_player.length);
	return returned;
}
// -----------------------------------------------------------------------------
void Board::Clear()
{
	_cells.fill({Cell::Type::EMPTY});
}
size_t Board::CountEmptyCells() const
{
	size_t count=0;
	std::array<Cell,kWidth>::const_iterator it=_cells[0].begin();
	for (; it!=_cells[kHeight-1].end(); ++it)
	{
		if (it->type==Cell::Type::EMPTY)
			++count;
	}
	return count;
}
void Board::DrawWalls()
{
	for (size_t y=0; y<kHeight; ++y)
		for (size_t x=0; x<kWidth; ++x)
		{
			if (x==0 || x==kWidth-1 || y==0 || y==kHeight-1)
				At(x,y).type=Cell::Type::WALL;
		}
}
bool Board::InBounds(size_t x,size_t y) const
{
	return x<kWidth && y<kHeight;
}
}
