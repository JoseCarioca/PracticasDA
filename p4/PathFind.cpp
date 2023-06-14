// ###### Config options ################

#define PRINT_PATHS 1

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1
               
#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

#ifdef PRINT_PATHS
#include "ppm.h"
#endif

using namespace Asedio;

Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){ 
    return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0); 
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

void DEF_LIB_EXPORTED calculatePath(AStarNode* originNode, AStarNode* targetNode
                   , int cellsWidth, int cellsHeight, float mapWidth, float mapHeight
                   , float** additionalCost, std::list<Vector3> &path) {
    
    int maxIter = cellsHeight*cellsWidth;
    List<AStarNode*> abiertas;
    abiertas.pushback(originNode);
    List<AstarNode*> cerradas; //vacia I guess
    AStarNode* c = originNode; 
    while(current != targetNode && maxIter > 0){
        float min = INF_F; //maximo posible para un float jsjs
        AstarNode* k = ()
        AStarNode* o = NULL; //no se pa que sirve
        for(List<AstarNode*>::iterator it=c->adjacents.begin(); it != c->adjacents.end; ++it){

        }
    }


    int maxIter = 100; 
    AStarNode* current = originNode;
    while(current != targetNode && maxIter > 0) { // @todo ensure current and target are connected
	    float min = INF_F;
	    AStarNode* o = NULL;    
	    for (List<AStarNode*>::iterator it=current->adjacents.begin(); it != current->adjacents.end(); ++it) {
		    float dist = _sdistance((*it)->position, targetNode->position);
            
            if(additionalCost != NULL) { 
                dist += additionalCost[(int)((*it)->position.y / cellsHeight)][(int)((*it)->position.x / cellsWidth)];
            }
		    //std::cout << (*it)->position.y << ", " << (*it)->position.x << std::endl;
		    if(dist < min) {
			    min = dist;
			    o = (*it);
		    }
	    }

	    current = o;

        if(current == NULL) {
            break;
        }

        path.push_back(current->position);
        --maxIter;
    }

}
//Esta funcion es feisima
//
int extrae_mejor(std::list<AstarNode*> abiertas){//ya veer
    List<AStarNode*>::iterator it=abiertas.begin();
    int count = 0;
    int mejor;
    AStarNode* 
    float min = INF_F, aux;
    while(it != abiertas.end()){
        aux = (*it)->F;
        if(aux < min){
            min = aux;
            mejor = count;
        }
        it++;
        count++;
    }
} 