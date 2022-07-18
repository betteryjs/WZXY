### 我在校园每日打卡签到



### 环境
```shell
Ubuntu 20.04
aliyun-cpp-sdk
nlohmann json
cpr
```






### 配置
- 复制 config.json.exp 到 config.json
- 在 config.json 填入我在校园账户 密码  以及要通知的手机号
- 填入 Aliyun accessKeyId  && accessKeySecret
- 将 wzxy.sh   `/root/wzxy/build/bin` 改为编译好的文件路径

### 编译 && 测试

```shell
mkdir build
cd build
cmake ..
make
make install
cd bin
./wzxy.sh or ./go_wzxy
```
