#include <iostream>

using namespace std;

int main(){
    for (int i = 0; i < 2; i ++)
    {
        float pointer[10];

        cout << " the pointer location is " << pointer << endl;
        for (int j = 0; j < 10; j++)
        {
            cout << pointer[j] << " ";
        }
        cout << endl;


        for (int j = 0; j < 10; j++)
        {
            pointer[j] = j;
        }
    }
}