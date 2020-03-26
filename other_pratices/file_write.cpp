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

    int8_t a = 18;
     cout << "The value of a is " << (int) a << endl;
    edge_output.seekg(0, ios::end);
    ios::pos_type ss = edge_output.tellg();
    size_t fsize = (size_t) ss;
    int num =  (unsigned)(fsize / (dim + 1) /sizeof(uint8_t));

    cout << "the size of all edges is " << num << "edges. \n" << endl;
    uint8_t each_num[dim];
    //while(!edge_output.eof())
    edge_output.seekg(0, ios::beg);
    for (int i = 0; i < 200; i++)
    {
        edge_output.seekg(4, ios::cur);
        edge_output.read((char *) each_num, dim*sizeof(uint8_t));
        /*cout << "this node has " << each_num << " edges " <<endl;
        for (int i = 0; i < each_num; i++)
        {
            edge_output.read((char*) & each_num, sizeof(int));
            cout << each_num << " ";
        }
        cout << endl;*/
        cout << (int) each_num[0] << "  ";
    }
}

void file_transfer()
{
    int dim;
    ofstream fvecs;
    fvecs.open("/home/y/yujianfu/similarity_search/datasets/ANN_SIFT1B/SIFT1B.fvecs");
    ifstream bvecs;
    bvecs.open("/home/y/yujianfu/similarity_search/datasets/ANN_SIFT1B/1milliard.p1.siftbin");
    bvecs.read((char*) & dim, 4);
    bvecs.seekg(0, ios::beg);
    uint8_t feature;
    float feature_int;
    int count = 0;

    while(bvecs.peek() != EOF)
    {
        count ++;
        bvecs.seekg(4, ios::cur);
        fvecs.write((char*) & dim, sizeof(int));
        for (int i = 0; i < dim; i++)
        {
            bvecs.read((char*) & feature, sizeof(uint8_t));
            feature_int = (float) feature;
            fvecs.write((char*) & feature_int, sizeof(float));
        }
        if ((count % 100000) == 0)
            cout << "Finish " << count << "instances " << endl;
    }
    fvecs.close();
    bvecs.close();

}

int main()
{
    //file_write();
    //file_read(); 
    file_transfer(); 
}

