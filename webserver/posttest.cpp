#include "workflow/WFFacilities.h"
#include <csignal>
#include <iostream>
#include <string>
#include "wfrest/HttpServer.h"
#include "wfrest/json.hpp"
#include "include/urlEncodeDecode.hpp"

using namespace wfrest;

using namespace std;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
    wait_group.done();
}

int main()
{
    signal(SIGINT, sig_handler);

    HttpServer svr;

    // Urlencoded Form
    // curl -v http://ip:port/post \
    // -H "body-type:application/x-www-form-urlencoded" \
    // -d 'user=admin&pswd=123456'
    svr.POST("/register", [](const HttpReq *req, HttpResp *resp)
    {
        if (req->content_type() != APPLICATION_URLENCODED)
        {
            resp->set_status(HttpStatusBadRequest);
            return;
        }
        std::map<std::string, std::string> &form_kv = req->form_kv();
        string name =form_kv["name"];
        string username =form_kv["username"];
        string password =form_kv["password"];
        string address =UrlDecode(form_kv["address"]);
        bool isSMS =form_kv["isSMS"].empty() ? false: true;

        cout << name << endl;
        cout << username << endl;
        cout << password << endl;
        cout << address << endl;
        cout << isSMS << endl;

        Json msg;
        msg["code"]=200;
        msg["msg"]="Submit success !";

        resp->Json(msg);
    });




    svr.GET("/register", [](const HttpReq *req, HttpResp *resp)
    {
        resp->File("html/register.html");

    });





    if (svr.start(8888) == 0)
    {
        svr.list_routes();

        wait_group.wait();
        svr.stop();
    } else
    {
        fprintf(stderr, "Cannot start server");
        exit(1);
    }
    return 0;
}
