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
    mema = max = n;
    ffree = 0;
    es.emplace(n, set<int>{0});
}

int Cpu::what_ffree() const {
    return ffree;
}

void Cpu::is_leaf() {
    leaf = true;
}

bool Cpu::yn_leaf() const {
    return leaf;
}

int Cpu::remove_process_cpu(int proc_id, int& back) {
    mem::const_iterator it = prl.find(proc_id);

    if (it == prl.end()) return 102;

    int a, b, new_tam, new_dir;
    a = new_tam = it->second.what_mem();
    b = new_dir = it->second.what_dir();
    max += a;

    if (b < ffree) ffree = b;
    naio::iterator itf = diro.find(b);

    if (itf != diro.begin()) {
        naio::iterator ita = itf;
        --ita;

        int c, d;
        c = ita->first;
        d = ita->second.second;

        if (c + d != b) {
            new_tam = new_tam + (b - c - d);
            new_dir = c + d;
            update_set_and_erase(c + d, b - c - d);
        }
    }
    else if (b != 0) {
        new_dir = 0;
        new_tam = new_tam + b;
        update_set_and_erase(0, b);
    }

    if (++itf != diro.end()) {
        int e;
        e = itf->first;

        if (a + b != e) {
            new_tam = new_tam + e - a - b;
            update_set_and_erase(a + b, e - a - b);
        }
    }
    else if (a + b != mema) {
        new_tam = new_tam + (mema - a - b);
        update_set_and_erase(a + b, mema - a - b);
    }

    insert_set(new_tam, new_dir);
    back = new_tam;
    diro.erase(b);
    prl.erase(proc_id);

    return 100;
}

int Cpu::what_mema() const {
    return mema;
}

void Cpu::advance(int delta) {
    mem::iterator it2 = prl.end();
    mem::iterator it = prl.begin();
    int back = 0;

    while (it != it2) {
        if (!(it->second.reducing(delta))) {
            mem::iterator aux = it;
            ++aux;
            remove_process_cpu(it->second.what_id(), back);
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

    map<int, Process>::const_iterator it = prl.lower_bound(identity); //quitar para la v2
    if (it != prl.end() and it->first == identity) return 102; //ya existe el proceso, quitar para la v2

    map<int, set<int>>::iterator it2 = es.lower_bound(memory);
    if (it2 == es.end()) return 103; //no hay suficiente memoria, quitar para la v2

    max -= memory;
    int direction = *(it2->second.begin()); //coge la dirección más baja del tamaño encontrado antes
    int mem_ava = it2->first;

    it2->second.erase(it2->second.begin()); //borra la dirección que vamos a usar

    bool need_check = true;

    if (mem_ava > memory) { //si no hemos usado toda la memoria de la dirección que hemos cogido guardamos la sobrante
        if (direction == ffree) {
            need_check = false;
            ffree = memory + direction;
        }
        es[mem_ava - memory].insert(memory + direction); //si existe ese tamaño simplemente añade la dirección, sinó la crea y lo añade
    }

    Process p(identity, memory, time, direction);
    prl.emplace(identity, p); //añadimos el proceso al mapa de procesos
    diro.emplace(direction, make_pair(identity, memory)); //añadimos la dirección al mapa de direcciones ocupadas

    if (it2->second.empty()) es.erase(it2); //por último, si el tamañao de memoria se ha quedado sin direcciones en el mapa de espacios libres lo borramos

    if (need_check and direction == ffree) {
        naio::iterator itc = diro.upper_bound(direction);
        if (itc == diro.end()) {
            if (direction + memory == mema) ffree = mema + 1;
            else ffree = direction + memory;
        }
        else ffree = check_ffree(itc->first, itc->second.second, itc);
    }
    return 100;
}

int Cpu::check_ffree(const int direction, const int tam, naio::iterator& it) {
    int aux = direction + tam;
    ++it;
    if (it == diro.end()) {
        if (aux == mema) return mema + 1;
        else return aux;
    }
    else {
        int newd = it->first;
        int newt = it->second.second;

        if (aux != newd) return aux;
        else return check_ffree(newd, newt, it);
    }
}

void Cpu::compactar() {
    naio::iterator itf = diro.end();
    naio::iterator it = diro.upper_bound(ffree);
    
    while (it != itf) {
        int proc_id = it->second.first;
        int mem = it->second.second;
        int back = 0;
        mem::iterator it2 = prl.lower_bound(proc_id);
        int time = it2->second.what_time();

        ++it;
        remove_process_cpu(proc_id, back); //no se si funciona pero aunque lo haga tengo que crear otras dos funciones que hagan lo mismo pero quitando algunas excepciones que no son necesarias;
        relocate(proc_id, mem, time, back);
        //add_process_cpu(proc_id, mem, time);
        dir::iterator pff = es.begin();
        while (pff != es.end()) {
            if (pff->second.empty()) {
                dir::iterator aaa = pff;
                ++pff;
                es.erase(aaa);
            }
            else ++pff;
        }
    }
}

void Cpu::relocate(int proc_id, int mem, int time, int back) {
    max -= mem;
    int direction = ffree;
    dir::iterator it2 = es.find(back);
    it2->second.erase(ffree); //borra la dirección que vamos a usar

    bool need_check = true;

    if (back > mem) { //si no hemos usado toda la memoria de la dirección que hemos cogido guardamos la sobrante
        if (direction == ffree) {
            need_check = false;
            ffree = mem + direction;
        }
        es[back - mem].insert(mem + direction); //si existe ese tamaño simplemente añade la dirección, sinó la crea y lo añade
    }

    Process p(proc_id, mem, time, direction);
    prl.emplace(proc_id, p); //añadimos el proceso al mapa de procesos
    diro.emplace(direction, make_pair(proc_id, mem)); //añadimos la dirección al mapa de direcciones ocupadas

    if (it2->second.empty()) es.erase(it2); //por último, si el tamañao de memoria se ha quedado sin direcciones en el mapa de espacios libres lo borramos

    if (need_check and direction == ffree) {
        naio::iterator itc = diro.upper_bound(direction);
        if (itc == diro.end()) {
            if (direction + mem == mema) ffree = mema + 1;
            else ffree = direction + mem;
        }
        else ffree = check_ffree(itc->first, itc->second.second, itc);
    }
}

bool Cpu::active_processes() const {
    return diro.empty();
}

void Cpu:: update_set_and_erase(int diff, int key) {
    dir::iterator ite = es.find(key);
    ite->second.erase(diff);

    if (ite->second.empty()) es.erase(ite);
}

void Cpu::insert_set(int value, int key) {
    dir::iterator ite = es.find(value);

    if (ite != es.end()) ite->second.insert(key);
    else {
        set<int> z;
        z.insert(key);
        es.insert(make_pair(value, z));
    }
}

int Cpu::get_memory(int mem, int identity) const {
    dir::const_iterator it2 = es.lower_bound(mem);
    map<int, Process>::const_iterator it = prl.lower_bound(identity);

    if (it != prl.end() and it->first == identity) return -1;
    if (it2 == es.end()) return -1;
    return it2->first;
}

int Cpu::space_left() const {
    return max;
}