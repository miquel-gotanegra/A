#ifndef GRAPH_CC
#define GRAPH_CC

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <random>
#include <list>
using namespace std;

typedef map<int,set<int> > GRAPH;
// un node el definim pel seu identificador(first)
//i per la llista de nodes al que es adjacent (second)
//Un graph es un conjunt de nodes

class Graph {

    private:
        GRAPH g;
        map<int,pair<double,double> > pos; // cada vertex te una posició xy en els grafs geomentrics
        string tipo ="";
        double dist = 0;
        int mida = 0;

        Graph(GRAPH grafo,int m,string t,int distancia =0){
        g = grafo;
        mida = m;
        tipo = t;
        if(tipo=="geom") dist = distancia;
        }

        //Esta funcion la professora nos dijo que no hacia falta asi que finalmente no la utilizamos, 
        //usamos otro metodo para no repetir la percolacion por aristas sobre la misma arista
        double probTwice(double prob){
            // basicamente tenemos que resolver la equación x + (1-x)*x = prob para que la nueva probabilidad 
            //conjunta en las dos aristas sea igual a la original
            // distribuimos x + (1-x)*x == x +(x - x^2) == 2x -x^2
            // -x^2 + 2x - (-1)*prob =0
            // solucionamos con la formula quadratica
            double a = -1, b= 2, c=prob*-1;
            double x1 = (-b + sqrt((b*b)-4*a*c))/(2*a);
            double x2 = (-b - sqrt((b*b)-4*a*c))/(2*a);
            if (x1 >=0 and x1 <= 1) return x1;
            else return x2;
        }
    

    public:

    Graph(){ 
        srand(time(NULL));
    }
    Graph(GRAPH g){
        this->g = g;
        this->tipo = "bin";
        this->mida = g.size();

    }
    int size(){
        return mida;
    }

    int num_aristas(){
        int suma =0;
        for (auto it = g.begin(); it != g.end(); it++){
            suma += it->second.size();
        }
        return suma;
    }

    void randomBinomial(int num_nodes, double prob) {
        // prob betwen 0 and 1, probabilitat de que dos nodes qualsevols siguin adjacents
        tipo="bin";
        mida = num_nodes;

        //inicialitzem el graph a 0, per si estem regenerant un graf ja creat
        g = GRAPH();

        
        for (int i = 0; i<num_nodes;++i){
            //aixo es necessari ja que si no si hi ha algun vertex que no es adjacent a cap altre no es crearia en el bucle de sota
            g[i];
            for (int j = i+1; j<num_nodes; ++j) {
                //aquest segon bucle garanteix que cada possible aresta es considerada nomes una vegada
                if (prob > ((rand()%1000)/1000.0) ) add_edge(i,j); // 4 digits de precisio
            }
        }

    }

    void randomGeometric(int num_nodes, double radius, int mida_espai = 1000) {
       
        tipo="geom";
        mida = num_nodes;

        g = GRAPH();
        dist = radius;


        for (int i = 0; i < num_nodes; ++i) {
            pos[i] = make_pair((rand()%mida_espai)*1.0 ,(rand()%mida_espai)*1.0);
            
            //aixo es necessari ja que si no si a algun vertex no n'hi arrives cap altre no es crearia amb el bucle de abaix 
            g[i];
        }
        for (int i = 0; i < num_nodes; ++i) {
            for (int j = i+1; j < num_nodes; ++j) {
                if (getDistance(pos[i],pos[j]) <= radius) add_edge(i,j);
            }
        }
    }

    void gridGraph(int m){
        tipo="grid";
        mida = m*m;
        g = GRAPH();
 
        for(int i =0; i <m; ++i){
            for(int j =0; j <m;++j){
                g[j + i*m];
                int p= j + i*m;
                pos[p] = make_pair(i,j);
                if(j > 0)           g[j+i*m].insert(j-1+i*m);
                if(j< (m-1))     g[j+i*m].insert(j+1+i*m);
                if(i > 0)           g[j+i*m].insert(j+(i-1)*m);
                if(i< (m-1))     g[j+i*m].insert(j+(i+1)*m);
            }
        }
    }

    void add_edge(int i, int j) {
        g[i].insert(j);
        g[j].insert(i);
    }

    double getDistance(pair<double,double> node_x, pair<double,double> node_y) {
        return sqrt(pow(node_x.first - node_y.first,2) + pow(node_x.second - node_y.second,2));
    }

    int connex_components() {
        
        vector<Graph> v = this->bfs();
        return v.size();

    }
    void printGraph(){
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        if(tipo=="bin"){
            cout << "---------------------Binomial graph----------------------" << endl;
            
            for (auto it = g.cbegin(); it != g.cend(); it++){
                cout << "node "<<it->first << " adjacent a:";
                for (auto edge = it->second.cbegin(); edge !=  it->second.cend(); edge++) 
                    cout << " " << *edge;
                cout << endl;
            }
        }
        else if(tipo=="geom"){
            cout << "---------------------Geometric graph---------------------" << endl;
            cout << "Distancia = " << dist << endl;

            for (auto it = g.cbegin(); it != g.cend(); it++){
                cout << "node "<<it->first << "(" << pos[it->first].first<<  "," << pos[it->first].second << ")" << " adjacent a:";
                for (auto edge = it->second.cbegin(); edge !=  it->second.cend(); edge++) 
                    cout << " " << *edge;
                cout << endl;
            }
        }
        else if(tipo=="grid"){ 
            cout << "------------------------Grid graph-----------------------" << endl;
            cout << "Mida : " << mida << "x" << mida << endl;

            for (auto it = g.cbegin(); it != g.cend(); it++){
                cout << "node "<<it->first << "(" << pos[it->first].first<<  "," << pos[it->first].second << ")" << " adjacent a:";
                for (auto edge = it->second.cbegin(); edge !=  it->second.cend(); edge++) 
                    cout << " " << *edge;
                cout << endl;
            }
        }    
        else{
            cout << "-------------------Uninitialized graph-------------------" << endl;
        }
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    }

