
/// @brief Reference to the function map label
extern "C" void* _funcmap;

/// @brief Gets the function map resource which contains every function
///        in the build and their binary addresses.
char* getFunctionMap();