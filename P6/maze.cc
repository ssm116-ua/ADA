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
 * @brief Función que comprueba los parámetros de entrada.
 * 
 * @param argc Número de argumentos.
 * @param argv Arreglo de argumentos.
 * @param paramF Parámetro -f.
 * @param paramT Parámetro -t.
 * @param paramP2D Parámetro --p2D.
 * @param paramIgnore Parámetro --ignore-naive.
 * @param fileName Nombre del archivo.
 * @return true Si los parámetros son válidos.
 * @return false Si los parámetros son incorrectos.
 */
bool checkParameters(int argc, char *argv[], bool &paramF, bool &paramT, bool &paramP2D, bool &paramIgnore, string &fileName)
{
    for (int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i],"-f") == 0)
        {
            paramF = true;
            i++;
            if (i >= argc)
            {
                cerr << "ERROR: missing filename." <<endl;
                return false;
            }
            fileName = argv[i];
        }
        else if(strcmp(argv[i],"-t") == 0)
        {
            paramT = true;
        }
        else if(strcmp(argv[i],"--p2D") == 0)
        {
            paramP2D = true;
        }
        else if(strcmp(argv[i],"--ignore-naive") == 0)
        {
            paramIgnore = true;
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

        int i = filas - 1;
        int j = columnas - 1;
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

        while (i > 0 || j > 0)
        {
            imprimir[i][j] = '*'; // Marcar el camino más corto con '*'

            // Determinar la dirección del siguiente paso
            if (i > 0 && j > 0 && almacen[i][j] == almacen[i - 1][j - 1] + 1)
            {
                i--; // Diagonal arriba-izquierda
                j--;
            }
            else if (i > 0 && almacen[i][j] == almacen[i - 1][j] + 1)
            {
                i--; // Arriba
            }
            else if (j > 0 && almacen[i][j] == almacen[i][j - 1] + 1)
            {
                j--; // Izquierda
            }
        }
        imprimir[0][0] = "*"; // Marcar la posición inicial


        printMatrixString(filas, columnas, imprimir);
}

/**
 * @brief Función que imprime las tablas de memoización y la iterativa, donde las celdas que no
 * son no son camino almacenan 'X' y las celdas que no se acceden almacenan '-'.
 * 
 * @param filas Número de filas.
 * @param columnas Número de columnas.
 * @param almacen Matriz de almacenamiento.
 * @param matrizIT Matriz iterativa.
 */
void parameterT(int filas, int columnas, vector<vector<int>> almacen, vector<vector<int>> matrizIT)
{
   vector<vector<string>> imprimir(filas, vector<string>(columnas, "-"));
   
    cout<< "Memoization table:" << endl;
    if(filas == 1 && columnas == 1) 
    {
        cout<< " 1"<<endl;
    }       
    else
    {
        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                if (almacen[i][j] == -1)
                {
                    cout<< "- ";
                }
                else if (almacen[i][j] == 0)
                {
                    cout<< "0 "; // Celda bloqueada
                }
                else if (almacen[i][j] == NO_EXIST)
                {
                    cout<< "X "; // Celda no utilizada en el camino
                }
                else
                {
                    cout<< almacen[i][j] << " "; // Valor válido
                }
            }
            cout<< endl;
        }
    }
    
    cout << "Iterative table:" << endl;
    if(filas == 1 && columnas == 1) 
    {
        cout<< " 1"<<endl;
    }   
    else
    {
        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                if (matrizIT[i][j] == NO_EXIST)
                {
                    cout << "X "; // Celda no utilizada en el camino
                }
                else
                {
                    cout<< (matrizIT[i][j]) << " "; // Valor válido
                }
            }
            cout << endl;
        }
    }
return;
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
 * @brief Función que resuelve el laberinto de forma recursiva con memoización.
 * 
 * @param pos_fila Fila actual.
 * @param pos_col Columna actual.
 * @param matrix Matriz del laberinto.
 * @param sol Matriz de almacenamiento para la memoización.
 * @return El número de pasos mínimos para llegar a al final del laberinto.
 */
int maze_memo(int pos_fila, int pos_col, vector<vector<int>> &matrix, vector<vector<int>> &sol)
{
    // Caso base: fuera de los límites o celda bloqueada
    if (pos_fila < 0 || pos_col < 0)
    {
        return NO_EXIST;
    }

    // Caso base: celda inicial
    if ( matrix[pos_fila][pos_col] == 0)
    {
        sol[pos_fila][pos_col] = NO_EXIST;
        return NO_EXIST;
    }
    
    // Caso base: celda no accesible
    if (sol[pos_fila][pos_col] != -1)
    {
        return sol[pos_fila][pos_col];
    }

    // Caso base: celda inicial
    if (pos_fila == 0 && pos_col == 0)
    {
        return sol[pos_fila][pos_col] = 1;
    }

    int diagonal = NO_EXIST, arriba = NO_EXIST, izquierda = NO_EXIST;
    
    // Comprobamos que las direcciones sean validas y si lo son
    // calculamos el valor de la celda
    if (pos_fila > 0 && pos_col > 0) 
    {
        diagonal = maze_memo(pos_fila - 1, pos_col - 1, matrix, sol);
    }

    if (pos_fila > 0)
    {
        arriba = maze_memo(pos_fila - 1, pos_col, matrix, sol);
    }

    if (pos_col > 0)
    {
        izquierda = maze_memo(pos_fila, pos_col - 1, matrix, sol);
    }

    if (diagonal == NO_EXIST && arriba == NO_EXIST && izquierda == NO_EXIST)
    {
        return sol[pos_fila][pos_col] = NO_EXIST;
    }

    return sol[pos_fila][pos_col] = 1 + min(diagonal, arriba, izquierda);
}

