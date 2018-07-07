/*******************************************************************************
High Scores
*******************************************************************************/
#pragma once
#include <array>
class HighScores
{
public:
	static const size_t kNumEntries=10;
	typedef std::pair<long,std::wstring> Entry;
	HighScores();
	~HighScores();
	Entry Get(size_t rank) const;
	void Insert(Entry);
	bool Load();
	void Populate();
	void Save();
private:
	void Sort();
	std::array<Entry,kNumEntries> _entries;
};
