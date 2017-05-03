#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "jpush.h"
#include "cJSON/cJSON.h"

size_t jpushWriteData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t data_size = size * nmemb;
    strcat((char *)stream, (char *)ptr);
    return data_size;
}

int jpushInit(cJSON **pushMsg, JPushNotification **note)
{
    *pushMsg = cJSON_CreateObject();
    *note = (JPushNotification *)malloc(sizeof(JPushNotification));
    (*note)->all = NULL;
    (*note)->ios = NULL;
    (*note)->android = NULL;
    (*note)->win = NULL;

    return 0;
}

int setPlatform(cJSON *pushMsg, int platform)
{
    if (platform & J_PUSH_PLATFORM_IOS && platform & J_PUSH_PLATFORM_ANDROID  && platform & J_PUSH_PLATFORM_WINPHONE)
    {
        cJSON_AddStringToObject(pushMsg, "platform", "all");
    }
    else
    {
        cJSON *platObj = cJSON_CreateArray();

        if (platform & J_PUSH_PLATFORM_IOS)
        {
            cJSON_AddItemToArray(platObj, cJSON_CreateString("ios"));
        }

        if (platform & J_PUSH_PLATFORM_ANDROID)
        {
            cJSON_AddItemToArray(platObj, cJSON_CreateString("android"));
        }

        if (platform & J_PUSH_PLATFORM_WINPHONE)
        {
            cJSON_AddItemToArray(platObj, cJSON_CreateString("winphone"));
        }

        cJSON_AddItemToObject(pushMsg, "platform", platObj);
    }

    return 0;
}

int setAudienceTarget(cJSON *pushMsg, int type, int targetCount, char *target[])
{
    if (type == J_PUSH_AUDIENCE_ALL)
    {
        cJSON_AddStringToObject(pushMsg, "audience", "all");
        return 0;
    }

    int i;
    cJSON *audienceObj = cJSON_CreateArray();
    cJSON *targetObj = cJSON_CreateObject();
    char targetType[16];
    memset(targetType, 0, 16);

    
    if (type == J_PUSH_AUDIENCE_TAG)
    {
        strcat(targetType, "tag");
    }
    else if (type == J_PUSH_AUDIENCE_TAGAND)
    {
        strcat(targetType, "tag_and");
    }
    else if (type == J_PUSH_AUDIENCE_ALIAS)
    {
        strcat(targetType, "alias");
    }
    else if (type == J_PUSH_AUDIENCE_REG)
    {
        strcat(targetType, "registration_id");
    }

    for (i = 0; i < targetCount; i++)
    {
        cJSON_AddItemToArray(audienceObj, cJSON_CreateString(target[i]));
    }

    cJSON_AddItemToObject(targetObj, targetType, audienceObj);
    cJSON_AddItemToObject(pushMsg, "audience", targetObj);

    return 1;
}

void setAllApns(JPushNotification *note, char *alert)
{
    Apns *apns = calloc(1, sizeof(Apns));

    if (alert != J_PUSH_STRING_NULL)
    {
        apns->alert = (char *)calloc(1, strlen(alert));
        strcat(apns->alert, alert);
    }
    else
    {
        apns->alert = J_PUSH_STRING_NULL;
    }

    note->all = apns;
}

void setAndroidApns(JPushNotification *note, char *alert, char *title, int builderId, int priority, char *category, int style, int alertType, char *bigText, char *bigPicPath, char *extras)
{
    Apns *apns = calloc(1, sizeof(Apns));

    if (alert != J_PUSH_STRING_NULL)
    {
        apns->alert = (char *)calloc(1, strlen(alert));
        strcat(apns->alert, alert);
    }
    else
    {
        apns->alert = J_PUSH_STRING_NULL;
    }

    if (title != J_PUSH_STRING_NULL)
    {
        apns->title = (char *)calloc(1, strlen(title));
        strcat(apns->title, title);
    }
    else
    {
        apns->title = J_PUSH_STRING_NULL;
    }

    apns->builderId = builderId;
    apns->priority = priority;
    apns->style = style;
    apns->alertType = alertType;

    if (category != J_PUSH_STRING_NULL)
    {
        apns->category = (char *)calloc(1, strlen(category));
        strcat(apns->category, category);
    }
    else
    {
        apns->category = J_PUSH_STRING_NULL;
    }

    if (bigText != J_PUSH_STRING_NULL)
    {
        apns->bigText = (char *)calloc(1, strlen(bigText));
        strcat(apns->bigText, bigText);
    }
    else
    {
        apns->bigText = J_PUSH_STRING_NULL;
    }

    if (bigPicPath != J_PUSH_STRING_NULL)
    {
        apns->bigPicPath = (char *)calloc(1, strlen(bigPicPath));
        strcat(apns->bigPicPath, bigPicPath);
    }
    else
    {
        apns->bigPicPath = J_PUSH_STRING_NULL;
    }

    if (extras != J_PUSH_STRING_NULL)
    {
        apns->extras = (char *)calloc(1, strlen(extras));
        strcat(apns->extras, extras);
    }
    else
    {
        apns->extras = J_PUSH_STRING_NULL;
    }

    note->android = apns;
}

