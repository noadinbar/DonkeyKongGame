#pragma once
#include <iostream>
#include <list>
#include <string>
using namespace std;

//we had to seprate between publics and privates because it relies on one another
class Result
{
	int _score = 0;
public:
	enum ResultValue { died, finished };
	void setScore(int score) { _score = score; }
	void loadResults(const string& filename);
	void saveResults(const string& filename) const;
	void addResult(int iteration, ResultValue result) { results.push_back({ iteration, result }); }
private:
	list<pair<size_t, ResultValue>> results; // pair: iteration, result
public:
	list<std::pair<size_t, ResultValue>>& getResults() { return results; }
};

