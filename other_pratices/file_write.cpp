#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;


void file_write()
{
    ofstream edge_output;
    edge_output.open("edges.bin", ios::binary);
    srand(time(0));
    for (int i = 0; i< 100; i++)
    {
        int length = 50;
        int * list = new int[length];
        for (int j = 0; j < length; j++)
        {
            list[j] = j;
        }

        edge_output.write((char*)&length, sizeof(int));

        for (int j = 0; j < length; j++)
        {
            edge_output.write((char*)&list[j], sizeof(int));
        }
    }

    edge_output.close();
}

void file_read()
{
    ifstream edge_output;
    edge_output.open("/home/y/yujianfu/similarity_search/datasets/ANN_SIFT1B/1milliard.p1.siftbin");
    streampos begin, end;
    begin = edge_output.tellg();
    edge_output.seekg(0, ios::end);
    end = edge_output.tellg();
    int num = (end - begin) / sizeof(int);
    cout << "the size of all edges is " << (end - begin)/sizeof(int) << "edges. \n" << endl;
    int each_num;
    edge_output.seekg(0, ios::beg);
    //while(!edge_output.eof())
    for (int i = 0; i < 100; i++)
    {
        edge_output.read((char *)& each_num, sizeof(int));
        /*cout << "this node has " << each_num << " edges " <<endl;
        for (int i = 0; i < each_num; i++)
        {
            edge_output.read((char*) & each_num, sizeof(int));
            cout << each_num << " ";
        }
        cout << endl;*/
        cout << each_num << "  ";
    }

}


int main()
{
    //file_write();
    file_read();  
}

