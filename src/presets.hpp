#ifndef AUTOMAT_PRESETS
#define AUTOMAT_PRESETS

#include <string>

struct Presets {
	/// @brief Game of Life definitions
	inline static std::string GOL_defs = 
		"DEAD,FFFFFF\n"
		"ALIVE,000000";
	/// @brief Game of Life rules
	inline static std::string GOL_rules =
		"ALIVE,01,ALIVE,DEAD\n"
		"ALIVE,23,ALIVE,ALIVE\n"
		"ALIVE,4567,ALIVE,DEAD\n"
		"DEAD,3,ALIVE,ALIVE";

	/// @brief WireWorld definitions
	inline static std::string WW_defs =
		"EMPTY,000000\n"
		"CONDUCTOR,FFFF00\n"
		"HEAD,0000FF\n"
		"TAIL,FF0000";

	/// @brief WireWorld rules
	inline static std::string WW_rules =
		"HEAD,,,TAIL\n"
		"TAIL,,,CONDUCTOR\n"
		"CONDUCTOR,12,HEAD,HEAD";

	/// @brief Brian's Brain definitions
	inline static std::string BB_defs =
		"DEAD,000000,50\n"
		"ALIVE,FFFFFF,30\n"
		"DYING,AAAAAA";

	/// @brief Brian's Brain rules
	inline static std::string BB_rules =
		"ALIVE,,,DYING\n"
		"DYING,,,DEAD\n"
		"DEAD,2,ALIVE,ALIVE";
};

#endif // !AUTOMAT_PRESETS