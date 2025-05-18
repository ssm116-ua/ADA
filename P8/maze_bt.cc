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


using namespace std;
using namespace chrono;

#define NO_EXIST INT_MAX

enum Step { N, NE, E, SE, S, SW, W, NW };	

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
 * @brief Función que resuelve el laberinto de forma recursiva sin almacen.
 * 
 * @param pos_fila Fila actual.
 * @param pos_col Columna actual.
 * @param matrix Matriz del laberinto.
 * @return El número de pasos mínimos para llegar a al final del laberinto.
 */
int maze_naive(int pos_fila, int pos_col, vector<vector<int>> matrix)
{
    // Caso base: fuera de los límites o celda bloqueada
    if(pos_fila == 0 && pos_col == 0 && matrix[pos_fila][pos_col] == 1)
    {
        return 1;
    }
    if (pos_fila < 0 || pos_col < 0 || matrix[pos_fila][pos_col] == 0)
    {
        return NO_EXIST;
    }

    // Caso base: celda inicial
    if (pos_fila == 0 && pos_col == 0)
    {
        return 1;
    }

    // Llamadas recursivas a las tres direcciones posibles
    int diagonal = maze_naive(pos_fila - 1, pos_col - 1, matrix);
    int arriba = maze_naive(pos_fila - 1, pos_col, matrix);
    int izquierda = maze_naive(pos_fila, pos_col - 1, matrix);

    // Si todos los caminos son inválidos, devolver NO_EXIST
    if (diagonal == NO_EXIST && arriba == NO_EXIST && izquierda == NO_EXIST)
    {
        return NO_EXIST;
    }

    // Retornar el mínimo de los caminos válidos
    return 1 + min(diagonal, arriba, izquierda);
}

/**
 * 
 */
int maze_bt(int rows, int columns, int pos_row, int pos_column, int &prev_row, int &prev_column, int &bestLength, 
    vector<vector<int>> matrix, vector<vector<int>> &storage, vector<vector<int>> &visitedPaths, 
    long long int &visitedNodes, long long int &exploratedNodes, long long int &leafNodes, 
    long long int &discardedNodes, long long int &discardedPromisingNodes)
{
    map<Step, tuple<int, int>> steps_inc;
    steps_inc[N] = make_tuple(-1, 0);
    steps_inc[NE] = make_tuple(-1, 1);
    steps_inc[E] = make_tuple(0, 1);
    steps_inc[SE] = make_tuple(1, 1);
    steps_inc[S] = make_tuple(1, 0);
    steps_inc[SW] = make_tuple(1, -1);
    steps_inc[W] = make_tuple(0, -1);
    steps_inc[NW] = make_tuple(-1, -1);

    visitedNodes++;

    // Caso base: fuera de los límites o celda bloqueada
    if (pos_row < 0 || pos_row >= rows || pos_column < 0 || pos_column >= columns ||
        matrix[pos_row][pos_column] == 0)
    {
        discardedNodes++;
        leafNodes++;
        return NO_EXIST;
    }
    // Caso base: celda final
    if (pos_row == rows - 1 && pos_column == columns - 1)
    {
        leafNodes++;
        storage[pos_row][pos_column] = 1; // Marcar el destino en el camino más corto
        return 1;
    }


    // Caso Recursivo: explorar las 8 direcciones posibles
    int localBestLength = NO_EXIST;
    for (auto it = steps_inc.begin(); it != steps_inc.end(); ++it)
    {
        int incx, incy;
        tie(incx, incy) = it->second;
        int newRow = pos_row + incx;
        int newCol = pos_column + incy;

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < columns)
        {
            // Solo explorar si la nueva casilla no tiene un valor menor ya almacenado
            if (matrix[newRow][newCol] == 1 && 
                (visitedPaths[newRow][newCol] == 0 || visitedPaths[newRow][newCol] > visitedPaths[pos_row][pos_column] + 1))
            {
                exploratedNodes++;
        
                // Actualizar el valor de la nueva casilla en visitedPaths
                if(visitedPaths[pos_row][pos_column] != NO_EXIST)
                {
                    visitedPaths[newRow][newCol] = visitedPaths[pos_row][pos_column] + 1;
                }
                
                int result = maze_bt(rows, columns, newRow, newCol, prev_row, prev_column, 
                    bestLength, matrix, storage, visitedPaths, visitedNodes, exploratedNodes, 
                    leafNodes, discardedNodes, discardedPromisingNodes);
        
                // Restaurar el valor de visitedPaths si el camino no es válido
                if (result == NO_EXIST)
                {
                    visitedPaths[newRow][newCol] = 0; // Restaurar para permitir otras rutas
                    discardedPromisingNodes++; // Se intentó un camino que no fue válido
                }
                else
                {
                    // Solo considerar resultados válidos (diferentes de NO_EXIST)
                    if (result + 1 < localBestLength)
                    {
                        localBestLength = result + 1;
                    }
                }
            }
        }
    }

    // Si no se encontró un camino mejor, marcar la celda como no parte del camino óptimo
    if (localBestLength < bestLength)
    {
        storage[pos_row][pos_column] = 1; // Solo marcar si es parte del camino más corto
    }
    else
    {
        storage[pos_row][pos_column] = 0; // Asegurarse de no marcar caminos no óptimos
    }

    return localBestLength == NO_EXIST ? NO_EXIST : localBestLength;
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
    int bestLength = NO_EXIST, pos_row = 0, pos_column = 0, prev_row = 0, prev_column = 0;
    long long int visitedNodes = 0, exploratedNodes = 0, leafNodes = 0, discardedNodes = 0, discardedPromisingNodes = 0;
    vector<vector<int>> matrix(rows, vector<int>(columns, 0));
    getMatrix(fileName, rows, columns, matrix);

    vector<vector<int>> matrix_storage(rows, vector<int>(columns, 0));
    vector<vector<int>> matrix_paths(rows, vector<int>(columns, NO_EXIST));
    
 

    auto start = steady_clock::now();

    bestLength = maze_naive(rows-1, columns-1, matrix);
    

    int valor = maze_bt(rows, columns, pos_row, pos_column, prev_row, prev_column,
        bestLength, matrix, matrix_storage, matrix_paths, visitedNodes, 
        exploratedNodes, leafNodes, discardedNodes, discardedPromisingNodes);
    
    auto end = steady_clock::now();

    time = duration_cast<milliseconds>(end - start).count();

    if(valor == NO_EXIST)
    {
        valor = 0;
    }

    cout<< valor <<endl;
    cout << visitedNodes << " " << exploratedNodes << " " << leafNodes << " " << discardedNodes << " " << discardedPromisingNodes << endl;
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