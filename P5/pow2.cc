// 51253936T Salvador Pardo Saiz
#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

void printTable()
{
        cout << "# Comparison Pow(2, n)."
        << endl
        << "# Average processing Msteps (millions of program steps)"
        << endl
        << "#" << " Size" <<setw(15) << "Pow2_1(n)" <<setw(15) <<"Pow2_2(n)"<<setw(15) <<"Pow2_3(n)" 
        << endl
        << "#===================================================================================="<< endl;
}


// coste: \ Theta (n) 
unsigned long pow2_1(unsigned n)
{
    if( n == 0)
    {
        return 1;
    }
    else
    {
        return 2 * pow2_1(n-1);
    }
}

// coste: \ Theta (n²) 
unsigned long pow2_2(unsigned n)
{
    unsigned long result = 1;
    if(n == 0)
    {
        return 1;
    }
    else
    {
        
        for (unsigned i = 0; i < n; i++) {  // Controla la potencia (multiplicaciones)
            unsigned long temp = 0;
            
            // Simula la multiplicación por 2 usando sumas
            for (unsigned long j = 0; j < result; j++) {
                temp += 2;
            }

            result = temp;  // Actualizamos el resultado
        }
    }
    return result;
}

// coste: \ Theta (log n) 
unsigned long pow2_3(unsigned n)
{
    unsigned long operation;
    if(n == 0)
    {
        return 1;
    }
    else
    {
        operation = pow2_3(n/2);

        if(n%2 == 0) // Si es par
        {
            return operation*operation;
        }
        else
        {
            return operation*operation*2;
        }

    }
}

int main()
{
    if(pow2_1(4) == pow(2,4) && pow2_2(8) && pow(2,8) && pow2_3(16) == pow(2,16))
    {
        printTable();
    }
    else
    {
        cout << "Falla algún módulo" <<endl;
    }
}