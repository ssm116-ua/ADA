#include <iostream>

using namespace std;

// 
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

unsigned long pow2_2(unsigned n)
{
    unsigned long result = 1;
    if(n == 0)
    {
        return 1;
    }
    else
    {
        
        for (int i = 0; i < n; i++) {  // Controla la potencia (multiplicaciones)
            int temp = 0;
            
            // Simula la multiplicación por 2 usando sumas
            for (int j = 0; j < result; j++) {
                temp += 2;
            }

            result = temp;  // Actualizamos el resultado
        }
    }
    return result;
}

//unsigned long pow2_3(unsigned n)
//{
    
//}

int main()
{
    cout<< pow2_1(3) << endl
    << pow2_2(3) << endl;
}