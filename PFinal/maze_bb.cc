// Salvador Pardo Saiz 51253936T
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <tuple>
#include <limits.h>
#include <iomanip>
#include <chrono>
#include <queue>
#include <cmath> 


using namespace std;
using namespace chrono;

#define NO_EXIST INT_MAX

enum Step { NE, E, S, SE, SW, W, NW, N };	

/**
 * @brief Estructura que representa un nodo en el árbol de búsqueda.
 * 
 * Contiene la posición del nodo, el costo acumulado, la cota optimista,
 * el camino recorrido y la matriz de celdas visitadas.
 */
struct Nodo 
{
    int row, col;
    int cost;
    int bound;
    vector<int> path;

    // Constructor para inicializar los atributos
    Nodo(int r, int c, int co, int b, vector<int> p)
        : row(r), col(c), cost(co), bound(b), path(move(p)) {}

    bool operator>(const Nodo &other) const 
    {
        return bound > other.bound;
    }
};

/**
 * @brief Función que comprueba los parámetros de entrada.
 * 
 * @param argc Número de argumentos.
 * @param argv Arreglo de argumentos.
 * @param paramF Parámetro -f.
 * @param paramP2D Parámetro --p2D.
 * @param fileName Nombre del archivo.
 * @return true Si los parámetros son válidos.
 * @return false Si los parámetros son incorrectos.
 */
bool checkParameters(int argc, char *argv[], bool &paramF, bool &paramP2D, bool &paramP, string &fileName)
{
    for (int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i],"-f") == 0)
        {
            paramF = true;
            i++;
            if (i >= argc)
            {
                cerr << "ERROR: missing filename." << endl;
                return false;
            }
            fileName = argv[i];
        }
        else if(strcmp(argv[i],"--p2D") == 0)
        {
            paramP2D = true;
        }
        else if(strcmp(argv[i],"-p") == 0)
        {
            paramP = true;
        }
        else
        {
            cerr << "ERROR: unknown option " <<argv[i] <<endl;
            return false;
        }
    }
    if(!paramF)
    {
        cerr << "ERROR: parameter -f is missing" <<endl;
        return false;
    }
    return true;
}

/**
 * @brief Función que lee el archivo y obtiene el número de rows y columns.
 * 
 * @param fileName Nombre del archivo.
 * @param rows Número de filas.
 * @param columns Número de columnas.
 * @return true Si se pudo leer el archivo correctamente.
 * @return false Si hubo un error al abrir el archivo.
 */
bool parameterF(string fileName, int &rows, int &columns)
{
    bool result = true;
    ifstream file;
    file.open(fileName,ios::in);
    if(file.is_open())
    {
        string line;
        getline(file, line);
        stringstream ss(line);
        ss >> rows >> columns;
        
    }
    else
    {
        cerr<< "ERROR: can't open file: " << fileName <<endl;
        result = false;
    }
    file.close();
    return result;
}

