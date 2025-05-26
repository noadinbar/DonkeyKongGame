#include "Result.h"
#include <limits>
#include <fstream> 

//from class with Amir
void Result::loadResults(const string& filename)
{
	ifstream results_file(filename);
	size_t size;
	results_file >> size;
	while (!results_file.eof() && size-- != 0) {
		size_t iteration;
		int result;
		results_file >> iteration >> result;
		this->addResult(static_cast<int>(iteration), static_cast<ResultValue>(result));
	}
	results_file.close();
}

//from class with Amir
void Result::saveResults(const string& filename) const
{
	ofstream results_file(filename, ios::trunc);
	results_file << results.size() << endl;;
	for (const auto& result : results) {
		results_file << result.first << ' ' << result.second << endl;;
	}
	results_file << _score << '\n';
	results_file.close();
}
