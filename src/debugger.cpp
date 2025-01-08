#include "debugger.hpp"
#include "utils.hpp"
#include "registers.hpp"
#include "linenoise.h"

#include <iomanip>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>

namespace debugger {
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
            if (args.size() < 1) {
                std::cerr << "Excepected address\n";
            } else {
                std::string addr {args[1], 2};
                set_breakpoint_at_address(std::stol(addr, 0, 16));
            }
        } else if (is_prefix(command, "register")) {
            if (is_prefix(args[1], "dump")) {
                dump_registers();
            } else if (is_prefix(args[1], "get")) {
                std::cout << get_register_value(m_pid, get_register_from_name(args[2])) << "\n";
            } else if (is_prefix(args[1], "set")) {

            }
        } else if (is_prefix(command, "memory")) {
            if (args.size() < 2) {
                std::cerr << "Excepected address\n";
            } else {
                std::string addr {args[2], 2};
                if (is_prefix(args[1], "read")) {
                    std::cout << std::hex << read_memory(std::stol(addr, 0, 16)) << "\n";
                } else if (is_prefix(args[1], "write")) {
                    if (args.size() < 3) {
                        std::cerr << "Excepected value";
                    } else {
                        std::string val {args[3], 2};
                        write_memory(std::stol(addr, 0, 16), std::stol(val, 0, 16));
                    }
                }
            }
        } else if(is_prefix(command, "stepi")) {
            single_step_instruction_with_breakpoint_check();
            auto line_entry = get_line_entry_from_pc(get_pc());
            print_source(line_entry->file->path, line_entry->line);
        } else {
            std::cerr << "Unknow command\n";
        } 
    }

    void Debugger::continue_execution() {
        step_over_breakpoint();
        ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);
        wait_for_signal();
    }

    void Debugger::set_breakpoint_at_address(std::intptr_t addr) {
        std::cout << "Set breakpoint at address 0x" << std::hex << addr << "\n";
        Breakpoint bp{m_pid, addr};
        bp.enable();
        m_breakpoints.insert({addr, std::move(bp)});
    }

    void Debugger::dump_registers() {
        for (const auto& rd : g_register_descriptors) {
            std::cout << rd.name << " 0x"
                    << std::setfill('0') << std::setw(16) << std::hex << get_register_value(m_pid, rd.r) << "\n";
        }
    }

    uint64_t Debugger::read_memory(uint64_t address) {
        return ptrace(PTRACE_PEEKDATA, m_pid, address, nullptr);
    }

    void Debugger::write_memory(uint64_t address, uint64_t value) {
        ptrace(PTRACE_POKEDATA, m_pid, address, value);
    }

    uint64_t Debugger::get_pc() {
        return get_register_value(m_pid, reg::rip);
    }

    void Debugger::set_pc(uint64_t pc) {
        return set_register_value(m_pid, reg::rip, pc);
    }

    void Debugger::step_over_breakpoint() {
        // -1 because execution will go past the breakpoint
        auto possible_breakpoint_location = get_pc() - 1;

        if (m_breakpoints.contains(possible_breakpoint_location)) {
            auto& bp = m_breakpoints[possible_breakpoint_location];

            if (bp.is_enabled()) {
                const auto& previous_instruction_address = possible_breakpoint_location;
                set_pc(previous_instruction_address);

                bp.disable();
                ptrace(PTRACE_SINGLESTEP, m_pid, nullptr, nullptr);
                wait_for_signal();
                bp.enable();
            }
        }
    }

    void Debugger::single_step_instruction() {
        ptrace(PTRACE_SINGLESTEP, m_pid, nullptr, nullptr);
        wait_for_signal();
    }

    void Debugger::single_step_instruction_with_breakpoint_check() {
        if (m_breakpoints.contains(get_pc())) {
            step_over_breakpoint();
        } else {
            single_step_instruction();
        }
    }

    void Debugger::wait_for_signal() {
        int wait_status;
        auto options = 0;
        waitpid(m_pid, &wait_status, options);
    }
}