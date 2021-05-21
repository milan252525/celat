#ifndef AUTOMAT
#define AUTOMAT

#include <string>
#include <vector>
#include <list>
#include <utility>
#include <unordered_map>

constexpr size_t DEFAULT_CELL = 0;

struct CellType {
    std::string name;
    std::string colour;
};

struct Rule {
    size_t originalState;
    std::vector<int> neighbors;
    size_t neighborState;
    size_t newState;
};

class Automat {
private:
    std::vector<Rule> rules;
    std::vector<CellType> cellTypes;
    size_t width;
    size_t height;

    std::vector<size_t> cells;
    std::unordered_map<std::string, size_t> name_to_index;

    std::pair<bool, size_t> cellNameToIndex(const std::string& name ) const;

    std::pair<bool, std::string> processDefinitions(const std::string& cellDefinitions);
    std::pair<bool, std::string> processRules(const std::string& rulesDefinitions);
  
public:
    Automat(const size_t width, const size_t height, const std::string& cellDefinitions, const std::string& rulesDefinitions);

    static std::vector<std::string> splitByDelim(const std::string& line, const char delim);
    static std::string DEFAULT_DEFINITIONS;
    static std::string DEFAULT_RULES;

    std::string getColourAt(size_t x, size_t y);
    void cellCycleType(size_t x, size_t y);
    void clearCells();
    void doOneEvolution();

    struct InvalidFormatException : public std::exception {
    private:
        std::string msg;
    public:
        InvalidFormatException(const std::string& msg) : msg(msg) {};
        virtual const char* what() const noexcept override { return msg.c_str(); }
    };
};

#endif // !AUTOMAT