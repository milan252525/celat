#ifndef AUTOMAT
#define AUTOMAT

#include <string>
#include <vector>
#include <list>
#include <utility>
#include <unordered_map>

/// @brief Structure holding cell definition
struct CellType {
    std::string name;
    std::string colour;
};

/// @brief Structure holding automat rule
struct Rule {
    size_t originalState;
    std::vector<unsigned int> neighbors{};
    size_t neighborState;
    size_t newState;
};

class Automat {
private:
    /// @brief vector of automat rules
    std::vector<Rule> rules;
    /// @brief vector of cell definitions
    std::vector<CellType> cellTypes;
    /// @brief width of the automat
    size_t width;
    /// @brief height of the automat
    size_t height;

    /// @brief wrap around borders
    bool overflowEdges;

    /// @brief test vector of automat cells
    std::vector<size_t> cells;
    /// @brief map mapping cell type names to index
    std::unordered_map<std::string, size_t> name_to_index;

    /// @brief transform cell type name into its index
    /// @param name name of the cell type
    /// @return std::pair (success, index)
    std::pair<bool, size_t> cellNameToIndex(const std::string& name ) const;

    /// @brief process cell definitions, save them into vector of CellType structures
    /// @param cellDefinitions string containing definitions separated by newline
    /// @return std::pair (success, error_message)
    std::pair<bool, std::string> processDefinitions(const std::string& cellDefinitions);

    /// @brief process rules, save them into vector of Rule structures
    /// @param rulesDefinitions string containing rules separated by newline
    /// @return std::pair (success, error_message)
    std::pair<bool, std::string> processRules(const std::string& rulesDefinitions);

    /// @brief Get amount of neighbours of specified type
    /// @param x coordinate of cell
    /// @param y coordinate of cell
    /// @param cellType name of the cell type
    /// @return amount
    unsigned int getNeighborsOfType(const size_t x, const size_t y, const size_t type) const;
    
    /// @brief get type of cell at coordinates
    /// @param x coordinate
    /// @param y coordinate
    /// @return index of cell type in this->cellTypes
    size_t getCellTypeAt(const size_t x, const size_t y) const;
  
public:
    /// @brief Automat constructor,
    /// processes cell definitions and rules
    /// @param width width of the grid
    /// @param height height of the grid
    /// @param cellDefinitions string of cell definitions, separated by newline
    /// @param rulesDefinitions string of rules, separated by newline
    /// @param overflowEdges wrap around borders
    Automat(const size_t width, const size_t height, const std::string& cellDefinitions, const std::string& rulesDefinitions, const bool overFlowEdges);

    /// @brief Split string by character delimiter
    /// @param line string to be split
    /// @param delim delimiter
    /// @return vector of strings
    static std::vector<std::string> splitByDelim(const std::string& line, const char delim);

    /// @brief get colour of cell at coordinates
    /// @param x coordinate
    /// @param y coordinate
    /// @return string containing RGB hex string
    std::string getColourAt(const size_t x, const size_t y) const;

    /// @brief Cycle cell type at coordinates
    /// @param x coordinate
    /// @param y coordinate
    void cellCycleType(const size_t x, const size_t y);
    
    /// @brief set all cells to default one (the first one defined)
    void clearCells();
    
    /// @brief Run one evolution of cells
    void doOneEvolution();

    /// @brief set all cells to random type
    void randomizeCells();

    /// @brief Custom exception for inalid rules
    struct InvalidFormatException : public std::exception {
    private:
        /// @brief Message of the exception
        std::string msg;
    public:
        /// @brief constructor
        InvalidFormatException(const std::string& msg) : msg(msg) {};
        /// @brief override of std::exception::what, returns error message
        virtual const char* what() const noexcept override { return msg.c_str(); }
    };
};

#endif // !AUTOMAT