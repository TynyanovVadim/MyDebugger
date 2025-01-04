#include "debugger.hpp"
#include "utils.hpp"

#include <iostream>
#include <linenoise.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>

void Debugger::run() {
    int wait_status;
    auto options = 0;
    waitpid(m_pid, &wait_status, options);

    char* line = nullptr;
    while ((line = linenoise("minidbg> ")) != nullptr) {
        handle_command(line);
        linenoiseHistoryAdd(line);
        linenoiseFree(line);
    }
}

void Debugger::handle_command(const std::string& line) {
    auto args = split(line, ' ');
    auto command = args[0];

    if (is_prefix(command, "continue")) {
        continue_execution();
    } else if (is_prefix(command, "break")) {
        std::string addr {args[1], 2};
        set_breakpoint_at_address(std::stol(addr, 0, 16));
    } else {
        std::cerr << "Unknow command\n";
    }
}

void Debugger::continue_execution() {
    ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);

    int wait_status;
    int options = 0;
    waitpid(m_pid, &wait_status, options);
}

void Debugger::set_breakpoint_at_address(std::intptr_t addr) {
    std::cout << "Set breakpoint at address 0x" << std::hex << addr << "\n";
    Breakpoint bp{m_pid, addr};
    bp.enable();
    m_breakpoints.insert({addr, std::move(bp)});
}
