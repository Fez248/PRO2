/** @file Cpu.hh
    @brief Especification of the class Cpu
*/

#ifndef CPU_HH
#define CPU_HH

#include "Process.hh"
#include <map>
#include <set>

/**
    @class Cpu
    @brief It represents a cpu, a processor

    The processor has a memory with limited space and it can contain processes, each one with a 'delta' time needed to be completed and stored
    in crhonological order
*/

class Cpu {
    private:
    int max_mem;
    int mema;
    map<int, Process> prl; //Map with, key -> id, value -> processes
    map<int, set<int>> es; //Free memory, map with, key -> empty space in front of a process, value -> A set with directions of memmory non consecutives with that space empty
    map<int, pair<int,int>> diro; //Occuped direction of memory with the id of the process and space needed for the process

    public:

    /**
        @brief Default creator

        \pre <em>True</em>
        \post A Cpu without the memory initialized
    */
    Cpu();

    /**
        @brief Creator

        \pre The Cpu is not initialized, n > 0
        \post The Cpu is initialized with a memory of n space and an identificator = cpu_id
    */
    Cpu(int n);

    int add_process_cpu(int identity, int memory, int time);

    /**
        @brief Removes a process from the cpu, it musn't be called if we haven't checked if the process exists with search_process_cpu

        \pre Initialized and proc_id > 0
        \post The process has been eliminated from the cpu
    */
    int remove_process_cpu(int proc_id);

    /**
        @brief Advances time to all the processes in delta unities

        \pre Initialized and delta > 0
        \post All the processes of our memory have been advanced in time
    */
    void advance(int delta);

    int what_mema() const;

    void write_cpu() const;
};

#endif
