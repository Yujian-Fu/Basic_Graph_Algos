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


//the visit ID and the finish ID of every point 
struct dfs_num
{
    int id;
    int visit_num;
    int finish_num;

    dfs_num(int id, int num)
    {
        this->id = id;
        this->visit_num = num;
    }
};


//
struct tr_edge
{
    //the start num is the physical position to load this data point
    unsigned start;
    unsigned end;
    //this is the edge type
    unsigned label;

    tr_edge(unsigned start, unsigned end)
    {
        this->start = start;
        this->end = end;
    }
};

#endif