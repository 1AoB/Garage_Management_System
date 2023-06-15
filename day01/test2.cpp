#include<iostream>
#include<opencv2/opencv.hpp>


using namespace std;
using namespace cv;


int main(){
	VideoCapture cam;

	cam.open(0);

	if(!cam.isOpened()){
		cout<<"Failed to open camera!"<<endl;
		exit(-1);
	}

	cout<<"Success to open camera!"<<endl;

	Mat image;
	cam.read(image);
	if(image.empty()){
		cout<<"read picture is empty!"<<endl;
		exit(-2);
	}
	imshow("OpenCv",image);
		
	imwrite("./image.jpg",image);

	waitKey(0);
	return 0;

}

