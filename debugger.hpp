#pragma once
#include <string>
#include <sys/types.h>

class Debugger {
public:
    Debugger (std::string prog_name, pid_t pid): m_prog_name{std::move(prog_name)}, m_pid{pid} {};

    void run();
private:
    std::string m_prog_name;
    pid_t m_pid;

    void handle_command(const std::string& line);
    void continue_execution();
};