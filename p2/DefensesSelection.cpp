// ###### Config options ################


// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

using namespace Asedio;

//funcion que da valor a las defensas
int DefensesValue(List<Defense*>::iterator defensa){
    float rango = (*defensa)->range;
    float dispersion = (*defensa)->dispersion;
    float dmg = (*defensa)->damage;
    float dps = (*defensa)->attacksPerSecond;
    float vida = (*defensa)->health;
    float coste = (*defensa)->cost;
    
    int resultado = (vida*2+dmg*1000+dps*5000+rango*30);

    return static_cast<int>(resultado);
}


int max(int a, int b) { return (a > b) ? a : b; }

void DEF_LIB_EXPORTED selectDefenses(std::list<Defense*> defenses, unsigned int ases, std::list<int> &selectedIDs
            , float mapWidth, float mapHeight, std::list<Object*> obstacles) {

    unsigned int cost = 0;
    unsigned int dinero = ases;
    int size = defenses.size();
    //primer defensa obligatoria
    std::list<Defense*>::iterator defensa = defenses.begin();
    dinero -= (*defensa)->cost;
    selectedIDs.push_back((*defensa)->id); //que es 0 podrias poner 0 pero bue
    ++defensa; //pasamos a la siguiente
    size--; //reducimos en 1 y por tanto en 1 el TAM de nuestro vector de valores

    

    //matriz nc para las posibilidades
    int **tabla = new int*[size];
    
    //vectores que guardan valor y coste de las defenses de id [i]
    int *valordef = new int[size];
    int *costes = new int[size];
    //guardamos valores
    for(int i=0; i<size; i++){
        valordef[i] = DefensesValue(defensa);
        costes[i] = (*defensa)->cost;
        ++defensa;
    }

    //ALGORITMO DE LA MOCHILA
    tabla[0] = new int[dinero]; //creacion dinamica de columnas
    for(int j=0; j<dinero; j++){
        
        if(j<costes[0]) tabla[0][j] = 0;
        else tabla[0][j] = valordef[0];
    }

    for(int i=1; i<size; i++){
        tabla[i] = new int[dinero];
        for(int j=0; j<dinero; j++){

            if(j<costes[i]) tabla[i][j] = tabla[i-1][j];
            else tabla[i][j] = max(tabla[i-1][j], tabla[i-1][j-costes[i]]+valordef[i]);
        }
        
    }
    //FIN ALG_MOCHILA

    //recuperacion de combinacion optima
    int i = size-1;
    int j = dinero-1;
    while(i>0 && j> 0){
        if(tabla[i][j] != tabla[i-1][j]){
            if(j-costes[i] > 0){
                selectedIDs.push_back(i+1);
                j-= costes[i];
            }
        }
        i--;
    }

}




