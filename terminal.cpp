#include "terminal.hpp"
#include <iostream>
#include <algorithm>

// Constructor 
Terminal::Terminal(int rows, int cols)
    : rows_(rows), cols_(cols),
      cursor_row_(0), cursor_col_(0),
      current_fg_color_(7), current_bg_color_(0),
      current_bold_(false), current_underline_(false),
      parser_state_(ParserState::NORMAL) {

    resize(rows, cols)
}

// Process Bytes 
void Terminal::processBytes(const char* bytes, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        char c = bytes[i];
        putChar(c);
    }
}

// basic getter implementations
const std::vector<std::vector<Cell>>& Terminal::getScreenBuffer() const {
    return screen_buffer_;
}

int Terminal::getCursorRow() const { return cursor_row_; }
int Terminal::getCursorCol() const { return cursor_col_; }
int Terminal::getRows() const { return rows_; }
int Terminal::getCols() const { return cols_; }

// clears screen and resets cursor 
void Terminal::reset() {
    clearScreen();
    cursor_row_ = 0;
    cursor_col_ = 0;

    current_fg_color_ = 7;
    current_bg_color_ = 0;
    current_bold_ = false;
    current_underline_ = false;
}

// implementing helper methods
void Terminal::putChar(char c) {
    if (cursor_row_ < rows_ && cursor_col_ < cols_) {
        screen_buffer_[cursor_row_][cursor_col_] = {
            c, current_fg_color_, current_bg_color_, current_bold_, current_underline_
        };

        if (++cursor_col_ >= cols_) {
            cursor_col_ = 0;
            newLine();
        }
    }
}

void Terminal::newLine() {
    cursor_col_ = 0;
    if (++cursor_row_ >= rows_) {
        scrollUp();
        cursor_row_ = rows_ - 1
    }
}

void Terminal::carriageReturn() {
    cursor_col_ = 0;
}

void Terminal::scrollUp() {
    for (int row = 1; row < rows_; ++row) {
        screen_buffer_[row - 1] = screen_buffer_[row];
    }

    screen_buffer_[rows_ - 1] = std::vector<Cell>(cols_, {' ', 7, 0, false, false});
}

void Terminal::clearScreen() {
    Cell empty_cell = {' ', 7, 0, false, false};
    for (auto& row: screen_buffer_) {
        std::fill(row.begin(), row.end(), empty_cell);
    }
}

void Terminal::resize(int rows, int cols) {
    rows_ = rows;
    cols_ = cols;
    screen_buffer_.resize(rows);
    for (auto& row : screen_buffer_) {
        row.resize(cols, {' ', 7, 0, false, false});
    }
}

void Terminal::backspace() { /* TODO */ }
void Terminal::clearLineFromCursor() { /* TODO */ }
void Terminal::moveCursor(int row, int col) { /* TODO */ }
void Terminal::moveCursorUp(int n) { /* TODO */ }
void Terminal::moveCursorDown(int n) { /* TODO */ }
void Terminal::moveCursorRight(int n) { /* TODO */ }
void Terminal::moveCursorLeft(int n) { /* TODO */ }
void Terminal::handleEscapeSequence(const std::string& sequence) { /* TODO */ }
void Terminal::handleCSISequence(const std::string& sequence) { /* TODO */ }
void Terminal::ensureCursorInBounds() { /* TODO */ }
