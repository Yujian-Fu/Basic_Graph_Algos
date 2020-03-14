#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
#include <list>
#include <bits/stdc++.h>
#include "buildKNN.h"


#define dimension 128
#define kmeans_iteration_times 10

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
