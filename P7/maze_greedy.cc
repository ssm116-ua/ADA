// Salvador Pardo Saiz 51253936T
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include <limits.h>

using namespace std;

#define NO_EXIST INT_MAX

/**
 * @brief Función que imprime la matriz.
 * 
 * @param filas Número de filas.
 * @param columnas Número de columnas.
 * @param matrix Matriz a imprimir.
 */
void printMatrixString(int filas, int columnas, vector<vector<string>> matrix)
{
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            cout << matrix[i][j];
        }
        cout << endl;
    }
    return;
}

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
bool checkParameters(int argc, char *argv[], bool &paramF, bool &paramP2D, string &fileName)
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
        else
        {
            cerr << "ERROR: unknown option " <<argv[i] <<endl;
            return false;
        }
    }
    if(!paramF)
    {
        cerr << "Falta el parámetro -f" <<endl;
        return false;
    }
    return true;
}

/**
 * @brief Función que lee el archivo y obtiene el número de filas y columnas.
 * 
 * @param fileName Nombre del archivo.
 * @param filas Número de filas.
 * @param columnas Número de columnas.
 * @return true Si se pudo leer el archivo correctamente.
 * @return false Si hubo un error al abrir el archivo.
 */
bool parameterF(string fileName, int &filas, int &columnas)
{
    bool result = true;
    ifstream file;
    file.open(fileName,ios::in);
    if(file.is_open())
    {
        string line;
        getline(file, line);
        stringstream ss(line);
        ss >> filas >> columnas;
        
    }
    else
    {
        cerr<< "ERROR: can't open file: " << fileName <<endl;
        result = false;
    }
    file.close();
    return result;
}

/**
 * @brief Función que imprime el laberinto con el camino más corto teniendo el caracter '*'.
 * 
 * @param filas Número de filas.
 * @param columnas Número de columnas.
 * @param matriz Matriz del laberinto.
 * @param almacen Matriz de almacenamiento.
 */
void maze_parser(int filas, int columnas, vector<vector<int>> matriz, vector<vector<int>> almacen)
{
        if(filas == 1 && columnas == 1 && matriz[0][0] == 1)
        {
            cout<< "*"<<endl;
            return;
        }
        else if (filas == 1 && columnas == 1 && matriz[0][0] == 0)
        {
            cout<< "0"<<endl;
            return;
        }

        vector<vector<string>> imprimir(filas, vector<string>(columnas, ""));

        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                if (matriz[i][j] == 0)
                {
                    imprimir[i][j] = "0";
                }
                else
                {
                    imprimir[i][j] = "1";
                }
            }
        }

        for(int i = 0; i < filas; i++)
        {
            for(int j = 0; j < columnas; j++)
            {
                if(almacen[i][j] != 0)
                {
                    imprimir[i][j] = "*";
                }
            }
        }

        printMatrixString(filas, columnas, imprimir);
}

/**
 * @brief Función que lee el archivo y obtiene la matriz.
 * 
 * @param fileName Nombre del archivo.
 * @param filas Número de filas.
 * @param columnas Número de columnas.
 * @param matrix Matriz a llenar.
 */
void getMatrix(string fileName, int filas, int columnas, vector<vector<int>>&matrix)
{
    ifstream file;
    file.open(fileName,ios::in);
    if(file.is_open())
    {
        string line;
        getline(file, line);

        for (int i = 0; i < filas; i++)
        {
            getline(file, line);
            stringstream ss(line); // Crear un stringstream para procesar la línea
            for (int j = 0; j < columnas; j++)
            {
                ss >> matrix[i][j]; // Extraer cada valor entero
            }
        }
    }
    file.close();
}

int maze_greedy(int rows, int columns, vector<vector<int>> matrix, vector<vector<int>> &matrixGreedy)
{
    int path = 1, row = 0, column = 0;
    bool anyMovement = true;

    if(matrix[row][column] == 0)
    {
        matrixGreedy[row][column] = 0;
        return 0;
    }
    else
    {
        matrixGreedy[row][column] = path;
    }

    while (row != rows - 1 || column != columns - 1)
    {
        if(anyMovement)
        {
            matrixGreedy[row][column] = path;
            if(row + 1 < rows && column + 1 < columns && matrix[row + 1][column + 1] == 1)
            {
                path++;
                row++;
                column++;
            }
            else if(row + 1 < rows && matrix[row + 1][column] == 1)
            {
                path++;
                row++;
            }
            else if(column + 1 < columns && matrix[row][column + 1] == 1)
            {
                path++;
                column++;
            }
            else
            {
                anyMovement = false;
            }
        }
        else
        {
            path = 0;
            break;
        }
    }
    return path;
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
    bool paramF = false, paramP2D = false;
    string fileName = "";

    if(!checkParameters(argc, argv, paramF, paramP2D, fileName))
    {
        return 1;
    }

    int filas, columnas;
    if(paramF)
    {
        if(!parameterF(fileName, filas, columnas))
        {
            return 1;
        }
    }

    vector<vector<int>> matriz(filas, vector<int>(columnas, 0));
    getMatrix(fileName, filas, columnas, matriz);

    vector<vector<int>> matrizGreedy_almacen(filas, vector<int>(columnas, 0)); 
    int valor = maze_greedy(filas, columnas, matriz, matrizGreedy_almacen);
    cout<< valor <<endl;
        

    if(paramP2D)
    {
        if(matriz[0][0] == 0 && filas == 1 && columnas == 1)
        {
            cout<<"0"<<endl;
        }
        else
        {
            maze_parser(filas, columnas, matriz, matrizGreedy_almacen);
        }
    }

    return 0;
}