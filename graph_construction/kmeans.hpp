#ifndef _kmeans_
#define _kmeans_
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <assert.h>
#include <chrono>
#include <unistd.h>

#include "struct_define.hpp"

#define kmeans_iteration_times 5
#define K 50
#define usekmeansplusplus true
#define kmeans_proportion 0.1

using namespace std;
using namespace std::chrono;

/*
Get proportion of the dataset with specific selecting strategy:
Random selection or Partitial selection
offset + proportion should be smaller than 1 
*/

void point_memory_collection(vector<point> dataset)
{
    for (int i = 0; i < dataset.size(); i++)
    {
        vector<float>().swap(dataset[i].data);
    }
    vector<point>().swap(dataset);
}


void get_proportion_dataset(char* filename, vector<point>& subset, float proportion, unsigned& dim, 
unsigned& dataset_size, float offset, bool random)
{
    auto start = high_resolution_clock::now(); 
    ifstream infile(filename, ios::binary);
    if(!infile.is_open())
    {
        cout << "file open error" << endl;
        exit(-1);
    }
    if (subset.size() != 0)
    {
        subset.clear();
    }
    //Get the number of nodes and the dimension
    infile.read((char*)&dim, 4);
    infile.seekg(0, ios::end);
    ios::pos_type ss = infile.tellg();
    size_t fsize = (size_t) ss;
    dataset_size = (unsigned) (fsize / (dim+1) / 4);
    unsigned proportion_data_size = (unsigned) (dataset_size * proportion);
    unsigned *full_index = new unsigned [dataset_size];
    for (int i = 0; i < dataset_size; i++)
    {
        //the sequential index for the whole dataset
        full_index[i] = i;
    }
    if (random)
    {
        //shuffle the origin dataset index, to get proportion data
        //cout << "the seed for shuffle function is " << time(0) << endl;
        shuffle(full_index, full_index + dataset_size, default_random_engine(time(0)));
    }
    unsigned *proportion_index = new unsigned [proportion_data_size];
    //copy the subset index from the origin dataset index

    memcpy(proportion_index, full_index+(int)(offset * dataset_size), proportion_data_size* sizeof(unsigned));
    
    //delete this
    /*for (int i = 0; i < proportion_data_size; i++)
    {
        cout << proportion_index[i] << " ";
    }*/
    delete [] full_index;
    float each_point[dim];
    for (size_t i = 0; i < proportion_data_size; i++)
    {
        infile.seekg(4*(dim + 1)*proportion_index[i]+4, ios::beg);
        infile.read((char*)(each_point), dim*4);
        
        point one_point = point(dim);
        one_point.data.insert(one_point.data.begin(), each_point, each_point+dim);

        subset.push_back(one_point);
        /*
        if (subset.size() > 1)
        {
            cout << "check the bug" << endl;
            for (int iterator = 0; iterator < subset[1].dim; iterator++)
            {
                cout << subset[1].data[iterator] << " ";
            }
            cout << endl;
        }
        */
        //delete [] each_point;
    }
    infile.close();
    delete [] proportion_index;
    //auto stop = high_resolution_clock::now();
    //auto duration = duration_cast<microseconds>(stop - start);
    //cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

}

//compute the distance between two nodes
float getDistance(point processing_node, point mean_node)
{
    float dis = 0;
    float compute_result = 0;
    assert(processing_node.dim = mean_node.dim);
    for (int i = 0; i < processing_node.dim; i++)
    {
        compute_result = (processing_node.data[i] - mean_node.data[i]) * (processing_node.data[i] - mean_node.data[i]);
        dis += compute_result;
        //cout << setprecision(16) << dis << " " << compute_result << " " << processing_node.data[i] << " " << mean_node.data[i] << endl;
    }
    return dis;
}

//get the cloeset centroid to a node
int get_closest_Label(point processing_node, vector<point> means)
{
    int closest_label;
    float dis, first;

    first = getDistance(processing_node, means[0]);
    closest_label = 0;
    for (int i = 1; i < K; i++)
    {
        dis = getDistance(processing_node, means[i]);
        if (dis < first)
        {
            first = dis;
            closest_label = i;
        }
    }
    return closest_label;
}


