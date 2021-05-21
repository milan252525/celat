#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <cctype>
#include <algorithm>
#include <regex>

#include "automat.hpp"

std::string Automat::DEFAULT_DEFINITIONS = "DEAD,FFFFFF\nALIVE,000000";
std::string Automat::DEFAULT_RULES = "ALIVE,01,ALIVE,DEAD\nALIVE,23,ALIVE,ALIVE\nALIVE,4567,ALIVE,DEAD\nDEAD,3,ALIVE,ALIVE";

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
	cells(std::vector<size_t>(width*height, 0)),
	name_to_index(std::unordered_map<std::string, size_t>())
{
	auto [success, error] = processDefinitions(cellDefinitions);
	if (!success) {
		throw InvalidFormatException(error);
	}
	auto [success_r, error_r] = processRules(rulesDefinitions);
	if (!success_r) {
		throw InvalidFormatException(error_r);
	}
}

std::pair<bool, std::string> Automat::processDefinitions(const std::string& cellDefinitions) {
	if (cellDefinitions.empty()) return { false, "At least two cell types must be defined!"};
	std::istringstream lines(cellDefinitions);
	size_t counter = 0;
	for (std::string cellLine; std::getline(lines, cellLine); )
	{
		if (cellLine.empty()) continue;
		try
		{
			std::vector<std::string> cellSplit = Automat::splitByDelim(cellLine, ',');
			CellType x;
			auto& name = cellSplit.at(0);
			name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
			x.name = name;
			x.colour = "#" + cellSplit.at(1);
			if (!std::regex_match(x.colour, std::regex("^#[A-Fa-f0-9]{6}$"))) {
				return { false, "Invalid colour at line: (Correct example: #FF0055)\n" + cellLine };
			}
			cellTypes.push_back(x);
			auto [_, inserted] = name_to_index.insert_or_assign(x.name, counter);
			if (!inserted) return { false, "Cell type already defined:\n" + cellLine };
			counter++;
		}
		catch (const std::exception&)
		{
			return {false, "Invalid cell definition at line:\n" + cellLine };
		}
	}
	if (counter <= 1) return { false, "At least two cell types must be defined!" };
	return { true, "" };
}

std::pair<bool, std::string> Automat::processRules(const std::string& rulesDefinitions) {
	if (rulesDefinitions.empty()) return { false, "At least one rule must be defined!" };
	std::istringstream rulesLines(rulesDefinitions);
	for (std::string ruleLine; std::getline(rulesLines, ruleLine); )
	{
		if (ruleLine.empty()) continue;
		try
		{
			std::vector<std::string> ruleSplit = Automat::splitByDelim(ruleLine, ',');
			Rule x;

			auto [exists, index] = cellNameToIndex(ruleSplit.at(0));
			if (!exists) return { false, ("Cell type " + ruleSplit.at(0) + " doesn't exist:\n" + ruleLine) };
			x.originalState = index;

			std::string neighborCount = ruleSplit.at(1);
			for (char& c : neighborCount) {
				x.neighbors.push_back((int)c - (int)'0');
			}

			auto [exists2, index2] = cellNameToIndex(ruleSplit.at(2));
			if (!exists2) return { false, ("Cell type " + ruleSplit.at(2) + " doesn't exist:\n" + ruleLine) };
			x.neighborState = index2;

			auto [exists3, index3] = cellNameToIndex(ruleSplit.at(3));
			if (!exists3) return { false, ("Cell type " + ruleSplit.at(3) + " doesn't exist:\n" + ruleLine) };
			x.newState = index3;

			rules.push_back(x);
		}
		catch (const std::exception&)
		{
			return { false, "Invalid rule definition at line:\n" + ruleLine };
		}
	}
	return { true, "" };
}

std::pair<bool, size_t> Automat::cellNameToIndex(const std::string& name) const {
	auto count = name_to_index.count(name);
	if (count == 0) return { false, 0 };
	else return { true, name_to_index.at(name) };
}

void Automat::doOneEvolution() {

}

std::string Automat::getColourAt(size_t x, size_t y) {
	size_t index = y * height + x;
	size_t cellType = cells.at(index);
	return cellTypes.at(cellType).colour;
}

void Automat::cellCycleType(size_t x, size_t y) {
	size_t index = y * height + x;
	cells.at(index)++;
	if (cells.at(index) >= cellTypes.size()) {
		cells.at(index) = 0;
	}
}

void Automat::clearCells() {
	std::fill(cells.begin(), cells.end(), 0);
}