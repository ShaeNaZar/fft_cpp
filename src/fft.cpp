#include <complex>
#include <vector>
#include <iostream>
#include <cmath>

#define _USE_MATH_DEFINES
#define DEG(x) (1 << x)

using namespace std;

int decompose(int N);
int inversion(int base, int ind);
complex<double> get_omega(int n);
complex<double> get_reversed_omega(int n);

vector<complex<double>> fft(vector<complex<double>> arr, bool reverse=false)
{
    int n = decompose(arr.size());
    int N = 1 << n;
    arr.resize(N, (complex<double>)(0.0, 0.0));
    vector<complex<double>> x (N);
    complex<double> omega;


    for(int k = 1; k <= n; k++)
    {   
        if(reverse)
        {
            omega = get_reversed_omega(n + 1 - k);
        }
        else{
            omega = get_omega(n + 1 - k);
        }
        complex<double> cur_omega (1.0, 0.0);
        for(int l = 0; l < DEG(n - k); l++)
        {   
            if(l!=0){
                cur_omega = cur_omega * omega;
            }
            for(int j = 0; j < DEG(k - 1); j++)
            {
                int ltl_idx = j * DEG(n + 1 - k) + l;
                int big_idx = ltl_idx + DEG(n - k);
                
                auto temp = arr[ltl_idx] + arr[big_idx];
                
                arr[big_idx] = (arr[ltl_idx] - arr[big_idx]) * cur_omega;
                arr[ltl_idx] = temp;
            }
        }

        double Nsqrt = sqrt(N);
        for(int i = 0; i < N; i++)
        {
            x[i] = arr[inversion(n, i)] / Nsqrt;
        }
    }
    return x;
}

int decompose(int N)
{
    N -= 1;
    int counter = 1;
    while(N >> counter > 0)
    {
        counter++;
    }
    return counter;
}

int inversion(int base, int ind)
{
    int result = 0;
    for(int i = 0; i < base; i++)
    {
        result = (result << 1) + (ind % 2);
        ind = ind >> 1; 
    }
    return result;
}

complex<double> get_omega(int n)
{
    double arg = -2 * M_PI / (1 << n);
    return complex<double>(cos(arg), sin(arg));
}

complex<double> get_reversed_omega(int n)
{
    double arg = 2 * M_PI / (1 << n);
    return complex<double>(cos(arg), sin(arg));
}

vector<complex<double>> fft_convolution(vector<complex<double>> left_arr, vector<complex<double>> right_arr)
{
    int len_l = left_arr.size();
    int len_r = right_arr.size();
    int size = (2 << decompose(len_l > len_r? len_l: len_r));
    left_arr.resize(size, 0);
    right_arr.resize(size, 0);

    left_arr = fft(left_arr);
    right_arr = fft(right_arr);

    double sqrtSize = sqrt(size);

    for(int i = 0; i < size; i++)
    {
        left_arr[i] = left_arr[i] * right_arr[i] * sqrtSize;
    }

    left_arr = fft(left_arr, true);
    left_arr.resize(len_l + len_r - 1);
    return left_arr;
}