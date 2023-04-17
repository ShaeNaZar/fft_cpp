#include <complex>
#include <vector>
#include <iostream>
#include <cmath>

#define MAX(a, b) (a > b ? a: b)
#define MIN(a, b) (a > b ? b: a)

using namespace std;

vector<complex<double>> convolution(vector<complex<double>> left_arr, vector<complex<double>> right_arr)
{
    int len_l = left_arr.size();
    int len_r = right_arr.size();
    int size = len_l + len_r - 1;
    vector<complex<double>> result (size, 0.0);
    for(int i = 0; i < size; i++)
    {
        for(int j = MAX(0, i - len_r); j <= MIN(i, len_l); j++)
        {
            result[i] += left_arr[j] * right_arr[i - j];
        }
    }
    return result;
}

