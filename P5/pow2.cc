// 51253936T Salvador Pardo Saiz
#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

#define tamVec 15

void printTable()
{
        cout << "# Comparison Pow(2, n)."
        << endl
        << "# Average processing steps"
        << endl
        << "#" << " n Values" <<setw(20) << "Pow2_1" <<setw(20) <<"Pow2_2"<<setw(20) <<"Pow2_3" 
        << endl
        << "#================================================================================="<< endl;
}

void fillVector(unsigned int v[])
{
    unsigned int n = 6;
    for (unsigned int i = 0; i < tamVec; i++)
    {
        v[i] = n;
        n++;
    }
    return;
}

// coste: \ Theta (n) 
unsigned long pow2_1(unsigned n, unsigned long &steps)
{
    steps++;
    if( n == 0)
    {
        return 1;
    }
    else
    {
        return 2 * pow2_1(n-1, steps);
    }
}

// coste: \ Theta (n²) 
unsigned long pow2_2(unsigned n, unsigned long &steps)
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
            steps++;
            // Simula la multiplicación por 2 usando sumas
            for (unsigned long j = 0; j < result; j++) 
            {
                temp += 2;
                steps++;
            }

            result = temp;  // Actualizamos el resultado
        }
    }
    return result;
}

// coste: \ Theta (log n) 
unsigned long pow2_3(unsigned n, unsigned long &steps)
{
    unsigned long operation;
    steps++;

    if(n == 0)
    {
        return 1;
    }
    else
    {
        operation = pow2_3(n/2, steps);
        
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
    unsigned int size[tamVec];
    unsigned long steps1 = 0, steps2 = 0, steps3 = 0;

    fillVector(size);    

    printTable();

    for (unsigned int i = 0; i < tamVec; i++)
    {
        steps1 = 0;
        steps2 = 0;
        steps3 = 0;

        if(pow2_1(size[i],steps1) != pow(2, size[i]) || pow2_2(size[i],steps2) != pow(2, size[i]) || pow2_3(size[i],steps3) != pow(2, size[i]))
        {
            cout << "Falla algún módulo" <<endl;
            return 1;
        }

        cout << setw(5) << size[i]<< setw(20) << steps1 << setw(20) << steps2 << setw(20) << steps3 <<endl;
    }
    return 0;
}