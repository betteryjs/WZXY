"""
百度map Document:
http://lbsyun.baidu.com/index.php?title=webapi/ip-api
decode address:
http://api.map.baidu.com/geocoder?output=json&key=f247cdb592eb43ebac6ccd27f796e2d2&location=34.445026,107.62972
encode address:
http://api.map.baidu.com/geocoding/v3/?address=北京市海淀区上地十街10号&output=json&ak=您的ak&callback=showLocation
c1 latitude(维度) c2 longitude(经度)

高德map Document
https://lbs.amap.com/api/webservice/guide/api/georegeo
decode address:
https://restapi.amap.com/v3/geocode/regeo?output=JSON&location=107.62972,34.445026&key=dac00dbecfd1ce57bfaa36c645d65ce4&radius=1000&extensions=all
https://restapi.amap.com/v3/geocode/regeo?output=JSON&location=114.466717,37.096624&key=dac00dbecfd1ce57bfaa36c645d65ce4&radius=1000&extensions=all
c1 longitude(经度) c2 latitude(维度)
encode address:
https://restapi.amap.com/v3/geocode/geo?address=%E5%8C%97%E4%BA%AC%E5%B8%82%E6%9C%9D%E9%98%B3%E5%8C%BA%E9%98%9C%E9%80%9A%E4%B8%9C%E5%A4%A7%E8%A1%976%E5%8F%B7&output=JSON&key=dac00dbecfd1ce57bfaa36c645d65ce4

"""