void setIosApns(JPushNotification *note, char *alert, char *sound, int badge, int contentAvailable, int mutableContent, char *category, char *extras)
{
    Apns *apns = calloc(1, sizeof(Apns));

    if (alert != J_PUSH_STRING_NULL)
    {
        apns->alert = (char *)calloc(1, strlen(alert));
        strcat(apns->alert, alert);
    }
    else
    {
        apns->alert = J_PUSH_STRING_NULL;
    }

    if (sound != J_PUSH_STRING_NULL)
    {
        apns->sound = (char *)calloc(1, strlen(sound));
        strcat(apns->sound, sound);
    }
    else
    {
        apns->sound = J_PUSH_STRING_NULL;
    }

    apns->badge = badge;
    apns->contentAvailable = contentAvailable;
    apns->mutableContent = mutableContent;

    if (category != J_PUSH_STRING_NULL)
    {
        apns->category = (char *)calloc(1, strlen(category));
        strcat(apns->category, category);
    }
    else
    {
        apns->category = J_PUSH_STRING_NULL;
    }

    if (extras != J_PUSH_STRING_NULL)
    {
        apns->extras = (char *)calloc(1, strlen(extras));
        strcat(apns->extras, extras);
    }
    else
    {
        apns->extras = J_PUSH_STRING_NULL;
    }

    note->ios = apns;
}

void setWinApns(JPushNotification *note, char *alert, char *title, char *openPage, char *extras)
{
    Apns *apns = calloc(1, sizeof(Apns));

    if (alert != J_PUSH_STRING_NULL)
    {
        apns->alert = (char *)calloc(1, strlen(alert));
        strcat(apns->alert, alert);
    }
    else
    {
        apns->alert = J_PUSH_STRING_NULL;
    }

    if (title != J_PUSH_STRING_NULL)
    {
        apns->title = (char *)calloc(1, strlen(title));
        strcat(apns->title, title);
    }
    else
    {
        apns->title = J_PUSH_STRING_NULL;
    }

    if (openPage != J_PUSH_STRING_NULL)
    {
        apns->openPage = (char *)calloc(1, strlen(openPage));
        strcat(apns->openPage, openPage);
    }
    else
    {
        apns->openPage = J_PUSH_STRING_NULL;
    }

    if (extras != J_PUSH_STRING_NULL)
    {
        apns->extras = (char *)calloc(1, strlen(extras));
        strcat(apns->extras, extras);
    }
    else
    {
        apns->extras = J_PUSH_STRING_NULL;
    }

    note->win = apns;
}

