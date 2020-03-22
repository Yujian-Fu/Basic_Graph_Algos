#include <fstream>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include "struct_define.hpp"

#define pool_size 20
#define num_start_node 1
#define test_cases 1000

//compute the distance between two nodes
float getDis(point processing_node, point mean_node)
{
    float dis = 0;
    float compute_result = 0;
    assert(processing_node.dim = mean_node.dim);
    for (int i = 0; i < processing_node.dim; i++)
    {
        compute_result = (processing_node.data[i] - mean_node.data[i]) * (processing_node.data[i] - mean_node.data[i]);
        dis += compute_result;
        //cout << setprecision(16) << dis << " " << compute_result << " " << processing_node.data[i] << " " << mean_node.data[i] << endl;
    }
    return dis;
}

void read_all_centroids(vector<point>& means)
{
    ifstream centroid_output;
    centroid_output.open("centroids.bin", ios::binary);
    int num_centroids;
    int dim;
    centroid_output.read((char*)& num_centroids, sizeof(int));
    cout << "the number of centroids is " << num_centroids << endl;
    centroid_output.read((char*)&dim, sizeof(int));
    cout << "the dim of centroids is " << dim << endl;
    cout << "Running " << test_cases << " test cases" << endl;
    cout << "The pool size is " << pool_size << endl;
    float feature;

    for (int i = 0; i < num_centroids; i++)
    {
        point each_centroid = point(dim);
        for (int j = 0; j < dim; j++)
        {
            centroid_output.read((char *)& feature, sizeof(float));
            each_centroid.data.push_back(feature);
        }
        means.push_back(each_centroid);
    }
}

void read_graph(vector<adj_list>& graph, vector<int>& centroid_ids)
{
    ifstream graph_file;
    graph_file.open("edges_final.bin", ios::binary);
    streampos begin, end;
    graph_file.seekg(0, ios::beg);
    int num_centroids;
    int centroid_id;
    graph_file.read((char*)& num_centroids, sizeof(int));

    for (int i= 0; i < num_centroids; i++)
    {
        graph_file.read((char *)& centroid_id, sizeof(int));
        centroid_ids.push_back(centroid_id);
    }
    

    int each_num;
    int each_edge;
    int position;
    int count = 0;
    while (graph_file.peek() != EOF)
    {
        graph_file.read((char*)& position, sizeof(int));
        graph_file.read((char* )& each_num, sizeof(int));
        adj_list each_list = adj_list(position);
        count ++;
        for (int i = 0; i < each_num; i++)
        {
            graph_file.read((char*)& each_edge, sizeof(int));
            count ++;
            each_list.edges.insert(each_edge);
        }
        graph.push_back(each_list);
        position ++;
    }
}


void read_query(vector<point> query)
{

}


void load_data(vector<point>& raw_data, char* filename)
{
    int dim, num_points;
    ifstream infile(filename, ios::binary);
    if (!infile.is_open())
    {
        cout << "file open error " << endl;
        exit(-1);
    }

    infile.read((char*)& dim, 4);
    infile.seekg(0, ios::end);
    ios::pos_type ss = infile.tellg();
    size_t fsize = (size_t) ss;
    num_points = (unsigned) (fsize / (dim + 1) /4);

    float each_point[dim];

    infile.seekg(0, ios::beg);
    for (int i = 0; i < num_points; i++)
    {
        infile.seekg(4, ios::cur);
        infile.read((char*)& (each_point), dim * 4);
        point one_point = point(dim);
        one_point.data.insert(one_point.data.begin(), each_point, each_point+dim);
        raw_data.push_back(one_point);
    }
    infile.close();
}


