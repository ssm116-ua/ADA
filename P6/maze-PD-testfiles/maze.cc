// Salvador Pardo Saiz 51253936T
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

#define NO_EXIST 999999

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

void maze_parser(int filas, int columnas, vector<vector<int>> matriz, vector<vector<int>> almacen)
{
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


        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                cout << imprimir[i][j];
            }
            cout << endl;
        }
}

void parameterT(int filas, int columnas, vector<vector<int>> sol)
{
    
   vector<vector<string>> imprimir(filas, vector<string>(columnas, "-"));
    
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            // Si la celda tiene un valor de -1, verificar las condiciones para marcar "X"
            if (sol[i][j] == -1)
            {
                imprimir[i][j] = "-";
            }
            else if (sol[i][j] == 0)
            {
                imprimir[i][j] = "0"; // Celda bloqueada
            }
            else if (sol[i][j] == NO_EXIST)
            {
                imprimir[i][j] = "X"; // Celda no utilizada en el camino
            }
            else
            {
                imprimir[i][j] = to_string(sol[i][j]); // Valor válido
            }
        }
    }
        
    cout << "Memoization table:" << endl;
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            cout << imprimir[i][j] << " ";
        }
        cout << endl;
    }
}

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

int maze_memo(int pos_fila, int pos_col, vector<vector<int>> &matrix, vector<vector<int>> &sol)
{
    if (pos_fila < 0 || pos_col < 0)
    {
        return NO_EXIST;
    }

    if ( matrix[pos_fila][pos_col] == 0)
    {
        sol[pos_fila][pos_col] = NO_EXIST;
        return NO_EXIST;
    }
    
    if (sol[pos_fila][pos_col] != -1)
    {
        return sol[pos_fila][pos_col];
    }
    if (pos_fila == 0 && pos_col == 0)
    {
        return sol[pos_fila][pos_col] = 1;
    }

    int diagonal = NO_EXIST, arriba = NO_EXIST, izquierda = NO_EXIST;

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

void maze_it_vector(int filas, int columnas, vector<vector<int>> &matrix, vector<vector<string>> &matrixIT)
{
    
    if (matrix[0][0] == 0)
    {
        matrixIT[0][0] = "X";
        return;
    }

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            if (i == 0 && j == 0)
            {
                matrixIT[i][j] = "1";
            }
            else if (i == 0)
            {
                if (matrix[i][j] == 0)
                {
                    matrixIT[i][j] = "X";
                }
                else
                {
                    matrixIT[i][j] = to_string(stoi(matrixIT[i][j - 1]) + 1);
                }
            }
            else if (j == 0)
            {
                if (matrix[i][j] == 0)
                {
                    matrixIT[i][j] = "X";
                }
                else
                {
                    matrixIT[i][j] = to_string(stoi(matrixIT[i - 1][j]) + 1);
                }
            }
            else
            {
                if (matrix[i][j] == 0)
                {
                    matrixIT[i][j] = "X";
                }
                else
                {
                    matrixIT[i][j] = to_string(min(stoi(matrixIT[i - 1][j - 1]), stoi(matrixIT[i - 1][j]), stoi(matrixIT[i][j - 1])) + 1);
                }
            }
        }
    }
    
}

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

    vector<vector<int>> sol(filas, vector<int>(columnas, 0)); // Matriz almacén, (cambia el nombre)
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            sol[i][j] = -1;
        }
    }
    
    int valor = maze_memo(filas-1, columnas-1, matriz, sol);
    if(valor >= NO_EXIST)
    {
        valor = 0;
    }
    cout<< valor<< " ";   
    
    char not_done = '?';

    cout<< not_done << " ";

    cout<< not_done <<endl;

    if(paramP2D)
    {
        if(valor != 0)
        {
            maze_parser(filas, columnas, matriz, sol);
        }
        else
        {
            cout<<"0"<<endl;
        }
    }
    
    if(paramT)
    {   
        if(filas == 1 && columnas == 1)
        {
            cout<<"Memoization table:"<<endl
            <<" 1"<<endl
            <<"Iterative table: "<<endl
            <<" 1"<<endl;
        }
        else
        {
            parameterT(filas, columnas, sol);
        }        
    }
    return 0;
    
}
