// ###### Config options ################

#define PRINT_DEFENSE_STRATEGY 1    // generate map images

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"
#include <cmath>

#ifdef PRINT_DEFENSE_STRATEGY
#include "ppm.h"
#endif

#ifdef CUSTOM_RAND_GENERATOR
RAND_TYPE SimpleRandomGenerator::a;
#endif

using namespace Asedio;


bool factibilidad(float x, float y, float mapWidth, float mapHeight, List<Defense*>::iterator defensa, List<Defense*> defenses, List<Object*> obstacles){

    bool esFactible = true;
    float radio = (*defensa)->radio;

    if(x - radio<0 || y - radio<0 || (x + radio > mapWidth) || y + radio > mapHeight){esFactible = false;} 

    List<Defense*>::iterator currentDefense = defenses.begin();
    while(esFactible && (*currentDefense)->id != (*defensa)->id){ //lo del id es porque no itera, puedes dejarlo en defensa
        
        Vector3 diferencia = Vector3(x,y,0)-(*currentDefense)->position;
        if(diferencia.length() < radio + (*currentDefense)->radio){esFactible = false;}
        ++currentDefense;
    }

    List<Object*>::iterator currentObstacle = obstacles.begin();
    while(esFactible && currentObstacle!= obstacles.end() ) {
        
        Vector3 diferencia = Vector3(x,y,0)-(*currentObstacle)->position;
        if(diferencia.length() < radio + (*currentObstacle)->radio){esFactible = false;}
        ++currentObstacle;
    }
    return esFactible;
}


float cellValueDefensas(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
	, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {

    List<Defense*>::iterator base = defenses.begin(); //primera defensa o sea, el centro de extracción
    float base_x = (*base)->position.x;
    float base_y = (*base)->position.y;
    float cellWidth = mapWidth / nCellsWidth; 
    float cellHeight = mapHeight / nCellsHeight; 
    float resultado = (1000/(abs(row*cellHeight+ cellHeight*0.5f - base_y) + abs(col*cellWidth+cellWidth*0.5f - base_x) + 0.01f)); //offset para que no de 0
    if(!freeCells) resultado = 0.0f;
	return resultado; 
}


float cellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
	, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth; 
    float cellHeight = mapHeight / nCellsHeight; 
    float centro_x = col*cellWidth+cellWidth*0.5f;
    float centro_y = row*cellHeight+cellHeight*0.5f;
    float mitad = mapWidth/2;
    float resultado = ((1000)/(abs(centro_x - mitad) + abs(centro_y - mitad) + 0.001f)); //offset para que no de 0
    if(!freeCells) resultado = 0.0f;
	return resultado; 
}


//funcion seleccion mejor celda (reordenar esa estructura) de mayor a menor
void ordenarValues(float **valoresCeldas,int numCeldas){
    int temp0;
    float temp1;
    for(int i=0;i<numCeldas-1;i++){
        for(int j=0;j<numCeldas-1;j++){
            if(valoresCeldas[j][1] < valoresCeldas[j+1][1]){
                temp0 = valoresCeldas[j][0]; //guarda la posicion de la celda
                temp1 = valoresCeldas[j][1]; //guarda valor dado por cellValue
                valoresCeldas[j][0] = valoresCeldas[j+1][0];
                valoresCeldas[j][1] = valoresCeldas[j+1][1];
                valoresCeldas[j+1][0] = temp0;
                valoresCeldas[j+1][1] = temp1;
            }
        }
    }
}


