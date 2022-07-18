//
// Created by yjs on 2022/7/7.
//


#include "include/json.hpp"

#include <iostream>
#include <bits/stdc++.h>


using namespace std;

using json = nlohmann::json;

int main() {

    ifstream pepole("/home/yjs/CPP/Demo/wzxy/build/bin/config.json");
    json people_json;
    pepole >> people_json;
    string accessKeyId = people_json["accessKeyId"].get<string>();
    string accessKeySecret = people_json["accessKeySecret"].get<string>();

    bool isSMS = false;
    int i = 1;

    auto c=people_json["names"][i];
    cout <<      c.at("name")<<endl;
    cout <<      c.at("isSchool")<<endl;




    return 0;

}