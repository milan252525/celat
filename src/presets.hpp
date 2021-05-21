#ifndef AUTOMAT_PRESETS
#define AUTOMAT_PRESETS

#include <string>

struct Presets {
	inline static std::string GOL_defs = 
		"DEAD,FFFFFF\n"
		"ALIVE,000000";
	inline static std::string GOL_rules =
		"ALIVE,01,ALIVE,DEAD\n"
		"ALIVE,23,ALIVE,ALIVE\n"
		"ALIVE,4567,ALIVE,DEAD\n"
		"DEAD,3,ALIVE,ALIVE";

	inline static std::string WW_defs =
		"EMPTY,000000\n"
		"CONDUCTOR,FFFF00\n"
		"HEAD,0000FF\n"
		"TAIL,FF0000\n";

	inline static std::string WW_rules =
		"HEAD,,,TAIL\n"
		"TAIL,,,CONDUCTOR\n"
		"CONDUCTOR,12,HEAD,HEAD";
};

#endif // !AUTOMAT_PRESETS