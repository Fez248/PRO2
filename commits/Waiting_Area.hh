/** @file Waiting_Area.hh
    @brief Especification of the class Waiting_Area
*/

#ifndef WAITING_AREA_HH
#define WAITING_AREA_HH

#include "Process.hh"

#ifndef NO_DIAGRAM // So it doesn't appear on the diagram (aunque aún no lo he conseguido hacer xd)
#include <list>
#include <map>
#endif

typedef map<string, list<Process>> area;
typedef list<Process> category;

/**
    @class Waiting_Area
    @brief It represents a waiting area

    Here we store the processes waiting to be sended to the cluster sorted by tehir priority
*/

class Waiting_Area {
    private:
    //Si tengo que buscar un proceso me como la búsqueda entera en la lista, no vendria mal un map
    map<string, list<Process>> wa;
    map<string, pair<int, int>> sera;
    //enviados y rechazados

    /**
        @brief Search for a process with a specific priority in the waiting area

        \pre Both proc_prio and proc_id initialized, proc_id > 0
        \post Returns true if the process exists and false if not
    */
    bool search_process(area::const_iterator it, int proc_id) const;

    public:
    /**
        @brief Default creator

        \pre <em>True</em>
        \post An empty waiting area
    */
    Waiting_Area();

    /**
        @brief Adds a process to the waiting area and it musn't be called if we haven't checked first if the process already exsists with search_process()
        (quizás sobra)

        \pre Both prio and a initialized
        \post Adds the process to the waiting area
    */
    void add_process(const string& prio, const Process& a);

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void read_waiting_area();
    /**
        @brief Reads a priority identificator and if everything it's correct it adds the priority to the waiting area

        \pre A priority identificator
        \post If the identificator already exists it shows an error message, if not, it adds the priority to the waiting area without pending processes
    */
    bool ap(const string& x);

    /**
        @brief Reads a priority identificator and if everything it's correct it removes this priority from the waiting area

        \pre A priority identificator
        \post If the identificator doesn't exist or if it has pending processes it shows an error message, if not, it removes this priority from the waiting area without pending
    */
    int bp(const string& x);

    /**
        @brief Reads a procces and a priority id and if everything goes right, adds the proccess to the waiting area with that priority id

        \pre A process and a pirority identificator ready to be read
        \post If the id priority doesn't exist, or if a process with that id and priority already exists it returns an error, if not it adds the process to the correspondent waiting area
    */
    int ape(const string& x, int identity, int memory, int time); 

    /**
        @brief Reads a positive number, n and tries to send n waiting processes to the cluster

        \pre A number n ready to be read in the console
        \post Sends n processes to the cluster by priority and time they have been waiting, and they are send to the best spot (the cpu with most memmory left, the nearest one to the root and the one more to the left)
        if a process can not be send to the cluster it comes back as a new process to the waiting area
    */
    void epc();

    /**
        @brief Reads a priority id and prints the processes waiting there

        \pre A priority id ready to be read at the console
        \post If the priority id doesn't exist it prints an error message, if not, it prints all the pending processes and the number of processes sended to the cluster and rejections
    */
    bool ipri(const string& x) const;

    /**
        @brief Prints all the processes waiting in the waiting area by priority

        \pre <em>True</em>
        \post All the processes have been printed on the console and for each priority the number of processes send and rejections
    */
    void iae () const;
};

#endif
