#pragma once

#include <cstring>

//#define STRLCPY(x,y) strncpy(x, y, sizeof(x)-1); *(x + (sizeof(x)-1)) = '\0'

//#define DEBUG 0

#ifdef DEBUG
#define debug do { std::cout << str << std::endl; } while( false )
#else
#define debug(str) do { } while ( false )
#endif