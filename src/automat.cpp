#include <string>
#include <vector>
#include <list>
#include <sstream>

#include "automat.hpp"

std::vector<std::string> Automat::splitByDelim(const std::string& line, const char delim) {
	std::vector<std::string> result;
	std::stringstream sstream(line);
	std::string buffer;
	while (std::getline(sstream, buffer, delim)) {
		result.push_back(buffer);
	}
	return result;
}

Automat::Automat(const size_t width, const size_t height, const std::string& cellDefinitions, const std::string& rulesDefinitions)
	: width(width),
	height(height),
	cellTypes(std::vector<CellType>()),
	rules(std::vector<Rule>()),
	cells(std::vector<size_t>(width* height))
{
	std::istringstream lines(cellDefinitions);
	for (std::string cellLine; std::getline(lines, cellLine); )
	{
		std::vector<std::string> cellSplit = Automat::splitByDelim(cellLine, ',');
		CellType x;
		x.name = cellSplit.at(0);
		x.colour = std::stoul(cellSplit.at(1));
		cellTypes.push_back(x);
	}

	std::istringstream rulesLines(rulesDefinitions);
	for (std::string ruleLine; std::getline(rulesLines, ruleLine); )
	{
		std::vector<std::string> ruleSplit = Automat::splitByDelim(ruleLine, ',');
		Rule x;
		x.originalState = ruleSplit.at(0);
		std::string neighborCount = ruleSplit.at(1);
		for (char& c : neighborCount) {
			x.neighbors.push_back((int)c - (int)'0');
		}
		x.neighborState = ruleSplit.at(2);
		x.newState = ruleSplit.at(3);
		rules.push_back(x);
	}
}

void Automat::doOneEvolution() {

}