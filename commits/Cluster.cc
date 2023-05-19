#include <iostream>
#include <list>
#include "Waiting_Area.hh"
#include "Process.hh"
#include "Cluster.hh"
using namespace std;

typedef map<string, Cpu> clus;

void Cluster::read() {
    conj.clear();
    read_bintree(cluster);
}

void Cluster::iec() const {
    write_bintree(cluster);
}

void Cluster::write_bintree(const BinTree<string>& cluster) const {
    if (!cluster.empty()) {
        cout << "(" << cluster.value();
        write_bintree(cluster.left());
        write_bintree(cluster.right());
        cout << ")";
    }
    else cout << " ";
}

bool Cluster::read_bintree(BinTree<string>& a) {
    string x;
    cin >> x;

    if (x != "*") {
        int n;
        cin >> n;
        auto it = conj.insert(make_pair(x, Cpu(n)));

        BinTree<string> left, right;
        
        if (read_bintree(left) and read_bintree(right)) it.first->second.is_leaf();
        a = BinTree<string>(x, left, right);

        return false;
    }
    else return true;
}

void Cluster::reread(BinTree<string>& a, string p) {
    if (!a.empty()) {
        string x = cluster.value();
        if (x != p) {
            BinTree<string> left, right;

            reread(left, p);
            reread(right, p);
            a = BinTree<string>(x, left, right);
        }
        else read_bintree(a);
    }
}

void Cluster::ipc() const {
    clus::const_iterator it2 = conj.end();

    for (clus::const_iterator it = conj.begin(); it != it2; ++it) {
        cout << it->first << endl;
        it->second.write_cpu();
    }
}

int Cluster::app(const string& x, int identity, int memory, int time) {
    clus::iterator it = conj.find(x);

    if (it == conj.end()) return 101;
    return it->second.add_process_cpu(identity, memory, time);
}

int Cluster::bpp(const string& x, int identity) {
    clus::iterator it = conj.find(x);

    if (it == conj.end()) return 101;

    return it->second.remove_process_cpu(identity);
}

bool Cluster::ipro(const string& x) const {
    clus::const_iterator it = conj.find(x);
    
    if (it == conj.end()) return false;
    it->second.write_cpu();
    return true;
}

void Cluster::at(int t) {
    clus::iterator itf = conj.end();

    for (clus::iterator it = conj.begin(); it != itf; ++it) it->second.advance(t);
}

bool Cluster::cmp(string x) {
    clus::iterator it = conj.lower_bound(x);

    if (it == conj.end() or it->first != x) return false;
    it->second.compactar();
    return true;
}

void Cluster::cmc() {
    clus::iterator itf = conj.end();
    for (clus::iterator it = conj.begin(); it != itf; ++it) it->second.compactar();
}

int Cluster::mc(string x) {
    clus::const_iterator it = conj.lower_bound(x);

    if (it == conj.end() or it->first != x) return 101;
    if (!it->second.active_processes()) return 102;
    if (!it->second.yn_leaf()) return 103;

    reread(cluster, x);
    conj.erase(it);

    return 100;
}

//void Cluster::at(int t) {
    
//}

//CODE HELL
Cluster::Cluster() {}
