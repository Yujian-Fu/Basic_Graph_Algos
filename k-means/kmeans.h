#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
#include <list>

#define kmeans_iteration_times 10
#define K 10

using namespace std;

struct point
{
    unsigned dim;
    float* data;

    point(unsigned dim = 0, float* data = nullptr)
    {
        this->dim = dim;
        this->data = data;
    }
};


void get_proportion_dataset(char* filename, vector<point>& subset, float proportion, unsigned& dim, 
unsigned& dataset_size, bool random = false, float offset)
{
    ifstream infile(filename, ios::binary);
    if(!infile.is_open)
    {
        cout << "file open error" << endl;
        exit(-1);
    }

    infile.read((char*)&dim, 4);
    ios::pos_type ss = infile.tellg();
    size_t fsize = (size_t) ss;
    dataset_size = (unsigned) (fsize / (dim+1) / 4);
    unsigned proportion_data_size = (unsigned) (dataset_size * proportion);
    unsigned full_index [dataset_size];
    for (int i = 0; i < dataset_size; i++)
    {
        full_index[i] = i;
    }
    if (random)
    {
        random_shuffle(full_index, full_index + dataset_size);
    }
    unsigned proportion_index[proportion_data_size];
    memcpy(proportion_index, full_index+(int)(offset * dataset_size), sizeof(proportion_index));
    for (size_t i = 0; i < proportion_data_size; i++)
    {
        float* each_point = new float[dim];
        infile.seekg(4*(dim + 1)*proportion_index[i], ios::beg);
        infile.read((char*)(each_point), dim*4);
        point one_point = point(dim, each_point);
        subset.push_back(one_point);
    }
    infile.close();
}

float getDistance(point processing_node, point mean_node)
{
    float dis = 0;
    assert(processing_node.dim = mean_node.dim);
    for (int i = 0; i < processing_node.dim; i++)
    {
        dis += (processing_node.data[i] - mean_node.data[i]) * (processing_node.data[i] - mean_node.data[i]);
    }
    return dis;
}

int get_closest_Label(point processing_node, point* means)
{
    int closest_label, second_closest_label;
    float dis, first, second;

    first = getDistance(processing_node, means[0]);
    second = first;
    closest_label = 0;
    second_closest_label = 0;
    for (int i = 1; i < K; i++)
    {
        dis = getDistance(processing_node, means[i]);
        if (dis < first)
        {
            first = dis;
            closest_label = i;
        }
        else if( first< dis < second)
        {
            second = dis;
            second_closest_label = i; 
        }
    }
    return closest_label;
}

int* get_second_Label(point processing_node, point* means)
{
    int labels[2];
    int closest_label, second_closest_label;
    float dis, first, second;

    first = getDistance(processing_node, means[0]);
    second = first;
    closest_label = 0;
    second_closest_label = 0;
    for (int i = 1; i < K; i++)
    {
        dis = getDistance(processing_node, means[i]);
        if (dis < first)
        {
            first = dis;
            closest_label = i;
        }
        else if( first< dis < second)
        {
            second = dis;
            second_closest_label = i; 
        }
    }
    labels[0] = closest_label;
    labels[1] = second_closest_label;
    return labels;
}

float getVar(vector<point>* clusters, point* means)
{
    float var = 0;
    for (int i = 0; i < K; i++)
    {
        vector<point> cluster = clusters[i];
        for (int j = 0; j < cluster.size(); i++)
        {
            var += getDistance(cluster[j], means[i]);
        }
    }
    return var;
}


point getMeans(vector<point> cluster)
{
    point mean = cluster[0];
    if (cluster.size() > 1)
    {
        for (int i = 1; i < cluster.size(); i++)
        {
            for (int j = 0; j < cluster[i].dim; j++)
            {
                mean.data[j] += cluster[i].data[j];
            }
        }
    }

    for (int i = 0; i < cluster[0].dim; i++)
    {
        mean.data[i] /= cluster.size();
    }

    return mean;
}


void kmeans_subset(vector<point> & subset, point*& means, unsigned& dim, unsigned& dataset_size)
{
    if (means[0].data == NULL)
    {
        for (int i = 0; i < K; i++)
        {
            means[i] = subset[i];
        }
    }
    else
    {
        vector<point>* clusters = new vector<point> [K];
        int subset_size = subset.size();
        int label;
        for (int i = 0; i < subset_size; i++)
        {
            label = get_closest_Label(subset.back(), means);
            clusters[label].push_back(subset.back());
            subset.pop_back();
        }
        vector<point>().swap(subset);
        float oldvar = -1;
        float newvar = getVar(clusters, means);
        while (abs(newvar - oldvar) > 1)
        {
            for (int i = 0; i < K; i++)
            {
                means[i] = getMeans(clusters[i]);
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
    }
}


void kmeans(char* filename, point*& means)
{
    float proportion = 0.1;
    vector<point> subset;
    unsigned dim;
    unsigned dataset_size;
    // this should be a user defined parameter to decide how many 
    // points are used in every k-means generation iteration
    for (int i = 0; i < kmeans_iteration_times; i++)
    {
        get_proportion_dataset(filename, subset, proportion, dim, dataset_size, true, 0);
        kmeans_subset(subset, means, dim, dataset_size);
    }
}