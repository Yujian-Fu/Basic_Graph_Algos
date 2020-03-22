#include <iostream>
#include "struct_define.hpp"
#include <assert.h>
#include <fstream>
#include "kmeans.hpp"

#define front_delete_probability 0.8
#define back_delete_probability 0.8

//typedef pair<int, int> backpoint;
//typedef pair<pair<int, int>, pair<int, int>> standfor;

using namespace std;

unsigned visit_counter = 0;
unsigned finish_counter = 0;

/*

void update_back_point(vector<tr_edge> & all_edges)
{
    if (w < backpoint(v)):
        backpoint(v) = w;
    
    for all (v, W) in B:
        if w != update_back_point(v):
            delete(v, w);
}

void R-test()
{
    for all edges(v, w) in T cluster:
        if v is not a leaf:
            for all (v, w) in T:
                z = min(backpoint)
            if v != z:
                if backpoint(v) < backpoint(z):
                    delete (z, backpoint(z))
                    add(z, backpoint(v));
                    standfor(z, backpoint(v)) = standfor((v, backpoint(v)));
                    backpoint(z) = backpoint(v);
                delete(v, backpoint(v));
                backpoint(v) = z;                    
}

void update_cross_edges(vector<tr_edge> all_edges, vector <dfs_num> dfs_tag)
{
    int common_ancestor;
    for (int i = 0; i < all_edges.size(); i++)
    {
        if (all_edges[i].label == 3)
        {
            common_ancestor = find_common_ancestor(all_edges[i].start, all_edge[i].end);
            if (common_ancestor < backpoint[all_edges[i].start])
            {
                tr_edge one_edge = tr_edge(all_edges[i].start, common_ancestor);
                
                all_edges.push_back(one_edge);
                standfor[<all_edges[i].start, common_ancertor>] = <all_edges[i].start, all_edge[i].end>
                backpoint[all_edges[i].start] = common_ancestor;
            }
        }
        all_edges.erase(i);
    }
}
void explore_edges()
{
    for all edges in EDGES:
        add_REACH(v)
}
*/

void read_centroids(vector<point>& means)
{
    ifstream centroid_output;
    centroid_output.open("centroids.bin", ios::binary);
    int num_centroids;
    int dim;
    centroid_output.read((char*)& num_centroids, sizeof(int));
    cout << "the number of centroids is " << num_centroids << endl;
    centroid_output.read((char*)&dim, sizeof(int));
    cout << "the dim of centroids is " << dim << endl;
    float feature;

    for (int i = 0; i < num_centroids; i++)
    {
        point each_centroid = point(dim);
        for (int j = 0; j < dim; j++)
        {
            centroid_output.read((char *)& feature, sizeof(float));
            each_centroid.data.push_back(feature);
            //cout << feature << " ";
        }
        //cout << endl;
        means.push_back(each_centroid);
    }
}



void read_edges(vector<adj_list>& all_edges)
{
    ifstream edges;
    edges.open("edges.bin", ios::binary);
    streampos begin, end;
    begin = edges.tellg();
    edges.seekg(0, ios::end);
    end = edges.tellg();
    edges.seekg(0, ios::beg);
    int num = (end - begin) / sizeof(int);
    cout << "the number of all edges is " << (end - begin) / sizeof(int) <<endl;
    int each_num;
    int each_edge;
    int position;
    int count = 0;
    while (edges.peek() != EOF)
    {
        //edges.read((char*)& each_edge, sizeof(int));
        //cout << each_edge << " ";
        edges.read((char*)& position, sizeof(int));
        edges.read((char* )& each_num, sizeof(int));
        adj_list each_list = adj_list(position);
        //cout << endl << "the edges for node " << position << endl;
        count ++;
        for (int i = 0; i < each_num; i++)
        {
            edges.read((char*)& each_edge, sizeof(int));
            count ++;
            //cout << each_edge << " ";
            each_list.edges.insert(each_edge);
        }
        all_edges.push_back(each_list);
        position ++;
    }
}

