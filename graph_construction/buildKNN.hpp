#ifndef _knn_cons_
#define _knn_cons_
#include <iostream>
#include <fstream>
#include <vector>

#include "kmeans.hpp"
#include "buildKNN_utils.hpp"
#include "struct_define.hpp"

using namespace std;

/*
Description: 
Build kNN graph with merge strategy, firstly build sub-kNN with multiple kmeans centroids and their nodes. 
input: char*: the filename of the .fvecs file.
output: vector<adj_list>: the edges for every nodes.
*/

void merge_kNN(char* filename)
{
    cout << segement_line << endl;

    cout << "Now start the merge kNN generation procession " << endl;
    cout << "The processing file " << filename << endl;
    cout << "The parameter list: k: " << num_neighbors << endl;
    cout << "The iteration time for kmeans " << kmeans_iteration_times << endl;
    cout << "The number of sub graph: " << K << endl;
    cout << "Using k-means++: " << usekmeansplusplus << endl;
    cout << "Proportion of dataset used in kmeans iteration: " << kmeans_proportion << endl;
    //define K kmeans centroids 
    vector<point> means;
    //generate K centroids with kmeans
    kmeans(filename, means);
    cout << "kmeans algorithm finished " << endl;
    cout << "Saving all centroids named 'centroids.bin' ..." << endl;
    ofstream all_centroids;
    all_centroids.open("centroids.bin", ios::binary);
    int num_centroid = means.size();
    int dim = means[0].dim;
    all_centroids.write((char* )& num_centroid, sizeof(int));
    all_centroids.write((char* )& dim, sizeof(dim));
    for (int i = 0; i < means.size(); i++)
    {
        for (int j = 0; j < means[i].dim; j++)
            all_centroids.write((char*) & means[i].data[j], sizeof(float));
    }
    all_centroids.close();
    cout << "Saving successfully " << endl;
    ifstream infile(filename, ios::binary);
    if (!infile.is_open())
    {
        cout << "file open error" << endl;
        exit(-1);
    }
    //read the dimension of the dataset
    infile.read((char*)&dim, 4);
    infile.seekg(0, ios::end);
    ios::pos_type ss = infile.tellg();
    size_t fsize = (size_t) ss;
    //get the node points of the dataset size
    int dataset_size =  (unsigned)(fsize / (dim + 1) /4);

    //every point that load from the memory
    float* each_point = new float[dim];
    vector<int> labels;
    //the clusters for each centroid
    vector<int> cluster_allocation[K];
    point processing_point = point(dim);
    infile.seekg(0, ios::beg);
    for (size_t i = 0; i < dataset_size; i++)
    {
        labels.clear();
        processing_point.data.clear();
        infile.seekg(4, ios::cur);
        infile.read((char*)each_point, dim * 4);
        /*for (int j = 0; j < dim; j++)
            cout << each_point[j] << " ";
        cout << endl;*/
        processing_point.data.insert(processing_point.data.begin(), each_point, each_point+dim);
        // get the label of two nearest neighbors of the target point
        get_second_Label(processing_point, means, labels);
        cluster_allocation[labels[0]].push_back(i);
        cluster_allocation[labels[1]].push_back(i);
        //cout << endl;
    }

    delete [] each_point;
    vector<int>().swap(labels);
    
    cout << "The number of nodes in various cluster for kNN construction: " << endl;
    for(int i = 0; i < K; i++)
    {
        cout << i+1 << " th cluster: " << cluster_allocation[i].size() << endl;
    }
    
    

    //the edges for all nodes, each node with a adj_list
    vector<adj_list> all_edges;
    for (int i = 0; i < dataset_size; i++)
    {
        adj_list each_list = adj_list(i);
        all_edges.push_back(each_list);
    }


    for (int i = 0; i < K; i++)
    {
        cout << "Now building and merging " << i << " th subgraph" << endl; 
        vector<vertex> vertex_nodes;
        vector<adj_list> adj_lists; 
        vertex_nodes.clear();
        adj_lists.clear();
        //build kNN graph on every cluster
        //load all nodes to vertex_nodes
        float point_data[dim];
        for (int j = 0; j < cluster_allocation[i].size(); j++)
        {
            infile.seekg(4*(dim + 1)*cluster_allocation[i][j]+4, ios::beg);
            infile.read((char*)(point_data), dim*4);
            //create a new vector to hold the point and edges
            vertex vertex_node = vertex(cluster_allocation[i][j], dim);
            vertex_node.data.insert(vertex_node.data.begin(), point_data, point_data+dim);
            adj_list vertex_edge = adj_list(cluster_allocation[i][j]);
            vertex_nodes.push_back(vertex_node);
            adj_lists.push_back(vertex_edge);
        }
        build_sub_graph(vertex_nodes, adj_lists);
        vector<vertex>().swap(vertex_nodes);
        merge_edges(all_edges, adj_lists);
        
        /*
        for (int it = 0; it < all_edges.size(); it++)
        {
            cout << endl << "the edges for node " << all_edges[it].id << " is " << endl;
                for(set<int>::iterator it_=all_edges[it].edges.begin() ;it_!=all_edges[it].edges.end();it_++)
                {
                    cout<<*it_<<"  ";
                }
                cout << endl;
        }
        */
        vector<adj_list>().swap(adj_lists);
    }
    //the out put part
    cout << "Saving edges of kNN graph named 'edges.bin' and 'edges.txt' " << endl;
    int num_all_edges = 0;
    ofstream edge_output;
    edge_output.open("edges.bin", ios::binary);
    ofstream txt_output;
    txt_output.open("edges.txt");
    int length;
    for (int i = 0; i < all_edges.size(); i++)
    {
        length = all_edges[i].edges.size();
        edge_output.write((char*)& i, sizeof(int));
        edge_output.write((char*)& length, sizeof(int));
        txt_output << endl << i << "  " << length << endl;
        for (set<int>::iterator j = all_edges[i].edges.begin();
         j != all_edges[i].edges.end(); j++)
        {
            edge_output.write((char*)& (*j), sizeof(int));
            txt_output << *j << " ";
        }
        num_all_edges += all_edges[i].edges.size();
    }
    edge_output.close();
    cout << "Saving successfully " << endl;
    cout << "the total size in this graph is " << num_all_edges << endl;
    /*
    ifstream edges;
    int each_edge;
    edges.open("edges.bin", ios::binary);
    while (!edges.eof())
    {
        edges.read((char*)& each_edge, sizeof(int));
        cout << each_edge << " ";
    }
    */
    return;
}

#endif
