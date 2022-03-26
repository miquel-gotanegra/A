#include <iostream>
#include "graph.cc"
#include <fstream>
#include <sys/types.h> // required for stat.h
#include <sys/stat.h> // no clue why required -- man pages say so
#include <bits/stdc++.h>
using namespace std;

void mkcarpeta(string dir){
mode_t nMode = 0733; // UNIX style permissions
int nError = 0;
#if defined(_WIN32)
  nError = mkdir(dir.c_str()); // can be used on Windows
#else 
  nError = mkdir(dir.c_str(),nMode); // can be used on non-Windows
#endif
}

void experimentacioConnex(string tipusGraf, int num_nodes){
    if(tipusGraf=="grid"){cout << "El grafs graella sempre son connexos" << endl;return;}
    else if(tipusGraf != "binomial" and tipusGraf != "geometric"){cout << tipusGraf << "Wrong graph type. Valid types: binomial, geometric o grid" << endl;return;}
    Graph g;
    ofstream myfile;
    if(tipusGraf=="binomial"){
        mkcarpeta("binomial_connectivity");
        myfile.open ("binomial_connectivity/Size"+to_string(num_nodes)+".csv");
        myfile << "Numero de components conexes de un graf binomial de num_nodes "+to_string(num_nodes)+" amb probabilitat PROB\n";
        myfile << "PROB;NUM_CC(mitjana)\n";
        for(double prob = 0; prob <= 1 ; prob += 0.01){
            double avg = 0;
            for (int j=0;j<10;++j){
                g.randomBinomial(num_nodes,prob);
                avg += g.connex_components();
            }
            avg /=10.0;
            myfile <<to_string(prob)+";"+to_string(avg)+"\n";
            
        }
    }
    else if(tipusGraf=="geometric"){
        mkcarpeta("geometric_connectivity");
        myfile.open ("geometric_connectivity/Size"+to_string(num_nodes)+".csv");
        myfile << "Numero de components conexes de un graf geometric de num_nodes "+to_string(num_nodes)+" amb radi R\n";
        myfile << "R;NUM_CC(mitjana)\n";
        for(double radi = 0; radi <= 800 ; ++radi){
            double avg = 0;
            for (int j=0;j<10;++j){
                g.randomGeometric(num_nodes,radi);
                avg += g.connex_components();
            }
            avg /=10.0;
            myfile <<to_string(radi)+";"+to_string(avg)+"\n";
            
        }
    }
    myfile.close();
}

