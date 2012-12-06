#include <iostream>
using namespace std;

int _data[]={1, 3, 5, 7, 13, 141, 1982, 10000};
size_t _size=8;

int binarySearch(const int x){

	int high=_size-1, low=0;
	while(high>=low){

		if(x==_data[(high+low)/2]) return high+low/2;
		else if(x<_data[(high+low)/2]){

			cout << "< high, low: " << high << ", " << low << endl;
			high=(high+low)/2-1;


		}
		else{

			cout << "> high, low: " << high << ", " << low << endl;
			low=(high+low)/2+1;

		}

	}
	return -1;

}

int main(){

	cout << "\n";
	cout << binarySearch(5) << endl;
	cout << binarySearch(12) << endl;
	return 0;

}

