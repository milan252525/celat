#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <regex>
#include <random>

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

Automat::Automat(
	const size_t width, 
	const size_t height, 
	const std::string& cellDefinitions, 
	const std::string& rulesDefinitions, 
	const bool overflowEdges
)
	: width(width),
	height(height),
	cellTypes(std::vector<CellType>()),
	rules(std::vector<Rule>()),
	cells(std::vector<size_t>(width*height, 0)),
	name_to_index(std::unordered_map<std::string, size_t>()),
	overflowEdges(overflowEdges)
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
		//skip empty lines
		if (cellLine.empty()) continue;
		try
		{
			//converting line into CellType structure
			std::vector<std::string> cellSplit = Automat::splitByDelim(cellLine, ',');
			CellType x;
			auto& name = cellSplit.at(0);
			name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
			x.name = name;
			x.colour = "#" + cellSplit.at(1);
			if (!std::regex_match(x.colour, std::regex("^#[A-Fa-f0-9]{6}$"))) {
				return { false, "Invalid colour at line: (Correct example: FF0055)\n" + cellLine };
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
		//skip empty lines
		if (ruleLine.empty()) continue;
		try
		{
			//converting line into Rule structure
			std::vector<std::string> ruleSplit = Automat::splitByDelim(ruleLine, ',');
			Rule x;

			auto [exists, index] = cellNameToIndex(ruleSplit.at(0));
			if (!exists) return { false, ("Cell type " + ruleSplit.at(0) + " doesn't exist:\n" + ruleLine) };
			x.originalState = index;

			//each character is separate digit
			std::string neighborCount = ruleSplit.at(1);
			for (char& c : neighborCount) {
				x.neighbors.push_back((unsigned int)c - (unsigned int)'0');
			}

			//always convert rule
			if (x.neighbors.size() > 0) {
				auto [exists2, index2] = cellNameToIndex(ruleSplit.at(2));
				if (!exists2) return { false, ("Cell type " + ruleSplit.at(2) + " doesn't exist:\n" + ruleLine) };
				x.neighborState = index2;
			}

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

unsigned int Automat::getNeighborsOfType(const size_t x, const size_t y, const size_t cellType) const {
	//Moore neighborhood
	int vectors[][2] = { {-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0} };
	unsigned int count = 0;
	for (auto& vector : vectors) {
		//conversion is safe, the number will never be large enough to overflow
		long long new_x = static_cast<long long>(x) + vector[0];
		long long new_y = static_cast<long long>(y) + vector[1];
		//handle overflow
		if (new_x < 0) {
			if (overflowEdges) new_x = width - 1;
			else continue;
		}
		else if (new_x >= (signed long long)width) {
			if (overflowEdges) new_x = 0;
			else continue;
		}
		if (new_y < 0) {
			if (overflowEdges) new_y = height - 1;
			else continue;
		}
		else if (new_y >= (signed long long)height) {
			if (overflowEdges) new_y = 0;
			else continue;
		}
		if (getCellTypeAt(static_cast<size_t>(new_x), static_cast<size_t>(new_y)) == cellType) {
			count++;
		}
	}
	return count;
}

size_t Automat::getCellTypeAt(const size_t x, const size_t y) const {
	size_t index = y * height + x;
	return cells.at(index);
}

void Automat::doOneEvolution() {
	std::vector<size_t> new_cells(cells);
	for (size_t index = 0; index < cells.size(); index++) {
		//convert index to coordinates
		size_t x = index % width;
		size_t y = index / height;
		for (Rule& rule : rules) {
			//only one rule gets applied
			bool applied = false;
			if (rule.originalState == cells.at(index)) {
				unsigned int neighborsofType = getNeighborsOfType(x, y, rule.neighborState);
				//empty size = always convert
				if (rule.neighbors.size() == 0) {
					new_cells.at(index) = rule.newState;
					applied = true;
				}
				for (unsigned int& amount : rule.neighbors) {
					if (amount == neighborsofType) {
						new_cells.at(index) = rule.newState;
						applied = true;
						break;
					}
				}
			}
			if (applied) break;
		}
	}
	cells = new_cells;
}

std::string Automat::getColourAt(const size_t x, const size_t y) const {
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

void Automat::randomizeCells() {
	std::random_device rd;
	std::mt19937 gen(rd());
	//random number [0, cellTypes.size())
	std::uniform_int_distribution<size_t> uniform_dist(0, cellTypes.size()-1);
	for (size_t index = 0; index < cells.size(); index++) {
		cells.at(index) = uniform_dist(gen);
	}
}