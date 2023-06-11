/** @file Cluster.cc
    @brief Implementation of the class Cluster
*/

#include <iostream>
#include <list>
#include "Waiting_Area.hh"
#include "Process.hh"
#include "Cluster.hh"
#include <queue>
using namespace std;

//Typedefs to code faster, keep lines shorter and to make it more readable
typedef map<string, Cpu> clus;
typedef map<string, Cpu>::iterator clus_it;
typedef map<string, Cpu>::const_iterator clus_ct;

typedef BinTree<string>& ord;
typedef const BinTree<string>& ord_ct;

typedef clus::iterator& cit;

void Cluster::read() {
    conj.clear();
    read_bintree(cluster);
}

void Cluster::iec() const {
    write_bintree(cluster);
}

void Cluster::write_bintree(ord_ct cluster) const {
    if (!cluster.empty()) {
        cout << "(" << cluster.value();
        write_bintree(cluster.left());
        write_bintree(cluster.right());
        cout << ")";
    }
    else cout << " ";
}

bool Cluster::read_bintree(ord   a) {
    string x;
    cin >> x;

    if (x != "*") {
        int n;
        cin >> n;
        auto it = conj.insert(make_pair(x, Cpu(n)));

        BinTree<string> left, right;
        bool left_check = read_bintree(left);
        bool right_check = read_bintree(right);    
        if (left_check and right_check) it.first->second.is_leaf();
        a = BinTree<string>(x, left, right);

        return false;
    }
    else return true;
}

void Cluster::reread(ord a, string p) {
    if (!a.empty()) {
        string x = a.value();
        if (x == p) read_bintree(a);
        else {
            BinTree<string> left, right;
            left = a.left();
            right = a.right();
            reread(left, p);
            reread(right, p);

            a = BinTree<string>(x, left, right);
        }
    }
}

void Cluster::ipc() const {
    clus_ct it2 = conj.end();

    for (clus_ct it = conj.begin(); it != it2; ++it) {
        cout << it->first << endl;
        it->second.write_cpu();
    }
}

int Cluster::app(const string& x, int identity, int memory, int time) {
    clus_it it = conj.find(x);

    if (it == conj.end()) return 101;
    return it->second.add_process_cpu(identity, memory, time);
}

int Cluster::bpp(const string& x, int identity) {
    clus_it it = conj.find(x);

    if (it == conj.end()) return 101;
    int back = 0;
    return it->second.remove_process_cpu(identity, back);
}

bool Cluster::ipro(const string& x) const {
    clus_ct it = conj.find(x);
    
    if (it == conj.end()) return false;
    it->second.write_cpu();
    return true;
}

void Cluster::at(int t) {
    clus_it itf = conj.end();

    for (clus_it it = conj.begin(); it != itf; ++it) it->second.advance(t);
}

bool Cluster::cmp(string x) {
    clus_it it = conj.lower_bound(x);

    if (it == conj.end() or it->first != x) return false;
    if (it->second.what_ffree() <= it->second.what_mema()) {
        it->second.compactar();
    }
    return true;
}

void Cluster::cmc() {
    clus_it itf = conj.end();
    for (clus_it it = conj.begin(); it != itf; ++it) {
        if (it->second.what_ffree() <= it->second.what_mema()) {
            it->second.compactar();
        }
    }
}   

int Cluster::mc(string x) {
    clus_ct it = conj.lower_bound(x);

    if (it == conj.end() or it->first != x) return 101;
    if (!it->second.active_processes()) return 102;
    if (!it->second.yn_leaf()) return 103;

    conj.erase(it);
    reread(cluster, x);

    return 100;
}

bool Cluster::recive_processes(Process a) {
    int mem, time, id;
    time = a.what_time();
    id = a.what_id();
    mem = a.what_mem();
    clus_it ite = conj.end();
    find_best(mem, id, ite, cluster);

    if (ite != conj.end()) {
        ite->second.add_process_cpu(id, mem, time);
        return true;
    }
    else return false;
}

void Cluster::find_best(const int mem, const int identity, cit ite, ord_ct can) {
    int no_space, free_space;
    no_space = free_space = -1;
    if (!can.empty()) {
        queue<BinTree<string>> need_visit;
        need_visit.push(can);

        while (!need_visit.empty()) {
            BinTree<string> ls, rs;
            ls = need_visit.front().left();
            rs = need_visit.front().right();
            string x = need_visit.front().value();
            need_visit.pop();

            clus_it it = conj.find(x);
            int aux = it->second.get_memory(mem, identity);

            if (aux != -1) {
                int aux2 = it->second.space_left();
                if (no_space == -1 or aux - mem < no_space or (aux - mem == no_space and aux2 > free_space)) {
                    no_space = aux - mem;
                    free_space = aux2;
                    ite = it;
                }
            }

            if (!ls.empty()) need_visit.push(ls);
            if (!rs.empty()) need_visit.push(rs);
        }
    }
}

Cluster::Cluster() {}