void generate_start_node(vector<point>& means, char* filename, int* start_node)
{
    ifstream infile(filename, ios::binary);
    int dim;
    infile.read((char*)& dim, 4);
    float data_point[dim];
    infile.seekg(0, ios::end);
    ios::pos_type ss = infile.tellg();
    size_t fsize = (size_t) ss;
    int num_points =  (unsigned)(fsize / (dim + 1) /4);
    //cout << endl << "the total number of nodes is " << num_points << " " << dim << endl;
    point each_point = point(dim);
    float min_dis = 1e09;
    float dis;
    int min_ID = -1;

    for (int i = 0; i < means.size(); i++)
    {
        min_dis = 1e09;
        min_ID = -1;
        infile.seekg(0, ios::beg);
        each_point.data.clear();
        for (int j = 0; j < num_points; j++)
        {
            infile.seekg(4, ios::cur);
            infile.read((char *)& data_point, dim*4);
            point one_point = point(dim);
            one_point.data.insert(one_point.data.begin(), data_point, data_point+dim);
            dis = getDistance(one_point, means[i]);
            //cout << setprecision(10) << dis << " ";
            if (dis < min_dis)
            {
                min_ID = j;
                min_dis = dis;
            }
        }
        start_node[i] = min_ID;
    }
}

void select_subset(vector<adj_list> adj_lists, vector<int>& selected_subset,int subset_limit, int start_node, bool* is_selected)
{
    int num_points = adj_lists.size();
    bool visited[num_points];
    for (int i = 0; i < num_points; i++)
    {visited[i] = false;}
    list <int> queue;
    queue.push_back(start_node);
    int processing_node;
    while (selected_subset.size() < subset_limit && 
    queue.size() != 0)
    {
        processing_node = queue.front();
        if (visited[processing_node] == false)
        {
            selected_subset.push_back(processing_node);
            visited[processing_node] = true;
            is_selected[processing_node] = true;
            for (set<int>::iterator it = adj_lists[processing_node].edges.begin();
            it != adj_lists[processing_node].edges.end(); it++)
            {
                queue.push_back(*it);
            }
        }
        queue.pop_front();
    }
}


/*
Generic function to find an element in vector and also its position.
It returns a pair of bool & int i.e.
 
bool : Represents if element is present in vector or not.
int : Represents the index of element in vector if its found else -1
 
*/
template < typename T>
pair<bool, int > findInVector(const vector<T>  & vecOfElements, const T  & element)
{
	pair<bool, int > result;
 
	// Find given element in vector
	auto it = find(vecOfElements.begin(), vecOfElements.end(), element);
 
	if (it != vecOfElements.end())
	{
		result.second = distance(vecOfElements.begin(), it);
		result.first = true;
	}
	else
	{
		result.first = false;
		result.second = -1;
	}

	return result;
}


void DFS(vector<adj_list> adj_lists, int processing_node, bool* visited_state, int* ancestor_list, int ancestor, vector<dfs_num>& dfs_tag)
{
    // processing_node is the physical position of node
    // it is the ID in adj_list
    int this_ancestor_num = visit_counter;
    visited_state[processing_node] = true;
    // the ancestor list should be the physical loaction or
    // the visited number?
    // should be the visited counter
    ancestor_list[this_ancestor_num] = ancestor;
    //cout << "processing node " << this_ancestor_num << " ";

    dfs_num visited_point = dfs_num(processing_node, visit_counter);
    visit_counter ++;
    // this is the position in adj list where is the 
    int position = -1;
    for (int i = 0; i < adj_lists.size(); i++)
    {
        if (processing_node == adj_lists[i].id)
        { position = i;
          break;}
    }
    
    for (set<int>::iterator it = adj_lists[position].edges.begin();
    it != adj_lists[position].edges.end(); it++)
    {
        if (visited_state[*it] == false)
        {
        DFS(adj_lists, *it, visited_state, ancestor_list, this_ancestor_num, dfs_tag);
        }
    }
    visited_point.finish_num = finish_counter;
    finish_counter++;
    dfs_tag.push_back(visited_point);
}