string parameterP(int rows, int columns, vector<vector<int>> matrix_storage)
{
    string path = "<";

    // Mapear las direcciones a sus valores numéricos
    map<pair<int, int>, int> direction_map = {
        {{-1, 0}, 1},  // N
        {{-1, 1}, 2},  // NE
        {{0, 1}, 3},   // E
        {{1, 1}, 4},   // SE
        {{1, 0}, 5},   // S
        {{1, -1}, 6},  // SW
        {{0, -1}, 7},  // W
        {{-1, -1}, 8}  // NW
    };

    // Encontrar la celda inicial del camino (primer '1' en la matriz)
    pair<int, int> current = {-1, -1};
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (matrix_storage[i][j] == 1)
            {
                current = {i, j};
                break;
            }
        }
        if (current.first != -1) break;
    }

    // Si no se encuentra un camino, devolver "<>"
    if (current.first == -1)
    {
        return "<>";
    }

    // Crear una copia de la matriz para marcar las celdas visitadas
    vector<vector<int>> visited(rows, vector<int>(columns, 0));
    visited[current.first][current.second] = 1;

    // Seguir el camino mínimo desde la celda inicial
    while (true)
    {
        bool found_next = false;

        // Buscar la siguiente celda en el camino
        for (auto it = direction_map.begin(); it != direction_map.end(); ++it)
        {
            pair<int, int> direction = it->first;
            int value = it->second;

            int next_row = current.first + direction.first;
            int next_col = current.second + direction.second;

            if (next_row >= 0 && next_row < rows && next_col >= 0 && next_col < columns &&
                matrix_storage[next_row][next_col] == 1 && visited[next_row][next_col] == 0)
            {
                path += to_string(value); // Agregar la dirección al camino
                current = {next_row, next_col};
                visited[next_row][next_col] = 1; // Marcar como visitada
                found_next = true;
                break;
            }
        }

        // Si no se encuentra una siguiente celda, terminar el recorrido
        if (!found_next)
        {
            break;
        }
    }

    path.append(">");
    return path;
}

/**
 * @brief Función que devuelve el mínimo de tres enteros.
 * 
 * @param a Primer entero.
 * @param b Segundo entero.
 * @param c Tercer entero.
 * @return El mínimo de los tres enteros.
 */
int min(int a, int b, int c)
{
    if(a < b)
    {
        if(a < c)
        {
            return a;
        }
        else
        {
            return c;
        }
    }
    else
    {
        if(b < c)
        {
            return b;
        }
        else
        {
            return c;
        }
    }
}

/**
 * @brief Función que imprime la matriz.
 * 
 * @param rows Número de filas.
 * @param columns Número de columnas.
 * @param matrix Matriz a printMatrix.
 */
void printMatrixString(int rows, int columns, vector<vector<string>> matrix)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            cout << matrix[i][j];
        }
        cout << endl;
    }
    return;
}

/**
 * @brief Función que imprime el laberinto con el camino más corto teniendo el caracter '*'.
 * 
 * @param rows Número de filas.
 * @param columns Número de columnas.
 * @param matrix Matriz del laberinto.
 * @param storage Matriz de almacenamiento.
 */
void maze_parser(int rows, int columns, vector<vector<int>> matrix, vector<vector<int>> storage)
{
        if(rows == 1 && columns == 1 && matrix[0][0] == 1)
        {
            cout<< "*"<<endl;
            return;
        }
        else if (rows == 1 && columns == 1 && matrix[0][0] == 0)
        {
            cout<< "0"<<endl;
            return;
        }

        vector<vector<string>> printMatrix(rows, vector<string>(columns, ""));

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                if (matrix[i][j] == 0)
                {
                    printMatrix[i][j] = "0";
                }
                else
                {
                    printMatrix[i][j] = "1";
                }
            }
        }

        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < columns; j++)
            {
                if(storage[i][j] != 0)
                {
                    printMatrix[i][j] = "*";
                }
            }
        }

        printMatrixString(rows, columns, printMatrix);
}

/**
 * @brief Función que lee el archivo y obtiene la matriz.
 * 
 * @param fileName Nombre del archivo.
 * @param rows Número de rows.
 * @param columns Número de columns.
 * @param matrix Matriz a llenar.
 */
void getMatrix(string fileName, int rows, int columns, vector<vector<int>>&matrix)
{
    ifstream file;
    file.open(fileName,ios::in);
    if(file.is_open())
    {
        string line;
        getline(file, line);

        for (int i = 0; i < rows; i++)
        {
            getline(file, line);
            stringstream ss(line); // Crear un stringstream para procesar la línea
            for (int j = 0; j < columns; j++)
            {
                ss >> matrix[i][j]; // Extraer cada valor entero
            }
        }
    }
    file.close();
}

