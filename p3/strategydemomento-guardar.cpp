// ###### Config options ################



// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"
#include "cronometro.h"

using namespace Asedio;

float defaultCellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
    , float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;

    Vector3 cellPosition((col * cellWidth) + cellWidth * 0.5f, (row * cellHeight) + cellHeight * 0.5f, 0);

    float val = 0;
    for (List<Object*>::iterator it=obstacles.begin(); it != obstacles.end(); ++it) {
	    val += _distance(cellPosition, (*it)->position);
    }

    return val;
}

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

void DEF_LIB_EXPORTED placeDefenses3(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , List<Object*> obstacles, List<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;

    //estructura matriz N² x 2
    int N = nCellsWidth*nCellsHeight;
    float** valores = new float* [N];
    for(int i=0; i<N;i++){
        valores[i] = new float[2]; // [][0] = pos [][1] valores cellValue
        valores[i][0] = i;
        valores[i][1] = defaultCellValue(i/nCellsWidth,i%nCellsWidth, freeCells
        , nCellsWidth, nCellsHeight ,mapWidth, mapHeight, obstacles, defenses);// i/ncellswidth=row y i%ncellsheight=col
    }


	//1, SIN PRE-ORDENACION
	cronometro t;
	t.activar();
	List<Defense*>::iterator currentDefense = defenses.begin();
		float posx, posy; int row, col, i = 0;

	while(currentDefense != defenses.end()) {
		//maximo valor del vector
		int max = 0;
		for(int i = 0; i<N; i++){
			if(valores[i][1] > valores[max][1]){
				max = i;
			}
		}
		row = (int)valores[max][0]/nCellsHeight;
		col = (int)valores[max][0]%nCellsWidth;

		valores[max][1] = -1; //invalidamos valor para proximas iteraciones - pop en listas.
		if(factibilidad(posx, posy, mapWidth,mapHeight,currentDefense,defenses,obstacles)){
				(*currentDefense)->position.x = posx;
				(*currentDefense)->position.y = posy;
				(*currentDefense)->position.z = 0;
				++currentDefense;

			}
	}//FIN 1
	t.parar();
	std::cout << (N) << '\t' << t.tiempo() << '\t' << t.tiempo()*2 << '\t' << t.tiempo()*3  << '\t' << t.tiempo()*4<< std::endl;

    //ordenacion por... burbuja?
    int aux0; float aux1;
		for(int i=0;i<N-1;i++){
			for(int j=0; j<N-1;j++){
				if(valores[j][1]<valores[j+1][1]){
					aux0 = valores[j][0];
					aux1 = valores[j][1];
					valores[j][0] = valores[j+1][0];
					valores[j][1] = valores[j+1][1];
					valores[j+1][0] = aux0;
					valores[j+1][1] = aux1;
				}
			}
		} //preordena



    while(currentDefense != defenses.end()) {
			row = (int)valores[i][0]/nCellsHeight;
			col = (int)valores[i][0]%nCellsWidth;
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

	cronometro c;
    long int r = 0;
    c.activar();
    do {
		List<Defense*>::iterator currentDefense = defenses.begin();
		while(currentDefense != defenses.end()) {

			(*currentDefense)->position.x = ((int)(_RAND2(nCellsWidth))) * cellWidth + cellWidth * 0.5f;
			(*currentDefense)->position.y = ((int)(_RAND2(nCellsHeight))) * cellHeight + cellHeight * 0.5f;
			(*currentDefense)->position.z = 0;
			++currentDefense;
		}

		++r;
    } while(c.tiempo() < 1.0);
    c.parar();
    std::cout << (nCellsWidth * nCellsHeight) << '\t' << c.tiempo() / r << '\t' << c.tiempo()*2 / r << '\t' << c.tiempo()*3 / r << '\t' << c.tiempo()*4 / r << std::endl;
}
