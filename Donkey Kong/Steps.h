#pragma once
#include <list>
#include <string>
#include <iostream>
using namespace std;


class Steps
{
	long randomSeed = 0;
	list<pair<size_t, char>> steps; // pair: iteration, step
public:
	list<pair<size_t, char>>& getSteps() { return steps; }
	void loadSteps(const string& filename);
	void saveSteps(const string& filename) const;
	long getRandomSeed() const { return randomSeed; }
	void setRandomSeed(long seed) { randomSeed = seed; }
	void addStep(size_t iteration, char step) { steps.push_back({ iteration, step }); }
};
