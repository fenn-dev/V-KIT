// Debug Logging
#ifdef debug
#include <iostream>
#define DLOG(msg) \
std::cout << msg
#else
#define DLOG(msg)
#endif