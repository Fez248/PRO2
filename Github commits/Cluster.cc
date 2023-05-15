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

void Cluster::read_bintree(BinTree<string>& a) {
    string x;
    cin >> x;

    if (x != "*") {
        int n;
        cin >> n;
        conj.insert(make_pair(x, Cpu(n)));

        BinTree<string> left;
        read_bintree(left);

        BinTree<string> right;
        read_bintree(right);

        a = BinTree<string>(x, left, right);
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

//void Cluster::at(int t) {
    
//}

//CODE HELL
Cluster::Cluster() {}
void Cluster::mc() {}
void Cluster::cmp() {}
void Cluster::cmc() {}
