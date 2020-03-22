#include <iostream>
#include <fstream>
#include <list>
#include <bits/stdc++.h>
#include <time.h>
#include "transition_reduction.hpp"
#include "buildKNN.hpp"
#include "search.hpp"
using namespace std;

int main()
{
    //test the get_proportion_dataset
    //string str = "/home/yujian/Downloads/similarity_search_datasets/ANN_SIFT10K/siftsmall_base.fvecs";
    string str = "/home/y/yujianfu/similarity_search/datasets/ANN_SIFT1M/sift_base.fvecs";
    char* filename = const_cast<char*>(str.c_str());
    clock_t start,finish;
    
    start = clock();
    merge_kNN(filename);
    finish = clock();
    cout << "Graph construciton takes " << (double)(finish - start) / CLOCKS_PER_SEC << " second." << endl;

    start = clock();
    transitive_reduction(filename);
    finish = clock();
    cout << "Transitive Reduction takes " << (double)(finish - start) / CLOCKS_PER_SEC << " second." << endl;
    
    start = clock();
    search(filename);
    finish = clock();
    cout << "Transitive Reduction takes " << (double)(finish - start) / CLOCKS_PER_SEC << " second." << endl;

}
