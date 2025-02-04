#pragma once

// Release a pointer and set it to nullptr
#define RELEASE(ptr) if(ptr) { delete ptr; ptr = nullptr; }

// Set a pointer to nullptr
#define NULLPTR(ptr) if(ptr) { ptr = nullptr; }