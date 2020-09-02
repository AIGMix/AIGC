#include "TidalClient.h"
#include "HttpHelper.h"

using namespace aigc;

static string G_TOKEN = "wc8j_yBJd20zOmx0";
static string G_BASE_URL = "https://api.tidalhifi.com/v1/";
static string G_VERSION = "1.9.1";



LoginKey TidalClient::Login(string sUserName, string sPassword, string sToken)
{
    map<string, string> parameters = {
        {"username", sUserName},
        {"password", sPassword},
        {"token", G_TOKEN},
        {"clientUniqueKey", "834e749c80337a6d"},
        {"clientVersion", G_VERSION},
    };

    vector<string> headers = {
        "Content-Type: application/x-www-form-urlencoded",
    };

    HttpHelper::Result result = HttpHelper::Post(G_BASE_URL + "login/username", parameters, headers);

    LoginKey ret;
    ret.bSuccess = result.bSuccess;
    ret.sErrmessage = result.sErrmessage;
    if (!result.bSuccess)
        return ret;

    ret.UserName = sUserName;
    ret.Password = sPassword;
    ret.AccessToken = "";
    ret.CountryCode = "";
    ret.UserID = "";
    ret.SessionID = "";
    return ret;
}

LoginKey TidalClient::Login(string sAccessToken)
{
    vector<string> headers = {
        "authorization:Bearer " + sAccessToken,
    };

    HttpHelper::Result result = HttpHelper::Post("https://api.tidal.com/v1/sessions", {}, headers);

    LoginKey ret;
    ret.bSuccess = result.bSuccess;
    ret.sErrmessage = result.sErrmessage;
    if (!result.bSuccess)
        return ret;

    ret.UserName = "";
    ret.Password = "";
    ret.AccessToken = sAccessToken;
    ret.CountryCode = "";
    ret.UserID = "";
    ret.SessionID = "";
    return ret;
}





template<typename T>
static T& Request(LoginKey oKey, string sPath, T& oRet, map<string, string> oParas = {}, int iRetry = 3)
{
    oParas.insert(pair<string,string>("countryCode", oKey.CountryCode));

    vector<string> headers = {};
    if (oKey.AccessToken.length() > 0)
        headers.push_back("authorization:Bearer " + oKey.AccessToken);
    else
        headers.push_back("X-Tidal-SessionId: " + oKey.SessionID);

    HttpHelper::Result result = HttpHelper::Post(G_BASE_URL + sPath, oParas, headers);

    return oRet;
}