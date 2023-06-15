#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "db.h"

#include <opencv2/opencv.hpp>
#include <map>
#include "aip-cpp-sdk/ocr.h"

#include <cstring>
#include <ctime>
#include <sstream>

using namespace std;
using namespace cv;
const string app_id = "32714526";
const string api_key = "pZkwfkgnDUAa5Ag69LoSN1oe";
const string secret_key = "4PKHvy7ZKSTF3mYPcj8CvXyXZ7v03QE0";

int num = 0;
char *max_id;
std::tm parse_datetime(const char *datetime_str)
{ // 解析日期时间字符串,返回对应的时间结构体
    std::tm tm = {};
    std::istringstream ss(datetime_str);
    ss >> tm.tm_year;   // 年份
    ss.ignore();        // 忽略分隔符 '-'
    ss >> tm.tm_mon;    // 月份
    ss.ignore();        // 忽略分隔符 '-'
    ss >> tm.tm_mday;   // 日
    ss.ignore();        // 忽略分隔符 ' '
    ss >> tm.tm_hour;   // 时
    ss.ignore();        // 忽略分隔符 ':'
    ss >> tm.tm_min;    // 分
    ss.ignore();        // 忽略分隔符 ':'
    ss >> tm.tm_sec;    // 秒
    tm.tm_year -= 1900; // 年份需要减去 1900
    tm.tm_mon -= 1;     // 月份需要减去 1
    return tm;
}

int diff_datetime(const char *datetime1, const char *datetime2)
{
    // 解析日期时间字符串，转换为时间戳
    std::tm tm1 = parse_datetime(datetime1);
    std::tm tm2 = parse_datetime(datetime2);
    std::time_t t1 = std::mktime(&tm1);
    std::time_t t2 = std::mktime(&tm2);
    // 计算时间差
    int diff = std::difftime(t2, t1);
    return diff;
}

string string_time()
{

    time_t now;
    time(&now);

    struct tm *tm_time = localtime(&now);

    char buf[256] = {0};
    strftime(buf, sizeof(buf), "%4Y-%02m-%02d %02H:%02M:%02S", tm_time);

    return string(buf);
}

void do_insert(string plate)
{
    cout << "Now do insert!" << endl;

    // cout<<"Please input license plate"<<endl;
    // cin >> plate;

    string str_time = string_time(); // now time

    char sql[512] = {0};
    sprintf(sql,
            "insert into parking values(NULL,'%s','%s','%s');",
            str_time.c_str(), // now time = car in time
            "",               // car out time
            plate.c_str());
    int ret = exec_db(sql);
    if (ret != 0)
    {
        cout << "Failed to  do_insert" << endl;
    }
    cout << "Congratulate success to insert to Datebase!" << endl;
}

int parking_callback(void *para, int f_num, char **f_value, char **f_name)
{
    cout << "parking_callback is used" << endl;
    if (f_value[0] == nullptr)
    {
        cout << "<parking.cpp> parking_callback f_value[0] is null" << endl;
        return -1;
    }
    max_id = f_value[0];
    cout << f_name[0] << ":" << f_value[0] << endl;
    return 0;
}

char inTime[64] = {0};
char outTime[64] = {0};

int parking_callback1(void *para, int f_num, char **f_value, char **f_name)
{
    cout << "parking_callback1 is used" << endl;
    if (f_value[0] == nullptr)
    {
        cout << "<parking.cpp> parking_callback f_value[0] is null" << endl;
        return -1;
    }

    cout << f_name[0] << ":" << f_value[0] << endl;
    if (strcmp(f_name[0], "in_time") == 0)
    {
        cout << "字段是" << f_name[0] << endl;
        strcpy(inTime, f_value[0]);
    }
    if (strcmp(f_name[0], "out_time") == 0)
    {

        cout << "字段是" << f_name[0] << endl;
        strcpy(outTime, f_value[0]);
    }
    return 0;
}

