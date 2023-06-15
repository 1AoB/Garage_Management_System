#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;


using namespace cv;



int  main(){

	VideoCapture cam;
	cam.open(0);

	if(!cam.isOpened()){
		cout<<"Failed to open camera!"<<endl;
		return -1;
	}
	cout<<"Success to open camera!"<<endl;
	return 0;
}