void select_start_node(vector<int>& start_node, point query, vector<point> centroids)
{
    float dis;
    int num_centroids = centroids.size();

    float dis_list [num_centroids];
    float copy_dis_list [num_centroids];

    for (int i = 0; i < num_centroids; i++)
    {
        dis_list[i] = getDis(query, centroids[i]);
    }

    for (int i = 0; i < num_centroids; i++)
    {
        copy_dis_list[i] = dis_list[i];
    }

    sort(dis_list, dis_list+num_centroids);


    for (int i = 0; i < num_start_node; i++)
    {
        dis = dis_list[i];
        for (int j = 0; j < num_centroids; j++)
        {
            if (copy_dis_list[j] == dis)
            {
                start_node.push_back(j);
                break;
            }
        }
    }
}

bool GreaterSort (pair<int, float> point1, pair<int, float> point2)
{
    return (point1.second < point2.second);
}


void search(char* filename)
{
    cout << segement_line << endl;
    cout << "Starting search test" << endl;
    srand(time(0));
    vector <int> centroid_ids;
    vector <adj_list> graph;
    vector <point> centroids;
    vector<point> query;
    vector<point> raw_data;
    vector<pair<int, float>> candidates;
    vector<int> start_node;
    int check_index;


    read_all_centroids(centroids);
    read_graph(graph, centroid_ids);
    //read_query(query);
    load_data(raw_data, filename);
    int num_points = raw_data.size();
    bool visited[num_points];
    bool inserted[num_points];

    int index[test_cases];
    for (int i = 0; i < test_cases; i++)
    {
        index[i] = rand()%num_points;
        query.push_back(raw_data[index[i]]);
    }

    float recall_1 = 0;
    float visited_num = 0;
    float inserted_num = 0;
    for (int i= 0; i < query.size(); i++)
    {
        for (int j = 0; j < num_points; j++)
        {
            visited[j] = false;
            inserted[j] = false;
        }
        start_node.clear();
        candidates.clear();
        select_start_node(start_node, query[i], centroids);
        //cout << "the query point with selected start nodes " << endl;
        //cout <<  index[i] <<  " ";
        // the start node is the index of start point 
        // not the physical location
        for (int j = 0; j < start_node.size(); j++)
        {
            int centroid_id = centroid_ids[start_node[j]];
            pair<int, float> each_node(centroid_id, getDis(raw_data[centroid_id], query[i]));
            inserted[centroid_id] = true;
            //cout << centroid_id << " ";
            candidates.push_back(each_node);
        }


        check_index = 0;
        
        while(check_index+1 < pool_size)
        {
  
            for (int k = 0; k < candidates.size(); k++)
            {
                if (visited[candidates[k].first] == false)
                {
                    //cout << endl << "test !!! " << k << " " << candidates.size() << " " << candidates[k].first << " " << visited[candidates[k].first] << endl;
                    visited[candidates[k].first] = true;
                    check_index = k;
                    break;
                }
                if (k == candidates.size()-1)
                {
                    check_index = k;
                }
            }

            //cout << "the check index is " << check_index;
            int location = candidates[check_index].first;
            
            for (set<int>::iterator it = graph[location].edges.begin();
            it != graph[location].edges.end(); it++)
            {
                if (inserted[*it] == false)
                {
                    inserted[*it] = true;
                    pair<int, float> each_node (*it, getDis(raw_data[*it], query[i]));
                    candidates.push_back(each_node);
                }
            }
            

            sort(candidates.begin(), candidates.end(), GreaterSort);

            if (candidates.size() > pool_size)
            {
                candidates.erase(candidates.begin() + pool_size, candidates.end());
            }
        }

        if (candidates[0].first == index[i])
            recall_1 += 1;
        
        
        for (int j = 0; j < num_points; j++)
        {
            if (visited[j] == true)
                visited_num ++;
            if (inserted[j] == true)
                inserted_num ++;
        }
        //cout << "the visited num: " << visited_num << " the inserted num: " << inserted_num << endl;
    }
    visited_num /= test_cases;
    inserted_num /= test_cases;
    recall_1 /= test_cases;
    cout << "The visited number, inserted number and the recall@1 is: " << visited_num << " " << inserted_num << " " << recall_1 << endl;
return;
}