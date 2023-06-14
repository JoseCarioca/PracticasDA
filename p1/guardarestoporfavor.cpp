// ###### Config options ################

//#define PRINT_DEFENSE_STRATEGY 1    // generate map images

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

#ifdef PRINT_DEFENSE_STRATEGY
#include "ppm.h"
#endif

#ifdef CUSTOM_RAND_GENERATOR
RAND_TYPE SimpleRandomGenerator::a;
#endif

using namespace Asedio;

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

	List<Defense*>::iterator currentDefense = defense.begin();
	while(esFactible && (*currentDefense)->id != (*defensa)->id){
		Vector3 diferencia = Vector3(x,y,0) - (*currentDefense)->position;
		if(diferencia.length() < radio + (*currentDefense)->radio) {esFactible = false;}
		++currentDefense;
	}
	return esFactible;
}

float cellValue(int row, int col, bool** freeCells, float cellWidth, float cellHeight
	, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
	float resultado;
	float mitad=mapWidth/2;
	centrox = col*cellWidth + cellWidth*0.5f;
	centroy = row*cellHeight + cellHeight*0.5f;
	resultado = 1000/(abs(centrox-mitad) + abs(centroy-mitad) +0.01f)//offset para evitar 1/0;
	if(!freeCells) resultado = 0;
	return resultado;
}

void DEF_LIB_EXPORTED placeDefenses(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , std::list<Object*> obstacles, std::list<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;

		int N = nCellsWidth*nCellsHeight;
		float **valores = new float *[N];
		for(int i=0; i<N;i++){
			valores[i] = new float[2] // [][0] = pos [][1] valores cellValue
			valores[i][0] = 0;
			valores[i][1] = cellValue(i/nCellsWidth, i%nCellsWidth, freeCells, cellWidth,cellHeight
, mapWidth, mapHeight, obstacles, defenses); // i/ncellswidth=row y i%ncellsheight=col
		}

		int aux0; float aux1;
		for(int i=0;i<N-1;i++){
			for(int j=0; j<N-1){
				if(valores[j][1]<valores[j+1][1]){
					aux0 = valores[j][0];
					aux1 = valores[j][1];
					valores[j][0] = valores[j+1][0];
					valores[j][1] = valores[j+1][1]
					valores[j+1][0] = aux0;
					valores[j+1][1] = aux1;
				}
			}
		} //preordena


    List<Defense*>::iterator currentDefense = defenses.begin();
		float posx, posy; int row, col, int i = 0;

    while(currentDefense != defenses.end()) {
			row = (int)valoresCeldas[i][0]/nCellsHeight;
			col = (int)valoresCeldas[i][0]%nCellsWidth;
			posx= col * cellWidth + cellWidth * 0.5f;
			posy= row * cellHeight + cellHeight * 0.5f;
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
           cellValues[i][j] = ((int)(cellValue(i, j))) % 256;
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
