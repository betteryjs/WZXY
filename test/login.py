import time

import requests


class WZXYDK(object):
    def __init__(self):
        self.username= "19992718542"
        self.password=  "ccbbaa"
        self.url = "https://gw.wozaixiaoyuan.com/basicinfo/mobile/login/username?username={}&password={}".format(self.username,self.password)
        self.headers = {
        "Accept-Encoding": "gzip, deflate, br",
        "Connection": "keep-alive",
        "User-Agent": "Mozilla/5.0 (iPhone; CPU iPhone OS 15_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 MicroMessenger/8.0.13(0x18000d32) NetType/WIFI Language/zh_CN miniProgram",
        "Content-Type": "application/json;charset=UTF-8",
        "Content-Length": "2",
        "Host": "gw.wozaixiaoyuan.com",
        "Accept-Language": "en-us,en",
        "Accept": "application/json, text/plain, */*",
    }

    def start(self):
        self.data = "{}"

        response = requests.post(url=self.url, data=self.data, headers=self.headers)
        print(response.text)


if __name__ == '__main__':
    wzxy = WZXYDK()
    wzxy.start()
