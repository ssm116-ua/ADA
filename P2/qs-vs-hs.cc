// Salvador Pardo Saiz 51253936-T
/*
ADA. 2024-25
Practice 2: "Empirical analysis by means of program-steps account of two sorting algorithms: Middle-Quicksort and Heapsort."
*/

#include <unistd.h>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <chrono>
#include <cstring>
#include <algorithm>

using namespace std;

const int sizeAverage = 30; // Número de iteraciones para calcular la media de los pasos
const int million = 1e6; // Valor que se utiliza para convertir los pasos en millones

//--------------------------------------------------------------
// copyArray:
// Copia el contenido de un array en otro.
// Parámetros:
// - v: array origen
// - w: array destino
// - n: tamaño de los arrays
//--------------------------------------------------------------
void copyArray(int *v, int *w, size_t n) 
{
    memcpy(w, v, n * sizeof(int));
}

//--------------------------------------------------------------
// printTable:
// Imprime el inicio de la tabla de resultados.
//--------------------------------------------------------------
void printTable()
{
        cout << "# QUICKSORT VERSUS HEAPSORT."
        << endl
        << "# Average processing Msteps (millions of program steps)"
        << endl
        << "# Number of samples (arrays of integer)"
        << endl
        << "#" <<setw(11) << "RANDOM ARRAYS" <<setw(11) <<"SORTED ARRAYS"<<setw(11) <<"REVERSE SORTED ARRAYS" 
        << endl
        << "#" << setw(11) <<"-------------------"<< setw(11) <<"-------------------" << setw(11) <<"-----------------------" << endl
        << "#" << setw(7) << "Size" 
        << setw(13) << "QuickSort" << setw(11) << "HeapSort" 
        << setw(11) << "QuickSort" << setw(11) << "HeapSort" 
        << setw(11) << "QuickSort" << setw(11) << "HeapSort" << endl
        << "#===================================================================================="<< endl;
}

//--------------------------------------------------------------
// stepsAverage:
// Calcula la media de los pasos de un array.
// Parámetros:
// - array: array que almacena los pasos de cada iteración.
// Devuelve:
// - Media de los pasos.
//--------------------------------------------------------------
double stepsAverage (long long array[])
{
    long long sum = 0;
    for (int i = 0; i < sizeAverage; i++)
    {
        sum += array[i];
    }
    return sum / sizeAverage;
}

//--------------------------------------------------------------
// stepsInMillions:
// Convierte los pasos en millones.
// Parámetros:
// - steps: pasos a convertir.
// Devuelve:
// - Pasos en millones.
//--------------------------------------------------------------
double stepsInMillions (long long steps)
{
    double stepsDouble = steps / 1e6;
    return stepsDouble;
}

//--------------------------------------------------------------
// Middle-Quicksort:
// The algorithm selects the middle element of the array as the "pivot".
// In a process called "partitioning", it rearranges the elements so that
// all elements smaller than the pivot are placed to its left, and
// all elements greater than the pivot are placed to its right.
// The process is then repeated recursively on the two resulting
// subarrays (left and right of the pivot).
//--------------------------------------------------------------

void middle_QuickSort(int *v, long left, long right, long long &steps) {
    long i, j;
    int pivot;
    if (left < right) {
        i = left; j = right;
        pivot = v[(i + j) / 2];
        // pivot based partitioning:
        do {
            while (v[i] < pivot)
            {
                i++;
                steps++;
            }

            while (v[j] > pivot) 
            {
                j--;
                steps++;
            }

            if (i <= j) {
                swap(v[i], v[j]);
                i++; j--;
            }
            steps++;
        } while (i <= j);
        // Repeat for each non-empty subarray:
        if (left < j) middle_QuickSort(v, left, j, steps);
        if (i < right) middle_QuickSort(v, i, right, steps);
    }
}

//--------------------------------------------------------------
// Heapsort:
// The algorithm works by repeatedly selecting the largest remaining element
// and placing it at the end of the vector in its correct position.
//
// To efficiently select the largest element, it builds a max-heap.
//
// The sink procedure is used for heap construction (or reconstruction).
//--------------------------------------------------------------

void sink(int *v, size_t n, size_t i, long long &steps)
// Sink an element (indexed by i) in a tree to maintain the heap property.
// n is the size of the heap.
{
    size_t largest;
    size_t l, r; // indices of left and right childs

    do {
        largest = i;  // Initialize largest as root
        l = 2 * i + 1;  // left = 2*i + 1
        r = 2 * i + 2;  // right = 2*i + 2

        // If the left child exists and is larger than the root
        if (l < n && v[l] > v[largest])
        {
            largest = l;
        }

        // If the right child exists and is larger than the largest so far
        if (r < n && v[r] > v[largest])
         {
            largest = r;
         } 

        // If the largest is still the root, the process is done
        if (largest == i)
        {
            break;
        } 

        // Otherwise, swap the new largest with the current node i and repeat the process with the children
        swap(v[i], v[largest]);
        steps++;
        i = largest;
    } while (true);
}

//--------------------------------------------------------------
// Heapsort algorithm (ascending sorting)
void heapSort(int *v, size_t n, long long &steps)
{
    // Build a max-heap with the input array ("heapify"):
    // Starting from the last non-leaf node (right to left), sink each element to construct the heap.
    for (size_t i = n / 2 - 1; true; i--) {
        sink(v, n, i, steps);
        
        if (i == 0) break; // As size_t is an unsigned type
        steps++;
    }

    // At this point, we have a max-heap. Now, sort the array:
    // Repeatedly swap the root (largest element) with the last element and rebuild the heap.
    for (size_t i = n - 1; i > 0; i--) {
        // Move the root (largest element) to the end by swapping it with the last element.
        swap(v[0], v[i]);
        // Rebuild the heap by sinking the new root element.
        // Note that the heap size is reduced by one in each iteration (so the element moved to the end stays there)
        sink(v, i, 0, steps);
        steps++;
        // The process ends when the heap has only one element, which is the smallest and remains at the beginning of the array.
    }
}

