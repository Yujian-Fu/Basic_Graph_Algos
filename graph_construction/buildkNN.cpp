#include <iostream>
#include <fstream>
#include <list>
#include <bits/stdc++.h>

#include "buildKNN.hpp"

using namespace std;

int main()
{
    string str = "/home/yujian/Downloads/similarity_search_datasets/ANN_SIFT10K/siftsmall_base.fvecs";
    char* filename = const_cast<char*>(str.c_str());
    float* data;
    unsigned num_points;
    unsigned dim;
    read_fvecs(filename, data, num_points, dim);
}
