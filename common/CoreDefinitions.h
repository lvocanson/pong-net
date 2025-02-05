#pragma once
#include <string>
//#include <debugapi.h>
//
//// Prints a message to the Visual Studio output window. (debug only)
//inline void DebugLog(const std::string& msg)
//{
//#if defined(DEBUG) | defined(_DEBUG)
//    OutputDebugStringA(msg.c_str());
//#endif
//}

// Release a pointer and set it to nullptr
#define RELEASE(ptr) if(ptr) { delete ptr; ptr = nullptr; }

// Set a pointer to nullptr
#define NULLPTR(ptr) if(ptr) { ptr = nullptr; }