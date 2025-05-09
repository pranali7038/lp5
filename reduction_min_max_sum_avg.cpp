#include <iostream>
#include <omp.h>
#include <climits>
using namespace std;

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    int arr[n];
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    int sum = 0, maxVal = INT_MIN, minVal = INT_MAX;

    #pragma omp parallel for reduction(+:sum) reduction(max:maxVal) reduction(min:minVal)
    for (int i = 0; i < n; i++) {
        sum += arr[i];

        if (arr[i] > maxVal)
            maxVal = arr[i];

        if (arr[i] < minVal)
            minVal = arr[i];
    }

    float avg = (float)sum / n;

    cout << "Sum     = " << sum << endl;
    cout << "Average = " << avg << endl;
    cout << "Maximum = " << maxVal << endl;
    cout << "Minimum = " << minVal << endl;

    return 0;
}


// Time complexity = O(n/p + log p)