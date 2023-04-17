#include <iostream>
#include <complex>
#include "src/fft.cpp"
#include <chrono>
using namespace std::chrono;


template<class T>
void print_vec(vector<T> arr){
    for(auto i = arr.begin(); i < arr.end(); i++)
    {
        cout << *i << " ";
    }
    cout << endl;
}

void test(int k_start, int k_stop)
{
    for(int k = k_start; k < k_stop; k++)
    {
        vector<complex<double>> arr(1 << k, 1.0); 
        auto start = high_resolution_clock::now();
        arr = fft(arr);
        auto stop = high_resolution_clock::now();
        cout << duration_cast<microseconds>(stop - start).count() << ", ";
    } 
    cout << endl;
}

using namespace std;
int main(){
    test(8, 23);
}

