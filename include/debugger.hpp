#pragma once

#include "breakpoint.hpp"

#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <cstdint>

class Debugger {
public:
    Debugger (std::string prog_name, pid_t pid): m_prog_name{std::move(prog_name)}, m_pid{pid} {}

    void run();
    void set_breakpoint_at_address(std::intptr_t addr);
private:
    std::unordered_map<std::intptr_t, Breakpoint> m_breakpoints;
    std::string m_prog_name;
    pid_t m_pid;

    void handle_command(const std::string& line);
    void continue_execution();
};