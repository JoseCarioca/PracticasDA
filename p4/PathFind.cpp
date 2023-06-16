// ###### Config options ################

#define PRINT_PATHS 1

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1
               
#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"
#include <limits>

#ifdef PRINT_PATHS
#include "ppm.h"
#endif

using namespace Asedio;


Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){ 
    return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0); 
}

void positionToCell(const Vector3 pos, int &i_out, int &j_out, float cellWidth, float cellHeight){ i_out = (int)(pos.y * 1.0f/cellHeight); j_out = (int)(pos.x * 1.0f/cellWidth); } 


//distancia de Manhattan
float manhattan(AStarNode* current, AStarNode* target){
    return _sdistance(current->position, target->position);
}
void DEF_LIB_EXPORTED calculateAdditionalCost(float** additionalCost
                   , int cellsWidth, int cellsHeight, float mapWidth, float mapHeight
                   , List<Object*> obstacles, List<Defense*> defenses) {

    float cellWidth = mapWidth / cellsWidth;
    float cellHeight = mapHeight / cellsHeight;

    for(int i = 0 ; i < cellsHeight ; ++i) {
        for(int j = 0 ; j < cellsWidth ; ++j) {
            Vector3 cellPosition = cellCenterToPosition(i, j, cellWidth, cellHeight);
            float cost = 0;
            if( (i+j) % 2 == 0 ) {
                cost = cellWidth * 100;
            }
            
            additionalCost[i][j] = cost;
        }
    }
}

void DEF_LIB_EXPORTED NOcalculateAdditionalCost(float** additionalCost
                   , int cellsWidth, int cellsHeight, float mapWidth, float mapHeight
                   , List<Object*> obstacles, List<Defense*> defenses) {
    
    float infinito_f = std::numeric_limits<float>::infinity();
    float cellWidth = mapWidth / cellsWidth;
    float cellHeight = mapHeight / cellsHeight;
    Vector3 cellPosition;
    //INICIALIZAMOS COSTE EN 0
    for(int i = 0 ; i < cellsHeight ; ++i) {
        for(int j = 0 ; j < cellsWidth ; ++j) {
            
            additionalCost[i][j] = 0;
        }
    }
    //LOS OBSTACULOS TIENEN COSTE INFINITO
    List<Object*>::iterator currentObstacle = obstacles.begin();
    int i=0, j=0;
    float radio; float tam; //numero de casillas que estorba el obstaculo
    int casillas; float ejex;
    int toca;

	while(currentObstacle != obstacles.end()){
        radio = (*currentObstacle)->radio;
        int col, row;
        positionToCell((*currentObstacle)->position,row,col,cellWidth,cellHeight);
        
    
        tam = radio/cellWidth; 
        casillas = ceil(tam); // numero de casillas horizontales desde el centro que quedan ocupadas
      
        if(tam >=1){
            for(int i=0; i<casillas;i++){
            ejex = sqrt(pow(radio,2) - pow(i,2));
            
            toca = casillas;
            while((toca)*cellWidth > ejex){
                toca--;
            }
            for(int x = -toca; x<toca;x++){
                if(row+i < cellsHeight && col+x > 0 && col+x < cellsWidth){
                    additionalCost[row+i][col+x] = INF_F; //podrias poner null, INF para casillas de abajo del circulo
                }
          
                if(row-i > 0 && col+x > 0 && col+x < cellsWidth){
                    additionalCost[row-i][col+x] = INF_F; //pone en INF casillas de arriba
                }
            }
        }//la idea es que el obstaculo inutiliza las casillas desde su centro en un radio
        }else{
          
            additionalCost[row][col] = INF_F; //si solo ocupa la casilla central -> poner esa en INF
        }
        

		++currentObstacle;
	}
    /*
    //LAS DEFENSAS UN COSTE ALTO Y SUS ALREDEDORES MEDIO
    //se debe evitar las defensas lo maximo posible
    List<Defense*>::iterator currentDefense = defenses.begin();
    ++currentDefense; //la primera es el centro de extraccion asi que no lo queremos evitar
    int columna, fila;
	while(currentDefense != defenses.end()){
        positionToCell((*currentDefense)->position,fila,columna,cellWidth,cellHeight);
		additionalCost[fila][columna] = additionalCost[fila][columna] + 20;  //centro
        if (columna>0) additionalCost[fila][columna-1] = additionalCost[fila][columna] + 10; //izquierda
        if (columna<cellsWidth) additionalCost[fila][columna+1] = additionalCost[fila][columna] + 10; //derecha
        if (fila>0) additionalCost[fila-1][columna] = additionalCost[fila][columna] + 10; //arriba
        if (fila< cellsHeight) additionalCost[fila+1][columna] = additionalCost[fila][columna] + 10; //abajo
		++currentDefense;
	}
    */
   
}
void fgh(AStarNode* n, const AStarNode* origin, const AStarNode* target){


}