/**
* @brief Función que carga una fila de la matriz en un array.
* 
* @param fila Fila a cargar.
* @param columnas Número de columnas.
* @param matrix Matriz a cargar.
* @param row Array donde se cargará la fila.
*/
void loadRow(int fila, int columnas, vector<vector<int>> &matrix, int row[])
{
   for (int j = 0; j < columnas ; j++)
   {
       row[j] = matrix[fila][j];
   }
}

/**
* @brief Función que carga una fila de un array en la matriz.
* 
* @param fila Fila a cargar.
* @param columnas Número de columnas.
* @param row Array a cargar.
* @param matrixIT Matriz donde se cargará la fila.
*/
void loadInMatrix(int fila, int columnas, int row[], vector<vector<int>> &matrixIT)
{
   for (int j = 0; j < columnas ; j++)
   {
       matrixIT[fila][j] = row[j];
   }
}

/**
* @brief Función que intercambia dos filas de un array.
* 
* @param columnas Número de columnas.
* @param filaAnterior Fila anterior.
* @param filaActual Fila actual.
*/
void swap(int columnas, int filaAnterior[], int filaActual[])
{
  for(int i = 0; i < columnas; i++)
  {
      filaAnterior[i] = filaActual[i];
  }
  return;
}

/**
* @brief Función que resuelve el laberinto de forma iterativa con 
* complejidad espacial mejorada.
* 
* @param filas Número de filas.
* @param columnas Número de columnas.
* @param matrix Matriz del laberinto.
* @param matrixIT_almacen Matriz de almacenamiento para la iterativa.
* @return El número de pasos mínimos para llegar a al final del laberinto.
*/
int maze_it_vector(int filas, int columnas, vector<vector<int>> matrix, vector<vector<int>> &matrixIT_almacen)
{
   int mayor = 0;

   if (matrix[0][0] == 0)
   {
       matrixIT_almacen[0][0] = NO_EXIST;
       return 0;
   }


   int filaAnterior[columnas] = {-1};
   int filaActual[columnas] = {-1};

   loadRow(0, columnas, matrix, filaActual);

   for(int i = 0; i < filas; i++)
   {
       for (int j = 0; j < columnas; j++)
       {
           if(i == 0)
           {
               if(matrix[i][0] == 1)
               {
                   filaActual[0] = 1;
               }

               if(matrix[i][j]== 0)
               {
                   filaActual[j] = NO_EXIST;
               }
               else
               {
                   if(j > 0)
                   {
                       if(filaActual[j - 1] == NO_EXIST)
                       {
                           filaActual[j] = NO_EXIST;
                       }
                       else
                       {
                           filaActual[j] = filaActual[j - 1] + 1;
                       }
                   }
               }
           }
           else
           {
               if(j == 0)
               {
                   if(filaAnterior[j] == NO_EXIST)
                   {
                       filaActual[j] = NO_EXIST;
                   }
                   else
                   {
                       filaActual[j] = filaAnterior[j] + 1;
                   }
               }
               else
               {
                   if(matrix[i][j] == 0)
                   {
                       filaActual[j] = NO_EXIST;
                   }
                   else
                   {
                       int minimo = 0;
                       minimo = min(filaAnterior[j - 1], filaAnterior[j], filaActual[j - 1]);

                       if(minimo != NO_EXIST)
                       {
                           filaActual[j] = minimo + 1;
                       }
                       else
                       {
                           filaActual[j] = NO_EXIST;
                       }
                   }
               }
           }
       }
       
       swap(columnas, filaAnterior, filaActual);
       loadRow(i, columnas, matrix, filaActual);
       loadInMatrix(i, columnas, filaAnterior, matrixIT_almacen);
   }

   if(matrixIT_almacen[filas - 1][columnas - 1] == NO_EXIST)
   {
       return 0;
   }
   else
   {
       mayor = matrixIT_almacen[filas - 1][columnas - 1];
   }

   return mayor;
}

