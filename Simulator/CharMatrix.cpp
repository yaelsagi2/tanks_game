
#include "CharMatrix.h"
#include <algorithm>
#include <stdexcept>
#include <ostream>

char CharMatrix::get(int r, int c) const {
    // This function retrieves a character at the specified row and column
    if (!checkBounds(r, c)) throw std::out_of_range("CharMatrix: index out of range");
    return data[r][c];
}

void CharMatrix::set(int r, int c, char ch) {
    // This function sets a character at the specified row and column
    if (!checkBounds(r, c)) {
        throw std::out_of_range("CharMatrix: index out of range");
    }
    data[r][c] = ch;
}

char CharMatrix::operator()(int r, int c) const {
    // This function allows access to the character at (r, c) using operator()
    if (!checkBounds(r, c)) throw std::out_of_range("CharMatrix: index out of range");
    return get(r, c);
}
char& CharMatrix::operator() (int r, int c) {
    // This function allows modification of the character at (r, c) using operator()
    if (!checkBounds(r, c)) throw std::out_of_range("CharMatrix: index out of range");
    return data[r][c];
}

bool CharMatrix::checkBounds(int r, int c) const {
    return r < rows && c < cols;
}

void CharMatrix::fill(char ch) {
    // This function fills the entire matrix with the specified character
    for (auto& row : data) {
        std::fill(row.begin(), row.end(), ch);
    }
}
