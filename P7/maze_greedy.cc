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
    if(path != 0)
    {
        matrixGreedy[rows-1][columns-1] = path;
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

    int rows, columns;
    if(paramF)
    {
        if(!parameterF(fileName, rows, columns))
        {
            return 1;
        }
    }

    vector<vector<int>> matrix(rows, vector<int>(columns, 0));
    getMatrix(fileName, rows, columns, matrix);

    vector<vector<int>> matrizGreedy_storage(rows, vector<int>(columns, 0)); 
    int valor = maze_greedy(rows, columns, matrix, matrizGreedy_storage);
    cout<< valor <<endl;
         

    if(paramP2D)
    {
        if(matrix[0][0] == 0 && rows == 1 && columns == 1)
        {
            cout<<"0"<<endl;
        }
        else
        {
            maze_parser(rows, columns, matrix, matrizGreedy_storage);
        }
    }

    return 0;
}