int cotaOptimista(int rows, int columns, int pos_row, int pos_column, int pasos)
{
    int num = 0, filas = (rows - 1) - pos_row, columnas = (columns - 1) - pos_column;

    if(filas > columnas)
    {
        num = filas + pasos;
    }
    else
    {
        num = columnas + pasos;
    }
    
    return num;
}

int maze_bb(int rows, int columns, int &bestLength,
    const vector<vector<int>> &matrix, vector<vector<int>> &storage,
    vector<vector<int>> &paths, long long int &visitedNodos,
    long long int &exploratedNodos, long long int &leafNodos,
    long long int &discardedNodos, long long int &discardedPromisingNodos,
    long long int &promisingButDiscardedNodos, long long int &bestSolutionFromLeaf,
    long long int &bestSolutionFromLeafPessimistic)
{
    if (matrix[0][0] == 0 || matrix[rows - 1][columns - 1] == 0)
    {
        return NO_EXIST;
    }

    map<Step, tuple<int, int>> steps_inc = 
    {
        {N, {-1, 0}}, 
        {NE, {-1, 1}}, 
        {E, {0, 1}}, 
        {SE, {1, 1}},
        {S, {1, 0}}, 
        {SW, {1, -1}}, 
        {W, {0, -1}}, 
        {NW, {-1, -1}}
    };

    //priority_queue<Nodo, vector<Nodo>, greater<Nodo>> pq;

        
    auto cmp = [](const Nodo &a, const Nodo &b) 
    {
        if (a.bound != b.bound) 
        {
            return a.bound > b.bound; // Ordenar por cota optimista
        }
        return a.cost > b.cost; // Ordenar por menor costo acumulado primero
    };

    priority_queue<Nodo, vector<Nodo>, decltype(cmp)> pq(cmp);

    // Inicializa la matriz de pasos mínimos
    paths.assign(rows, vector<int>(columns, NO_EXIST));
    paths[0][0] = 1; // Marca el inicio del camino con un costo de 1

    pq.emplace(0, 0, 1, cotaOptimista(rows, columns, 0, 0, 1), vector<int>());
    exploratedNodos++;

    vector<int> bestPath;

    while (!pq.empty()) 
    {
        Nodo current = pq.top(); pq.pop();
        visitedNodos++;

        if (current.row == rows - 1 && current.col == columns - 1) 
        {
            bestSolutionFromLeaf++;
            bestSolutionFromLeafPessimistic++;
            leafNodos++;

            if (current.cost < bestLength) 
            {
                bestLength = current.cost;
                bestPath = current.path;

                // Actualiza la matriz storage para reflejar el camino más corto
                for (int i = 0; i < rows; i++) 
                {
                    for (int j = 0; j < columns; j++) 
                    {
                        storage[i][j] = 0; // Reinicia la matriz
                    }
                }

                int r = 0, c = 0; // Comienza desde la celda inicial
                storage[r][c] = 1; // Marca el inicio del camino

                for (int step : bestPath) 
                {
                    auto delta = steps_inc[static_cast<Step>(step - 1)];
                    r += get<0>(delta);
                    c += get<1>(delta);
                    storage[r][c] = 1; // Marca las celdas del camino
                }
            }
            continue;
        }

        for (auto it = steps_inc.begin(); it != steps_inc.end(); ++it)
        {
            Step dir = it->first;
            tuple<int, int> delta = it->second;

            int dr = get<0>(delta), dc = get<1>(delta);
            int nr = current.row + dr, nc = current.col + dc;

            if (nr >= 0 && nr < rows && nc >= 0 && nc < columns &&
                matrix[nr][nc] == 1)
            {
                int newCost = current.cost + 1;

                if (newCost < paths[nr][nc]) 
                {
                    paths[nr][nc] = newCost;

                    Nodo next = current;
                    next.row = nr;
                    next.col = nc;
                    next.cost = newCost;
                    next.bound = cotaOptimista(rows, columns, nr, nc, next.cost);
                    next.path.push_back(dir + 1);

                    // Si el nodo prometedor es descartado por superar la cota optimista
                    if (next.bound >= bestLength) 
                    {
                        promisingButDiscardedNodos++; // Incrementa el contador de nodos prometedores descartados
                        discardedPromisingNodos++;   // Incrementa el contador de nodos no prometedores
                        continue; // Pasa a la siguiente iteración
                    }

                    pq.emplace(next.row, next.col, next.cost, next.bound, next.path);
                    exploratedNodos++;
                }
                else 
                {
                    discardedNodos++; // Incrementa el contador de nodos no factibles
                    continue; // Pasa a la siguiente iteración
                }
            }
        }
    }

    return bestLength == NO_EXIST ? 0 : bestLength;
}

