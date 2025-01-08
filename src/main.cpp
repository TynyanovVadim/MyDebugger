#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>

#include "debugger.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Program name not specified";
        return -1;
    }

    auto prog = argv[1];

    auto pid = fork();
    if (pid == 0) { // we`re in child procces
        ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
        execl(prog, prog, nullptr);
    } else if (pid >= 1) { // we`re in parent procces
        debugger::Debugger dbg{prog, pid};
        dbg.run();
    }

    return 0;
}