void tag_all_edges(vector<adj_list> adj_lists, vector<tr_edge>& all_edges, int start_node, vector <dfs_num>& dfs_tag, int location_range)
{
    //do a dfs and set the number
    //divide the clusters according to the number value
    //vector <tr_edge> all_edges;
    int * ancestor_list = new int[adj_lists.size()];
    for (int i = 0; i < adj_lists.size(); i++)
    {ancestor_list[i] = -1;}
    bool* visited_state = new bool[location_range];
    for (int i = 0; i < location_range; i++)
    {visited_state[i] = false;}
    //the start node is the physical position of a node, ancestor node is the ancestor
    //for every node, visited state is whether a node is visited, dfs_tag is the 
    //visit and finish counter for a node
    visit_counter = 0;
    finish_counter = 0;
    
    DFS(adj_lists, start_node, visited_state, ancestor_list, -1, dfs_tag);
    
    assert(adj_lists.size() == dfs_tag.size());
    int start_dfsnum, start_comnum, end_dfsnum, end_comnum;
    int start_flag, end_flag;
    // this is the location in dfs_tag for start and end node
    int start_position, end_position;
    
    for (int i = 0; i < adj_lists.size(); i++)
    {
        for (set<int>::iterator it = adj_lists[i].edges.begin(); it != adj_lists[i].edges.end();
        it++)
        {
            tr_edge per_edge = tr_edge(adj_lists[i].id, *it);
            start_flag = 0;
            end_flag = 0;
            for (int j = 0; j < dfs_tag.size(); j++)
            {
                if (dfs_tag[j].id == per_edge.start)
                {
                    start_position = j;
                    start_flag = 1;
                }
                else if (dfs_tag[j].id == per_edge.end)
                {
                    end_position = j;
                    end_flag = 1;
                }

                if (start_flag == 1 && end_flag == 1)
                    break;
            }

            if (ancestor_list[dfs_tag[end_position].visit_num] == dfs_tag[start_position].visit_num)
            {
                //it is the tree edge
                per_edge.label = 0;
            }
            else
            {
                start_dfsnum = dfs_tag[start_position].visit_num;
                start_comnum = dfs_tag[start_position].finish_num;

                end_dfsnum = dfs_tag[end_position].visit_num;
                end_comnum = dfs_tag[end_position].finish_num;
                
                if (start_dfsnum < end_dfsnum && start_comnum > end_comnum)
                {
                    per_edge.label = 1;
                }
                else if (start_dfsnum > end_dfsnum && start_comnum < end_comnum)
                {
                    per_edge.label = 2;
                }
                else if (start_dfsnum > end_dfsnum && start_comnum > end_comnum)
                {
                     per_edge.label = 3;
                }
                else
                {
                    cout << "error happen in deciding label characteristics" << endl;
                    exit(-1);
                }
            }
            all_edges.push_back(per_edge);
        }
    }
    delete [] ancestor_list;
    delete [] visited_state;
    /*
    for (int i = 0; i < all_edges.size(); i++)
    {
        cout << all_edges[i].start << " " << all_edges[i].end << " " << all_edges[i].label << " ";
    }
    cout << "total num of edges " << all_edges.size() << endl;
    */
}

void delete_front_edges(vector <tr_edge>& all_edges)
{
    srand(time(0));
    float delete_probability =front_delete_probability;
    int delete_decision;
    auto it = all_edges.begin();
    while (it != all_edges.end())
    {
        
        if ((*it).label == 1)
        {
            delete_decision = rand()%1000;
            if (delete_decision < 1000*delete_probability)
            {
                it = all_edges.erase(it);
            }
            else
            {++it;}
        }
        else
        {++it;}
    }
}

void update_back_edges(vector <tr_edge>& all_edges, map<int, int>& backpoints, vector<dfs_num> dfs_tag)
{
    int back_position, end_position;
    int back_flag, end_flag;
    float delete_probability = back_delete_probability;
    int delete_decision;
    srand(time(0));

    for (int i = 0; i < all_edges.size(); i++)
    {
        if (all_edges[i].label == 2)
        {
            back_flag = 0;
            end_flag = 0;
            for (int j = 0; j < dfs_tag.size(); j++)
            {
                if (dfs_tag[j].id == backpoints[all_edges[i].start])
                {
                    back_position = j;
                    back_flag = 1;
                }
                else if (dfs_tag[j].id == all_edges[i].end)
                {
                    end_position = j;
                    end_flag = 1;
                }

                if (back_flag == 1 && end_flag == 1)
                    break;
            }
            if (dfs_tag[end_position].visit_num < dfs_tag[back_position].visit_num)
                backpoints[all_edges[i].start] = all_edges[i].end;
        }
    }
    auto it = all_edges.begin();
    while (it != all_edges.end())
    {
        if ((*it).label == 2)
        {
            if (backpoints[(*it).start] != (*it).end)
            {
                delete_decision = rand()%1000;
                if (delete_decision < 1000*delete_probability)
                {it = all_edges.erase(it);}
                else
                {it++;}
            }
            else
            {it++;}
        }
        else
        {it++;}
    }
}


void merge_sub_graph(vector<adj_list>& final_graph, vector<tr_edge> all_edges)
{
    for (int i = 0; i < all_edges.size(); i++)
    {
        final_graph[all_edges[i].start].edges.insert(all_edges[i].end);
    }
}

