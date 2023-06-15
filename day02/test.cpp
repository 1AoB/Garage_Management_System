#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>
#include<map>

#include"aip-cpp-sdk/ocr.h"

using namespace std;
using namespace cv;
const string app_id = "32714526";
const  string api_key = "pZkwfkgnDUAa5Ag69LoSN1oe";
const string secret_key = "4PKHvy7ZKSTF3mYPcj8CvXyXZ7v03QE0";

int num = 0;

//车牌识别的函数
/*
 * 读取一帧图像,保存成文件
 * */
int capture_image(){
	//定义一个摄像头
	VideoCapture cam;

	//打开摄像头
	cam.open(0);

	if(!cam.isOpened())
	{
		cout<<"Failed to open camera!"<<endl;
		exit(-1);
	}
	cout<<"Success to open canmera!"<<endl;

	//图片用矩阵来存
	Mat image;

	//从摄像头中读取一帧图像
	cam.read(image);

	if(image.empty())
	{
		cout<<"imaage is empty!"<<endl;
		exit(-2);
	}
	
	/*string save_name = "./in/image";
	save_name += to_string(num++);
	save_name += ".jpg";
	//imwrite("./image.jpg",image);
	imwrite(save_name,image);*/
	imwrite("./image.jpg",image);
	return 0;
}

int main()
{
	int ret = capture_image();
	if(ret != 0){
		cout<<"Failed to save image!"<<endl;
		return -1;
	}
	aip::Ocr client(app_id,api_key,secret_key);

	string image;
	aip::get_file_content("image.jpg",&image);

	map<string,string>options;
	//不支持检测多车牌
	options["multi_detect"]="false";
	//车牌图像发给百度智能云做车牌识别
	Json::Value result =  client.license_plate(image,options);
	//Json数据格式
	//pkg-config --cflags jsoncpp
	//Json 的目录 -I/usr/include/jsoncpp	
	cout<<result.toStyledString()<<endl;

	if(!result["error_code"].isNull()){
		cout<<"error_code:"<<result["error_code"] <<endl;
		cout<<"未识别到车牌号!"<<endl;
		cout<<"蜂鸣器响6秒"<<endl;
		return -1;
	}
	

	//识别成功才写入文件
	//如果是入场,写入in文件 //需要传入一个标识符,是入场还是出厂
	//string save_name = "./in/image";
	//save_name += to_string(num++);
	//save_name += ".jpg";
	//imwrite(save_name,image);
	//如果是出厂,写入out文件
	
	string plate = result["words_result"]["number"].asString();
	cout<<plate<<endl;
	cout<<result["words_result"]["number"]<<endl;
	return 0;
}

//g++ test.cpp -I/usr/include/jsoncpp -lopencv_core -lopencv_imcodecs -lcurl
