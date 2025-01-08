#pragma once

#include "breakpoint.hpp"

#include <string>
#include <linux/types.h>
#include <unordered_map>

namespace debugger {
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
        void step_over_breakpoint();
        void single_step_instruction();
        void single_step_instruction_with_breakpoint_check();
        void wait_for_signal();

        void dump_registers();
        uint64_t get_pc();
        void set_pc(uint64_t pc);

        void write_memory(uint64_t address, uint64_t value);
        uint64_t read_memory(uint64_t address);
    };
}