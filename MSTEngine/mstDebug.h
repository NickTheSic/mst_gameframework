#pragma once
#include <iostream>

#define dbglog(dbmsg) std::cout << dbmsg << std::endl;
#define dbgval(val)   std::cout << #val << ": " << (val) << std::endl;

//#define dbglogcall(func) std::cout << #func << std::endl; func;