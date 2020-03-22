#include <fstream>
#include <iostream>
#include "struct_define.hpp"
#include "transition_reduction.hpp"

void read_graph(vector<adj_list>& graph, vector<int> centroid_ids)
{
    ifstream graph_file;
    graph_file.open("edges_final.bin", ios::binary);
    streampos begin, end;
    graph_file.seekg(0, ios::beg);
    int num_centroids;
    graph_file.read((char*) num_centroids, sizeof(int));

    for (int i= 0; i < num_centroids; i++)
    {

    }
    

    int each_num;
    int each_edge;
    int position;
    int count = 0;
    while (graph_file.peek() != EOF)
    {
        //graph_file.read((char*)& each_edge, sizeof(int));
        //cout << each_edge << " ";
        graph_file.read((char*)& position, sizeof(int));
        graph_file.read((char* )& each_num, sizeof(int));
        adj_list each_list = adj_list(position);
        //cout << endl << "the graph_file for node " << position << endl;
        count ++;
        for (int i = 0; i < each_num; i++)
        {
            graph_file.read((char*)& each_edge, sizeof(int));
            count ++;
            //cout << each_edge << " ";
            each_list.edges.insert(each_edge);
        }
        graph.push_back(each_list);
        position ++;
    }
}

int main()
{
    vector <int> centroid_ids;
    vector <adj_list> graph;
    vector <point> centroids;

    read_centroids(centroids);
    read_graph(graph, centroid_id);

    for 

}