int setNotification(cJSON *pushMsg, JPushNotification *note)
{
    cJSON *notification = cJSON_CreateObject();
    if (note->all != NULL)
    {
        if (note->all->alert != J_PUSH_STRING_NULL)
        {
            cJSON_AddStringToObject(notification, "alert", note->all->alert);
            free(note->all->alert);
        }
        if (note->all->title != J_PUSH_STRING_NULL)
        {
            cJSON_AddStringToObject(notification, "title", note->all->title);
            free(note->all->title);
        }

        free(note->all);
    }
    else
    {
        if (note->ios != NULL)
        {
            cJSON *platformNoteIOS = cJSON_CreateObject();
            if (note->ios->alert != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteIOS, "alert", note->ios->alert);
                free(note->ios->alert);
            }

            if (note->ios->sound != J_PUSH_STRING_NULL) 
            {
                cJSON_AddStringToObject(platformNoteIOS, "sound", note->ios->sound);
                free(note->ios->sound);
            }

            if (note->ios->badge != J_PUSH_INT_NULL) cJSON_AddNumberToObject(platformNoteIOS, "badge", note->ios->badge);
            if (note->ios->contentAvailable != J_PUSH_INT_NULL) cJSON_AddBoolToObject(platformNoteIOS, "content-available", note->ios->contentAvailable);
            if (note->ios->mutableContent != J_PUSH_INT_NULL) cJSON_AddBoolToObject(platformNoteIOS, "mutable-content", note->ios->mutableContent);

            if (note->ios->category != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteIOS, "category", note->ios->category);
                free(note->ios->category);
            }
            if (note->ios->extras != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteIOS, "extras", note->ios->extras);
                free(note->ios->extras);
            }

            cJSON_AddItemToObject(notification, "ios", platformNoteIOS);
            free(note->ios);
        }

        if (note->android != NULL)
        {
            cJSON *platformNoteAndroid = cJSON_CreateObject();

            if (note->android->alert != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteAndroid, "alert", note->android->alert);
                free(note->android->alert);
            }

            if (note->android->title != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteAndroid, "title", note->android->title);
                free(note->android->title);
            }

            if (note->android->builderId != J_PUSH_INT_NULL) cJSON_AddNumberToObject(platformNoteAndroid, "builder_id", note->android->builderId);
            if (note->android->priority != J_PUSH_INT_NULL) cJSON_AddNumberToObject(platformNoteAndroid, "priority", note->android->priority);

            if (note->android->category != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteAndroid, "category", note->android->category);
                free(note->android->category);
            }

            if (note->android->style != J_PUSH_INT_NULL) cJSON_AddNumberToObject(platformNoteAndroid, "style", note->android->style);
            if (note->android->alertType != J_PUSH_INT_NULL) cJSON_AddNumberToObject(platformNoteAndroid, "alert_type", note->android->alertType);

            if (note->android->bigText != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteAndroid, "big_text", note->android->bigText);
                free(note->android->bigText);
            }

            if (note->android->bigPicPath != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteAndroid, "big_pic_path", note->android->bigPicPath);
                free(note->android->bigPicPath);
            }

            if (note->android->extras != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteAndroid, "extras", note->android->extras);
                free(note->android->extras);
            }

            cJSON_AddItemToObject(notification, "android", platformNoteAndroid);
            free(note->android);
        }

        if (note->win != NULL)
        {
            cJSON *platformNoteWIN = cJSON_CreateObject();

            if (note->win->alert != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteWIN, "alert", note->win->alert);
                free(note->win->alert);
            }

            if (note->win->title != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteWIN, "title", note->win->title);
                free(note->win->title);
            }

            if (note->win->openPage != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteWIN, "_open_page", note->win->openPage);
                free(note->win->openPage);
            }

            if (note->win->extras != J_PUSH_STRING_NULL)
            {
                cJSON_AddStringToObject(platformNoteWIN, "extras", note->win->extras);
                free(note->win->extras);
            }

            cJSON_AddItemToObject(notification, "winphone", platformNoteWIN);
            free(note->win);
        }
    }

    cJSON_AddItemToObject(pushMsg, "notification", notification);

    return 1;
}

void setMessage(cJSON *pushMsg, char *msgContent, char *title, char *contentType, char *extras)
{
    cJSON *message = cJSON_CreateObject();

    if (msgContent != J_PUSH_STRING_NULL) cJSON_AddStringToObject(message, "msg_content", msgContent);
    if (title != J_PUSH_STRING_NULL) cJSON_AddStringToObject(message, "title", title);
    if (contentType != J_PUSH_STRING_NULL) cJSON_AddStringToObject(message, "content_type", contentType);
    if (extras != J_PUSH_STRING_NULL) cJSON_AddStringToObject(message, "extras", extras);

    cJSON_AddItemToObject(pushMsg, "message", message);
}

void setSmsMessage(cJSON *pushMsg, char *content, int delayTime)
{
    cJSON *smsMessage = cJSON_CreateObject();

    if (content != J_PUSH_STRING_NULL) cJSON_AddStringToObject(smsMessage, "content", content);
    if (delayTime != J_PUSH_INT_NULL) cJSON_AddNumberToObject(smsMessage, "delay_time", delayTime);

    cJSON_AddItemToObject(pushMsg, "sms_message", smsMessage);
}

void setOptions(cJSON *pushMsg, int sendno, int timeToLive, int overrideMsgId, int apnsProduction, int bigPushDuration)
{
    cJSON *options = cJSON_CreateObject();

    if (sendno != J_PUSH_INT_NULL) cJSON_AddNumberToObject(options, "sendno", sendno);
    if (timeToLive != J_PUSH_INT_NULL) cJSON_AddNumberToObject(options, "time_to_live", timeToLive);
    if (overrideMsgId != J_PUSH_INT_NULL) cJSON_AddNumberToObject(options, "override_msg_id", overrideMsgId);
    if (apnsProduction != J_PUSH_INT_NULL) cJSON_AddBoolToObject(options, "apns_production", apnsProduction);
    if (bigPushDuration != J_PUSH_INT_NULL) cJSON_AddBoolToObject(options, "big_push_duration", bigPushDuration);

    cJSON_AddItemToObject(pushMsg, "options", options);
}

char *jpushEval(cJSON *pushMsg)
{

    char *postData = cJSON_Print(pushMsg);

    CURL *curl;
    // CURLcode res;
    char *str = (char *)calloc(1024, sizeof(char));

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, J_PUSH_API);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_USERPWD, J_PUSH_APP_KEY":"J_PUSH_SECRET);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, jpushWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return str;
}

int jpushClean(cJSON *pushMsg)
{
    cJSON_Delete(pushMsg);

    return 0;
}
