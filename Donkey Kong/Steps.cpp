#include "Steps.h"
#include <fstream>
#include <filesystem>

//from class with Amir
void Steps::loadSteps(const string& filename)
{
	ifstream steps_file(filename);
	steps_file >> this->randomSeed;
	size_t size;
	steps_file >> size;
	while (!steps_file.eof() && size-- != 0) {
		size_t iteration;
		char step;
		steps_file >> iteration >> step;
		this->addStep(iteration, step);
	}
	steps_file.close();
}

//from class with Amir
void Steps::saveSteps(const string& filename) const
{
	ofstream steps_file(filename, ios::trunc);
	steps_file << randomSeed << '\n' << steps.size();
	for (const auto& step : steps) {

		steps_file << '\n' << step.first << ' ' << step.second;
	}
	steps_file.close();
}
