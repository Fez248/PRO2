/**
    @mainpage Simulation of a cluster
*/

/**
    @file program.cc
    @brief Main program <em>Simulation of a cluster</em>.
*/

#include "Waiting_Area.hh"
#include "Cluster.hh"
#include "Process.hh"
#include "Cpu.hh"
using namespace std;

int main() {

    /**
    This programm allows us to create a cluster (connected cpus) in a form of a binary tree. Each cpu 
    with his proccess and his memmory. We also have a waiting area where pending processes stay till they 
    are sended to the cluster. All the errors that may occur are handled inside the functions in order to keep the main
    pogram simple and readable.
    */

    Cluster clust;
    clust.read();

    Waiting_Area wait;
    wait.read_waiting_area();

    string comand;
    cin >> comand;

    //puedo sacar factor comun del cout << "#" << comand << endl; o sin el endl por si luego hay otra cosa
    while(comand != "fin") {
        if (comand == "configurar_cluster" or comand == "cc") {
            cout << "#" << comand << endl;
            clust.read();
        }
        else if (comand == "modificar_cluster" or comand == "mc") {
            string x;
            cin >> x;

            cout << "#" << comand << " " << x << endl;
            int output = clust.mc(x);

            if (output == 101) cout << "error: no existe procesador" << endl;
            else if (output == 102) cout << "error: procesador con procesos" << endl;
            else if (output == 103) cout << "error: procesador con auxiliares" << endl;
        }
        else if (comand == "alta_prioridad" or comand == "ap") {
            string x;
            cin >> x;

            cout << "#" << comand << " " << x << endl;
            if (!wait.ap(x)) cout << "error: ya existe prioridad" << endl;
        }
        else if (comand == "baja_prioridad" or comand == "bp") {
            string x;
            cin >> x;

            cout << "#" << comand << " " << x << endl;
            int output = wait.bp(x);

            if (output == 101) cout << "error: no existe prioridad" << endl;
            else if (output == 102) cout << "error: prioridad con procesos" << endl;
        }
        else if (comand == "alta_proceso_espera" or comand == "ape") {
            string x;
            int identity, memory, time;
            cin >> x >> identity >> memory >> time;

            cout << "#" << comand << " " << x << " " << identity << endl;
            int output = wait.ape(x, identity, memory, time);

            if (output == 101) cout << "error: no existe prioridad" << endl;
            else if (output == 102) cout << "error: ya existe proceso" << endl;
        }
        else if (comand == "alta_proceso_procesador" or comand == "app") {
            string x;
            int identity, memory, time;
            
            cin >> x >> identity >> memory >> time;

            cout << "#" << comand << " " << x << " " << identity << endl;
            int output = clust.app(x, identity, memory, time);

            if (output == 101) cout << "error: no existe procesador" << endl;
            else if (output == 102) cout << "error: ya existe proceso" << endl;
            else if (output == 103) cout << "error: no cabe proceso" << endl;
        }
        else if (comand == "baja_proceso_procesador" or comand == "bpp") {
            string x;
            int identity;
        
            cin >> x >> identity;
            int output = clust.bpp(x, identity);
            
            cout << "#" << comand << " " << x << " " << identity << endl;
            if (output == 101) cout << "error: no existe procesador" << endl;
            else if (output == 102) cout << "error: no existe proceso" << endl;
        }
        else if (comand == "enviar_procesos_cluster" or comand == "epc") wait.epc();
        else if (comand == "avanzar_tiempo" or comand == "at") {
            int x;
            cin >> x;

            cout << "#" << comand << " " << x << endl;
            clust.at(x);
        }
        else if (comand == "imprimir_prioridad" or comand == "ipri") {
            string x;
            cin >> x;

            cout << "#" << comand << " " << x << endl;
            if (!wait.ipri(x)) cout << "error: no existe prioridad" << endl;
        }
        else if (comand == "imprimir_area_espera" or comand == "iae") {
            cout << "#" << comand << endl;
            wait.iae();
        }
        else if (comand == "imprimir_procesador" or comand == "ipro") {
            string x;
            cin >> x;

            cout << "#" << comand << " " << x << endl;
            if(!clust.ipro(x)) cout << "error: no existe procesador" << endl;
        }
        else if (comand == "imprimir_procesadores_cluster" or comand == "ipc") {
            cout << "#" << comand << endl;
            clust.ipc();
        }
        else if (comand == "imprimir_estructura_cluster" or comand == "iec") {
            cout << "#" << comand << endl;
            clust.iec();
            cout << endl;
        }
        else if (comand == "compactar_memoria_procesador" or comand == "cmp") {
            string x;
            cin >> x;

            cout << "#" << comand << " " << x << endl;
            if(!clust.cmp(x)) cout << "error: no existe procesador" << endl;
        }
        else {
            cout << "#" << comand << endl;
            clust.cmc();
        }
        cin >> comand;
    }
}