// ###### Config options ################


// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

using namespace Asedio;

//funcion que da valor a las defensas
int DefensesValue(List<Defense*>::iterator defensa){
    //float radio = (*defensa)->radio;
    float rango = (*defensa)->range;
    float dispersion = (*defensa)->dispersion;
    float dmg = (*defensa)->damage;
    float dps = (*defensa)->attacksPerSecond;
    float vida = (*defensa)->health;
    float coste = (*defensa)->cost;
    
    //float resultado = dmg*dps*rango*(vida+dmg*80.0f+dps*500.0f+rango*8.0f)/(coste*radio);
    //float resultado = (vida+dmg*dps*100+rango*10)/(coste*radio);
    float resultado = (vida*2+dmg*1000+dps*5000+rango*30)/(coste);
    //printf("resultado es: %d\n",static_cast<int>(resultado));
    return static_cast<int>(resultado);
}

void DEF_LIB_EXPORTED selectDefenses(std::list<Defense*> defenses, unsigned int ases, std::list<int> &selectedIDs
            , float mapWidth, float mapHeight, std::list<Object*> obstacles) {

    unsigned int cost = 0;
    unsigned int dinero = ases;
    int size = defenses.size();
    //primer defensa obligatoria
    std::list<Defense*>::iterator defensa = defenses.begin();
    cost += (*defensa)->cost;
    dinero -= cost;
    selectedIDs.push_back((*defensa)->id); //que es 0 podrias poner 0 pero bue
    ++defensa; //pasamos a la siguiente
    size--; //reducimos en 1 y por tanto en 1 el TAM de nuestro vector de valores

    

    //TODO
    //CREA UNA LISTA DE ID DE LOS MAS PROMETEDORES
    // DONDE EL ALGORITMO DE LA MOCHILA VAYA POPEANDO LOS RESULTADOS

    //matriz nc para las posibilidades
    int tabla[size][dinero];
    
    printf("numero defensas: %d\n",size);
    int **valores = new int *[size];
    
    for(int i=0; i<size; i++){
        valores[i] = new int[3];
        valores[i][0] = (*defensa)->id;
        valores[i][1] = DefensesValue(defensa);
        //printf("valor de %f es %f\n",valores[i][0], valores[i][1]);
        valores[i][2] = (*defensa)->cost;
        ++defensa;
    }
    // /*
    int temp0;
    int temp1, temp2;
    for(int i=0; i<size-1;i++){
        for(int j=0; j<size-1; j++){
            if(valores[j][1] < valores[j+1][1]){
                temp0 = valores[j][0]; //guarda id defensa
                temp1 = valores[j][1]; //guarda valor dado por valor
                temp2 = valores[j][2]; //coste
                valores[j][0] = valores[j+1][0];
                valores[j][1] = valores[j+1][1];
                valores[j][2] = valores[j+1][2];
                valores[j+1][0] = temp0;
                valores[j+1][1] = temp1;
                valores[j+1][2] = temp2;
            }
        }
    }//preordena
    //*/


    ///*
    std::list<Defense*>::iterator it = defenses.begin();
    ++it; //la primera ya esta comprada
    int i = 0;
    while(it != defenses.end()){
        printf("valor de %d es %d\n",valores[i][0], valores[i][1]);
        if(cost + valores[i][2] <=ases){
            selectedIDs.push_back(static_cast<int>(valores[i][0]));
            cost += valores[i][2];
        }
        i++;
        ++it;
    }
    //*/

    /*
    std::list<Defense*>::iterator it = defenses.begin();
    while(it != defenses.end()) {
        if(cost + (*it)->cost <= ases) {
            selectedIDs.push_back((*it)->id);
            cost += (*it)->cost;
        }
        ++it;
    }
    */

}
