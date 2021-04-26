#include <string>

class automat {
private:
    /* data */
public:

};

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