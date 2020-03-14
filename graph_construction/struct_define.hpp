#include <vector>
#include <set>

#define dimension 128

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