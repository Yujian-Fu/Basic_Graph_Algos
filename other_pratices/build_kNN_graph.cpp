#include <bits/stdc++.h>

using namespace std;

class Point
{
    public:
    int ID;
    double x, y;

    Point(int ID, double x, double y)
    {
        this->ID = ID;
        this->x = x;
        this->y = y;
    }
};

double* classifyAPoint(Point arr[], int n, int k, Point p)
{
    double distance[n];
    for (int i = 0; i < n; i++)
        distance[i] = sqrt((arr[i].x - p.x)*(arr[i].x - p.x) +
                       (arr[i].y - p.y) * (arr[i].y - p.y)); 
    sort(distance, distance+n);
    return distance;
}

int main()
{
    int n = 3;
    Point arr[10];
    arr[0] = Point(0, 2.0, 3.0);
    arr[1] = Point(1, -1.5, 4.3);
    arr[2] = Point(2, 0.3, 7.6);
    
}

