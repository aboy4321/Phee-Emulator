#ifndef PTY_HPP
#define PTY_HPP

#include <cstddef>
#include <sys/types.h>

class PTY {
    public:
        // defining contructor and destructor
        PTY();
        ~PTY();
        
        // indicates status of PTY
        bool isRunning() const;
        // function to retrieve master fd
        int getMasterFD() const;
        // reads data from PTY
        ssize_t read(char* buffer, size_t size);
        // writes data to the PTY
        void write(const char* data, size_t size);

    private:
        // the master file descriptor
        int masterfd;
        // stores process id of child shell process
        pid_t child_pid;
        // tracking status of session
        bool running;
};

#endif

