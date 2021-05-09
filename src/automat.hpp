#ifndef AUTOMAT
#define AUTOMAT

#include <string>
#include <vector>
#include <list>

constexpr size_t DEFAULT_CELL = 0;



struct CellType {
    std::string name;
    std::string colour;
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
    static std::string DEFAULT_DEFINITIONS;
    static std::string DEFAULT_RULES;

    std::string getColourAt(size_t x, size_t y);
    void cellCycleType(size_t x, size_t y);
    void clearCells();
    void doOneEvolution();
};

#endif // !AUTOMAT