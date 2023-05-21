/** @file Cluster.hh
    @brief Especification of the class Cluster
*/

#ifndef CLUSTER_HH
#define CLUSTER_HH

#include "Process.hh"
#include "BinTree.hh"
#include "Cpu.hh"
#include <map>

/**
    @class Cluster
    @brief It represents a clsuter

    The cluster is a binary tree where his components are processors, cpus.
    It needs to have at least one cpu.
*/

class Cluster {
    private:
    BinTree<string> cluster;
    map<string, Cpu> conj;

    bool read_bintree(BinTree<string>& cluster);

    void write_bintree(const BinTree<string>& cluster) const;

    void reread(BinTree<string>& a, string p);

    void find_best(const int mem, int no_space, int free_space, int identity, map<string, Cpu>::iterator& ite, BinTree<string> can);

    public:

    /**
        @brief Default creator

        \pre <em>True</em>
        \post An empty cluster
    */
    Cluster();

    /**
        @brief Searchs for a specific cpu on the cluster

        \pre Initialized cpu_id
        \post Returns true if the cpu exists and false if not
    */
    bool search_cpu(const string& cpu_id) const;
    
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void read();

    /**
        @brief Modifies the cluster

        \pre An id of a cpu of the existing cluster and a new cluster
        \post If the cpu doesn't exist, if it has any active process or if it has auxiliar cpus it shows an error message, if not,
        the new cluster it's added where the cpu was
    */
    int mc(string x);

    /**
        @brief Reads a processor id and a process, if everything goes right, adds the process to that processor

        \pre A cpu id and a process
        \post If the cpu doesn't exist or if it already has a process with that id or the is no space left it shows an error message, if not, it adds the process to the cpu
    */
    int app(const string& x, int identity, int memmory, int time);

    /**
        @brief It reads a process and the id of a cpu, if everything goes right it removes the process from the cpu

        \pre A process and a cpu id
        \post If the cpu doesn't exist or if the process doesn't exist neither it shows an error message, if not, it removes the process from the cpu
    */
    int bpp(const string& x, int identity);

    /**
        @brief Makes time pass by

        \pre A number t >= 0
        \post The time has been advanced in t unities
    */
    void at(int t);

    /**
        @brief Finds a cpu in the cluster and prints his processes

        \pre A cpu id ready to be read at the console
        \post If the cpu doesn't exist it shows an error message, if not, it prints his processes with their information
    */
    bool ipro(const string& x) const;

    /**
        @brief Prints all the active processes from the cluster by increasing id order

        \pre <em>True</em>
        \post All the processes have been printed
    */
    void ipc() const;

    /**
        @brief Prints the cluster structure

        \pre <em>True</em>
        \post The cluster has been printed
    */
    void iec() const;

    /**
        @brief Compacts the memmory of a cpu

        \pre A cpu id
        \post If the cpu id doesn't exist it prints an error message, if not, it compacts it's memmory moving all the processes to the beggining of the memory
    */
    bool cmp(string x);

    /**
        @brief Compacts all the cpus

        \pre <em>True</em>
        \post All the memmory moduls from all the cpus have been compacted
    */
    void cmc();

    bool recive_processes(Process a);
};

/**
void Cluster::find_best(const int mem, int no_space, int free_space, int identity, clus::iterator& ite, BinTree<string> can) {
    if (!can.empty()) {
        string x = can.value();
        clus::iterator it = conj.find(x);
        if (no_space == -1 and it->second.get_memory(mem, identity != -1)) {
            no_space = it->second.get_memory(mem, identity) - mem;
            free_space = it->second.space_left();
            ite = it;
        }
        else {
            if (!(can.left()).empty()) {
                string x = (can.left()).value();
                clus::iterator it = conj.find(x);
                int aux = it->second.get_memory(mem, identity);

                if (aux != -1) {
                    int aux2 = it->second.space_left();
                    if (no_space == -1 or aux - mem < no_space) {
                        no_space = aux - mem;
                        free_space = aux2;
                        ite = it;
                    }
                    else if (aux - mem == no_space and aux2 > free_space) {
                        no_space = aux - mem;
                        free_space = aux2;
                        ite = it;
                    }
                }
            }

            if (!(can.right()).empty()) {
                string x = (can.right()).value();
                clus::iterator it = conj.find(x);
                int aux = it->second.get_memory(mem, identity);

                if (aux != -1) {
                    int aux2 = it->second.space_left();
                    if (no_space == -1 or aux - mem < no_space) {
                        no_space = aux - mem;
                        free_space = aux2;
                        ite = it;
                    }
                    else if (aux - mem == no_space and aux2 > free_space) {
                        no_space = aux - mem;
                        free_space = aux2;
                        ite = it;
                    }
                }
            }
        }
        find_best(mem, no_space, free_space, identity, ite, can.left());
        find_best(mem, no_space, free_space, identity, ite, can.right());
    }
} 
*/

#endif