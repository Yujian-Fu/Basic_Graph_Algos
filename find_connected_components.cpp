#include <iostream>
#include <list>
#include <stack>

using namespace std;

class Graph
{
    int V;
    list<int> *adj;

    void fillOrder(int v, bool visited[], stack<int> &Stack);

    void DFSUtil(int v, bool visited[]);

    public:
    Graph(int v)
    {
        this-> V = v;
        adj = new list<int> [V];
    }

    ~Graph()
    {
        delete [] adj;
    }

    void DFSUtil(int v, bool visited[])
    {
        visited[v] = true;
        cout << v << " ";

        list<int> ::iterator i;
        for (i = adj[v].begin(); i != adj[v].end(); i++)
        {
            if (!visited[*i])
                DFSUtil(*i, visited);
        }
    }

    Graph getTranspose()
    {
        Graph g(V);
        for (int v = 0; v < V; v++)
        {
            list<int>::iterator i;
            for (i = adj[v].begin(); i!= adj[v].end(); i++)
            {
                g.adj[*i].push_back(v);
            }
        }
    }

    void addEdge(int s, int e)
    {
        adj[s].push_back(e);
    }

    void fillOrder(int v, bool visited[], stack<int> & Stack)
    {
        visited[v] = true;

        list<int>::iterator i;
        for (i = adj[v].begin(); i != adj[v].end(); i++)
        {
            if (!visited[*i])
            {
                fillOrder(*i, visited, Stack);
            }
        }

        Stack.push(v);
    }

    


};

