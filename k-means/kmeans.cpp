#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>

#define k 3
#define dimension 2

using namespace std;

struct  Tuple
{
    int dim;
    float * attrs;
    Tuple (int dim = dimension)
    {
        this->dim = dim;
        this->attrs = new float[dim];
        for (int i= 0; i < this->dim; i++)
            this->attrs[i] = 0;
    }
};


float getDistXY(Tuple t1, Tuple t2)
{
    float dis = 0;
    for (int i = 0; i < t1.dim; i++)
    {
        dis += (t1.attrs[i] - t2.attrs[i]) * (t1.attrs[i] - t2.attrs[i]);
    }

    return sqrt(dis);
}

int clusterOfTuple(Tuple means[], Tuple tuple)
{
    float dist = getDistXY(means[0], tuple);
    float tmp;
    int label = 0;
    for (int i = 1; i < k; i++)
    {
        tmp = getDistXY(means[i], tuple);
        if (tmp < dist)
        {
            dist = tmp;
            label = i;
        }
    }
    return label;
}

float getVar(vector<Tuple> clusters[], Tuple means[])
{
    float var = 0;
    for (int i = 0; i <k; i++)
    {
        vector<Tuple> t = clusters[i];
        for (int j = 0; j < t.size(); j ++)
        {
            var += getDistXY(t[j], means[i]);
        }
    }

    return var;
}

Tuple getMeans(vector<Tuple> cluster)
{
    int num = cluster.size();
    Tuple t;
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < t.dim; j++)
            t.attrs[j] += cluster[i].attrs[j];
    }

    for (int j = 0; j < t.dim; j++)
        t.attrs[j] /= num;

    return t;
}

void show_process(vector<Tuple> clusters[], Tuple means[])
{
    int label, i, j;
    for (label = 0; label < k; label ++)
    {
    cout << "this is the " << label+1 << " th cluster with mean" << endl;
    for (i = 0; i < means[label].dim; i++)
    {
        cout << means[label].attrs[i] << "  ";
    }
    cout << endl;
    vector<Tuple> t = clusters[label];
    for (i = 0; i < t.size(); i++)
    {
        for (j= 0; j < t[i].dim; j++)
            cout << t[i].attrs[j] << ",  ";
        cout << endl;
    }
    cout << endl;
}
}

void output_txt(vector<Tuple> clusters[])
{
    int label, i, j;
    for (label = 0; label < k; label ++)
    {
    cout << "this is the " << label+1 << " th cluster with mean" << endl;
    cout << endl;
    vector<Tuple> t = clusters[label];
    cout << "test sucessful" << endl;
    for (i = 0; i < t.size(); i++)
    {
        for (j= 0; j < t[i].dim; j++)
            cout << t[i].attrs[j] << ",  ";
        cout << endl;
    }
    cout << endl;
}
}

vector<Tuple>* KMeans(vector<Tuple*> tuples)
{
    vector<Tuple> * clusters = new vector<Tuple> [k];
    Tuple means[k];
    int i = 0;

    for (i = 0; i < k; i++)
    {
        for (int j = 0; j < means[i].dim; j++)
        {
            means[i].attrs[j] = tuples[i]->attrs[j];
            cout << means[i].attrs[j] << " ";
        }
        cout << endl;
    }

    int label = 0;

    for (i = 0; i!= tuples.size(); i++)
    {
        label = clusterOfTuple(means, *tuples[i]);
        cout << label << endl;
        clusters[label].push_back(*tuples[i]);
    }

    show_process(clusters, means);

    float odlVar = -1;
    float newVar = getVar(clusters, means);
    while(abs(newVar - odlVar) >= 1)
    {
        for (i = 0; i < k; i++)
        {
            means[i] = getMeans(clusters[i]);
        }

        odlVar = newVar;
        newVar = getVar(clusters, means);

        for (i = 0; i < k; i++)
        {
            clusters[i].clear();
        }

        for (i = 0; i != tuples.size(); i++)
        {
            label = clusterOfTuple(means, *tuples[i]);
            clusters[label].push_back(*tuples[i]);
        }

        show_process(clusters, means);
    }

    output_txt(clusters);
    return clusters;
}



int main()
{
    /*
    char fname[256];
    cout << "please enter the file name you want to save the data" << endl;
    cin >> fname;
    */
    
    ifstream infile;
    infile.open("./data.txt");
    if(!infile){
        cout << "file open fails" <<endl;
        return 0;
    }
    else
    {
    cout << "file open successly" << endl;
    int count = 0;
    vector <Tuple*> tuples;
    Tuple * tuple = new Tuple;
    float output = 0;
    while (!infile.eof()){
        infile >> tuple->attrs[count%tuple->dim];
        if (count%tuple->dim == 0 && count != 0)
        {
            tuples.push_back(tuple);
            for (int i= 0; i < tuple->dim; i++)
                cout << tuple->attrs[i] << " ";
            cout << count << endl;
            tuple = new Tuple;
        }
        count ++;
    }


    for (int ix = 0; ix != tuples.size(); ix ++)
    {
        cout << "(";
        for (int j = 0; j < tuples[ix]->dim; j++)
            cout << tuples[ix]->attrs[j] << "," ;
        cout << ")" << endl;
    }
    
    vector<Tuple>* clusters = KMeans(tuples);
    vector<Tuple> cluster = clusters[1];
    cout << "test successful" << endl;
    cout << "the cluster size is " << cluster.size() << endl;
    }
    return 0;
}


