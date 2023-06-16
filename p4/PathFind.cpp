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

void showcosts(float** additionalCost,int cellsWidth, int cellsHeight){
    for (int i = 0; i < cellsHeight; ++i)
    {
        for (int j = 0; j < cellsWidth; ++j)
        {

            printf("%-3.0f ", additionalCost[i][j]);
        }
        printf("\n");
    }
    printf("\n\n\n");
}

//distancia de Manhattan
float manhattan(AStarNode* current, AStarNode* target){
    return _sdistance(current->position, target->position);
}

//la copio aqui para mirar unas cosas
bool factibilidad(float x, float y, float mapWidth, float mapHeight, List<Defense*>::iterator defensa,
List<Defense*> defenses, List<Object*> obstacles){
	bool esFactible = true;

	float radio = (*defensa)->radio;

	if(x<radio || y<radio || x + radio > mapWidth || y + radio > mapHeight){ esFactible = false;}

	List<Object*>::iterator currentObstacle = obstacles.begin();
	while(esFactible && currentObstacle != obstacles.end()){
		Vector3 diferencia = Vector3(x,y,0) - (*currentObstacle)->position;
		if(diferencia.length() < radio + (*currentObstacle)->radio) {esFactible = false;}
		++currentObstacle;
	}

	List<Defense*>::iterator currentDefense = defenses.begin();
	while(esFactible && (*currentDefense)->id != (*defensa)->id){
		Vector3 diferencia = Vector3(x,y,0) - (*currentDefense)->position;
		if(diferencia.length() < radio + (*currentDefense)->radio) {esFactible = false;}
		++currentDefense;
	}
	return esFactible;
}

void DEF_LIB_EXPORTED calculateAdditionalCost(float** additionalCost
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
            //CAMBIAR 99 A INF PARA ENTREGAR
            for(int x = -toca; x<toca;x++){
                if(row+i < cellsHeight && col+x > 0 && col+x < cellsWidth){
                    additionalCost[row+i][col+x] = 999; //podrias poner null, INF para casillas de abajo del circulo
                }
          
                if(row-i > 0 && col+x > 0 && col+x < cellsWidth){
                    additionalCost[row-i][col+x] = 999; //pone en INF casillas de arriba
                }
            }
        }//la idea es que el obstaculo inutiliza las casillas desde su centro en un radio
        }else{
          
            additionalCost[row][col] = 999; //si solo ocupa la casilla central -> poner esa en INF
        }
        

		++currentObstacle;
	}
    
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
    
    showcosts(additionalCost,cellsWidth,cellsHeight);
   
}

AStarNode* extrae_mejor(std::list<AStarNode*> abiertas){//ya veer
    List<AStarNode*>::iterator it=abiertas.begin();
    int count = 0;
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
        count++;
    }
    return mejor;
} 

void DEF_LIB_EXPORTED calculatePath(AStarNode* originNode, AStarNode* targetNode
                   , int cellsWidth, int cellsHeight, float mapWidth, float mapHeight
                   , float** additionalCost, std::list<Vector3> &path) {
    
    float cellWidth = mapWidth / cellsWidth;
    float cellHeight = mapHeight / cellsHeight;
    //notas: cellsWidth es nº casillas de ancho, mapWidth tam en u de ancho. y su division el tam de una casilla

    
    int maxIter = cellsHeight*cellsWidth;
    List<AStarNode *> abiertas;
    abiertas.push_back(originNode);
    List<AStarNode *> cerradas; // vacia I guess
    AStarNode *c = originNode;
    while (c != targetNode &&  !abiertas.empty() && maxIter > 0)
    {
        float min = INF_F; // maximo posible para un float jsjs
        // AStarNode* k = (); //no recuerdo que es
        AStarNode *o = NULL; // no se pa que sirve

        //extrae mejor 
        c = extrae_mejor(abiertas);
        std::cout << c->position.x << " "  << c->position.y << " " << c->F << " " << c->G << " " << c->H << std::endl;
        abiertas.remove(c); //se elimina mejor de abiertas
        cerradas.push_back(c); //add current to closed



        
    }
    

    float ** camino = new float*[cellsHeight];
    for (int i = 0; i < cellsHeight; ++i) {
        camino[i] = new float[cellsWidth]{};
    }

    maxIter = 100; 
    AStarNode* current = originNode;
    printf("height:%i width:%i\n",cellsHeight,cellsWidth);
    while(current != targetNode && maxIter > 0) { // @todo ensure current and target are connected
	    float min = INF_F;
	    AStarNode* o = NULL;    
	    for (List<AStarNode*>::iterator it=current->adjacents.begin(); it != current->adjacents.end(); ++it) {
		    float dist = _sdistance((*it)->position, targetNode->position);
            
            if(additionalCost != NULL) { //se puede usar para obstaculos y tal
                dist += additionalCost[(int)((*it)->position.y / cellsHeight)][(int)((*it)->position.x / cellsWidth)];
            }
		    //std::cout << "1. " << (*it)->position.y << ", " << (*it)->position.x << "\n" << std::endl;
            //std::cout << "2. " << (*it)->position.y / cellsHeight<< ", " << (*it)->position.x/ cellsWidth << "\n" << std::endl;
            //std::cout << "3. " << ((*it)->position.y - cellHeight*0.5)/cellHeight<< ", " << ((*it)->position.x - cellWidth*0.5)/cellWidth << "\n" << std::endl;
            
		    if(dist < min) {
			    min = dist;
                //std::cout << min << " " << std::endl;
			    o = (*it);
		    }
	    }

	    current = o;

        if(current == NULL) {
            break;
        }

        path.push_back(current->position);
        camino[(int)((current->position.y - cellHeight*0.5)/cellHeight)][(int)((current->position.x - cellWidth*0.5)/cellWidth)] = maxIter;
        --maxIter;
    }
    /*
    camino[(int)((targetNode->position.y - cellHeight*0.5)/cellHeight)][(int)((targetNode->position.x - cellWidth*0.5)/cellWidth)] = -1;
    for (int i = 0; i < cellsHeight; ++i) {
        for (int j = 0; j < cellsWidth; ++j) {
            std::cout << camino[i][j] << " ";
        }
        std::cout << std::endl;
    }
    printf("\n\n\n");
    */
    //show_costs(camino,cellsWidth,cellsHeight);

}
//Esta funcion es feisima
//


