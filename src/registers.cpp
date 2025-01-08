#include "registers.hpp"

#include <cstdint>
#include <sys/ptrace.h>
#include <stdexcept>

namespace debugger {
    uint64_t get_register_value(pid_t pid, reg r) {
        user_regs_struct regs;
        ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
        
        auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                               [&r](auto&& rd) { return rd.r == r; });

        if (it == end(g_register_descriptors)) {
            throw std::out_of_range{"Unknown register number"};
        }

        return *(reinterpret_cast<uint64_t*>(&regs) + (it - begin(g_register_descriptors)));
    }

    void set_register_value(pid_t pid, reg r, uint64_t value) {
        user_regs_struct regs;
        ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
        
        auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                               [&r](auto&& rd) { return rd.r == r; });

        if (it == end(g_register_descriptors)) {
            throw std::out_of_range{"Unknown register number"};
        }

        *(reinterpret_cast<uint64_t*>(&regs) + (it - begin(g_register_descriptors))) = value;
        ptrace(PTRACE_SETREGS, pid, nullptr, &regs);
    }

    uint64_t get_register_value_from_dwarf_register (pid_t pid, int regnum) {
        auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                            [&regnum](auto&& rd) { return rd.dwarf_r == regnum; });
        if (it == end(g_register_descriptors)) {
            throw std::out_of_range{"Unknown dwarf register"};
        }

        return get_register_value(pid, it->r);
    }

    std::string get_register_name(reg r) {
        auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                            [&r](auto&& rd) { return rd.r == r; });
        return it->name;
    }

    reg get_register_from_name(const std::string& name) {
        auto it = std::find_if(begin(g_register_descriptors), end(g_register_descriptors),
                            [&name](auto&& rd) { return rd.name == name;});
        return it->r;
    }
}