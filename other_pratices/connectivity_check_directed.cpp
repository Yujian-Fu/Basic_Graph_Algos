/*
* C++ program to check whether a directed graoh is connected using BFS
*/

#include <iostream>
#include <list>
#include <queue>

using namespace std;

/*
* Class Declaration
*/

class Graph
{
    public:
        int V;
        list <int> *adj;
    public:
        Graph(int V)
        {
            this->V = V;
            adj = new list<int> [V];
        }

        ~Graph()
        {
            delete [] (adj);
        }

        void addEdge(int v, int w);
        void BFS(int s, bool visited[]);
        Graph getTranspose();
        bool isConnected();
        void showAll();
};

void Graph::addEdge (int v, int w){
    adj[v].push_back(w);
}

void Graph::BFS(int s, bool visited[])
{
    list <int> record;
    list <int>::iterator i;
    visited[s] = true;
    record.push_back(s);
    while (!record.empty())
    {
        s = record.front();
        record.pop_front();
        for (i = adj[s].begin(); i != adj[s].end(); i++)
        {
            if (!visited[*i]) {
                visited[*i] = true;
                record.push_back(*i);
            }
        }

    }
}

Graph Graph::getTranspose()
{
    Graph gt(V);
    list <int> :: iterator i;
    for (int v= 0; v < V; v++)
    {
        for (i = adj[v].begin(); i!= adj[v].end(); i++)
        {
            gt.adj[*i].push_back(v);
        }
    }
    return gt;
}

bool Graph::isConnected()
{
    bool visited[V];
    for (int i = 0; i < V; i++)
    {
        visited[i] = false;
    }
    BFS(0, visited);
    
    for (int i = 0; i < V; i++)
    {
        cout << i << visited[i] << endl;
        if (visited[i] == false)
            return false;
    }

    Graph gt = getTranspose();
    for (int i = 0; i < V; i++)
    {
        visited[i] = false;
    }
    gt.BFS(0, visited);
    for (int i = 0; i < V; i++)
    {
        cout << i << visited[i] << endl;
        if (visited[i] == false)
            return false;
    }
    return true;
}

void Graph::showAll()
{
    list <int>::iterator i;
    for (int v = 0; v < V; v++)
    {
        for (i = adj[v].begin(); i != adj[v].end(); i++)
        {
            cout << "Edge: From " << to_string(v) << " to " << to_string(*i) << endl;
        }
    }
}



int main(){
    string  * a = new string ("hello");
    cout << *a << endl;
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 3);
    g.showAll();

    if (g.isConnected())
    {
        cout << "connected" << endl;
    }

    g.~Graph();
    cout << "now start" << endl;
    g.showAll();
    return 0;

}







