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

void ordenar_monticulo(std::vector<std::pair<float,int>>& celvalues4){
    //creamos el monticulo 'inverso'con la funcion make_heap y el parametro greater
    std::make_heap(celvalues4.begin(), celvalues4.end(), std::greater<std::pair<float,int>>());
    //se ordena en orden decreciente
    std::sort_heap(celvalues4.begin(), celvalues4.end(), std::greater<std::pair<float,int>>());
 
}

void cajanegra(){
  //aqui se realizaran las pruebas con diferentes inputs
  float p02[] = {}; int i02[] = {};
  float p1 = 1;  int i1 = 1; 
  float p2[] = {0,0,0,0,0,0,0}; int i2[] = {0,1,2,3,4,5,6};
  float p3[] = {-1,2,3,4,5,6,7,8}; int i3[] = {0,1,2,3,4,5,6,7}; 
  float p4[] = {8,7,6,5,4,3,2,1}; int i4[] = {0,1,2,3,4,5,6,7}; 
  
  float p12 = 1;  int i12 = 1; 
  float p22[] = {0,0,0,0,0,0,0}; int i22[] = {0,1,2,3,4,5,6};
  float p32[] = {-1,2,3,4,5,6,7,8}; int i32[] = {0,1,2,3,4,5,6,7}; 
  float p42[] = {8,7,6,5,4,3,2,1}; int i42[] = {0,1,2,3,4,5,6,7}; 

  std::vector<std::pair<float,int>> p14 = {{1,1}};
  std::vector<std::pair<float,int>> p24;
  for(int i=0; i<7; i++){
      p24.emplace_back(p2[i],i);
  }
  std::vector<std::pair<float,int>> p34;
  for(int i=0; i<8; i++){
      p34.emplace_back(p3[i],i);
  }
  std::vector<std::pair<float,int>> p44;
  for(int i=0; i<8; i++){
      p44.emplace_back(p4[i],i);
  }

  //por fusion
  merge_sort(&p1,0,0,&i1); //un único elemento
  printf("%f\n",p1);
  merge_sort(p2,0,6,i2); //elementos repetidos
  for (int i = 0 ; i <*(&p2 + 1) - p2; i++) 
    std::cout << p2[i] << '\t' << i2[i] << std::endl;
  printf("\n");
  merge_sort(p3,0,7,i3); //ordenado ascendentemente
  for (int i = 0 ; i <*(&p3 + 1) - p3; i++) 
    std::cout << p3[i] << '\t' << i3[i] << std::endl;
  printf("\n");
  merge_sort(p4,0,7,i4); //ordenado descendentemente
  for (int i = 0 ; i <*(&p4 + 1) - p4; i++) 
    std::cout << p4[i] << '\t' << i4[i] << std::endl;
  printf("\n\n");

  //rapida
  ordenacion_rapida(&p12,0,0,&i12); //un único elemento
  printf("%f\n",p12);
  printf("\n");
  ordenacion_rapida(p22,0,6,i22); //elementos repetidos
  for (int i = 0 ; i <*(&p22 + 1) - p22; i++) 
    std::cout << p22[i] << '\t' << i22[i] << std::endl;
  printf("\n");
  ordenacion_rapida(p32,0,7,i32); //ordenado ascendentemente
  for (int i = 0 ; i <*(&p32 + 1) - p32; i++) 
    std::cout << p32[i] << '\t' << i32[i] << std::endl;
  printf("\n");
  ordenacion_rapida(p42,0,7,i42); //ordenado descendentemente
  for (int i = 0 ; i <*(&p42 + 1) - p42; i++) 
    std::cout << p42[i] << '\t' << i42[i] << std::endl;
  printf("\n");

  //monticulo
  ordenar_monticulo(p14); //un único elemento
  printf("%f %i\n",p14[0].first, p14[0].second);
  ordenar_monticulo(p24);  //elementos repetidos
  for (int i = 0 ; i <7; i++) 
    std::cout << p24[i].first << '\t' << p24[i].second << std::endl;
  printf("\n");
  ordenar_monticulo(p34); //ordenado ascendentemente
  for (int i = 0 ; i <8; i++) 
    std::cout << p34[i].first << '\t' << p34[i].second << std::endl;
  printf("\n");
  ordenar_monticulo(p44); //ordenado descendentemente
  for (int i = 0 ; i <8; i++) 
    std::cout << p44[i].first << '\t' << p44[i].second << std::endl;
  printf("\n\n");

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

    //estructura matriz N² x 2 ... se cambió a array NxN guarda celvalues más otro array NxN guarda posiciones. 
    //operaciones como 'swap' deben hacerse a los dos arrays
    int N = nCellsWidth*nCellsHeight;
    
    //restrucutrando el codigo
    int *posiciones = new int [N];
    float *celvalues  = new float [N];
    for(int i=0; i<N;i++){
        posiciones[i]= i;
        celvalues[i] = defaultCellValue(i/nCellsWidth,i%nCellsWidth, freeCells
        , nCellsWidth, nCellsHeight ,mapWidth, mapHeight, obstacles, defenses);// i/ncellswidth=row y i%ncellsheight=col
    }

    //cajanegra();

    /* 
    ***********************
    1. SIN PRE-ORDENACION
    ***********************
    */

    //se crea copia de celvalues
    float *celvalues1  = new float [N];
    for(int i=0; i<N;i++){
        celvalues1[i] = celvalues[i];
    }
    cronometro t;
    t.activar();

    List<Defense*>::iterator currentDefense = defenses.begin();
    float posx, posy; int row, col, i = 0;
    while(currentDefense != defenses.end()) {
      //maximo valor del vector
      int max = 0;
      for(int i = 0; i<N; i++){
        if(celvalues1[i] > celvalues1[max]){
          max = i;
        }
      }
      row = (int)posiciones[max]/nCellsHeight;
      col = (int)posiciones[max]%nCellsWidth;
      posx= col * cellWidth + cellWidth * 0.5f;
      posy= row * cellHeight + cellHeight * 0.5f;
      celvalues1[max] = -1; //invalidamos valor para proximas iteraciones - pop en listas.
      if(factibilidad(posx, posy, mapWidth,mapHeight,currentDefense,defenses,obstacles)){
          (*currentDefense)->position.x = posx;
          (*currentDefense)->position.y = posy;
          (*currentDefense)->position.z = 0;
          ++currentDefense;

        }
    }//FIN 1
    t.parar();


    /* 
    ***********************
    2. ORDENACION POR FUSION
    ***********************
    */
    float *celvalues2  = new float [N];
    for(int i=0; i<N;i++){
        posiciones[i]= i; //reseteamos las posiciones
        celvalues2[i] = celvalues[i];
    }
    cronometro t2;
    t2.activar();
    merge_sort(celvalues2,0,N,posiciones);

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
    t2.parar();
    
    /* 
    ***********************
    3. ORDENACION RAPIDA
    ***********************
    */
    float *celvalues3  = new float [N];
    for(int i=0; i<N;i++){
        posiciones[i]= i; //reseteamos las posiciones
        celvalues3[i] = celvalues[i];
    }
    cronometro t3;
    t3.activar();
    ordenacion_rapida(celvalues3,0,N,posiciones);

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
    t3.parar();


    /* 
    ***********************
    4. ORDENACION POR MONTICULOS
    ***********************
    */
    //para facilitar el guardado de posiciones usamos un vector de pares
    std::vector<std::pair<float,int>> celvalues4;
    for(int i=0; i<N; i++){
      celvalues4.emplace_back(celvalues[i],i);
    }
    cronometro t4;
    t4.activar();

    ordenar_monticulo(celvalues4);

    currentDefense = defenses.begin();
    i = 0;
    while(currentDefense != defenses.end()) {
      row = (int)celvalues4[i].second/nCellsHeight;
      col = (int)celvalues4[i].second%nCellsWidth;
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
    t4.parar();
    


long int r = 1;
  
   std::cout << (N) << '\t' << t.tiempo()/ r << '\t' << t2.tiempo() / r << '\t' << t3.tiempo() / r << '\t' << t4.tiempo() / r << std::endl;
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
