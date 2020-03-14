#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
#include <list>
#include "kmeans.h"

#define dimension 128
#define num_neighbors 10

using namespace std;

struct  vertex
{
    unsigned id;
    unsigned dim;
    
    float* data;

    vertex (int id, float* data, int dim = dimension)
    {
        this->dim = dim;
        this->id = id;
        this->data = data;
    }
};

struct adj_list
{
    unsigned id;
    set<int> edges;

    adj_list(unsigned id)
    {
        this->id = id;
    }
};

void kmeans(char*, vector<float*> &);
void kmeans_subset(float* &, vector<float*>&, unsigned&, unsigned&);
void load_data (vector<vertex>, char*);
void merge_kNN(vector<vertex>& nodes);
void nnDescent_kNN(vector<vertex>);
void scalable_kNN(vector<vertex>);
void degree(vector<vertex>);
void build_sub_graph(vector<vertex>&, vector <vertex>, unsigned);
void merge_sub_graph(vector<vertex>&);
unsigned get_size(char*);
void get_proportion_dataset(char* filename, float* &subset, float proportion);

unsigned get_size(char* filename)
{
    ifstream infile(filename, ios::binary);
    if(!infile.is_open)
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

unsigned* get_neighbors_index(vector<vertex>& nodes, int target)
{
    unsigned k_index[num_neighbors + 1];
    float k_dis[num_neighbors + 1];
    float dis;
    float largest_dis;
    unsigned largest_index;
    for (int i = 0; i < num_neighbors; i++)
    {
        k_index[i] = 0;
    }

    for (int i = 0; i < num_neighbors+1; i++)
    {

        dis = getDistance_vertex(nodes[target], nodes[i]);
        k_index[i] = i;
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
            k_index[largest_index] = i;
            k_dis[largest_index] = dis;
            largest_dis = dis;
            for (int j=0; j < num_neighbors + 1; i++)
            {
                if (k_dis[j] > largest_dis)
                {
                    largest_dis = k_dis[j];
                    largest_index = j;
                }
            }
            break;
        }
    }

    return k_index;
}

void build_sub_graph(vector<vertex>& nodes, vector<adj_list>& adj_lists)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        unsigned* neighbors = get_neighbors_index(nodes, i);
        for (int j = 1; j < num_neighbors+1; j++)
        {
            for (int k = 0; k < adj_lists.size(); k++)
            {
                if (neighbors[j] == adj_lists[k].id)
                {
                    adj_lists[k].edges.insert(neighbors[j]);
                }
                break;
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