#include<iostream>
#include<fstream>

using namespace std;

int main() {

 ifstream myReadFile;
 myReadFile.open("data.txt");
 float output;
 if (myReadFile.is_open()) {
 while (!myReadFile.eof()) {


    myReadFile >> output;
    cout<<output;


 }
}
myReadFile.close();
return 0;
}