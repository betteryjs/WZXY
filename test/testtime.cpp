//
// Created by yjs on 2022/5/7.
//
#include <ctime>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// get current local time stamp
int64_t getCurrentLocalTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    return tmp.count();

    // return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


int main(){

//    time_t time1 = time(0);
//    cout << "time1 = " << time1 << endl;//1498122787
//    char * strTime = ctime(&time1);
//    cout << "strTime = " << strTime << endl;//Thu Jun 22 17:13:07 2017
//
//    time_t startTime = 1498122787;
//    double betweenSecond = difftime(time1, startTime);//该函数返回 time1 和 time2 之间相差的秒数。
//    cout << "betweenSecond = " << betweenSecond << endl;//Thu Jun 22 17:13:07 2017


    cout << to_string(getCurrentLocalTimeStamp())<<endl;
    // 1651882332240
    // 1651882234047

}
