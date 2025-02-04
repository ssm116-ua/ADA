/*
ADA 2024-25
Práctica 1: "Empirical analysis of Quicksort average-time complexity."
*/

//--------------------------------------------------------------
// Middle QuickSort

#include <unistd.h>
#include <iostream>
#include <math.h>
#include <chrono>

using namespace std;
using namespace chrono;

const int sizeAverage = 30;

double TimeAverage (double array[])
{
    double sum = 0;
    for (int i = 0; i < sizeAverage; i++)
    {
        sum += array[i];
    }
    return sum / sizeAverage;
}

void middle_QuickSort(int * v, long left, long right){

    long i,j;
    int pivot,aux; 
    if (left<right)
    {
        i=left; j=right;
        pivot=v[(i+j)/2];
        do
        {
            while (v[i]<pivot) i++;
            while (v[j]>pivot) j--;
            if (i<=j)
            {
                aux=v[i]; v[i]=v[j]; v[j]=aux;
                i++; j--;
            }
       } while (i<=j);
       if (left<j)  middle_QuickSort(v,left,j);
       if (i<right) middle_QuickSort(v,i,right);
    }
}

int main()
{
    srand(0);

    cout << "# QuickSort CPU-times in milliseconds:"
        << endl
        << "# Size \t Average CPU time (ms.)"
        << endl
        << "# ----------------------------"
        << endl;

    for (int exp = 15; exp < 20; exp++)
    {
        double* times = new double[sizeAverage];
        size_t size = size_t( pow(2,exp) );
        int* v = new int [size];
        if (!v)
        {
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        cout << size << "\t\t" << std::flush;


        for (size_t j = 0; j < size; j++)
        {
            v[j] = rand();
        } 
        
        for (int i = 0; i < sizeAverage; i++)
        {
            auto start = steady_clock::now();
            middle_QuickSort(v, 0, size-1);
            auto end = steady_clock::now();
            
            times[i] = duration_cast<milliseconds>(end-start).count();
        }
        
        
        
        cout<< TimeAverage(times) <<endl;
    }
    return 0;
}