/**
 * @brief Función que resuelve el laberinto de forma iterativa con matriz.
 * 
 * @param filas Número de filas.
 * @param columnas Número de columnas.
 * @param matrix Matriz del laberinto.
 * @param matrixIT Matriz de almacenamiento para la iterativa.
 * @return El número de pasos mínimos para llegar a al final del laberinto.
 */
int maze_it_matrix(int filas, int columnas, vector<vector<int>> &matrix, vector<vector<int>> &matrixIT)
{
    int mayor = 0;
    if (matrix[0][0] == 0)
    {
        matrixIT[0][0] = NO_EXIST;
        return 0;
    }

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            //Posición inicial
            if (i == 0 && j == 0) 
            {
                matrixIT[i][j] = 1;
            }
            else if (i == 0) // Primera fila
            {
                if (matrix[i][j] == 0)
                {
                    matrixIT[i][j] = NO_EXIST;
                }
                else
                {
                    if(matrixIT[i][j - 1] == NO_EXIST)
                    {
                        matrixIT[i][j] = NO_EXIST;
                    }
                    else
                    {
                        matrixIT[i][j] = matrixIT[i][j - 1] + 1;
                    }
                }
            }
            else if (j == 0) // Primera columna
            {
                if (matrix[i][j] == 0)
                {
                    matrixIT[i][j] = NO_EXIST;
                }
                else
                {
                    if(matrixIT[i - 1][j] == NO_EXIST)
                    {
                        matrixIT[i][j] = NO_EXIST;
                    }
                    else
                    {
                        matrixIT[i][j] = matrixIT[i - 1][j] + 1;
                    }
                }
            }
            else
            {
                if (matrix[i][j] == 0)
                {
                    matrixIT[i][j] = NO_EXIST;
                }
                else
                {
                    int minimo = 0;
                    minimo = min(matrixIT[i - 1][j - 1], matrixIT[i - 1][j], matrixIT[i][j - 1]);

                    if(minimo != NO_EXIST)
                    {
                        matrixIT[i][j] = minimo + 1;
                    }
                    else
                    {
                        matrixIT[i][j] = NO_EXIST;
                    }
                }
            }
        }
    }

    if(matrixIT[filas - 1][columnas - 1] == NO_EXIST)
    {
        return 0;
    }
    else
    {
        mayor = matrixIT[filas - 1][columnas - 1];
    }

    return mayor;
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

/**
 * @brief Función principal.
 * 
 * @param argc Número de argumentos.
 * @param argv Arreglo de argumentos.
 * @return int Código de salida.
 */
int main(int argc, char *argv[])
{
    bool paramF = false, paramT = false, paramP2D = false, paramIgnore = false;
    string fileName = "";

    if(!checkParameters(argc, argv, paramF, paramT, paramP2D, paramIgnore, fileName))
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


    if (!paramIgnore)
    {
        int valor = maze_naive(filas-1, columnas-1, matriz);
        if(valor >= NO_EXIST)
        {
            valor = 0;
        }
        cout<< valor<< " ";
    }
    else
    {
        cout<< "- ";
    }

    vector<vector<int>> almacen(filas, vector<int>(columnas, 0)); 
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            almacen[i][j] = -1;
        }
    }
    
    int valor = maze_memo(filas-1, columnas-1, matriz, almacen);
    if(valor == NO_EXIST)
    {
        valor = 0;
    }
    cout<< valor<< " ";   
    
    
    
    vector<vector<int>> matrizIT(filas, vector<int>(columnas, 0));    
    cout<< maze_it_matrix(filas, columnas, matriz, matrizIT) << " ";  
   
    vector<vector<int>> matrizIT_almacen(filas, vector<int>(columnas, 0)); 
    cout<< maze_it_vector(filas, columnas, matriz, matrizIT_almacen) <<endl;

    if(paramP2D)
    {
        if(valor != 0)
        {
            maze_parser(filas, columnas, matriz, almacen);
        }
        else
        {
            cout<<"0"<<endl;
        }
    }
    
    if(paramT)
    {   
        parameterT(filas, columnas, almacen, matrizIT);     
    }
    return 0;
    
}
