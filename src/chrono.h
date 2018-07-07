/*******************************************************************************
High precision chronometer.
Usage:
	{
	Chrono c;
		...
		c.Start();
		Piece of code to time
		c.Stop();
		...
	} // Prints result on destruction.
*******************************************************************************/
#pragma once
#include <cstdint>
class Chrono
{
public:
	Chrono();
	~Chrono();
	void Start();
	void Stop();
	double Result();
private:
	double _freq;
	int64_t _counter;
	int _numSamples;
	double _acc;
};
