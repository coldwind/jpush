#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jpush.h"
#include "cJSON/cJSON.h"

int main(int argc, char *argv[])
{
    cJSON *jpush;
    JPushNotification *note;
    jpushInit(&jpush, &note);

    // 设置谁送的平台
    setPlatform(jpush, J_PUSH_PLATFORM_IOS | J_PUSH_PLATFORM_ANDROID);

    // 设置audience
    char *target[3] = {"value1","value2","value3"}; // 推送的audience值
    setAudienceTarget(jpush, J_PUSH_AUDIENCE_TAG, 3, target);

    // setAllApns(note, "hello world!"); // 注:设置此函数后不需要再设置具体平台 所有平台全部推送

    // 设置ios平台的notification
    setIosApns(note, "ios alert", "sound", 1, 1, 1, "category", "extras");

    // 设置安卓平台的notification
    setAndroidApns(note, "android alert", "android title", 1, 2, J_PUSH_STRING_NULL, J_PUSH_INT_NULL, J_PUSH_INT_NULL, "android big text", "android big pic", J_PUSH_STRING_NULL);

    // 设置WINPHONE的notification
    setWinApns(note, "win alert", "title", "openpage", "extra");

    // 合并notification
    setNotification(jpush, note);

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

    printf("return:%s\n", str);

    return 0;
}
