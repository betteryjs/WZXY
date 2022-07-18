//
// Created by yjs on 2022/1/29.
//

#include <cpr/cpr.h>
#include <iostream>
#include <sys/stat.h>

#include <sys/types.h>
#include <string>
#include <random>
#include "include/json.hpp"
#include <cstdlib>
#include <alibabacloud/core/AlibabaCloud.h>
#include <alibabacloud/core/CommonRequest.h>
#include <alibabacloud/core/CommonClient.h>
#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <chrono>


using json = nlohmann::json;

using namespace std;
using namespace std::chrono;

using namespace AlibabaCloud;


#include <sys/stat.h>

int exist(const string name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}


using namespace std;

class GLogHelper {
public:
    GLogHelper(char *program) {
        google::InitGoogleLogging(program);
        FLAGS_stderrthreshold = google::INFO; //除了将日志输出到文件之外，还将此错误级别和更高错误级别的日志同时输出到 stderr，
        FLAGS_colorlogtostderr = true;
        FLAGS_logtostderr = false; //设置日志消息是否转到标准输出而不是日志文件
//        FLAGS_v = 0; //只记录此错误级别和更高错误级别的 VLOG 日志信息
        FLAGS_logbufsecs = 0;        //缓冲日志输出，默认为30秒，此处改为立即输出
        FLAGS_max_log_size = 100;  //最大日志大小为 100MB
        FLAGS_stop_logging_if_full_disk = true;     //当磁盘被写满时，停止日志输出
        string home = "./log/";  //要先创建此目录,否则运行报错.
        create_log_dir(home);
//        FLAGS_log_dir = home; // 将日志文件输出到本地

        //glog init
        string info_log = home + "INFO__";
        google::SetLogDestination(google::INFO, info_log.c_str());
        string warning_log = home + "WARNING_";
        google::SetLogDestination(google::WARNING, warning_log.c_str());
        string error_log = home + "ERROR_";
        google::SetLogDestination(google::ERROR, error_log.c_str());
        string fatal_log = home + "FATAL_";
        google::SetLogDestination(google::FATAL, fatal_log.c_str());
        // 信号处理
        google::InstallFailureSignalHandler();      //捕捉 core dumped
//        google::InstallFailureWriter(&SignalHandle);    //默认捕捉 SIGSEGV 信号信息输出会输出到 stderr，可以通过下面的方法自定义输出>方式：



    }

    ~GLogHelper() {
        google::ShutdownGoogleLogging();
    }

public:
    void create_log_dir(const string &dir_name) {
        if (exist(dir_name)) {
            cout << "path  exists " << endl;

        } else {
            mkdir(dir_name.c_str(), 0775);
        }
    }
};



namespace do_string{
    using namespace std;
    static const std::string WHITE_SPACE = " \n\r\t\f\v";
    /////
    std::string ltrim(const std::string &);
    std::string rtrim(const std::string &);
    std::string trim(const std::string &);
    std::vector<std::string> split(const std::string& ,const std::string& );
    std::vector<std::string> split(const std::string& ,const std::string& ,bool );
    /////
    std::string ltrim(const std::string &s)
    {
        size_t start = s.find_first_not_of(WHITE_SPACE);
        return (start == std::string::npos) ? "" : s.substr(start);
    }
    std::string rtrim(const std::string &s)
    {
        size_t end = s.find_last_not_of(WHITE_SPACE);
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }
    std::string trim(const std::string &s) {
        return rtrim(ltrim(s));
    }
    std::vector<std::string> split(const std::string& str,const std::string& delim) {
        return split(str,delim,false);
    }

    std::vector<std::string> split(const std::string& str,const std::string& delim,bool is_trim) {
        std::vector<string> res;
        if (str.empty()){
            return res;
        }
        std::string string1 = str + delim; //*****扩展字符串以方便检索最后一个分隔出的字符串
        std::string::size_type pos;
        std::size_t size = string1.size();
        for (int i = 0; i < size; ++i) {
            pos = string1.find(delim, i); //pos为分隔符第一次出现的位置，从i到pos之前的字符串是分隔出来的字符串
            if (pos != string::npos) { //如果查找到，如果没有查找到分隔符，pos为string::npos
                string s = string1.substr(i, pos - i);//*****从i开始长度为pos-i的子字符串
                if(is_trim){
                    res.push_back(trim(s));//两个连续空格之间切割出的字符串为空字符串，这里没有判断s是否为空，所以最后的结果中有空字符的输出，
                }else{
                    res.push_back(s);//两个连续空格之间切割出的字符串为空字符串，这里没有判断s是否为空，所以最后的结果中有空字符的输出，
                }
                i = pos + delim.size() - 1;
            }
        }
        return res;
    }
}