void do_update(string plate)
{
    char id[64] = {0}; // char *id;
    cout << "Now do update!" << endl;

    string str_time = string_time(); // now time

    char sql[512] = {0};
    sprintf(sql,
            "select max(id) from parking where license_plate='%s';",
            plate.c_str());
    int ret = query_db(sql, parking_callback, nullptr);
    if (ret != 0)
    {
        cout << "Failed to do_query" << endl;
        return;
    }
    strcpy(id, max_id);
  //  cout << "max_id = " << max_id << "111111111111111111111111" << endl;
  //  cout << "id = " << id << "11111111111111111111111111111" << endl;
    memset(sql, 0, sizeof(sql));

    string now_time = string_time();
    sprintf(sql,
            "update parking set out_time ='%s' where id = '%s';",
            now_time.c_str(),
            id);
    ret = query_db(sql, parking_callback, nullptr);
    if (ret != 0)
    {
        cout << "Failed to do_query" << endl;
        return;
    }
   // cout << "max_id = " << max_id << "22222222222222222222222222222" << endl;
  //  cout << "id = " << id << "2222222222222222222222222222222" << endl;

    memset(sql, 0, sizeof(sql));
    cout << "id:" << id << endl;
    sprintf(sql,
            "select in_time from parking where id = '%s';",
            id);
    ret = query_db(sql, parking_callback1, nullptr);
  //  cout << sql << endl;
   // cout << "2222222222222222222" << endl;
    if (ret != 0)
    {
        cout << "Failed to do_query" << endl;
        return;
    }

    memset(sql, 0, sizeof(sql));
   // cout << "id:" << id << endl;
    sprintf(sql,
            "select out_time from parking where id = '%s';",
            id);
    ret = query_db(sql, parking_callback1, nullptr);
   // cout << sql << endl;
   // cout << "333333333333333333333" << endl;
    if (ret != 0)
    {
        cout << "Failed to do_query" << endl;
        return;
    }
    cout << inTime << "," << outTime << endl;
    int seconds = diff_datetime(inTime, outTime);
    cout << "在车库中待了" << seconds << "seconds,应收费" << seconds * 0.1 << "块" << endl;
    sleep(6);
}

void do_query()
{
    cout << "Now do query!" << endl;
    string plate;
    cout << "Please input license plate" << endl;
    cin >> plate;

    char sql[512] = {0};
    sprintf(sql,
            "select max(id) from parking where license_plate='%s';",
            plate.c_str());

    int ret = query_db(sql, parking_callback, nullptr);

    if (ret != 0)
    {
        cout << "<parking.cpp> Failed to do_query" << endl;
    }
}

// 车牌识别的函数
/*
 * 读取一帧图像,保存成文件
 * */
int capture_image(int command, string &picture)
{
    // 定义一个摄像头
    VideoCapture cam;

    // 打开摄像头
    cam.open(0);

    if (!cam.isOpened())
    {
        cout << "Failed to open camera!" << endl;
        exit(-1);
    }
    cout << "Success to open canmera!" << endl;

    // 图片用矩阵来存
    Mat image;

    // 从摄像头中读取一帧图像
    cam.read(image);

    if (image.empty())
    {
        cout << "imaage is empty!" << endl;
        exit(-2);
    }

    // 如果是入场,写入in文件
    if (command == 1)
    {
        string save_name = "./in/image";
        save_name += to_string(num++);
        save_name += ".jpg";
        picture = save_name;
        imwrite(save_name, image);
    }
    else if (command == 0)
    {
        string save_name = "./out/image";
        save_name += to_string(num++);
        save_name += ".jpg";
        picture += save_name;
        imwrite(save_name, image);
    }

    return 0;
}

// 蜂鸣器
void tone(int hz)
{
    int fd = open("/dev/input/event0", O_WRONLY);
    if (fd < 0)
    {
        perror("open");
        return;
    }
    struct input_event ev;
    ev.type = EV_SND;
    ev.code = SND_TONE;
    ev.value = hz;
    write(fd, &ev, sizeof(ev));
    close(fd);
}