void DEF_LIB_EXPORTED placeDefenses(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , std::list<Object*> obstacles, std::list<Defense*> defenses) {

            
    //matriz se convierte en vector para guardar los valores que asocio a cada celda + para centro de extraccion
    //bucle que recorra a todas las celdas y llame a cellvalue para que retorne un valor conoforme a prometedores
    printf("-1");
    int numCeldas = nCellsHeight*nCellsWidth;
    float **valoresCeldas = new float *[nCellsHeight*nCellsWidth];
    for(int i=0; i<numCeldas;i++){
        valoresCeldas[i] = new float[2]; //[0] = pos [1] = valor_base 
        valoresCeldas[i][0]= i;
        valoresCeldas[i][1]= ((cellValue(i/nCellsWidth, i%nCellsWidth, freeCells, nCellsWidth, nCellsHeight
	, mapWidth, mapHeight, obstacles, defenses))); // i/ncellswidth=row y i%ncellsheight=col
        
    }
    printf("0");
    int temp0;
    float temp1;
    for(int i=0;i<numCeldas-1;i++){
        for(int j=0;j<numCeldas-1;j++){
            if(valoresCeldas[j][1] < valoresCeldas[j+1][1]){
                temp0 = valoresCeldas[j][0]; //guarda la posicion de la celda
                temp1 = valoresCeldas[j][1]; //guarda valor dado por cellValue
                valoresCeldas[j][0] = valoresCeldas[j+1][0];
                valoresCeldas[j][1] = valoresCeldas[j+1][1];
                valoresCeldas[j+1][0] = temp0;
                valoresCeldas[j+1][1] = temp1;
            }
        }
    } //preordena el vector valoresCeldas
    printf("1");
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 
    List<Defense*>::iterator currentDefense = defenses.begin();
    float posx,posy;
    int row,col;
    int i = 0;
    
    //colocacion centro de extraccion
    while(currentDefense == defenses.begin()){
        row = (int)valoresCeldas[i][0]/nCellsHeight;
        col = (int)valoresCeldas[i][0]%nCellsWidth;
        posx= col * cellWidth + cellWidth * 0.5f;
        posy= row * cellHeight + cellHeight * 0.5f;
        if(factibilidad(posx, posy, mapWidth,mapHeight,currentDefense,defenses,obstacles)){
            (*currentDefense)->position.x = posx;
            (*currentDefense)->position.y = posy;
            (*currentDefense)->position.z = 0; 
            ++currentDefense;
        i++;
        }
    }
    
    //ESTRUCTURA PARA GUARDAR VALORES DE CELDAS PARA DEFENSAS
    //bucle que recorra a todas las celdas y llame a cellvalueDefensas para que retorne un valor conoforme a prometedores
    float **valoresCeldasDef = new float *[nCellsHeight*nCellsWidth];
    for(int i=0; i<numCeldas;i++){
        valoresCeldasDef[i] = new float[2]; //[0] = pos [1] = valor_base 
        valoresCeldasDef[i][0]= i;
        valoresCeldasDef[i][1]= ((cellValueDefensas(i/nCellsWidth, i%nCellsWidth, freeCells, nCellsWidth, nCellsHeight
	, mapWidth, mapHeight, obstacles, defenses)));
    }

    ordenarValues(valoresCeldasDef,numCeldas); //preordena el vector valoresCeldasDef
    printf("2");
    i = 0;
    while(currentDefense != defenses.end()) { 

        row = (int)valoresCeldasDef[i][0]/nCellsHeight;
        col = (int)valoresCeldasDef[i][0]%nCellsWidth;
        posx= row * cellWidth + cellWidth * 0.5f;
        posy= col * cellHeight + cellHeight * 0.5f;
        if(factibilidad(posx, posy, mapWidth,mapHeight,currentDefense,defenses,obstacles)){
            (*currentDefense)->position.x = posx;
            (*currentDefense)->position.y = posy;
            (*currentDefense)->position.z = 0; 
            ++currentDefense; 
        }
        i++;    
        
    }

#ifdef PRINT_DEFENSE_STRATEGY

    float** cellValues = new float* [nCellsHeight]; 
    for(int i = 0; i < nCellsHeight; ++i) {
       cellValues[i] = new float[nCellsWidth];
       for(int j = 0; j < nCellsWidth; ++j) {
           cellValues[i][j] = ((int)(cellValue(i, j, freeCells, nCellsWidth, nCellsHeight
	, mapWidth, mapHeight, obstacles, defenses))) % 256;
       }
    }
    dPrintMap("strategy.ppm", nCellsHeight, nCellsWidth, cellHeight, cellWidth, freeCells
                         , cellValues, std::list<Defense*>(), true);

    for(int i = 0; i < nCellsHeight ; ++i)
        delete [] cellValues[i];
	delete [] cellValues;
	cellValues = NULL;

#endif
    
}

// Devuelve la posición en el mapa del centro de la celda (i,j)
// i - fila
// j - columna
// cellWidth - ancho de las celdas
// cellHeight - alto de las celdas
Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){ return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0); }

// Devuelve la celda a la que corresponde una posición en el mapa
// pos - posición que se quiere convertir
// i_out - fila a la que corresponde la posición pos (resultado)
// j_out - columna a la que corresponde la posición pos (resultado)
// cellWidth - ancho de las celdas
// cellHeight - alto de las celdas
void positionToCell(const Vector3 pos, int &i_out, int &j_out, float cellWidth, float cellHeight){ i_out = (int)(pos.y * 1.0f/cellHeight); j_out = (int)(pos.x * 1.0f/cellWidth); } 