#include <iostream>
#include <map>
#include <set>
#include "Process.hh"
#include "Cpu.hh"
using namespace std;

typedef map<int, Process> mem;
typedef map<int, set<int>> dir;
typedef map<int, pair<int,int>> naio;

Cpu::Cpu(int n) {
    mema = n;
    es.emplace(n, set<int>{0});
}

int Cpu::remove_process_cpu(int proc_id) {
    map<int, Process>::const_iterator it = prl.find(proc_id);

    //Checks if the process exists
    if (it == prl.end()) return 102;

    int a, b, new_tam, new_dir;
    a = new_tam = it->second.what_mem();
    b = new_dir = it->second.what_dir();

    map<int, pair<int,int>>::iterator itf = diro.find(b);

    if (itf != diro.begin()) {
        map<int, pair<int,int>>::iterator ita = itf;
        --ita;

        int c, d;
        c = ita->first;
        d = ita->second.second;

        if (c + d != b) {
            new_tam = new_tam + (b - c - d); //+d
            new_dir = c + d; //c

            map<int, set<int>>::iterator ite = es.find(b - (c + d));
            ite->second.erase(c + d);

            if (ite->second.empty()) es.erase(ite);
        }
    }
    else if (b != 0) {
        new_dir = 0;
        new_tam = new_tam + b;
        
        map<int, set<int>>::iterator ite = es.find(b);
        ite->second.erase(0);
        if (ite->second.empty()) es.erase(ite);
    }

    if (++itf != diro.end()) {
        int e;
        e = itf->first;

        if (a + b != e) {
            new_tam = new_tam + e - a - b; //f

            map<int, set<int>>::iterator ite = es.find(e - (a + b));
            ite->second.erase(a + b);

            if (ite->second.empty()) es.erase(ite);
        }
    }
    else if (a + b != mema) {
        new_tam = new_tam + (mema - a - b);

        map<int, set<int>>::iterator ite = es.find(mema - a - b);
        ite->second.erase(a + b);
        if (ite->second.empty()) es.erase(ite);
    }

    map<int, set<int>>::iterator aux = es.find(new_tam);

    if (aux != es.end()) aux->second.insert(new_dir);
    else {
        set<int> z;
        z.insert(new_dir);
        es.insert(make_pair(new_tam, z));
    }

    diro.erase(b);
    prl.erase(proc_id);

    return 100;
}

/*
int Cpu::add_process_cpu(int identity, int memory, int time) {
    mem::const_iterator it = prl.find(identity);

    if (it != prl.end()) return 102;
    if (memory > mema) return 103;

    //Finds the key equal or greater to the memory needed to store the process and returns an iterator pointing to it
    //Takes the first direction with that memory founded on the line before this one
    //Deletes the direction of the memory where it has been stored from the map of free memory
    dir::iterator it2 = es.lower_bound(memory); //es.upper_bound(memory - 1)
    int direction = *it2->second.begin(); 
    it2->second.erase(direction); 

    //If the memory used is less than memory available in that direction that 
    //means we need to relocate the memory that we haven't used on our free memory map
    if (it2->first > memory) {
        dir::iterator is = es.find(it2->first - memory);

        if (is == es.end()) {
            set<int> b;
            b.insert(memory + direction);
            es.insert(make_pair(it2->first - memory, b));
        }
        else is->second.insert(memory + direction);
    }

    //Inserts the process on the map of active processes of the cpu
    Process a = Process(identity, memory, time, direction);
    prl.insert(make_pair(identity, a));

    diro.insert(make_pair(direction, make_pair(identity, memory)));

    if (it2->second.empty()) es.erase(it2);
    if (mema != memory) mema = (--es.end())->first;
    else mema = 0;

    return 100;
} */

int Cpu::what_mema() const {
    return mema;
}

void Cpu::advance(int delta) {
    mem::iterator it2 = prl.end();
    mem::iterator it = prl.begin();

    while (it != it2) {
        if (!(it->second.reducing(delta))) {
            mem::iterator aux = it;
            ++aux;
            remove_process_cpu(it->second.what_id());
            it = aux;
        }
        else ++it;
    } 
}

void Cpu::write_cpu() const {
    naio::const_iterator itf = diro.end();

    for (naio::const_iterator it = diro.begin(); it != itf; ++it) {
        int a = it->second.first;
        mem::const_iterator ita = prl.find(a);
        ita->second.wr_process();
    }
}

int Cpu::add_process_cpu(int identity, int memory, int time) {
    map<int, Process>::const_iterator it = prl.lower_bound(identity);

    if (it != prl.end() and it->first == identity) return 102; //ya existe el proceso
    
    map<int, set<int>>::iterator it2 = es.lower_bound(memory);
    //cout << memory << endl;
    //cout << es.begin()->first << " " << *es.begin()->second.begin() << endl;
    if (it2 == es.end()) return 103; //no hay suficiente memoria

    int direction = *(it2->second.begin()); //coge la dirección más baja del tamaño encontrado antes
    int mem_ava = it2->first;

    it2->second.erase(it2->second.begin()); //borra la dirección que vamos a usar

    if (mem_ava > memory) { //si no hemos usado toda la memoria de la dirección que hemos cogido guardamos la sobrante
        es[mem_ava - memory].insert(memory + direction); //si existe ese tamaño simplemente añade la dirección, sinó la crea y lo añade
    }

    Process p(identity, memory, time, direction);
    prl.emplace(identity, p); //añadimos el proceso al mapa de procesos
    diro.emplace(direction, make_pair(identity, memory)); //añadimos la dirección al mapa de direcciones ocupadas

    if (it2->second.empty()) es.erase(it2); //por último, si el tamañao de memoria se ha quedado sin direcciones en el mapa de espacios libres lo borramos

    return 100;
}
