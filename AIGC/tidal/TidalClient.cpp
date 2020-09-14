#include "TidalClient.h"
#include "HttpHelper.h"

using namespace aigc;

static string G_TOKEN = "wc8j_yBJd20zOmx0";
static string G_BASE_URL = "https://api.tidalhifi.com/v1/";
static string G_VERSION = "1.9.1";



LoginKey TidalClient::Login(string userName, string password, string token)
{
    map<string, string> parameters = {
        {"username", userName},
        {"password", password},
        {"token", G_TOKEN},
        {"clientUniqueKey", "834e749c80337a6d"},
        {"clientVersion", G_VERSION},
    };

    vector<string> headers = {
        "Content-Type: application/x-www-form-urlencoded",
    };

    HttpHelper::Response result = HttpHelper::Post(G_BASE_URL + "login/username", parameters, headers);

    LoginKey ret;
    ret.success = result.success;
    ret.errMessage = result.errMessage;
    if (!result.success)
        return ret;

    ret.userName = userName;
    ret.password = password;
    ret.accestoken = "";
    ret.countryCode = "";
    ret.userID = "";
    ret.sessionID = "";
    return ret;
}

LoginKey TidalClient::Login(string accessToken)
{
    vector<string> headers = {
        "authorization:Bearer " + accessToken,
    };

    HttpHelper::Response result = HttpHelper::Post("https://api.tidal.com/v1/sessions", {}, headers);

    LoginKey ret;
    ret.success = result.success;
    ret.errMessage = result.errMessage;
    if (!result.success)
        return ret;

    ret.userName = "";
    ret.password = "";
    ret.accestoken = accessToken;
    ret.countryCode = "";
    ret.userID = "";
    ret.sessionID = "";
    return ret;
}





template<typename T>
static T& Request(LoginKey key, string path, T& ret, map<string, string> paras = {}, int retry = 3)
{
    paras.insert(pair<string, string>("countryCode", key.countryCode));

    vector<string> headers = {};
    if (key.accestoken.length() > 0)
        headers.push_back("authorization:Bearer " + key.accestoken);
    else
        headers.push_back("X-Tidal-SessionId: " + key.sessionID);

    HttpHelper::Response result = HttpHelper::Post(G_BASE_URL + path, paras, headers);

    return ret;
}