class WZXY {
private:
    int64_t getCurrentLocalTimeStamp() {
        std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now());
        auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
        return tmp.count();

        // return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

public:
    WZXY(json & config,const string &accessKeyId, const string &accessKeySecret, const bool &isSMS, const string & GDKey) :
            configData(config), accessKeyId(accessKeyId),
            accessKeySecret(accessKeySecret), isSMS(isSMS), GDKey(GDKey) {
         this->name=configData.at("name");
         this->username=configData.at("username");
         this->password=configData.at("password");
         this->callPhone=configData.at("callPhone");

         LOG(INFO) << "init " << name;
         LOG(INFO) << configData << endl;





    };


    cpr::Payload get_dk_data(){

//        latitude_longitude=[34.156167,108.90548] # 维度 经度
        vector <string> c_number{"36.4", "36.5", "36.6", "36.7"};
        uniform_int_distribution<unsigned> u(0, c_number.size() - 1);
        default_random_engine e;

        string answers = "[\"0\",\"1\",\"" + c_number[u(e)] + "\"]";
        string  longitude{};
        string  latitude{};

        string country=  "中国";

        string city{};
        string district{};
        string province{};
        string township{};
        string towncode{};
        string street{};
        string areacode{};
        string citycode{};



        if(this->configData.at("havelocation")){
              longitude=this->configData.at("location")[0];
              latitude=this->configData.at("location")[1];
        }
        else{
            // get longitude latitude

            string address=this->configData.at("address");
            cpr::Response response = cpr::Get(cpr::Url{"https://restapi.amap.com/v3/geocode/geo?address="+address+"&output=JSON&key="+this->GDKey});
            if (response.status_code == 200){

                LOG(INFO) << " Get location data Success , response.status_code is " << response.status_code;

                json response_json=json::parse(response.text);
                if(response_json.at("status")=="1"){
                    if(response_json.at("geocodes").size()>0){
                        string location=response_json.at("geocodes")[0].at("location").get<string>();
                        vector<string> location_vector=do_string::split(location, ",");
                         longitude=location_vector[0];
                         latitude=location_vector[1];
                        LOG(INFO) << " Get location data Success longitude is  "<< longitude ;
                        LOG(INFO) << "latitude  is " << latitude;

                    }


                }else{
                    LOG(ERROR)<< " no location data ";
                }

            }else{
                LOG(ERROR)<< " requests error response.status_code is  "<< response.status_code;
            }
        }

        cpr::Response response = cpr::Get(cpr::Url{"https://restapi.amap.com/v3/geocode/regeo?output=JSON&location="+longitude+","+latitude+"&key="+this->GDKey+"&radius=1000&extensions=all"});
        if (response.status_code == 200){

            LOG(INFO) << " Get map data Success , response.status_code is " << response.status_code;

            json map_reesponse=json::parse(response.text);
            if(map_reesponse.at("status")=="1"){

                city=map_reesponse["regeocode"]["addressComponent"].at("city").get<string>();
                district=map_reesponse["regeocode"]["addressComponent"].at("district").get<string>();
                province= map_reesponse["regeocode"]["addressComponent"].at("province").get<string>();
                township=map_reesponse["regeocode"]["addressComponent"].at("township").get<string>();
                towncode=map_reesponse["regeocode"]["addressComponent"].at("towncode").get<string>();
                street=map_reesponse["regeocode"]["addressComponent"].at("streetNumber").at("street").get<string>();
                areacode=map_reesponse["regeocode"]["addressComponent"].at("adcode").get<string>();
                citycode=map_reesponse["regeocode"]["addressComponent"].at("citycode").get<string>();
                formatted_address=map_reesponse["regeocode"].at("formatted_address").get<string>();


                LOG(INFO) << " city " << city;
                LOG(INFO) << " district " << district;
                LOG(INFO) << " province " << province;
                LOG(INFO) << " township " << township;
                LOG(INFO) << " towncode " << towncode;
                LOG(INFO) << " street " << street;
                LOG(INFO) << " areacode " << areacode;
                LOG(INFO) << " citycode " << citycode;


            }else{
                LOG(ERROR)<< " no map data ";
            }



        }else{

            LOG(ERROR)<< " requests error response.status_code is  "<< response.status_code;

        }




            cpr::Payload home_data{
                {"answers",   answers},
                {"latitude",  latitude},
                {"longitude", longitude},
                {"country",   country},
                {"city",      city},
                {"district",  district},
                {"province",  province},
                {"township",  township},
                {"street",    street},
                {"areacode", areacode},
                {"towncode", towncode},
                {"citycode",citycode},
                {"timestampHeader", to_string(getCurrentLocalTimeStamp())}
        };
        return home_data;










    }







