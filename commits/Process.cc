#include <iostream>
#include "Process.hh"
using namespace std;

/*Process::Process() {
    int proc_id, int mem;
    double delta;

    cin >> proc_id >> mem >> delta;
    
    ide = proc_id;
    memo = mem;
    time = delta;
}*/

Process::Process(int identity, int memory, int delta, int direction) {
    ide = identity;
    memo = memory;
    time = delta;
    dir = direction;
}

bool Process::reducing(int t) {
    time = time - t;
    return time > 0;
}

int Process::what_id() const {
    return ide;
}

int Process::what_mem() const {
    return memo;
}

double Process::what_time() const {
    return time;
}

void Process::wr_process() const {
    //posicion, nombre, memoria, tiempo
    cout << dir << " " << ide << " " << memo << " " << time << endl;
}

void Process::wr2_process() const {
    //posicion, nombre, memoria, tiempo
    cout << ide << " " << memo << " " << time << endl;
}


int Process::what_dir() const {
    return dir;
}



