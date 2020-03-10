#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
#include <list>

#define dimension 128

using namespace std;

struct  vertex
{
    int id;
    int dim;
    
    list <int> * edges;
    float* data;

    vertex (int id, int* edges, float* data, int dim = dimension)
    {
        this->dim = dim;
        this->id = id;
        this->edges = new list<int>;
        this->data = data;
    }
};

vector<vertex> load_data (char*);
void merge_kNN(vector<vertex>);
void nnDescent_kNN(vector<vertex>);
void scalable_kNN(vector<vertex>);
void degree(vector<vertex>);

int main()
{
    char* file_name = " ";
    vector<vertex> nodes;
    vector<vertex> data_points = load_data(nodes, file_name);
    nnDescent_kNN(data_points);

}

void merge_kNN(vector<vertex> & data_points){}

void load_data (vector<vertex> nodes, char* filename)
{
    float* data_load = NULL;
    unsigned num_points, dim;
    read_fvecs(filename, data_load, num_points, dim);
    for (int i = 0; i < num_points; i++)
    {
        float one_point[dim];
        for (int j = 0; j < dim; j++)
        {
            one_point[j] = *(data_load + i*dim + j);
        }
        vertex node = vertex(i, NULL, one_point, dim);
        nodes.push_back(node);
    }
}


void read_fvecs(char* filename, float* &data, unsigned &num_points, unsigned &dim)
{
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