// ###### Config options ################

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"
#include "cronometro.h"

#include <array>

using namespace Asedio;
void merge_sort(float *array, int p, int r, int* ind);
void merge(float *array, int p, int q, int r, int *ind);

void ordenacion_rapida(float *array, int izq, int der, int *indices){
  int i = izq;
  int j = der;
  float aux = *(array + izq); //pivote
  int ind_aux = *(indices + izq);

  if(izq < der){
    while(i<j){
      while(*(array + j) <= aux && i<j){j--;}
      *(array + i) = *(array + j);
       *(indices + i) = *(indices + j);
      while(*(array + i)>= aux && i<j){i++;}
      *(array + j)=*(array + i);
      *(indices + j)=*(indices+ i);
    }
    *(array + i) = aux;
    *(indices + i) = ind_aux;
    ordenacion_rapida(array,izq,i-1,indices);
    ordenacion_rapida(array,j+1,der,indices);
  }
}


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

    //restrucutrando el codigo
    int *posiciones = new int [N];
    float *celvalues  = new float [N];
    for(int i=0; i<N;i++){
        posiciones[i]= i;
        celvalues[i] = defaultCellValue(i/nCellsWidth,i%nCellsWidth, freeCells
        , nCellsWidth, nCellsHeight ,mapWidth, mapHeight, obstacles, defenses);// i/ncellswidth=row y i%ncellsheight=col
    }


    //3 ORDENACION RAPIDA

    //cronometro t3;
    //t3.activar();
    ordenacion_rapida(celvalues,0,N,posiciones);

    currentDefense = defenses.begin();
    i = 0;
    while(currentDefense != defenses.end()) {
      row = (int)posiciones[i]/nCellsHeight;
      col = (int)posiciones[i]%nCellsWidth;
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
    //t3.parar();


}

void merge(float *array, int izq, int medio, int der, int* indices)
{
    //variables
    int i, j, k;
    int tam1 = (medio -izq) + 1;
    int tam2 = (der - medio);
    float *arr_izq, *arr_der;
    int *ind_izq, *ind_der;
    //creamos subarrays
    arr_izq = (float*)malloc(tam1 * sizeof(float));
    arr_der = (float*)malloc(tam2 * sizeof(float));
    ind_izq = (int*)malloc(tam1 * sizeof(int));
    ind_der = (int*)malloc(tam2 * sizeof(int));
    //se copian los datos
    for (i = 0; i < tam1; i++)
    {
        arr_izq[i] = *(array + izq + i);
        ind_izq[i] = *(indices + izq + i);
    }
    for (j = 0; j < tam2; j++)
    {
        arr_der[j] = *(array + medio + j + 1);
        ind_der[j] = *(indices + medio + j + 1);
    }
    i = 0;
    j = 0;
    //Fusion de datos
    for (k = izq; k < der + 1; k++)
    {
        if (i == tam1)
        {
            *(array + k) = *(arr_der + j);
            *(indices + k) = *(ind_der + j);
            j =  j+ 1;
        }
        else if(j == tam2)
        {
            *(array + k) = *(arr_izq + i);
            *(indices + k) = (*ind_izq + i);
            i = i + 1;
        }
        else
        {
            if (*(arr_izq + i) > *(arr_der + j))
            {
                *(array + k) = *(arr_izq+ i);
                *(indices + k) = *(ind_izq+ i);
                i = i + 1;
            }
            else
            {
                *(array + k) = *(arr_der + j);
                *(indices + k) = *(ind_der + j);
                j = j + 1;
            }
        }
    }
}
void merge_sort(float *array, int izq, int der, int * indices)
{
    if (izq < der)
    {
        int medio = (izq + der)/2;

        merge_sort(array, izq, medio,indices);
        merge_sort(array, medio + 1, der,indices);

        merge(array, izq, medio, der, indices);
    }
}
