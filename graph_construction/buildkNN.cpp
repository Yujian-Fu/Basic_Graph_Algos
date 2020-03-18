#include <iostream>
#include <fstream>
#include <list>
#include <bits/stdc++.h>

#include "buildKNN.hpp"

using namespace std;

int main()
{

    //test the get_proportion_dataset
    cout << "-----------------------------------------------------" << endl;
    string str = "/home/yujian/Downloads/similarity_search_datasets/ANN_SIFT10K/siftsmall_base.fvecs";
    char* filename = const_cast<char*>(str.c_str());
    vector<point> subset;
    float proportion = 0.5;
    unsigned dim;
    unsigned dataset_size;
    float offset = 0;
    bool random = false;
    get_proportion_dataset(filename, subset, proportion, dim, dataset_size, offset, random);
    
    vector<point> means;

    /*
    float sum_dis = 0;
    for (int i = 0; i < subset.size(); i++)
    {
        sum_dis += getDistance(subset[0], subset[i]);
        cout << sum_dis << " ";
    }
    */

    /*kmeansplusplus(subset, means);

    cout << "now is the means in this kmeans_subset iteration" << endl;

    for (int i = 0; i < means.size(); i++)
    {
        for (int j = 0; j < means[i].dim; j++)
        {
            cout << means[i].data[j] << " ";
            
        }
        cout << endl;
        cout << endl;
    }

    kmeans_subset(subset, means);*/
    merge_kNN(filename);

    
    

    
    

    /*
    //test read_fvecs function
    cout << "-----------------------------------------------------" << endl;
    string str = "/home/yujian/Downloads/similarity_search_datasets/ANN_SIFT10K/siftsmall_base.fvecs";
    char* filename = const_cast<char*>(str.c_str());
    float* data;
    unsigned num_points;
    unsigned dim;
    read_fvecs(filename, data, num_points, dim);
    */
    
}
