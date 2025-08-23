#include <iostream>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include "MatrixRecorder.h"
namespace GameManager_206480972_206899163 {

    struct ConsoleGuard { // Helper struct to manage console cursor visibility and position during replay
        ConsoleGuard()  { std::cout << "\x1b[?25l\x1b[H"; }  // hide cursor, home
        ~ConsoleGuard() { std::cout << "\x1b[?25h\x1b[0m" << std::flush; } // show cursor, reset
    };

// Constructs a MatrixRecorder, opens the output file, and writes the header.
MatrixRecorder::MatrixRecorder(const std::string& path, std::size_t rows, std::size_t cols)
    : rows_(rows), cols_(cols), closed_(false)
{
    if (rows_ == 0 || cols_ == 0) {
        throw std::invalid_argument("rows/cols must be > 0");
    }
    out_.open(path, std::ios::binary | std::ios::trunc);
    if (!out_) throw std::runtime_error("Failed to open file for writing: " + path);

    const char magic[4] = {'M','X','R','1'};
    out_.write(magic, 4);
    write_u32(static_cast<uint32_t>(rows_));
    write_u32(static_cast<uint32_t>(cols_));
}

MatrixRecorder::~MatrixRecorder() { //Destructor. Ensures the file is closed.
    try { close(); } catch (...) {}
}

void MatrixRecorder::addMapView(const std::vector<std::string>& grid) {
    // This function adds a copy of the current board view to the file.
    if (!out_) throw std::runtime_error("Recorder not open");
    if (grid.size() != rows_) {
        throw std::invalid_argument("grid rows mismatch");
    }
    for (const auto& row : grid) {
        if (row.size() != cols_) {
            throw std::invalid_argument("grid cols mismatch");
        }
    }
    char tag = 'F';
    out_.write(&tag, 1);
    for (const auto& row : grid) {
        out_.write(row.data(), static_cast<std::streamsize>(row.size()));
    }
    if (!out_) throw std::runtime_error("Write error");
}

void MatrixRecorder::close() {
    // This function closes the output file if it is open.
    if (!closed_) {
        if (out_) out_.flush();
        out_.close();
        closed_ = true;
    }
}

void MatrixRecorder::replay(const std::string& path, int fps) {
    // This function replays a recorded game on the console at a fixed FPS.
    if (fps <= 0) throw std::invalid_argument("fps must be > 0");
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Failed to open file for reading: " + path);
    char magic[4];
    in.read(magic, 4);
    if (in.gcount() != 4 || std::string(magic, 4) != "MXR1") {
        throw std::runtime_error("Invalid or corrupt file (bad magic)");
    }
    uint32_t rows = read_u32(in);
    uint32_t cols = read_u32(in);
    if (!in) throw std::runtime_error("Invalid or corrupt file (bad header)");
    ConsoleGuard guard;
    const auto frame_delay = std::chrono::milliseconds(1000 / fps);
    std::vector<char> frame(static_cast<std::size_t>(rows) * cols);
    while (true) {
        char tag;
        in.read(&tag, 1);
        if (!in) break;              // normal EOF
        if (tag != 'F') break;       // unexpected; stop safely
        in.read(frame.data(), static_cast<std::streamsize>(frame.size()));
        if (!in) break;              // truncated; stop safely
        std::cout << "\x1b[H";       // cursor home (row=1, col=1)
        for (uint32_t r = 0; r < rows; ++r) {
            const char* line = frame.data() + r * cols;
            std::cout.write(line, cols) << "\x1b[K\n";  // print row, clear to EOL
        }
        std::cout.flush();
        std::this_thread::sleep_for(frame_delay);
    }
}

void MatrixRecorder::write_u32(uint32_t v) {
    // Little-endian write - This function writes a 32-bit unsigned integer to the output stream.
    char buf[4] = {
        static_cast<char>(v & 0xFF),
        static_cast<char>((v >> 8) & 0xFF),
        static_cast<char>((v >> 16) & 0xFF),
        static_cast<char>((v >> 24) & 0xFF)
    };
    out_.write(buf, 4);
}

uint32_t MatrixRecorder::read_u32(std::istream& in) {
    // Little-endian read - This function reads a 32-bit unsigned integer from the input stream.
    char buf[4];
    in.read(buf, 4);
    if (!in) return 0;
    return (static_cast<uint32_t>(static_cast<unsigned char>(buf[0]))      ) |
           (static_cast<uint32_t>(static_cast<unsigned char>(buf[1])) <<  8) |
           (static_cast<uint32_t>(static_cast<unsigned char>(buf[2])) << 16) |
           (static_cast<uint32_t>(static_cast<unsigned char>(buf[3])) << 24);
}
}