//--------------------------------------------------------------
// quickSortLoop:
// Realiza un bucle para calcular la media de los pasos de QuickSort.
// Parámetros:
// - vRandom: array de enteros aleatorios.
// - vRandomCopy: copia del array de enteros aleatorios.
// - vStepsAverageQuick: array que almacena los pasos de cada iteración.
// - size: tamaño de los arrays.
// - stepsQuickSort: pasos de QuickSort.
//--------------------------------------------------------------
void quickSortLoop(int *vRandom, int *vRandomCopy, long long *vStepsAverageQuick, size_t size,long long &stepsQuickSort)
{
    for (int i = 0; i < sizeAverage; i++)
    {
        stepsQuickSort = 0;
        copyArray(vRandom, vRandomCopy, size);
        middle_QuickSort(vRandomCopy,0 ,size-1, stepsQuickSort);
        vStepsAverageQuick[i] = stepsQuickSort;
    }
}

//--------------------------------------------------------------
// heapSortLoop:
// Realiza un bucle para calcular la media de los pasos de HeapSort.
// Parámetros:
// - vRandom: array de enteros aleatorios.
// - vRandomCopy: copia del array de enteros aleatorios.
// - vStepsAverageHeap: array que almacena los pasos de cada iteración.
// - size: tamaño de los arrays.
// - stepsHeapSort: pasos de HeapSort.
//--------------------------------------------------------------
void heapSortLoop(int *vRandom, int *vRandomCopy, long long *vStepsAverageHeap, size_t size,long long &stepsHeapSort)
{
    for (int i = 0; i < sizeAverage; i++)
    {
        stepsHeapSort= 0;
        copyArray(vRandom, vRandomCopy, size);
        heapSort(vRandomCopy,size, stepsHeapSort);
        vStepsAverageHeap[i] = stepsHeapSort;
    }
}

int main ()
{
    srand(0); // Semilla para la generación de números aleatorios.
    printTable();
    for(int exp = 15; exp <= 20; exp++) // Bucle para recorrer los tamaños de los arrays.
    {
        // Declaración de variables.
        long long stepsQuickSort = 0, stepsHeapSort = 0;
        size_t size = size_t( pow(2,exp) );
        long long* vStepsAverageQuick = new long long [sizeAverage];
        long long* vStepsAverageHeap = new long long [sizeAverage];
        int* vRandom = new int [size];
        int* vRandomCopy = new int [size];
        int* vSorted = new int [size];
        int* vSortedCopy = new int [size];
        int* vReverse = new int [size];
        int* vReverseCopy = new int [size];


        if (!vRandom || !vSorted || !vReverse) // Si no se ha podido reservar memoria, se muestra un mensaje de error.
        {
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        cout << setw(9) <<size<< std::flush; // Imprime el tamaño del array para cada iteración.

        for (size_t j = 0; j < size; j++) // Bucle para rellenar los arrays con números aleatorios.
        {
            vRandom[j] = rand();
        } 

        // Se copian los arrays de enteros aleatorios, ordenados y ordenados al revés.
        copyArray(vRandom, vSorted, size);
        sort(vSorted, vSorted+size);
        copyArray(vSorted, vSortedCopy, size);
        copyArray(vSorted, vReverse, size);
        reverse(vReverse, vReverse+size);
        copyArray(vReverse, vReverseCopy, size);

       
        quickSortLoop(vRandom, vRandomCopy, vStepsAverageQuick, size, stepsQuickSort);
        cout<< fixed << setprecision(3) << setw(11) <<stepsInMillions(stepsAverage(vStepsAverageQuick))<<std::flush;
        
        heapSortLoop(vRandom, vRandomCopy, vStepsAverageHeap, size, stepsHeapSort);
        cout<< fixed << setprecision(3) << setw(11) <<stepsInMillions(stepsAverage(vStepsAverageHeap)) <<std::flush;

        stepsQuickSort = 0;
        middle_QuickSort(vSorted,0 ,size-1, stepsQuickSort);
        cout<< fixed << setprecision(3) << setw(11) <<stepsInMillions(stepsQuickSort) << std::flush;

        stepsHeapSort = 0;
        heapSort(vSortedCopy,size, stepsHeapSort);
        cout<< fixed << setprecision(3) << setw(11) <<stepsInMillions(stepsHeapSort) << std::flush;

        stepsQuickSort = 0;
        middle_QuickSort(vReverseCopy,0 ,size-1, stepsQuickSort);
        cout<< fixed << setprecision(3) << setw(11) <<stepsInMillions(stepsQuickSort) << std::flush;

        stepsHeapSort = 0;
        heapSort(vReverse,size, stepsHeapSort);
        cout<< fixed << setprecision(3) << setw(11) <<stepsInMillions(stepsHeapSort)<< endl << std::flush;

        // Liberación de memoria.
        delete[] vStepsAverageQuick;
        delete[] vStepsAverageHeap;
        delete[] vRandom;
        delete[] vRandomCopy;
        delete[] vSorted;
        delete[] vReverse;
    }
    

    

    return 0;
}
 