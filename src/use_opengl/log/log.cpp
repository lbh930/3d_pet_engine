#include "log.hpp"
#include <iostream>
#include <fstream>  // For file handling
// Clear all logs in "program_log.txt"
void LogClearAll()
{
    std::ofstream logfile("program_log.txt", std::ios_base::trunc);  // Open in trunc mode to clear file
    logfile.close();
}
