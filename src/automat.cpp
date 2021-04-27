#include <string>
#include <vector>
#include <list>

constexpr size_t WIDTH = 32;
constexpr size_t HEIGHT = 32;
constexpr size_t DEFAULT_CELL = 0;

struct cell_type {
    std::string name;
    std::string colour;
};

struct rule {
    std::string original_state;
    int neighbors;
    std::string neighbor_state;
    std::string new_state;
};

class automat {
private:
    std::vector<rule> rules;
    std::vector<cell_type> cell_types;
    size_t cell_types_amount;

    std::list<size_t> cells;

    bool process_definitions(const std::string& definitions) {

    }

    bool process_rules(const std::string& rules) {

    }

    
public:
    automat(const std::string& definitions, const std::string& rules) :
        rules(std::vector<rule>()), 
        cell_types(std::vector<cell_type>()),
        cells(std::list<size_t>(WIDTH * HEIGHT, DEFAULT_CELL))
    {
        bool success = process_rules(rules);
    }

    

};

