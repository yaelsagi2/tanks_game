#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstddef>
#include <cstdint>

namespace GameManager_206480972_206899163 {

/**
 * @brief Records the state of the game board to a binary file for replay.
 * 
 * File format:
 * [magic: "MXR1"][rows:u32][cols:u32] then repeated:
 *   ['F'][rows*cols bytes of chars]
 */
class MatrixRecorder {
public:
    /**
     * @brief Constructs a MatrixRecorder and opens the output file.
     * @param path Path to the output file.
     * @param rows Number of rows in the board.
     * @param cols Number of columns in the board.
     */
    MatrixRecorder(const std::string& path, std::size_t rows, std::size_t cols);

    /**
     * @brief Destructor. Closes the file if open.
     */
    ~MatrixRecorder();

    // Rule of 5
    /**
     * @brief Move constructor.
     */
    MatrixRecorder(MatrixRecorder&& other) noexcept;

    /**
     * @brief Move assignment operator.
     */
    MatrixRecorder& operator=(MatrixRecorder&& other) noexcept;

    // non-copyable
    MatrixRecorder(const MatrixRecorder&) = delete;
    MatrixRecorder& operator=(const MatrixRecorder&) = delete;

    /**
     * @brief Adds a copy of the current board view to the file.
     * @param grid The board as a vector of strings (size rows, each string length == cols).
     */
    void addMapView(const std::vector<std::string>& grid);

    /**
     * @brief Closes the output file.
     */
    void close();

    /**
     * @brief Replays a recorded game on the console at a fixed FPS.
     * @param path Path to the recorded file.
     * @param fps Frames per second (default 10).
     */
    static void replay(const std::string& path, int fps = 10);

private:
    void write_u32(uint32_t v);
    static uint32_t read_u32(std::istream& in);

    std::ofstream out_;
    std::size_t rows_, cols_;
    bool closed_{false};
};

}