void transitive_reduction(char* filename)
{
    cout << segement_line << endl;
    cout << "Start pruning with transitive reduciton " << endl;
    cout << "the probability for deleting forward edges: " << front_delete_probability << endl;
    cout << "the probability for deleting back edges: " << back_delete_probability << endl;
    vector<point> means;
    read_centroids(means);
    vector<adj_list> adj_lists;
    read_edges(adj_lists);
    int num_points = adj_lists.size();

    //load the edge in an .bin file and reconstruct the adj list to edge set
    int num_centroids = means.size();
    int start_node[num_centroids];
    generate_start_node(means, filename, start_node);
    cout << endl << "the selected start node for all centroids are: ";
    for (int i = 0; i < num_centroids; i++)
    cout << start_node[i] << " ";
    cout << endl;

    
    float subset_proportion = 0.2;
    bool is_selected [num_points];
    for (int i = 0; i < num_points; i++)
    {   is_selected[i] = false; }
    
    vector <int> selected_subset;
    vector <adj_list> sub_adj_list;
    vector<tr_edge> all_edges;
    vector <dfs_num> dfs_tag;
    vector <adj_list> final_graph;
    for (int i = 0; i < num_points; i++)
    {
        adj_list each_node = adj_list(i);
        final_graph.push_back(each_node);
    }
    for (int i = 0; i < num_centroids; i++)
    {
        selected_subset.clear();
        sub_adj_list.clear();
        select_subset(adj_lists, selected_subset, int(subset_proportion*num_points), 
        start_node[i], is_selected);
        
        for (int j = 0; j < selected_subset.size(); j++)
        {
            adj_list each_list = adj_list(selected_subset[j]);
            assert (adj_lists[selected_subset[j]].id == selected_subset[j]);
            for (set<int>::iterator it = adj_lists[selected_subset[j]].edges.begin();
            it != adj_lists[selected_subset[j]].edges.end(); it++)
            {
                auto search_result = find(selected_subset.begin(), selected_subset.end(), *it);
                if (search_result != selected_subset.end())
                {
                    each_list.edges.insert(*it);
                }
            }
            sub_adj_list.push_back(each_list);
        }


        
        all_edges.clear();
        dfs_tag.clear();
        tag_all_edges(sub_adj_list, all_edges, start_node[i], dfs_tag, num_points);
        cout << "Pruning " << i+1 << " th cluster " << endl;
        cout << "the total number of edges is " << all_edges.size() << endl;

        delete_front_edges(all_edges);
        cout << "the total number of all edges after deleting forward edges is " << all_edges.size() << endl;

        
        map<int, int> backpoints;
        map<pair<int, int>, pair<int, int>> standfor;

        for (int j = 0; j < sub_adj_list.size(); j++)
        {
            backpoints[sub_adj_list[j].id] = sub_adj_list[j].id;
            for (set<int>:: iterator it = sub_adj_list[j].edges.begin();
            it != sub_adj_list[j].edges.end(); it++)
            {
                pair<int, int> one_edge (sub_adj_list[j].id, *it);
                standfor[one_edge] = one_edge;
            }
        }
        update_back_edges(all_edges, backpoints, dfs_tag);
        cout << "the total number of all edges after deleting back edges is " << all_edges.size() << endl;

                

        merge_sub_graph(final_graph, all_edges);
    }

    //compute the total edges in the final graph
    int count = 0;
    for (int i = 0; i < num_points; i++)
    {
        count += final_graph[i].edges.size();
    }
    cout << "merged graph with edges " << count << endl;


    count = 0;
    for (int i = 0; i < num_points; i++)
    {
        if (is_selected[i] == false)
            count ++;
    }
    cout << count << " unvisited nodes in total " << endl;

    
    for (int i = 0; i < num_points; i++)
    {
        if (is_selected[i] == false)
        {
            for (set<int>::iterator it = adj_lists[i].edges.begin();
            it != adj_lists[i].edges.end(); it++)
            {
                final_graph[i].edges.insert(*it);
                final_graph[*it].edges.insert(i);
            }
        }
    }



    // check the connectivity of final graph
    cout << "check the connectivity of tha final graph ... " << endl;
    bool connectivity_check_passed = true;
    bool visited[num_points];
    for (int j = 0; j < num_points; j++)
    {visited[j] = false;}
    list <int> queue;
    queue.push_back(final_graph[rand()%(final_graph.size())].id);
    int processing_node;
    int position;
    while (queue.size() != 0)
    {

    processing_node = queue.front();
    for (int j= 0; j < final_graph.size(); j++)
    {
        if (final_graph[j].id == processing_node)
        {
            position = j;
            break;
        }
    }
    if (visited[processing_node] == false)
    {
        assert (final_graph[position].id == processing_node);
        visited[processing_node] = true;
        for (set<int>::iterator it = final_graph[position].edges.begin();
        it != final_graph[position].edges.end(); it++)
        {
            queue.push_back(*it);
        }
    }
    queue.pop_front();
    }
    for (int i = 0; i < final_graph.size(); i++)
    {
        if (visited[i] == false)
        {
            cout << "Connectivity check not passed ! " << endl;
            connectivity_check_passed = false;
            break;
        }    
    }
    if (connectivity_check_passed == true)
        cout <<"Connectivity check passed " << endl;

    //the out put part to output the edges
    int num_all_edges = 0;
    cout << "Saving all edges to edges_final.bin and edges_final.txt" << endl;
    ofstream edge_output;

    edge_output.open("edges_final.bin", ios::binary);

    ofstream txt_output;
    txt_output.open("edges_final.txt");
    int length;

    edge_output.write((char* )& num_centroids, sizeof(int));
    int centroid_location;
    for (int i = 0; i < num_centroids; i++)
    {
        centroid_location = start_node[i];
        edge_output.write((char*) & centroid_location, sizeof(int));
    }

    for (int i = 0; i < final_graph.size(); i++)
    {
        length = final_graph[i].edges.size();
        edge_output.write((char*)& i, sizeof(int));
        edge_output.write((char*)& length, sizeof(int));
        txt_output << endl << i << "  " << length << endl;
        for (set<int>::iterator j = final_graph[i].edges.begin();
         j != final_graph[i].edges.end(); j++)
        {
            edge_output.write((char*)& (*j), sizeof(int));
            txt_output << *j << " ";
        }
        num_all_edges += final_graph[i].edges.size();
    }
    edge_output.close();
    cout << "File saved successfully " << endl;
    cout << "The total size for this graph is " << num_all_edges << endl;

}
    



    
    
    /*
    map<int, int> backpoints;
    map(pair<int, int>, pair<int, int>) standfor;


    for (int i = 0; i < adj_lists.size(); i++)
    {
        backpoints[adj_lists[i].id] = adj_lists[i].id;
        for (set<int> iterator j = adj_lists[i].edges.begin();
         j != adj_lists[i].edges.end(); j++)
        {
            standfor[pair<adj_lists[i].id, *j>] = pair<adj_lists[i].id, *j>;
        }
    }

    update_back_point(all_edges, dfs_tag);
    update_cross_edges(all_edges, dfs_tag);
    
    R-test(cluster_edges);
    for all edges in standfor:
        replace
    */