    void start() {


        this->load_session();
        if (this->dk()) {
            // success
            if (isSMS) {

                this->send_sms();
            }
        } else {
            this->login();
            this->dk();
            if (isSMS) {

                this->send_sms();
            }

        }


    }

    int login() {
        cpr::Response response = cpr::Get(cpr::Url{"https://gw.wozaixiaoyuan.com/basicinfo/mobile/login/username"},
                                          cpr::Header{{"user-agent",
                                                       "Mozilla/5.0 (iPhone; CPU iPhone OS 15_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 MicroMessenger/8.0.18(0x18001225) NetType/WIFI Language/zh_CN miniProgram/wxce6d08f781975d91"}},
                                          cpr::Parameters{
                                                  {"username",  this->username},
                                                  {"password",  this->password},
                                                  {"phoneInfo", "3____iphone%3B+cpu+iphone+os+15_3+like+mac+os+x"},
                                          });

        if (response.status_code == 200) {
            this->JWSESSION = response.cookies["JWSESSION"];

            if (!this->JWSESSION.empty()) {
                LOG(INFO) << this->JWSESSION;
                ofstream session("cookie_json/" + name + ".json");
                json session_json;
                session_json["JWSESSION"] = this->JWSESSION;
                session << session_json;

                return 0;
            } else {
                LOG(INFO) << response.text;
            }
        } else {
            LOG(INFO) << response.status_code;

        }
        return 1;

    }

    int dk() {

        vector <string> c_number{"36.4", "36.5", "36.6", "36.7"};
        uniform_int_distribution<unsigned> u(0, c_number.size() - 1);
        default_random_engine e;

        string answers = "[\"0\",\"1\",\"" + c_number[u(e)] + "\"]";
        string timestampHeader = to_string(getCurrentLocalTimeStamp());


        /*
         *  'Accept-Encoding': 'gzip,compress,br,deflate',
            'Connection': 'keep-alive',
            'Content-Length': '478',
            'Cookie': '',
            'Host': 'student.wozaixiaoyuan.com',
            'JWSESSION': '8e69e0003fd641ddb5b545459deb3416',
            'Referer': 'https://servicewechat.com/wxce6d08f781975d91/186/page-frame.html',
            'User-Agent': 'Mozilla/5.0 (iPad; CPU OS 15_4_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 MicroMessenger/8.0.20(0x18001431) NetType/WIFI Language/zh_CN',
            'content-type': 'application/x-www-form-urlencoded',
            'token': ''
         * */
        cpr::Response response = cpr::Post(cpr::Url{"https://student.wozaixiaoyuan.com/health/save.json"},
                                           cpr::Header{
                                                   {"Accept-Encoding", "gzip,compress,br,deflate"},
                                                   {"Connection",      "keep-alive"},
                                                   {"Cookie",          ""},
                                                   {"Host",            "student.wozaixiaoyuan.com"},
                                                   {"User-Agent",      "Mozilla/5.0 (iPad; CPU OS 15_4_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 MicroMessenger/8.0.20(0x18001431) NetType/WIFI Language/zh_CN"},
                                                   {"content-type",    "application/x-www-form-urlencoded"},
                                                   {"Referer",         "https://servicewechat.com/wxce6d08f781975d91/186/page-frame.html"},
                                                   {"JWSESSION",       this->JWSESSION}},
                                           cpr::Payload{
                                                   get_dk_data()
                                           });

        if (response.status_code == 200) {
            LOG(INFO) << "response.text: " << response.text;
            int code = json::parse(response.text)["code"].get<int>();
            time_t now = time(0);
            if (code == 0) {
                LOG(INFO) << "DK success";
                this->msg_dict = json::object(
                        {
                                {"code", {
                                        {"message", "打卡成功"},
                                        {"address", formatted_address},
                                        {"time", do_string::trim(string(ctime(&now)))}
                                }
                                }
                        });
                LOG(INFO) << this->msg_dict.dump() << endl;
                return 0;

            } else {
                LOG(ERROR) << "DK error";
                this->msg_dict = json::object(
                        {
                                {"code", {
                                        {"message", "打卡失败  "+json::parse(response.text).at("message").get<string>()},
                                        {"address", formatted_address},
                                        {"time", do_string::trim(string(ctime(&now)))}
                                }
                                }
                        });
                LOG(ERROR) << this->msg_dict.dump() << endl;
                return 1;
            }

        } else {
            LOG(ERROR) << response.status_code;
        }

        return 0;
    }


