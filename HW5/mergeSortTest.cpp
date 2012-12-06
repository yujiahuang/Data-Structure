#include <iostream>
using namespace std;

void mergeSort(int* data, size_t size);

int size=11;
typedef int T;

int main(){

int testArray[]={5, 10, 11, 125, 456, 1, 0, -48, 66, 120};
for(int i=0; i<size; i++){

cout << testArray[i] << " ";

}
cout << endl;
mergeSort(testArray, size);
for(int i=0; i<size; i++){

cout << testArray[i] << " ";

}

return 0;

}

		//swap
		void swap(T& a, T& b){

			T tmp = a;
			a=b;
			b=tmp;

		}

		// merge sort TODO
		void mergeSort(T* data, size_t size){
			if(size<2);	
			else if(size==2){

				if(data[1]<data[0]) swap(data[0], data[1]);

			}
			else{

				size_t leftS=size/2;
				size_t rightS=size-leftS;
				T left[leftS], right[rightS];

				//copy
				for(size_t i=0; i<leftS; i++){

					left[i]=data[i];

				}
				for(size_t j=0; j<rightS; j++){

					right[j]=data[j+leftS];

				}

				//recursive
				mergeSort(left, leftS);
				mergeSort(right, rightS);

				//merge
				size_t leftI=0, rightI=0, i=0;
				while(i<size && leftI<leftS && rightI<rightS){

					if(left[leftI]<right[rightI]){

						data[i]=left[leftI];
						leftI++;

					}
					else{

						data[i]=right[rightI];
						rightI++;

					}
					i++;

				}
				while(i<size && leftI<leftS){
	
					data[i]=left[leftI];
					leftI++;
					i++;

				}
				while(i<size && rightI<rightS){

					data[i]=right[rightI];
					rightI++;
					i++;

				}

			}
	
			for(size_t mm=0; mm<size; mm++) cout << data[mm] << " ";
			cout << endl;	
		}
		
