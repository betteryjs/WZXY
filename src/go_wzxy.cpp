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
    WZXY(const string &name, const string &u_name, const string &u_pwd, const string &callPhone,
         const string &accessKeyId, const string &accessKeySecret, const bool &isSMS,const bool & isSchool) :
            name(name), username(u_name), password(u_pwd), callPhone(callPhone), accessKeyId(accessKeyId),
            accessKeySecret(accessKeySecret), isSMS(isSMS) ,isSchool(isSchool){};

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
        cpr::Payload home_data{
                {"answers",   answers},
                {"latitude",  "34.445026"},
                {"longitude", "107.62972"},
                {"country",   "中国"},
                {"city",      "宝鸡市"},
                {"district",  "岐山县"},
                {"province",  "陕西省"},
                {"township",  "凤鸣镇"},
                {"street",    "凤鸣古街"},
                {"areacode", "610323"},
                {"towncode", "610323111000"},
                {"citycode","0917"},
                {"timestampHeader", timestampHeader}
        };
        cpr::Payload school_data{
                {"answers",         answers},
                {"latitude",        "34.156167"},
                {"longitude",       "108.90548"},
                {"country",         "中国"},
                {"city",            "西安市"},
                {"district",        "长安区"},
                {"province",        "陕西省"},
                {"township",        "韦曲街道"},
                {"street",          "西长安街"},
                {"areacode",        "610116"},
                {"citycode",        "156610100"},
                {"towncode",        "610116001000"},
                {"timestampHeader", timestampHeader}
        };
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
        cpr::Payload data{};
        if(isSchool){
           data=school_data;
        }else{
            data=home_data;
        }

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

                                                   data
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
                                        {"time", string(ctime(&now))}
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
                                        {"time", string(ctime(&now))}
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
    string name;
    string username;
    string password;
    string JWSESSION;
    json msg_dict;
    string callPhone;
    string accessKeyId;
    string accessKeySecret;
    bool isSMS;
    bool  isSchool;


};


int main(int argc, char *argv[]) {

    GLogHelper log(argv[0]);


    ifstream pepole("config.json");
    json people_json;
    pepole >> people_json;
    string accessKeyId = people_json["accessKeyId"].get<string>();
    string accessKeySecret = people_json["accessKeySecret"].get<string>();
    bool isSMS = true;

    for (int i = 0; i < people_json["names"].size(); ++i) {
        string
        name = people_json["names"][i].at("name");
        string
        password = people_json["names"][i].at("password");
        string
        username = people_json["names"][i].at("username");
        string
        callPhone = people_json["names"][i].at("callPhone");

        bool isSchool=people_json["names"][i].at("isSchool");
        LOG(INFO) << "init " << name;
        LOG(INFO) << people_json["names"][i] << endl;

        WZXY wzxy(name, username, password, callPhone, accessKeyId, accessKeySecret, isSMS,isSchool);
        wzxy.start();

    }

    return 0;
}