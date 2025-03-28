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

int maize_naive(int pos_fila, int pos_col, vector<vector<int>> matrix)
{
    // Casos base 
    if(pos_fila < 0 || pos_col < 0|| matrix[pos_fila][pos_col] == 0)
    {
        return NO_EXIST;
    }
    else if(matrix[pos_fila][pos_col] == 1 && pos_fila == 0 && pos_col == 0)
    {
        return 1;
    }
    else
    {
        return 1 + min(maize_naive(pos_fila-1, pos_col-1, matrix), maize_naive(pos_fila-1, pos_col, matrix), maize_naive(pos_fila, pos_col-1, matrix));
    }
}

int maize_memo(int pos_fila, int pos_col, vector<vector<int>> matrix, vector<vector<int>> &sol)
{
    if(pos_fila < 0 || pos_col < 0|| matrix[pos_fila][pos_col] == 0)
    {
        return NO_EXIST;
    }

    if(sol[pos_fila][pos_col] != -1)
    {
        return sol[pos_fila][pos_col];
    }

    if (pos_fila == 0 && pos_col == 0)
    {
        return sol[pos_fila][pos_col] = 1;
    }

    return sol[pos_fila][pos_col] = 1 + min(maize_memo(pos_fila-1, pos_col-1, matrix, sol), maize_memo(pos_fila-1, pos_col, matrix, sol), maize_memo(pos_fila, pos_col-1, matrix, sol));
;

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
        cout<< maize_naive(filas-1, columnas-1, matriz)<< " ";
    }
    else
    {
        cout<< "- ";
    }

    vector<vector<int>> sol(filas, vector<int>(columnas, 0));
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            sol[i][j] = -1;
        }
    }
    
        
    cout<< maize_memo(filas-1, columnas-1, matriz, sol)<< " ";
       
        cout<< "? ";
        cout<< "? " <<endl;
    
    if(paramP2D)
    {
        int i = filas - 1;
        int j = columnas - 1;
        vector<vector<char>> imprimir(filas, vector<char>(columnas, 0));

        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                if (matriz[i][j] == 0)
                {
                    imprimir[i][j] = '0';
                }
                else
                {
                    imprimir[i][j] = '1';
                }
            }
        }

        while (i > 0 || j > 0)
        {
            imprimir[i][j] = '*'; // Marcar el camino más corto con '*'

            // Determinar la dirección del siguiente paso
            if (i > 0 && j > 0 && sol[i][j] == sol[i - 1][j - 1] + 1)
            {
                i--; // Diagonal arriba-izquierda
                j--;
            }
            else if (i > 0 && sol[i][j] == sol[i - 1][j] + 1)
            {
                i--; // Arriba
            }
            else if (j > 0 && sol[i][j] == sol[i][j - 1] + 1)
            {
                j--; // Izquierda
            }
        }
    imprimir[0][0] = '*'; // Marcar la posición inicial


        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                cout << imprimir[i][j] << " ";
            }
            cout << endl;
        }
        
    }
    return 0;
}
