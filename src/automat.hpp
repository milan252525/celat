#ifndef AUTOMAT
#define AUTOMAT

#include <string>
#include <vector>
#include <list>

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
    size_t width;
    size_t height;
    std::list<size_t> cells;


public:
    automat(size_t width, size_t height);
};

#endif // !AUTOMAT