    vector<Graph> bfs() {
        
        vector<Graph> subgraphs;
        map<int,bool> visited;

        for (auto it = g.cbegin(); it != g.cend(); it++){
             visited[it->first] = false;
         }
        map<int,bool>::iterator it;
        for(it = visited.begin(); it != visited.end(); it++){
            if(!it->second){
                GRAPH sub;
                queue<int> q;
                q.push(it->first);
                it->second = true;

                while(!q.empty()){
                    set<int> s = g[q.front()];
                    sub[q.front()] = s;

                    for (auto setit = s.cbegin(); setit != s.cend(); setit++){
                        if(!visited[*setit]){
                            q.push(*setit);
                            visited[*setit]= true;
                        }
                    }
                    q.pop();
                }
                subgraphs.push_back(Graph(sub,mida,tipo,dist));
            }
            
        }
        return subgraphs;
    }

    void nodePercolation(double prob, bool output = false){
        // prob := probabilidad de que un nodo SIGA en el grafo despues del proceso de percolacion

        queue<int> nodosABorrar;


        for(auto it = g.begin(); it != g.end();++it){
            if(prob < ((rand()%10000)/10000.0)){  //4 digitos de precision
                int node = it->first;
                for(auto sit = it->second.cbegin(); sit != it->second.cend();++sit){
                    g[*sit].erase(node);
                }
                nodosABorrar.push(node);
                if(output) cout << "REMOVED NODE " << node << endl;
            }
        }

        //esto es basicamente porque si lo borro dentro del bucle se vuelve un poco loco,
        //ya que al borrar la posicion el puntero apunta a otro lugar y se pude salir de bounds
        while(!nodosABorrar.empty()){
            g.erase(nodosABorrar.front());
            nodosABorrar.pop();
        }
    }

    void edgePercolation(double prob,bool output = false){
        // prob := probabilidad de que una arista SIGA en el grafo despues del proceso de percolacion

        //ESTA FUNCION FINALMENTE NO LA USAMOS, UTILIZAMOS EL SET nodesVisitats PARA GARANTIZAR QUE CADA ARISTA SOLO CUENTE UNA VEZ
        // en este caso hay que modificar la probabilidad de que una arista qualquiera se mantenga, porque cada arista la visitamos dos veces
        //double newProb = probTwice(prob);

        queue<int> aristasABorrar;
        set<int> nodesVisitats;

        for(auto it = g.begin(); it != g.end();++it){
            set<int>::iterator sit = it->second.begin();
            nodesVisitats.insert(it->first);
            while(sit != it->second.end()){
                //si no hemos visto esta arista, aka si no hemos pasdo por el nodo que conecta
                //i si la probabilidad coincide, la borramos
                if(nodesVisitats.find(*sit) != nodesVisitats.end() and prob < ((rand()%10000)/10000.0)) 
                    aristasABorrar.push(*sit);
                ++sit;
            }
            //esto es basicamente porque si lo borro dentro del bucle se vuelve un poco loco,
            //ya que al borrar la posicion el puntero apunta a otro lugar y se pude salir de bounds
            while(!aristasABorrar.empty()){
                it->second.erase(aristasABorrar.front());
                g[aristasABorrar.front()].erase(it->first);
                if(output) cout << "REMOVED EDGE (" << it->first << "," << aristasABorrar.front() << ")" << endl;
                aristasABorrar.pop();
            }
        }
        
            

    }
    bool allCC_Complex(){
        //este grafo devuelve cierto si todas las componentes conexas de un grafo son complejas
        //Una componente connexa no es compleja si es un arbol o es uniciclo

        //Sabemos que: todo grafo(V,E) conexo tal que |E|=|V|-1 es un arbol, ya si hubiera un ciclo alguno de los vertices
        //no estaria conectado, lo qual es contradiccion ya que la componente es conexa
        //Un grafo uniciclo es un arbol al que le añadimos una arista, por tanto |E|=|V|. 
        //Por el razonamiento anterior sabeos que todo grafo connexo tal que |E|=|V| tiene exactamente un ciclo.

        vector<Graph> comp_conexas = bfs();
        for(int i=0;i<comp_conexas.size();++i){
            int g = comp_conexas[i].size();
            int e = comp_conexas[i].num_aristas();
            //comp_conexas[i].printGraph();
            if(g-e < 0) return false;
        }
        return true;
    }
/*
    int bfs() {
        int components_connexes = 0;

        map<int,bool> visited;

        for (auto it = g.cbegin(); it != g.cend(); it++){
             visited[it->first] = false;
         }
        map<int,bool>::iterator it;
        for(it = visited.begin(); it != visited.end(); it++){
            if(!it->second){
                ++components_connexes;
                queue<int> q;
                q.push(it->first);
                it->second = true;
                while(!q.empty()){
                    set<int> s = g[q.front()];
                    for (auto setit = s.cbegin(); setit != s.cend(); setit++){
                        if(!visited[*setit]){
                            q.push(*setit);
                            visited[*setit]= true;
                        }
                    }
                    q.pop();
                }
            }
            
        }
        return components_connexes;
    }

*/
};

// c++ graph.cc -o graph
// g++ -o exec graph.cc (LINUX)



// CAMBIO PRUEBA GIT

#endif