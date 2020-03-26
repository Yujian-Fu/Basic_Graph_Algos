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
    int dim;
    ifstream edge_output;
    edge_output.open("/home/y/yujianfu/similarity_search/datasets/ANN_SIFT1B/1milliard.p1.siftbin");
    streampos begin, end;
    begin = edge_output.tellg();
    edge_output.read((char*) & dim, 4);
    cout << "The dimension of this dataset is " << dim << endl;

    edge_output.seekg(0, ios::end);


    end = edge_output.tellg();


    int num = (size_t) end / sizeof(int);
    cout << "the size of all edges is " << num << "edges. \n" << endl;
    float each_num[dim];
    //while(!edge_output.eof())
    edge_output.seekg(0, ios::beg);
    for (int i = 0; i < 200; i++)
    {
        edge_output.seekg(4, ios::cur);
        edge_output.read((char *)& each_num, sizeof(float));
        /*cout << "this node has " << each_num << " edges " <<endl;
        for (int i = 0; i < each_num; i++)
        {
            edge_output.read((char*) & each_num, sizeof(int));
            cout << each_num << " ";
        }
        cout << endl;*/
        cout << each_num[0] << "  ";
    }
}


int main()
{
    //file_write();
    file_read();  
}

