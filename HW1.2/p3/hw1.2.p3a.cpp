#include <iostream>
#include <vector>
using namespace std;

bool compare(int a, int b){

	return (a<b);

}

void mySwap(int a, int b){

	int tmp=a;
	a=b;
	b=a;

}

//
//DO NOT CHANGE FOLLOWING
//
void selectionSort(vector<int>& array){

	for (size_t i = 0, n = array.size(); i < n - 1; ++i) {
	
		size_t pivot = i;
		for (size_t j = i+1; j < n; ++j) {
	
			if (!compare(array[pivot], array[j])) pivot = j;

		}

		if (pivot != i)	mySwap(array[pivot], array[i]);
	
	}

}
//
//DO NOT CHANGE ABOVE
//

int main(){

	int num;
	cout << "How many numbers? ";
	cin >> num;
	vector<int> numArr(num);

	for(int i=0; i<num; i++){
	
		cin >> numArr[i];
	
	}

	cout << "Before sort:" << endl;
	for(int i=0; i<num; i++){
	
		cout << numArr[i] << " ";
	
	}

	selectionSort(numArr);

	cout << "\nAfter sort:" << endl;
	for(int i=0; i<num; i++){
	
		cout << numArr[i] << " ";
	
	}

	return 0;

}