/**
 * @brief Función principal.
 * 
 * @param argc Número de argumentos.
 * @param argv Arreglo de argumentos.
 * @return int Código de salida.
 */
int main(int argc, char *argv[])
{
    bool paramF = false, paramP2D = false, paramP = false;
    string fileName = "";

    if(!checkParameters(argc, argv, paramF, paramP2D, paramP,fileName))
    {
        return 1;
    }

    int rows, columns;
    if(paramF)
    {
        if(!parameterF(fileName, rows, columns))
        {
            return 1;
        }
    }

    double time = 0;
    int bestLength = NO_EXIST;
    long long int visitedNodos = 0, exploratedNodos = 0, leafNodos = 0, discardedNodos = 0, 
    discardedPromisingNodos = 0, promisingButDiscardedNodos, bestSolutionFromLeaf = 0, 
    bestSolutionFromLeafPessimistic = 0;
    vector<vector<int>> matrix(rows, vector<int>(columns, 0));
    getMatrix(fileName, rows, columns, matrix);

    vector<int> path;
    vector<vector<int>> matrix_storage(rows, vector<int>(columns, 0));
    vector<vector<int>> matrix_paths(rows, vector<int>(columns, NO_EXIST));
    if(matrix[0][0] == 1)
    {
        matrix_storage[0][0] = 1; // Marca el inicio del camino
    }
    
 

    auto start = steady_clock::now();
    vector<vector<int>> matrizIT_almacen(rows, vector<int>(columns, 0)); 
    bestLength = maze_it_vector(rows, columns, matrix, matrizIT_almacen);
    if(bestLength == 0)
    {
        bestLength = NO_EXIST;
    }

    int valor = maze_bb(rows, columns, bestLength, matrix,
        matrix_storage, matrix_paths, visitedNodos, 
        exploratedNodos, leafNodos, discardedNodos, discardedPromisingNodos, 
        promisingButDiscardedNodos, bestSolutionFromLeaf, bestSolutionFromLeafPessimistic);
    
    auto end = steady_clock::now();

    time = duration_cast<milliseconds>(end - start).count();

    if(valor == NO_EXIST)
    {
        valor = 0;
    }

    cout<< valor <<endl;

    cout << visitedNodos << " " << exploratedNodos << " " << leafNodos << " " 
    << discardedNodos << " " << discardedPromisingNodos << " " 
    << promisingButDiscardedNodos << " " << bestSolutionFromLeaf << " " 
    << bestSolutionFromLeafPessimistic << endl;    

    cout << fixed << setprecision(3) << time/1000 <<endl;
         

    if(paramP2D)
    {
        if((matrix[0][0] == 0 && rows == 1 && columns == 1) || valor == 0)
        {
            cout<<"0"<<endl;
        }
        else
        {
            maze_parser(rows, columns, matrix, matrix_storage);
        }
    }

    if(paramP)
    {
        if((matrix[0][0] == 0 && rows == 1 && columns == 1) || valor == 0)
        {
            cout<<"<0>"<<endl;
        }
        else
        {
            cout << parameterP(rows, columns, matrix_storage);
        }
    }
    
    return 0;
}