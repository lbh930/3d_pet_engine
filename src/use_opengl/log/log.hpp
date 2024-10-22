#include <iostream>
#include <fstream>  // For file handling

// Log messages to "program_log.txt"
template <typename... T>
void Log(T... message)
{
    std::ofstream logfile("program_log.txt", std::ios_base::app);  // Open in append mode
    logfile << "[LOG] ";
    (logfile << ... << message);  // Fold expression to log each argument
    logfile << std::endl;
    logfile.close();
}

// Error messages to "program_log.txt"
template <typename... T>
void Error(T... message)
{
    std::ofstream logfile("program_log.txt", std::ios_base::app);  // Open in append mode
    logfile << "[ERROR] ";
    (logfile << ... << message);  // Fold expression to log each argument
    logfile << std::endl;
    logfile.close();
}

// Clear all logs in "program_log.txt"
void LogClearAll();
