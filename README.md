# jpush
jpush 是一款C语版的极光推送客户端代码，支持v3版的API。example代码在main.c里，具体使用方式下面文档也有说明

注意：设置Apns的函数参数名对应极光推送官网文档的参数名 只不过将断峰改为了驼峰，详见[极光官方文档](https://docs.jiguang.cn/jpush/server/push/rest_api_v3_push/)。

如有任何问题，请联系我：vegeta.ma@163.com，谢谢.

# 配置
打开jpush.h
```
#define J_PUSH_APP_KEY "your appkey" // AppKey
#define J_PUSH_SECRET "your secret" // Secret 
```
在此处设置你的appkey和secret

# example
```
gcc -Wall main.c jpush.c cJSON/cJSON.c -g -lm -lcurl -o jpush_test
./jpush_test
```

# 使用说明
## include
```
#include <curl/curl.h>
#include "cJSON/cJSON.h"
#include "jpush.h"
```
## code
```
cJSON *jpush;
JPushNotification *note;
jpushInit(&jpush, &note);

// 设置谁送的平台
setPlatform(jpush, J_PUSH_PLATFORM_IOS | J_PUSH_PLATFORM_ANDROID);

// 设置audience
char *target[3] = {"value1","value2","value3"}; // 推送的audience值
setAudienceTarget(jpush, J_PUSH_AUDIENCE_TAG, 3, target); // 3 是target数组的长度

// 设置notification
// 设置Apns的函数参数名对应极光推送官网文档的参数名 只不过将断峰改为了驼峰
// setAllApns(note, "hello world!"); // 注:设置此函数后不需要再设置具体平台 所有平台全部推送

setIosApns(note, "ios alert", "sound", 1, 1, 1, "category", "extras"); // 设置ios平台的notification
setAndroidApns(note, "android alert", "android title", 1, 2, J_PUSH_STRING_NULL, J_PUSH_INT_NULL, J_PUSH_INT_NULL, "android big text", "android big pic", J_PUSH_STRING_NULL); // 设置安卓平台的notification
setWinApns(note, "win alert", "title", "openpage", "extra"); // 设置WINPHONE的notification
setNotification(jpush, note); // 合并notification

// 设置message
setMessage(jpush, "message", "aaa", "bbb", "ccc");

// 设置sms_message
setSmsMessage(jpush, "message", 1);

// 设置options
setOptions(jpush, 1, 2, 3, 0,1);

// 推送
char *str = jpushEval(jpush);

// 回收
jpushClean(jpush);
```