    void send_sms() {
        AlibabaCloud::InitializeSdk();
        AlibabaCloud::ClientConfiguration configuration("cn-hangzhou");
        // specify timeout when create client.
        configuration.setConnectTimeout(1500);
        configuration.setReadTimeout(4000);
        AlibabaCloud::Credentials credential(this->accessKeyId, this->accessKeySecret);
        AlibabaCloud::CommonClient client(credential, configuration);
        AlibabaCloud::CommonRequest request(AlibabaCloud::CommonRequest::RequestPattern::RpcPattern);
        request.setHttpMethod(AlibabaCloud::HttpRequest::Method::Post);
        request.setDomain("dysmsapi.aliyuncs.com");
        request.setVersion("2017-05-25");
        request.setQueryParameter("Action", "SendSms");
        request.setQueryParameter("PhoneNumbers", this->callPhone);
        request.setQueryParameter("TemplateCode", "SMS_199216241");
        time_t now = time(0);
        request.setQueryParameter("TemplateParam", this->msg_dict.dump());

        request.setQueryParameter("SignName", "我在校园ABC");
        auto response = client.commonResponse(request);
        if (response.isSuccess()) {
            LOG(INFO) << "request success." << endl;
            LOG(INFO) << "request: " << response.result().payload();

        } else {

            LOG(ERROR) << "error: " << response.error().errorMessage() << endl;
            LOG(ERROR) << "request id:" << response.error().requestId() << endl;
        }

        AlibabaCloud::ShutdownSdk();
    }


    void load_session() {
        if (exist("cookie_json/" + name + ".json")) {
            ifstream session("cookie_json/" + name + ".json");
            json session_json;
            session >> session_json;
            this->JWSESSION = session_json["JWSESSION"].get<string>();
        } else {
            LOG(INFO) << " log in init ";
            this->login();
        }

    }







private:
    json & configData;
    string name;
    string username;
    string password;
    string JWSESSION;
    json msg_dict;
    string callPhone;
    string accessKeyId;
    string accessKeySecret;
    string GDKey;
    bool isSMS;
    string  formatted_address{};


};


int main(int argc, char *argv[]) {

    GLogHelper log(argv[0]);


    ifstream pepole("config.json");
    json people_json;
    pepole >> people_json;
    string accessKeyId = people_json["accessKeyId"].get<string>();
    string accessKeySecret = people_json["accessKeySecret"].get<string>();
    string GDKey = people_json["GDKey"].get<string>();
    bool isSMS = false;

    for (int i = 0; i < people_json["names"].size(); ++i) {
        WZXY wzxy(people_json["names"][i],accessKeyId, accessKeySecret, isSMS,GDKey);
        wzxy.start();

    }

    return 0;
}