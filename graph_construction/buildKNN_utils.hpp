#ifndef _knn_utils_
#define _knn_utils_
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <assert.h>
#include "struct_define.hpp"
#include "kmeans.hpp"

#define num_neighbors 10


using namespace std;


unsigned get_size(char* filename)
{
    ifstream infile(filename, ios::binary);
    if(!infile.is_open())
    {
        cout << "file open error" << endl;
        exit(-1);
    }

    unsigned dim;
    infile.read((char*)&dim, 4);
    ios::pos_type ss = infile.tellg();
    size_t fsize = (size_t) ss;
    unsigned dataset_size = (unsigned) (fsize / (dim+1) / 4);
    return dataset_size;
}

float getDistance_vertex(vertex processing_node, vertex mean_node)
{
    float dis = 0;
    assert(processing_node.dim = mean_node.dim);
    for (int i = 0; i < processing_node.dim; i++)
    {
        dis += (processing_node.data[i] - mean_node.data[i]) * (processing_node.data[i] - mean_node.data[i]);
    }
    return dis;
}

vector<unsigned> get_neighbors_index(vector<vertex>& nodes, int target)
{
    vector<unsigned> neighbor_index;
    float k_dis[num_neighbors + 1];
    float dis;
    float largest_dis;
    unsigned largest_index;
    for (int i = 0; i < num_neighbors+1; i++)
    {
        neighbor_index.push_back(0);
    }

    for (int i = 0; i < num_neighbors+1; i++)
    {
        dis = getDistance_vertex(nodes[target], nodes[i]);
        neighbor_index[i] = nodes[i].id;
        k_dis[i] = dis;
        if (i == 0)
        {
            largest_index = 0;
            largest_dis = dis;
        }
        else
        {
            if (dis > largest_dis)
            {
                largest_dis = dis;
                largest_index = i;
            }
        }
    }

    for (int i = num_neighbors+2; i< nodes.size(); i++)
    {
        dis = getDistance_vertex(nodes[target], nodes[i]);
        if (dis < largest_dis)
        {
            neighbor_index[largest_index] = nodes[i].id;
            k_dis[largest_index] = dis;
            largest_dis = dis;
            for (int j=0; j < num_neighbors + 1; j++)
            {
                if (k_dis[j] > largest_dis)
                {
                    largest_dis = k_dis[j];
                    largest_index = j;
                }
            }
        }
    }

    return neighbor_index;
}

void merge_edges(vector<adj_list>& all_edges, vector<adj_list>& adj_lists)
{
    for (int i = 0; i < adj_lists.size(); i++)
    {
        unsigned ID = adj_lists[i].id;
        assert (ID == all_edges[ID].id);
        for (set<int>::iterator j = adj_lists[i].edges.begin(); j != adj_lists[i].edges.end(); j++)
        {
            all_edges[ID].edges.insert(*j);
        }
    }
}


void build_sub_graph(vector<vertex>& nodes, vector<adj_list>& adj_lists)
{
    int complete_flag1, complete_flag2;
    for (int i = 0; i < nodes.size(); i++)
    {
        vector<unsigned> neighbors = get_neighbors_index(nodes, i);
        /*for (int j = 0; j < neighbors.size(); j++)
        {
            cout << neighbors[j] << "  ";
        }
        cout << endl;
        */
        for (int j = 0; j < neighbors.size(); j++)
        {
            if (neighbors[j] != nodes[i].id)
            {
                complete_flag1 = 0;
                complete_flag2 = 0;
                for (int k = 0; k < adj_lists.size(); k++)
                {
                    if (adj_lists[k].id == nodes[i].id)
                    {
                        adj_lists[k].edges.insert(neighbors[j]);
                        complete_flag1 = 1;
                    }
                    
                    else if (adj_lists[k].id == neighbors[j])
                    {
                        //cout << "add " << neighbors[j] << " in " << adj_lists[k].id << " list " << endl;
                        adj_lists[k].edges.insert(nodes[i].id);
                        complete_flag2 = 1;
                    }

                    if (complete_flag1 == 1 && complete_flag2 == 1)
                        break;
                        
                }

                /*if (complete_flag1 == 0 || complete_flag2 == 0)
                {
                    cout << "flag error" << endl;
                    exit(0);
                }*/
            }
        }
    }
}

void read_fvecs(char* filename, float* &data, unsigned &num_points, unsigned &dim)
{
    ifstream infile(filename, ios::binary);
    if (!infile.is_open())
    {
        cout << "file open error" << endl;
        exit(-1);
    }

    infile.read((char*)&dim, 4);
    infile.seekg(0, ios::end);
    ios::pos_type ss = infile.tellg();
    size_t fsize = (size_t) ss;
    num_points =  (unsigned)(fsize / (dim + 1) /4);
    data = new float[(size_t)num_points * (size_t)dim];

    infile.seekg(0, ios::beg);
    for (size_t i = 0; i < num_points; i++)
    {
        infile.seekg(4, ios::cur);
        infile.read((char*)(data + i*dim), dim * 4);
    }

    for (size_t i= 0; i < num_points * dim; i++)
    {
        cout << (float)data[i];
        if (!i)
        {
            cout << " ";
            continue;
        }

        if (i % (dim - 1) != 0)
        {
            cout << " ";
        }
        else
        {
            cout << endl;
        }
    }

    infile.close();
}



#endif