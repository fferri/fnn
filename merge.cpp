#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	ifstream a(argv[1]), b(argv[2]);
	string la1, la2, lb1, lb2;
	while(getline(a, la1) && getline(a, la2) && getline(b, lb1) && getline(b, lb2)) {
		cout << la1 << endl << la2 << endl << lb1 << endl << lb2 << endl;
	}
	while(getline(a, la1) && getline(a, la2)) {
		cout << la1 << endl << la2 << endl;
	}
	while(getline(b, lb1) && getline(b, lb2)) {
		cout << lb1 << endl << lb2 << endl;
	}
}
