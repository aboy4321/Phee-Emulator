#include "pty.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <pty.h>
#include <stdio.h>

// creating constructor
PTY::PTY() : masterfd(-1), child_pid(-1), running(true) {
    // creating pseudo terminal
    pid_t pid = forkpty(&masterfd, nullptr, nullptr, nullptr);

    // error handling
    if (pid == -1) {
        perror("fork failed");
        running = false;
        return;
    }
    if (pid == 0) {
        execlp("/bin/bash", "bash", (char *)nullptr);
        perror("execlp failed");
        exit(1);
    }
    // storing child process
    child_pid = pid;
}

// creating destructor
PTY::~PTY() {
    if (masterfd != -1) {
        close(masterfd);
    }
}

// implementing getter functions
bool PTY::isRunning() const { return running; }
int PTY::getMasterFD() const { return masterfd; }

// wrapping standard read and write function with :: operator
ssize_t PTY::read(char* buffer, size_t size) {
    if (masterfd == -1) return -1;
    return ::read(masterfd, buffer, size);
}

void PTY::write(const char* data, size_t size) {
    if (masterfd == -1) return;
    ::write(masterfd, data, size);
}