AStarNode* extrae_mejor(std::list<AStarNode*> abiertas){//ya veer
    List<AStarNode*>::iterator it=abiertas.begin();
    AStarNode *mejor;
    //AStarNode*;
    float min = INF_F; float aux;
    while(it != abiertas.end()){
        aux = (*it)->F;
        if(aux < min){
            min = aux;
            mejor = *it; //count;
        }
        it++;
    }
    return mejor;
} 

bool dentrode(std::list<AStarNode*> lista, AStarNode* busqueda){
    for (auto nodo : lista){
        if (nodo == busqueda)
            return true;}
     return false;
}

void DEF_LIB_EXPORTED calculatePath(AStarNode* originNode, AStarNode* targetNode
                   , int cellsWidth, int cellsHeight, float mapWidth, float mapHeight
                   , float** additionalCost, std::list<Vector3> &path) {
    
    float cellWidth = mapWidth / cellsWidth;
    float cellHeight = mapHeight / cellsHeight;
    //notas: cellsWidth es nº casillas de ancho, mapWidth tam en u de ancho. y su division el tam de una casilla
    
    int maxIter = 0; //no se si usarlo
    float ** camino = new float*[cellsHeight]; 

    for (int i = 0; i < cellsHeight; ++i) {
        camino[i] = new float[cellsWidth]{};
    }

    List<AStarNode *> abiertas;
    abiertas.push_front(originNode);
    List<AStarNode *> cerradas; // vacia I guess
    AStarNode *c = originNode;
    //fgh manualmente
    c->G = 0.0f; 
    c->H = _sdistance(c->position, targetNode->position);
    c->F = c->H;
    float coste; //para guardar coste de distancia y obstaculos
    int i,j;
    
    while (c != targetNode &&  !abiertas.empty())
    {

        float min = INF_F; // maximo posible para un float jsjs
        // AStarNode* k = (); //no recuerdo que es
        //AStarNode *o = NULL; // no se pa que sirve

        //extrae mejor 
        c = extrae_mejor(abiertas);
        abiertas.remove(c); //se elimina mejor de abiertas
        cerradas.push_back(c); //add current to closed
        //IFFFFFFF
        path.push_back(c->position);
        
        for(List<AStarNode*>::iterator it = c->adjacents.begin(); it != c->adjacents.end(); ++it){
            positionToCell((*it)->position,i,j,cellWidth,cellHeight);
            
            coste = c->G + cellWidth; //sabemos que tam celda de ancho y largo son iguales
            (*it)->H = additionalCost[i][j] + _sdistance((*it)->position, targetNode->position) ;
            (*it)->F = (*it)->G + (*it)->H;
            if( (dentrode(abiertas,(*it)) || dentrode(cerradas,(*it)) ) && coste < (*it)->G){
                abiertas.remove(*it);
                cerradas.remove(*it); //esto lo puedo quitar porque no pasa nunca (con una buena heurisitca)
            }
            
            if( !dentrode(abiertas,(*it)) && !dentrode(cerradas,(*it)) )  
            {
                (*it)->G = coste;
                abiertas.push_back(*it);
                //set priority quere rank to g(it) + h(it)
                (*it)->parent = c;
            }
        }
    }
    /*
    //recupera camino
    if(c == targetNode) {
        auto n = targetNode;
        while(n != originNode){
            path.push_back(c->position);
            n = n->parent;
    }
    
}*/
}


