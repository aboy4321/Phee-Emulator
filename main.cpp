#include "pty.hpp"
#include <sys/select.h>
#include <unistd.h>
#include <iostream>

int main() {
    PTY pty; // constructor, launching bash
    
    if (!pty.isRunning()) {
        std::cerr << "failed to initialize, exiting" << std::endl;
        return 1;
    }

    int masterfd = pty.getMasterFD();
    fd_set read_fds;

    // main loop
    while (pty.isRunning()) {
        FD_ZERO(&read_fds);
        FD_SET(masterfd, &read_fds); // watching for output
        FD_SET(STDIN_FILENO, &read_fds); // watching for input
        
        // waiting for file activity 
        int activity = select(masterfd + 1, &read_fds, nullptr, nullptr, nullptr);

        if (activity < 0) {
            perror("select err");
            break;
        }

        // checking for data to read
        if (FD_ISSET(masterfd, &read_fds)) {
            char buffer[1024];
            ssize_t nbytes = pty.read(buffer, sizeof(buffer) - 1);
            if (nbytes > 0) {
                buffer[nbytes] = '\0'; // terminate string
                std::cout << buffer;
                std::cout.flush();
            } else {
                // shell session exited
                std::cout << "session ended" << std::endl;
                break;
            }
        }
        // checking for user typed input to send to shell
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            char input_buffer[1024];
            ssize_t nbytes = ::read(STDIN_FILENO, input_buffer, sizeof(input_buffer));
            if (nbytes > 0) {
                pty.write(input_buffer, nbytes); // sending input to shell
            }
        }
    }
    return 0;
}
