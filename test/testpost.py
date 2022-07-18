import time

import requests
GDKey="f247cdb592eb43ebac6ccd27f796e2d2"

class WZXYDK(object):
    def __init__(self):
        self.utl = "https://student.wozaixiaoyuan.com/health/save.json"
        self.headers = {

            'Accept-Encoding': 'gzip,compress,br,deflate',
            'Connection': 'keep-alive',
            'Content-Length': '478',
            'Cookie': '',
            'Host': 'student.wozaixiaoyuan.com',
            'JWSESSION': '56ba607bafd647169d9baaa4c7bec011',
            'Referer': 'https://servicewechat.com/wxce6d08f781975d91/186/page-frame.html',
            'User-Agent': 'Mozilla/5.0 (iPad; CPU OS 15_4_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 MicroMessenger/8.0.20(0x18001431) NetType/WIFI Language/zh_CN',
            'content-type': 'application/x-www-form-urlencoded',
            'token': ''
        }

    def start(self):
        times= str(int(time.time()*1000))
        data1={
            'answers': """["0","1","36.6"]""",
            'latitude': '34.445026',
            'longitude': '107.62972',
            'country': '中国',
            'city': '宝鸡市',
            'district': '岐山县',
            'province': '陕西省',
            'township': '凤鸣镇',
            'street': '凤鸣古街',
            'areacode': '610323',
            'towncode': '610323111000',
            'citycode': '0917',
            'timestampHeader':times,
            #
            # 'signatureHeader': '4e2f66702f5f94a290c4d40c5661481e146a9121a5ac274a4a90dadac9509091'

        }
        data2={
            'answers': """["0","1","36.6"]""",
            "latitude":  "34.156167",
            "longitude":      "108.90548",
            "country":        "中国",
            "city":            "西安市",
            "district":       "长安区",
            "province":       "陕西省",
            "township":        "韦曲街道",
               "towncode":"610116001000",
            "street":          "西长安街",
            "areacode":        "610116",
            "citycode":        "156610100",
            "timestampHeader": times

        }
        self.data = data1
        print(times)
        response = requests.post(url=self.utl, data=self.data, headers=self.headers);
        print(response.text)



    def get_post_data(self):
        latitude_longitude=[34.156167,108.90548] # 维度 经度
        url= "https://restapi.amap.com/v3/geocode/regeo?output=JSON&location={},{}&key={}&radius=1000&extensions=all".format(latitude_longitude[1],latitude_longitude[0],GDKey)
        map_reesponse=requests.get(url).json()
        status=map_reesponse["status"] # 1
        latitude=latitude_longitude[0]
        longitude=latitude_longitude[1]
        country=  "中国"
        city=map_reesponse["regeocode"]["addressComponent"].get("city")
        district=map_reesponse["regeocode"]["addressComponent"].get("district")
        province= map_reesponse["regeocode"]["addressComponent"].get("province")
        township=map_reesponse["regeocode"]["addressComponent"].get("township")
        towncode=map_reesponse["regeocode"]["addressComponent"].get("towncode")
        street=map_reesponse["regeocode"]["addressComponent"].get("streetNumber").get('street')
        areacode=map_reesponse["regeocode"]["addressComponent"].get("adcode")
        citycode=map_reesponse["regeocode"]["addressComponent"].get("citycode")
        formatted_address=map_reesponse["regeocode"].get("formatted_address")
        for key , value in map_reesponse["regeocode"].items():
            print(key , "   " , value)



    def get_latitude_longitude(self,address):
        address="北京市朝阳区阜通东大街6号"
        url="https://restapi.amap.com/v3/geocode/geo?address={}&output=JSON&key=dac00dbecfd1ce57bfaa36c645d65ce4".format(address)
        latitude_longitude_map=requests.get(url).json()
        if latitude_longitude_map.get("status")=="1":

            print(latitude_longitude_map.get('geocodes'))
        if len(latitude_longitude_map.get('geocodes'))>0:
            location=latitude_longitude_map.get('geocodes')[0].get("location")
            print(location.split(","))
            longitude=location.split(",")[0]
            latitude=location.split(",")[1]













if __name__ == '__main__':
    wzxy = WZXYDK()
    wzxy.get_post_data()
