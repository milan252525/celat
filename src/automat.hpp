#ifndef AUTOMAT
#define AUTOMAT

#include <string>
#include <vector>
#include <list>

constexpr size_t DEFAULT_CELL = 0;

const std::string DEFAULT_DEFINITIONS = "LIVE,FFFFFF\nBLACK,000000";
const std::string DEFAULT_RULES = "LIVE,01,LIVE,DEAD\nLIVE,23,LIVE,LIVE\nLIVE,4567,LIVE,DEAD\nDEAD,3,LIVE,LIVE";

struct CellType {
    std::string name;
    unsigned long colour;
};

struct Rule {
    std::string originalState;
    std::vector<int> neighbors;
    std::string neighborState;
    std::string newState;
};

class Automat {
private:
    std::vector<Rule> rules;
    std::vector<CellType> cellTypes;
    size_t width;
    size_t height;
    std::vector<size_t> cells;

public:
    Automat(const size_t width, const size_t height, const std::string& cellDefinitions, const std::string& rulesDefinitions);

    static std::vector<std::string> splitByDelim(const std::string& line, const char delim);

    void doOneEvolution();
};

#endif // !AUTOMAT