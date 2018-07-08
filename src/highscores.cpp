#include "highscores.h"
#include <algorithm>
#include <fstream>
#include <sstream>
HighScores::HighScores():_entries()
{
}
HighScores::~HighScores()
{
}
HighScores::Entry HighScores::Get(size_t rank) const
{
	return _entries.at(rank);
}
void HighScores::Insert(Entry new_entry)
{
	if (!Qualifies(new_entry.first))
		return;
	new_entry.second=new_entry.second.substr(0,32);
	_entries.back()=new_entry;
	Sort();
}
bool HighScores::Load()
{
	std::wifstream file("scores.txt");
	if (!file)
		return false;
	std::wstring s;
	for (auto& e: _entries)
	{
		if (!std::getline(file,s,L'\0'))
		{
			file.close();
			return false;
		}
		std::wstringstream ss(s);
		ss>>e.first;
		std::getline(file,e.second,L'\0');
	}
	file.close();
	//Sort();
	return true;
}
void HighScores::Populate()
{
	int i=10;
	for (auto& e: _entries)
	{
		e.first=i--;
		e.second=L"Snakety Snake";
	}
}
bool HighScores::Qualifies(int score) const
{
	return (score>_entries.back().first);
}
void HighScores::Save()
{
	std::wofstream file("scores.txt",std::ios::trunc);
	for (auto& e: _entries)
	{
		file<<e.first<<L'\0'<<e.second<<L'\0';
	}
	file.close();
}
void HighScores::Sort()
{
	std::sort(_entries.begin(),_entries.end(),std::greater<Entry>());
}