// test whether the sub_adj_list is strong connected
        /*
        bool visited[num_points];
        for (int j = 0; j < num_points; j++)
        {visited[j] = false;}
        list <int> queue;
        queue.push_back(start_node[i]);
        int processing_node;
        int position;
        int loop_counter = 0;
        while (queue.size() != 0)
        {

        processing_node = queue.front();
        for (int j= 0; j < sub_adj_list.size(); j++)
        {
            if (sub_adj_list[j].id == processing_node)
            {
                position = j;
                break;
            }
        }
        if (visited[processing_node] == false)
        {
            loop_counter++;
            assert (sub_adj_list[position].id == processing_node);
            visited[processing_node] = true;
            for (set<int>::iterator it = sub_adj_list[position].edges.begin();
            it != sub_adj_list[position].edges.end(); it++)
            {
                queue.push_back(*it);
            }
        }
        queue.pop_front();
        }
        cout << "loop_counter is " << loop_counter << endl;
        for (int j = 0; j < sub_adj_list.size(); j++)
        {
            if (visited[sub_adj_list[j].id] == false)
                cout << "wrong" << " " << sub_adj_list[j].id << " ";       
        }
        int counter = 0;
        for (int j = 0; j < num_points; j++)
        {
            if (visited[j] == false)
                counter ++;
        }
        cout << "the sum of counetr is " << counter << " " << num_points << endl;;
        cout << "the sum of sun_adj_list " << sub_adj_list.size();
        exit(0);
        */
        
