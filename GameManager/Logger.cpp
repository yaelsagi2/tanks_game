#include "Logger.h"
#include <iostream>

namespace GameManager_206480972_206899163 {

Logger::Logger(bool enabled) : file_name(""), enabled(enabled) {}

void Logger::open(const std::string& inputFilename) {
    if (!enabled) return;


    // Remove any .txt from inputFilename if present
    std::string base_name = inputFilename;
    if (base_name.size() > 4 && base_name.substr(base_name.size() - 4) == ".txt") {
        base_name = base_name.substr(0, base_name.size() - 4);
    }
    this->file_name = base_name;

    std::string regular_path = "output_" + base_name + ".txt";
    std::string detailed_path = "detailed_output_" + base_name + ".txt";

    regular_out.open(regular_path);
    if (!regular_out.is_open()) {
        std::cerr << "Logger: Failed to open " << regular_path << "\n";
    }

    detailed_out.open(detailed_path);
    if (!detailed_out.is_open()) {
        std::cerr << "Logger: Failed to open " << detailed_path << "\n";
    }
    std::cout << "Logger opening files: " << regular_path << " and " << detailed_path << std::endl;
}

Logger::~Logger() {
    // Destructor: Closes the output files if open
    if (regular_out.is_open()) {
        regular_out.close();
    }
    if (detailed_out.is_open()) {
        detailed_out.close();
    }
}


void Logger::logStep(int step, const std::string& message) const{
    // Log a message for a specific step in both regular and detailed logs
    if (regular_out.is_open()) {
        regular_out << "Step " << step << ": " << message << "\n";
    }
    if (detailed_out.is_open()) {
        detailed_out << "Step " << step << ": " << message << "\n";
    }
}


void Logger::logFinal(const std::string& message, bool write_to_reg) const{
    // Log the final result message in detailed log
    if(regular_out.is_open() && (write_to_reg)) {
        regular_out << message << "\n";
    }
    if (detailed_out.is_open()) {
        detailed_out << "== Final Result ==\n" << message << "\n";
    }
}


void Logger::logLineDetailed(const std::string& message) const{
    // Log a single line message in the detailed log
    if (detailed_out.is_open()) {
        detailed_out << message << std::endl;
    }
}


void Logger::logLine(const std::string& message, bool add_newline) const{
    // Log a single line message in regular
    if (regular_out.is_open()) {
        regular_out << message;
        if (add_newline) regular_out << std::endl;
    }
}

void Logger::logActionSummary(const std::string& action, bool ignored, bool killed, bool last) const {
    // Log a summary of an action in the regular log
    if (!regular_out.is_open()) return;

    regular_out << action;
    if (ignored) regular_out << " (ignored)";
    if (killed) regular_out << " (killed)";
    if (!last) regular_out << ", " ; 
}

void Logger::logActionDetailed(int step, const std::string& message, const std::string& reason) const{
    // Log detailed information about an action in the detailed log
    if (!detailed_out.is_open()) return;
    detailed_out << "Step " << step << ": " << message;
    if (!reason.empty()) detailed_out << " (ignored - " << reason << ")";
    detailed_out << "\n";
}


std::string Logger::getOutputFilename(const std::string& inputFile) {
    // Helper function: Extracts the file's name from a path for output file naming
    size_t pos = inputFile.find_last_of("/\\");
    return (pos == std::string::npos) ? inputFile : inputFile.substr(pos + 1);
}

void Logger::logActionDetailed(const std::string& message, const std::string& extra) {
    // Log detailed information about an action in the detailed log
    if (!enabled) return;
    detailed_out << message;
    if (!extra.empty()) detailed_out << " (" << extra << ")";
    detailed_out << std::endl;
}
}