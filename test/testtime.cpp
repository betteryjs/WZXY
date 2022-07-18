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



    cout << to_string(getCurrentLocalTimeStamp())<<endl;

}
