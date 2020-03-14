#include <iostream>
#include <fstream>
#include <vector>

#include "kmeans.hpp"
#include "struct_define.hpp"
#include "buildKNN_utils.hpp"



using namespace std;



void merge_kNN(char* filename)
{
    point* means = new point[K];
    kmeans(filename, means);
    ifstream infile(filename, ios::binary);
    if (!infile.is_open)
    {
        cout << "file open error" << endl;
        exit(-1);
    }
    unsigned dim;
    infile.read((char*)&dim, 4);
    infile.seekg(0, ios::end);
    ios::pos_type ss = infile.tellg();
    size_t fsize = (size_t) ss;
    int dataset_size =  (unsigned)(fsize / (dim + 1) /4);

    float* each_point = new float[(size_t) dim];
    int* labels;
    vector<int> * cluster_allocation = new vector<int> [K];
    vector<int> cluster;
    for (size_t i = 0; i < dataset_size; i++)
    {
        infile.seekg(4, ios::cur);
        infile.read((char*)each_point, dim * 4);
        point processing_point = point(dim, each_point);
        labels = get_second_Label(processing_point, means);
        for (int i = 0; i < 2; i++)
            cluster_allocation[labels[i]].push_back(i);
    }

    vector<adj_list> all_edges;
    for (int i = 0; i < dataset_size; i++)
    {
        adj_list each_list = adj_list(i);
        all_edges.push_back(each_list);
    }

    for (int i = 0; i < K; i++)
    {
        vector<vertex> vertex_nodes;
        vector<adj_list> adj_lists; 
        vertex_nodes.clear();
        adj_lists.clear();
        cluster = cluster_allocation[i];
        for (int j = 0; j < cluster.size(); i++)
        {
            float* point_data = new float[(size_t) dim];
            infile.seekg(cluster[j]*(dim + 1)*4, ios::beg);
            infile.read((char*)(point_data), dim*4);
            vertex vertex_node = vertex(cluster[j], point_data, dim);
            adj_list vertex_edge = adj_list(cluster[j]);
            vertex_nodes.push_back(vertex_node);
            adj_lists.push_back(vertex_edge);
        }
        build_sub_graph(vertex_nodes, adj_lists);
        merge_edges(all_edges, adj_lists);
    }

    return;
}