//get 2 closest centroids ID to the target point
void get_second_Label(point processing_node, vector<point> means, vector<int> & labels)
{
    labels.clear();
    int closest_label, second_closest_label;
    float dis, first, second;
    float dis_list[K];
    
    for (int i = 0; i < K; i++)
    {
        dis_list[i] = getDistance(processing_node, means[i]);
    }

    for(int i = 0; i < K; i++)
    {
        if (i == 0)
        {
            first = dis_list[0];
            closest_label = 0;
        }
        else
        {
            if (dis_list[i] < first)
            {
                first = dis_list[i];
                closest_label = i;
            }
        }
    }

    second = 0;
    for (int i = 0; i < K; i++)
    {
        if (i != closest_label)
        {
            if (second == 0)
            {
                second = dis_list[i];
                second_closest_label = i;
            }
            else
            {
                if (dis_list[i] < second)
                {
                    second = dis_list[i];
                    second_closest_label = i;
                }
            }
        }
    }
    //cout << "the first node is " << closest_label << " the second is " << second_closest_label <<endl;
    labels.push_back(closest_label);
    labels.push_back(second_closest_label);
    return;
}

//get the std of all nodes in a cluster
float getVar(vector<point>* clusters, vector<point> means)
{
    float var = 0;
    assert (means.size() == K);
    for (int i = 0; i < means.size(); i++)
    {
        for (int j = 0; j < clusters[i].size(); j++)
        {
            var += getDistance(clusters[i][j], means[i]);
        }
    }
    return var;
}

//for a given cluster, return the means of this cluster
point getMeans(vector<point> cluster)
{
    point data_point = point(cluster[0].dim);

    for (int i = 0; i < cluster[0].dim; i++)
    {
        data_point.data.push_back(cluster[0].data[i]);
    }

    for (int i = 0; i < cluster.size(); i++)
    {
        for (int j = 0; j < cluster[i].dim; j++)
        {
            data_point.data[j] += cluster[i].data[j];
        }
    }


    for (int i = 0; i < cluster[0].dim; i++)
    {
        data_point.data[i] /= cluster.size();
    }

    return data_point;
}

void kmeansplusplus(vector<point> & subset, vector<point>& means)
{
    assert(means.size() == 0 && subset.size() != 0);
    cout << "using kmeans++ algotirhm " << endl;
    unsigned dim = subset[0].dim;
    unsigned dataset_size = subset.size();
    
    float smallest_dis, dis, sum_dis, random_dis;

    means.push_back(subset[0]);

    vector<float> distance_list;
    for (int i = 0; i < dataset_size; i++)
    {
        distance_list.push_back(0);
    }

    
    while(means.size() < K)
    {
        sum_dis = 0;
        assert(means.size() > 0);

        for (int i = 0; i < subset.size(); i++)
        {
            smallest_dis = getDistance(subset[i], means[0]);
            for (int j = 1; j < means.size(); j++)
            {
                dis = getDistance(subset[i], means[j]);
                if (dis < smallest_dis)
                {
                    smallest_dis = dis;
                }
            }
            
            distance_list[i] = smallest_dis;

            sum_dis = sum_dis + smallest_dis;
        }

        random_dis = rand() % ((int) sum_dis); 

        for (int i = 0; i < dataset_size; i++)
        {
            random_dis -= distance_list[i];
            if (random_dis < 0)
            {
                means.push_back(subset[i]);
                cout << means.size() << " centroids added " << endl;
                break;
            }
        }
    }
}

