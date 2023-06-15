#include<iostream>
#include"bubble_sort.h"


using namespace std;

void Print(int arr[],int len){
	for(int i = 0 ; i < len ; i++)
	{
		cout<<arr[i]<<' ';
	}
	cout<<endl;
}

int main(){
	int arr[] = {7,3,8,9,10,12,1,5,6,7,6};

	int len = sizeof(arr)/sizeof(int);

	printf("before:\n");
	Print(arr,len);

	bubble_sort(arr,len);
	printf("after:\n");
	Print(arr,len);

	return 0;
}
