#pragma once
#include <iostream>

#define dbglog(dbmsg) std::cout << dbmsg << std::endl;
#define dbgval(val)   std::cout << #val << ": " << (val) << std::endl;

#if DEBUG
#define dbgbreak(reason) *(int*)(0) = 5;
#else
#define dbgbreak(reason);
#endif

#if SLOW_CODE
#define doonce(thing); {static bool doonce; if (!doonce){doonce = true; thing;}}
#else
#define doonce(thing);
#endif

//#define dbglogcall(func) std::cout << #func << std::endl; func;