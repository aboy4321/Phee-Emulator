#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>

// representing char position
struct Cell {
    // char displayed
    char character;
    // text & background color
    uint8_t foreground_color;
    uint8_t background_color;
    // determining bold and underlined text
    bool bold;
    bool underline;
};

class Terminal {
    public: 
        Terminal(int rows = 24, int cols = 80);
        // processing bytes from pty
        void processBytes(const char* bytes, size_t size);

        // getter methods
        const std::vector<std::vector<Cell>>& getScreenBuffer() const;
        int getCursorRow() const;
        int getCursorCol() const;
        int getRows() const;
        int getCols() const;
        
        // clearing terminal
        void reset();
    
    private:
        // screen dimensions
        int rows_, cols_;

        // 2d vector for terminal display
        std::vector<std::vector<Cell>> screen_buffer_;
        
        // cursor tracking state
        int cursor_row_, cursor_col_;

        // text attributes
        uint8_t current_fg_color_;
        uint8_t current_bg_color_;
        bool current_bold_;
        bool current_underline_;

        // parser state machine
        enum class ParserState {
            NORMAL,
            ESCAPE,
            CSI,
        };
        ParserState parser_state_;

        std::string escape_sequence_;

        // helper methods
        void putChar(char c);
        void newLine();
        void carriageReturn();
    
        void backspace();
        void clearScreen();
        void clearLineFromCursor();
        void moveCursor(int row, int col);
        void moveCursorUp(int n = 1);
        void moveCursorDown(int n = 1);
        void moveCursorRight(int n = 1);
        void moveCursorLeft(int n = 1);
        void handleEscapeSequence(const std::string& sequence);
        void handleCSISequence(const std::string& sequence);
        void resize(int rows, int cols);
        void scrollUp();
        void ensureCursorInBounds();
};

#endif // !TERMINAL_HPP
