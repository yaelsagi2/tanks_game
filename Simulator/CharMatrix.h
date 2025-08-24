// CharMatrix.h
#pragma once
#include <vector>
#include <stdexcept>
#include <ostream>

/**
 * @class CharMatrix
 * @brief A simple 2D matrix of characters with bounds checking and utility methods.
 */
class CharMatrix {  
private:
    int rows; ///< Number of rows in the matrix
    int cols; ///< Number of columns in the matrix
    std::vector<std::vector<char>> data; ///< 2D vector storing the matrix data

public:
    /**
     * @brief Default constructor that initializes an empty CharMatrix.
     */
    CharMatrix() : rows(0), cols(0) {}
    /**
     * @brief Constructs a CharMatrix with given dimensions and fill character.
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param fill Character to fill the matrix with (default is space).
     */
    CharMatrix(int rows, int cols, char fill = ' ')
        : rows(rows), cols(cols), data(rows, std::vector<char>(cols, fill)) {}

    /**
     * @brief Returns the number of rows.
     */
    int getRows() const { return rows; }

    /**
     * @brief Returns the number of columns.
     */
    int getCols() const { return cols; }

    /**
     * @brief Gets the character at (r, c) with bounds checking.
     * @param r Row index.
     * @param c Column index.
     * @return Character at the specified position.
     * @throws std::out_of_range if indices are out of bounds.
     */
    char get(int r, int c) const;

    /**
     * @brief Sets the character at (r, c) with bounds checking.
     * @param r Row index.
     * @param c Column index.
     * @param ch Character to set.
     * @throws std::out_of_range if indices are out of bounds.
     */
    void set(int r, int c, char ch);

    /**
     * @brief Checks if the given indices are within bounds.
     * @param r Row index.
     * @param c Column index.
     * @return True if indices are valid, false otherwise.
     */
    bool checkBounds(int r, int c) const;

    /**
     * @brief Const function call operator for element access with bounds checking.
     * @param r Row index.
     * @param c Column index.
     * @return Character at the specified position.
     */
    char operator()(int r, int c) const;

    /**
     * @brief Non-const function call operator for element access with bounds checking.
     * @param r Row index.
     * @param c Column index.
     * @return Reference to the character at the specified position.
     */
    char& operator()(int r, int c);

    /**
     * @brief Fills the entire matrix with a specified character.
     * @param ch Character to fill the matrix with.
     */
    void fill(char ch);

};