void experimentacioPercolation(string tipusPercolation, string tipusExperiment,string tipusGraf,int num_nodes,double prob,double radius,int num_it=10){
    //aqui no se si hem de generar un graf per cada mostra o pendre diferentes mostres sobre el mateix graf
    if(tipusGraf!="binomial" and tipusGraf!="geometric" and tipusGraf!="grid"){cout << "Wrong percolation type. Valid types: binomial, geometric o grid";return;}
    if(tipusExperiment!="connex" and tipusExperiment!="complex"){cout << "Wrong experiment type. Valid types: connex, complex";return;}
    ofstream myfile;
    vector<Graph> grafos(num_it);
    string eType;
    if(tipusExperiment == "connex") eType="Connex";
    else eType="Complex";
    //Generem num_it grafs amb les característiques que ens donen
    for (int i=0;i<num_it;++i){
        if(tipusGraf=="binomial") grafos[i].randomBinomial(num_nodes,prob);
        else if(tipusGraf=="geometric") grafos[i].randomGeometric(num_nodes,radius);
        else if(tipusGraf=="grid")grafos[i].gridGraph(num_nodes);
        if (!(grafos[i].connex_components() == 1)) --i;
        //grafos[i].printGraph();
    }

    //Apliquem les percolacions per cada probabilitat i fem la mitjana
    if(tipusPercolation == "nodes"){
        string dir = tipusGraf +"_nodePercolation"+eType;
        mkcarpeta(dir.c_str());
        myfile.open (dir+"/Size"+to_string(num_nodes)+".csv");
        if(tipusExperiment == "connex"){
            myfile << "Numero de components conexes de un graf "+tipusGraf +" conex de num_nodes "+to_string(num_nodes)+" despres d'aplicanr percolacio per nodes amb probabilitat PROB_PERC\n";
            myfile << "PROB_PERC;NUM_CC(mitjana)\n";
        }
        else {
            myfile << "Probabilitat de que un graf"+tipusGraf +" conex de num_nodes "+to_string(num_nodes)+" sigui complex despres d'aplicar percolacio per nodes amb probabilitat PROB_PERC\n";
            myfile << "PROB_PERC;PROB_COMPLEX\n";
        }
        for(double probPerc = 0; probPerc <= 1 ; probPerc += 0.01){
            double avg = 0;
            for (int i=0;i<num_it;++i){
                Graph g = grafos[i];
                g.nodePercolation(probPerc);
                if(tipusExperiment == "connex") avg += double (g.connex_components());
                else if(g.allCC_Complex()) avg +=1.0;
            }
            avg /=double(num_it);
            myfile <<to_string(probPerc)+";"+to_string(avg)+"\n";
            
        } 

    }
    else if(tipusPercolation == "edges"){
        string dir = tipusGraf +"_edgePercolation"+eType;
        mkcarpeta(dir.c_str());
        myfile.open (dir+"/Size"+to_string(num_nodes)+".csv");
        if(tipusExperiment == "connex"){
            myfile << "Numero de components conexes de un graf "+tipusGraf +" conex de num_nodes "+to_string(num_nodes)+" despres d'aplicanr percolacio per arestes amb probabilitat PROB_PERC\n";
            myfile << "PROB_PERC;NUM_CC(mitjana)\n";
        }
        else {
            myfile << "Probabilitat de que un graf"+tipusGraf +" conex de num_nodes "+to_string(num_nodes)+" sigui complex despres d'aplicar percolacio per arestes amb probabilitat PROB_PERC\n";
            myfile << "PROB_PERC;PROB_COMPLEX\n";
        }
        for(double probPerc = 0; probPerc <= 1.0 ; probPerc += 0.01){
            double avg = 0;
            for (int i=0;i<num_it;++i){
                Graph g = grafos[i];
                g.edgePercolation(probPerc);
                if(tipusExperiment == "connex") avg += double (g.connex_components());
                else if(g.allCC_Complex()) avg +=1.0;
            }
            avg /=double(num_it);
            myfile <<to_string(probPerc)+";"+to_string(avg)+"\n";
            
        }
    }
    else{cout << "Wrong percolation type. Accepted: nodes, edges." << endl;return;}
    
}

int main() {

    /*
    Graph g;
    g.randomBinomial(10,0.3);
    g.edgePercolation(0.3);
    g.printGraph();
    cout << g.connex_components() << endl;
    g.allCC_Complex();
    */

    

   //COMPONENTS_CONEXES
   
    vector<string> v = {"binomial","geometric","grid"};
    vector<int> num_nodes = {10,25,50,100,200};
    for(int i =0;i<v.size();++i){
        for(int n=0;n<num_nodes.size();++n) experimentacioConnex(v[i],num_nodes[n]);
    }
    
    
    
    
   //PERCOLATION

    //int num_iteracions; //numero de grafs sobre els que fem l'experiment(ESTA COM A DEFAULT A LA FUNCIO)
    vector<string> exp = {"connex","complex"}; 
    for(int e=0;e<exp.size();++e){

        //BINOMIAL
        vector<int> num_nodes =         {10,25,50,100,200};
        vector<double> connect_prob =   {0.35,0.2,0.10,0.05,0.03};
        for(int i=0;i<num_nodes.size();++i){
            experimentacioPercolation("nodes",exp[e],"binomial",num_nodes[i],connect_prob[i],0);
            experimentacioPercolation("edges",exp[e],"binomial",num_nodes[i],connect_prob[i],0);
        }
        //GEOMETRIC
        vector<int> connect_dist = {600,450,300,190,150};
        for(int i=0;i<num_nodes.size();++i){
            experimentacioPercolation("nodes",exp[e],"geometric",num_nodes[i],0,connect_dist[i]);
            experimentacioPercolation("edges",exp[e],"geometric",num_nodes[i],0,connect_dist[i]);
        }

        //GRID
        num_nodes = {10,25,50,100};
        for(int i=0;i<num_nodes.size();++i){
            experimentacioPercolation("nodes",exp[e],"grid",num_nodes[i],0,0);
            experimentacioPercolation("edges",exp[e],"grid",num_nodes[i],0,0);
        }
    }
    
    return 0;
}

