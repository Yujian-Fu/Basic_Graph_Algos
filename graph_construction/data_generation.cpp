#include<iostream>
#include<fstream>
#include <cstdlib>
#include <time.h>
#include <iomanip>
#include <cmath>


#define total_point 1000
using namespace std;

int main()
{
    cout << "the rand max is" << RAND_MAX <<endl;
    srand((unsigned) time(NULL));
    float pos;
    ofstream ofile;
    ofile.open("data.txt", ios::out);
    for (int i= 0; i < total_point; i++)
    {
        pos = 10*(rand()/(double)RAND_MAX);
        ofile << setprecision(2) << pos << " ";
        if ((i+1) % 4 == 0)
            ofile << endl;
    }
}