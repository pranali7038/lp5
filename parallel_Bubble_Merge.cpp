#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace chrono;

// Sequential Bubble Sort
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Parallel Bubble Sort
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped;
    #pragma omp parallel
    {
        for (int i = 0; i < n-1; i++) {
            swapped = false;
            #pragma omp for
            for (int j = 0; j < n-i-1; j++) {
                if (arr[j] > arr[j+1]) {
                    swap(arr[j], arr[j+1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }
}

// Merge Function for Merge Sort
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    
    while (i < n1) {
        arr[k++] = L[i++];
    }
    
    while (j < n2) {
        arr[k++] = R[j++];
    }
}

// Sequential Merge Sort
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid);
            
            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }
        
        merge(arr, left, mid, right);
    }
}

int main() {
    int n, choice;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    cout << "Choose Sorting Algorithm: \n";
    cout << "1. Bubble Sort\n";
    cout << "2. Merge Sort\n";
    cout << "Enter choice (1 or 2): ";
    cin >> choice;

    if (choice == 1) {
        // Bubble Sort
        vector<int> arrSeq = arr;
        auto start = high_resolution_clock::now();
        sequentialBubbleSort(arrSeq);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Sequential Bubble Sort Time: " << duration.count() << " microseconds" << endl;

        vector<int> arrPar = arr;
        start = high_resolution_clock::now();
        parallelBubbleSort(arrPar);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Parallel Bubble Sort Time: " << duration.count() << " microseconds" << endl;
    }
    else if (choice == 2) {
        // Merge Sort
        vector<int> arrSeq = arr;
        auto start = high_resolution_clock::now();
        sequentialMergeSort(arrSeq, 0, n-1);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Sequential Merge Sort Time: " << duration.count() << " microseconds" << endl;

        vector<int> arrPar = arr;
        start = high_resolution_clock::now();
        parallelMergeSort(arrPar, 0, n-1);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Parallel Merge Sort Time: " << duration.count() << " microseconds" << endl;
    }
    else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}


// Complexity:
// sequential bubble sort = O(n²)
// paralle buuble sort = O(n² / p)

// sequential merge sort = O(n lon n)
// paralle buuble sort = O(n log n/ p)
