#include <iostream>
#include <list>
#include "Waiting_Area.hh"
#include "Process.hh"
#include "Cluster.hh"
using namespace std;

typedef map<string, list<Process>> area;
typedef list<Process> category;
typedef map<string, pair<int, int>> siu;

Waiting_Area::Waiting_Area() {}

bool Waiting_Area::search_process(area::const_iterator it, int proc_id) const {

    for (category::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
        if (it2->what_id() == proc_id) return true;
    }

    return false;
}

//Preguntar al profe
void Waiting_Area::read_waiting_area() {
    int n;
    cin >> n;

    for (int i = 0; i < n; ++i) {
        string prio;
        list<Process> a;
        pair<int, int> b(0, 0);

        cin >> prio;
        wa.insert(make_pair(prio, a));
        sera.insert(make_pair(prio, b));
    }   
}

void Waiting_Area::add_process(const string& prio, const Process& a) {
    area::iterator it = wa.find(prio);
    category::iterator it2 = it->second.end();
    it->second.insert(it2, a);
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

bool Waiting_Area::ap(const string& x) {
    area::iterator it = wa.find(x);

    if (it != wa.end()) return false;

    category a;
    wa.insert(make_pair(x, a));
    sera.insert(make_pair(x, make_pair(0, 0)));
    return true;
}

int Waiting_Area::bp(const string& x) {
    area::iterator it = wa.find(x);

    if (it == wa.end()) return 101;
    if (!(it->second.empty())) return 102;

    sera.erase(x);
    wa.erase(it);
    return 100;
}

int Waiting_Area::ape(const string& x, int identity, int memory, int time) {
    Process a = Process(identity, memory, time, 0);

    area::iterator it = wa.find(x);

    if (it == wa.end()) return 101;
    else if (search_process(it, identity)) return 102;

    add_process(x, a);
    return 100;
} 

bool Waiting_Area::ipri(const string& x) const {
    area::const_iterator it = wa.find(x);

    if (it == wa.end()) return false;
    else {
        for(category::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            it2->wr2_process();
        }

        siu::const_iterator ita = sera.find(x);
        cout << ita->second.first << " " << ita->second.second << endl;
    }

    return true;
}

void Waiting_Area::iae() const {
    area::const_iterator it2 = wa.end();

    for (area::const_iterator it = wa.begin(); it != it2; ++it) {
        string name = it->first;
        cout << name << endl;
        category::const_iterator itf2 = it->second.end();

        for (category::const_iterator itf = it->second.begin(); itf != itf2; ++itf) itf->wr2_process();

        map<string, pair<int, int>>::const_iterator its = sera.find(name);
        cout << its->second.first << " " << its->second.second << endl;
    }
}

void Waiting_Area::epc(int n, Cluster& clust) {
    area::iterator it = wa.begin();
    area::iterator itf = wa.end();
    int i = 0;

    while (i < n and it != itf) {
        category::iterator it2 = it->second.begin();
        category::iterator it2f = it->second.end();
        siu::iterator cat = sera.find(it->first);

        while (i < n and it2 != it2f) {
            category::iterator its = it2;
            Process a = *it2;

            ++its;
            
            if (clust.recive_processes(a)) {
                ++i;
                cat->second.first += 1;
                it->second.erase(it2);
            }
            else {
                it->second.erase(it2);
                it->second.insert(it->second.end(), a);
                cat->second.second += 1;
            }
            it2 = its;
        }
    }
}