int car_in_out(int command, string &plate)
{

    string picture;
    int ret = capture_image(command, picture);
    if (ret != 0)
    {
        cout << "Failed to save image!" << endl;
        return 0;
    }
    aip::Ocr client(app_id, api_key, secret_key);

    string image;
    aip::get_file_content(picture.c_str(), &image);

    map<string, string> options;
    // 不支持检测多车牌
    options["multi_detect"] = "false";
    // 车牌图像发给百度智能云做车牌识别
    Json::Value result = client.license_plate(image, options);
    // Json数据格式
    // pkg-config --cflags jsoncpp
    // Json 的目录 -I/usr/include/jsoncpp
    cout << "识别到的内容:" << result.toStyledString() << endl;

    if (!result["error_code"].isNull())
    {
        cout << "error_code:" << result["error_code"] << endl;
        cout << "未识别到车牌号!" << endl;
        cout << "蜂鸣器响1秒" << endl;
        tone(366);
        sleep(1);
        tone(0);
        sleep(1);
        return 0;
    }

    plate = result["words_result"]["number"].asString();
    cout << plate << endl; // 车牌号
    return 1;
}
void light()
{
    int fd = open("/sys/class/leds/led3/brightness", O_WRONLY);

    if (fd == -1)
    {
        perror("Failed to open file.\n");
        return;
    }

    char data1[] = "1";
    char data0[] = "0";
    write(fd, data1, sizeof(data1)); // 1
    sleep(3);
    write(fd, data0, sizeof(data0)); // 0
    sleep(1);
    close(fd);
}
void key_event()
{
    cout << "请按键放行！" << endl;

    int fd = open("/dev/input/event1", O_RDONLY);
    if (fd < 0)
    {
        perror("Key_event=>  open error ");
        return;
    }

    // 阻塞读取按键事件
    struct input_event ev;
    read(fd, &ev, sizeof(ev));
    // 处理按键事件
    // 按下volum+
    if (ev.code == KEY_UP)
    {
        // 按键按下
        if (ev.value == 1)
        {
            cout << "按下volum+" << endl;
        }
        light();
        cout << "抬起volum+" << endl;
    }
    // 按下volum-
    if (ev.code == KEY_DOWN)
    {
        // 按键按下
        if (ev.value == 1)
        {
            cout << "按下volum-" << endl;
        }
        light();
        cout << "抬起volum-" << endl;
    }
    cout << "车辆已放行！" << endl;
    close(fd);
}
int main()
{
    int ret = open_db("parking.db");
    if (ret != 0)
    {
        cout << "<main.cpp>Failed to open database." << endl;
        return -1;
    }
    cout << "congratulate to success to open database!" << endl;
    string sql = "create table if not exists parking(id integer primary key autoincrement , in_time text not null,out_time text,license_plate text not null)";
    ret = exec_db(sql.c_str());
    if (ret != 0)
    {
        cout << "<main.cpp> failed to exec_db" << endl;
        close_db();
        return -1;
    }
    cout << "congratulate to success to create table-parking!" << endl;

    string plate; // 车牌号
    printf("车库管理系统开始运行…\n");
    while (1)
    {
        int command = -1;
        int flag = -1;
        printf("======================\n");
        printf("入场:1 , 出场:0 ,离开管理系统:2\n");
        printf("======================\n");
        scanf("%d", &command);
        if (command == 1)
        {
            cout << "注意入场时机" << endl;
            flag = car_in_out(command, plate);
            if (flag == 0)
                continue;
            do_insert(plate);
            key_event();
        }
        else if (command == 0)
        {
            cout << "注意出场时机" << endl;
            flag = car_in_out(command, plate);
            if (flag == 0)
                continue;
            do_update(plate);
            key_event();
        }
        else if (command == 2)
        {
            break;
        }
        else
        {
            continue;
        }
    }

    close_db();
    printf("Bye.\n");
    return 0;
}
