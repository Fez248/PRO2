/** @file Cpu.hh
    @brief Especification of the class Cpu
*/

#ifndef CPU_HH
#define CPU_HH

#include "Process.hh"
#include <map>
#include <set>

typedef map<int, pair<int, int>>::iterator& dit;

/**
    @class Cpu
    @brief It represents a cpu, a processor

    The processor has a memory with limited space and it can contain 
    processes, each one with a 'delta' time needed to be completed and 
    stored in crhonological order
*/

class Cpu {
    private:
    int ffree, mema, max;               //ffree (value of the first free
                                        //direction of the memory)

                                        //mema (total memory of the cpu)

                                        //max (total ffree memory)
    bool leaf = false;
    map<int, Process> prl;              //key -> id, value -> processes

    map<int, set<int>> es;              //key -> empty space in front 
                                        //of a process
                                        //value -> set with directions of 
                                        //memmory with that empty space

    map<int, pair<int,int>> diro;       //key -> direction of occuped memory
                                        //value -> map with
                                        //key2 -> process id
                                        //value2 -> size of the process

    /**
        @brief Searches for the lowest direction of free memory of the cpu.

        \pre <em>True</em>
        \post A Cpu without the memory initialized
    */
    int check_ffree(const int direction, const int tam, dit it);

    void update_set_and_erase(int key, int diff);

    void insert_set(int key, int value);

    void relocate(int proc_id, int mem, int time, int back);
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

    void is_leaf();

    bool yn_leaf() const;

    int add_process_cpu(int identity, int memory, int time);

    /**
        @brief Removes a process from the cpu, it musn't be called if we haven't checked if the process exists with search_process_cpu

        \pre Initialized and proc_id > 0
        \post The process has been eliminated from the cpu
    */
    int remove_process_cpu(int proc_id, int& back);

    /**
        @brief Advances time to all the processes in delta unities

        \pre Initialized and delta > 0
        \post All the processes of our memory have been advanced in time
    */
    void advance(int delta);

    int what_mema() const;

    void write_cpu() const;

    void compactar();

    bool active_processes() const;

    int get_memory(int mem, int identity) const;

    int space_left() const;

    int what_ffree() const;
};

#endif