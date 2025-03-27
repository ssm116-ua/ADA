#include <iostream>
#include <fstream>

using namespace std;

bool checkParameters(int argc, char *argv[])
{
    bool paramF = false, paramT = false, paramP2D = false, paramIgnore = false;
    string fileName = "";

    for (int i = 1; i < argc; i++)
    {
        if(argv[i] == "-f")
        {
            paramF = true;
            i++;
            fileName = argv[i];
        }
        else if(argv[i] == "-t")
        {
            paramT = true;
        }
        else if(argv[i] == "--p2D")
        {
            paramP2D = true;
        }
        else if(argv[i] == "--ignore-naive")
        {
            paramIgnore = true;
        }
        else
        {
            cout << "Parámetros incorrectos" <<endl;
            return false;
        }
    }
    
    if(paramF)
    {
        ifstream file;
        file.open(fileName,ios::in);
        if(!file.is_open())
        {
            cout<< "El fichero no se ha encontrado o no puede abrirse." <<endl;
        }
        file.close();
    }

    return true;
}


void maize_naive()
{

}