//get the 
void kmeans_subset(vector<point> & subset, vector<point>& means)
{
    if (means.size() == 0)
    {
        
        if (usekmeansplusplus)
        {
            cout << "initializing the kmeans++ centroids" << endl;
            kmeansplusplus(subset, means);
        }
        else
        {
            cout << "initializing the random centroids without kmeans++" << endl;
            for (int i = 0; i < K; i++)
            {
                means.push_back(subset[(rand()%subset.size())]);
            }
        }
    }
    else
    {
        assert(means.size() == K);
    }

    vector<point>* clusters = new vector<point> [K];
    int subset_size = subset.size();
    int label;
    for (int i = 0; i < subset_size; i++)
    {
        label = get_closest_Label(subset[i], means);
        clusters[label].push_back(subset[i]);
    }

    float oldvar = -1;
    float newvar = getVar(clusters, means);
    while (abs(newvar - oldvar) > 1)
    {
        means.clear();
        
        for (int i = 0; i < K; i++)
        {   
            //cout << "check successful" << clusters[i].size() << endl;
            if (clusters[i].size() == 0)
            {
                cout << "cannot cluster to k centroids" << endl;
                exit(-1);
            }
            means.push_back(getMeans(clusters[i]));
        }
        
        
        oldvar = newvar;
        newvar = getVar(clusters, means);
        for (int i = 0; i < K; i++)
        {
            clusters[i].clear();
        }
        for (int i = 0; i < subset.size(); i++)
        {
            label = get_closest_Label(subset[i], means);
            clusters[label].push_back(subset[i]);
        }
    }
    /*cout << "here we print the mean point for one iteration with " << means.size() << " centroids "<< endl << endl;
    for(int i= 0; i < means.size(); i++)
    {
        cout << "this is the " << i+1 << " th centroid" << endl;
        for (int j = 0; j < means[i].dim; j++)
        {
            cout << means[i].data[j] << " ";
        }
        cout << endl << endl;
    }*/

    /*for (int i = 0; i < K; i++)
    {
        cout << "cluster instances: " << clusters[i].size() << endl;
    }*/
    delete [] clusters;
}


void kmeans(char* filename, vector<point> & means)
{
    float proportion = kmeans_proportion;
    // kmeans_iteration_times is a parameter that decides how many times we generate
    // kmeans centroids, the larger, the more accurate.
    vector<point> subset;
    unsigned dim;
    unsigned dataset_size;
    float var_record[kmeans_iteration_times-1];
    vector<point>  mean_record;
    int var;
    for (int i = 0; i < kmeans_iteration_times; i++)
    {
        cout << i+1 << " th kmeans iteration in " << kmeans_iteration_times << endl;
        usleep(1000000);
        //cout << "the seed for main loop is " << time(0) << endl;
        srand((unsigned)time(0));
        //get a proportion of the origin dataset to subset and read its dimension and dataset size.
        get_proportion_dataset(filename, subset, proportion, dim, dataset_size, 0, true);
        kmeans_subset(subset, means);


        
        if (i > 0)
        {
            /*
            cout << "the recorded mean" << endl;
            for (int a = 0; a < mean_record.size(); a++)
            {
                for (int b = 0; b < 10; b++)
                {
                    cout << mean_record[a].data[b] << "   " ;
                }
                cout << endl;
            }
            cout << endl;
            */
            var = 0;
            for (int it = 0; it < means.size(); it++)
            {
                for (int it_ = 0; it_ < means[0].dim; it_++)
                {
                    var += (mean_record[it].data[it_] - means[it].data[it_]) * (mean_record[it].data[it_] - means[it].data[it_]);
                }
            }
            mean_record.clear();
            var_record[i-1] = var;
        }
        
        /*
        cout << "the generated mean" << endl;
        for (int a = 0; a < means.size(); a++)
        {
            for (int b = 0; b < 10; b++)
            {
                cout << means[a].data[b] << "   " ;
            }
            cout << endl;
        }
        cout << endl;

        cout << "the selected subset" << endl;
        for (int a = 0; a < 5; a++)
        {
            for (int b = 0; b < 10; b++)
            {
                cout << subset[a].data[b] << "   " ;
            }
            cout << endl;
        }
        cout << endl;
        */

        for (int j = 0; j < means.size(); j++)
        {
            mean_record.push_back(means[j]);
        }
    }
    /*cout << "the final subset size is " << subset.size() << endl;
    cout << "the var for means generation are " << endl;
    for (int i = 0; i < kmeans_iteration_times-1; i++)
    {
        cout << var_record[i] << "  ";
    }
    cout << endl;
    */
    //point_memory_collection(subset);
}

#endif