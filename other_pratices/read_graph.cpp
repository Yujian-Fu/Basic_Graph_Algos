#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream graph_file;
    graph_file.open("/home/yujian/Desktop/efanna_graph/tests/gist.100NN.graph");
    streampos begin, end;
    begin = graph_file.tellg();
    graph_file.seekg(0, ios::end);
    end = graph_file.tellg();
    graph_file.seekg(0, ios::beg);

    int num = (end - begin) / (sizeof(int));

    cout << "The total num of this graph file is " << num << endl;

    int each_num;
    for (int i= 0; i < 5; i++)
    {
        graph_file.read((char *)& each_num, sizeof(int));
        cout << each_num << " ";
    }


}