/*
* Check the connectivity of a graph with DFS
*/

#include <list>
#include <iostream>
#include <stack>

using namespace std;

class Graph
{
    private:
        int V;
        list<int> *adj;
        void DFSUtil(int v, bool visited[]);

    public:
        Graph(int v)
        {
            V = v;
            adj = new list <int> [V];
        }

        ~Graph()
        {
            delete [] adj;
        }

        void addEdge(int s, int e);
        void isConnected();
        Graph getTranspose();
        bool isSC();
};

void Graph::DFSUtil(int v, bool visited[])
{
    visited[v] = true;
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); i++)
    {
        if (!visited[*i])
            DFSUtil(*i, visited);
    }
}

Graph Graph::getTranspose()
{
    Graph g(V);
    for (int v = 0; v < V; v++)
    {
        list<int>::iterator i;
        for (i = adj[v].begin(); i!=adj[v].end(); i++)
        {
            g.adj[*i].push_back(v);
        }
    }
    return g;
}

void Graph::addEdge(int s, int e)
{
    adj[s].push_back(e);
}

bool Graph::isSC()
{
    bool visited[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;
    
    DFSUtil(0, visited);

    for (int i = 0; i < V; i++)
        if (visited[i] == false)
        {
            return false;
        }

    Graph gr = getTranspose();

    for (int i = 0; i < V; i++)
    {
        visited[i] = false;
    }

    gr.DFSUtil(0, visited);
    for (int i = 0; i < V; i++)
    {
        if (visited[i] == false)
            return false;
    }

    return true;
}







