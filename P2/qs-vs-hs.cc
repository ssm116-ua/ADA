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

const int sizeAverage = 30;

void copyArray(int *v, int *w, size_t n)
{
    memcpy(w, v, n * sizeof(int));
}

void printTable()
{
        cout << "# QUICKSORT VERSUS HEAPSORT."
        << endl
        << "# Average processing Msteps (millions of program steps)"
        << endl
        << "# Number of samples (arrays of integer)"
        << endl
        << "# \t    RANDOM ARRAYS \t SORTED ARRAYS \t\tREVERSE SORTED ARRAYS"
        << endl
        << "# \t    -------------------  -------------------  -----------------------"
        << endl
        << "#   Size    QuickSort  HeapSort  QuickSort  HeapSort  QuickSort  HeapSort"
        << endl
        << "#============================================================================"
        << endl;
}


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
        if (left < j) middle_QuickSort(v, left, j);
        if (i < right) middle_QuickSort(v, i, right);
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
            largest = l;

        // If the right child exists and is larger than the largest so far
        if (r < n && v[r] > v[largest])
            largest = r;

        // If the largest is still the root, the process is done
        if (largest == i)
        {
            steps++;
            break;
        } 

        // Otherwise, swap the new largest with the current node i and repeat the process with the children
        swap(v[i], v[largest]);
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
        steps++;
        if (i == 0) break; // As size_t is an unsigned type
    }

    // At this point, we have a max-heap. Now, sort the array:
    // Repeatedly swap the root (largest element) with the last element and rebuild the heap.
    for (size_t i = n - 1; i > 0; i--) {
        // Move the root (largest element) to the end by swapping it with the last element.
        swap(v[0], v[i]);
        // Rebuild the heap by sinking the new root element.
        // Note that the heap size is reduced by one in each iteration (so the element moved to the end stays there)
        sink(v, i, 0, steps);
        // The process ends when the heap has only one element, which is the smallest and remains at the beginning of the array.
    }
}

int main ()
{
    srand(0);
    printTable();
    for(int exp = 15; exp <= 20; exp++)
    {
        long long stepsQuickSort = 0, stepsHeapSort = 0;
        size_t size = size_t( pow(2,exp) );
        int* vRandom = new int [size];
        int* vSorted = new int [size];
        int* vReverse = new int [size];

        if (!vRandom || !vSorted || !vReverse)
        {
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        cout << size << "\t\t" << std::flush; 

        for (size_t j = 0; j < size; j++)
        {
            vRandom[j] = rand();
        } 

        copyArray(vRandom, vSorted, size);
        sort(vSorted, vSorted+size);
        copyArray(vRandom, vReverse, size);
        reverse(vReverse, vReverse+size);

        for (int i = 0; i < sizeAverage; i++)
        {
            
            middle_QuickSort(vRandom, 0, size-1, stepsQuickSort);
            
            
        }
    }
    

    

    return 0;
}
