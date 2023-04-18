#include <iostream>
#include <complex>
#include "src/fft.cpp"
#include "src/convolution.cpp"
#include <chrono>
#include <fstream>
#include <string>
using namespace std;
using namespace std::chrono;


template<class T>
void print_vec(vector<T> arr){
    for(auto i = arr.begin(); i < arr.end(); i++)
    {
        cout << *i << " ";
    }
    cout << endl;
}
vector<complex<double>> split_complex(string line)
{
    vector<complex<double>> result;
    string word = "";
    for (auto x : line)
    {
        if (x == ' ')
        {
            result.push_back(stod(word));
            word = "";
        }
        else {
            word = word + x;
        }
    }
    result.push_back(stod(word));
    return result;
}

vector<vector<complex<double>>> load_arrays()
{
    std::ifstream myfile; 
    myfile.open("data.txt");
    std::string mystring;
    vector<vector<complex<double>>> result;
    if ( myfile.is_open() ) 
        {     
            while ( myfile.good() ) {
            std::getline (myfile, mystring);
            result.push_back(split_complex(mystring));
        }          
    }
    return result;
}

double diff(vector<complex<double>> left_arr, vector<complex<double>> right_arr)
{
    double result = 0;

    int N = left_arr.size() > right_arr.size()? right_arr.size(): left_arr.size(); 

    for(int i = 0; i < N; i++)
    {
        double absolut = abs(left_arr[i] - right_arr[i]);
        result += absolut * absolut;
    }
    return sqrt(result / N);
}

void test(int k_start, int k_stop)
{
    cout << "fft execution with start = "<< k_start <<  " and finish = " << k_stop << ": "; 
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

void test_dft(int k_start, int k_stop)
{
    cout << "dft execution with start = "<< k_start <<  " and finish = " << k_stop << ": "; 
    for(int k = k_start; k < k_stop; k++)
    {
        vector<complex<double>> arr(1 << k, 1.0); 
        auto start = high_resolution_clock::now();
        arr = dft(arr);
        auto stop = high_resolution_clock::now();
        cout << duration_cast<microseconds>(stop - start).count() << ", ";
    } 
    cout << endl;
}

void reverse_test()
{
    vector<complex<double>> arr {1, 2, 3, 4, 5, 6, 7, 8};
    auto x = fft(fft(arr), true);
    cout << "diff with original vector and both transforms(fft): " << diff(arr, x) << endl;
    x = fft(fft(arr, true));
    cout << "diff with original vector and both transforms in reversed order(fft): " << diff(arr, x) << endl;

    x = dft(dft(arr), true);
    cout << "diff with original vector and both transforms(dft): " << diff(arr, x) << endl;
    x = dft(dft(arr, true));
    cout << "diff with original vector and both transforms in reversed order(dft): " << diff(arr, x) << endl;
}

void conv_test(int k_start, int k_stop, bool fixed)
{
    vector<complex<double>> arr2(100, 1.0);
    for(int k = k_start; k < k_stop; k++)
    {
        vector<complex<double>> arr1(1 << k, 1.0);
        if(!fixed)
        {
            arr2.resize(1 << k, 1.0);
        }
        
        auto start = high_resolution_clock::now();
        arr1 = convolution(arr1, arr2);
        auto stop = high_resolution_clock::now();
        cout << duration_cast<microseconds>(stop - start).count() << ", ";
    } 
    cout << endl;
}

void fft_test(int k_start, int k_stop, bool fixed)
{
    vector<complex<double>> arr2(100, 1.0);
    for(int k = k_start; k < k_stop; k++)
    {
        vector<complex<double>> arr1(1 << k, 1.0);
        if(!fixed)
        {
            arr2.resize(1 << k, 1.0);
        }
        
        auto start = high_resolution_clock::now();
        arr1 = fft_convolution(arr1, arr2);
        auto stop = high_resolution_clock::now();
        cout << duration_cast<microseconds>(stop - start).count() << ", ";
    } 
    cout << endl;
}



int main()
{
    auto y = load_arrays();
    auto x = convolution(y[0], y[1]);
    //test(1, 14);
    //est_dft(1, 14);
    //reverse_test();
    //cout << "dff and fft comparing: " << diff(dft(y[1]), fft(y[1])) << endl;
    //conv_test(2, 20, true);
    //fft_test(2, 20, true);


    cout << "convolutions comparing diff: " << diff(convolution(y[0], y[1]), fft_convolution(y[0], y[1])) << endl;
    
}

