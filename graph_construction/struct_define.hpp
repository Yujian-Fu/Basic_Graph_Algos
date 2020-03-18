#ifndef _struct_definition_
#define _struct_definition_
#include <vector>
#include <set>

#define dimension 128

using namespace std;

struct  vertex
{
    unsigned id;
    unsigned dim;
    
    vector<float> data;

    vertex (int id, int dim = dimension)
    {
        this->dim = dim;
        this->id = id;
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


struct point
{
    unsigned dim;
    vector<float> data;

    point(unsigned dim = 0)
    {
        this->dim = dim;
    }
};

#endif