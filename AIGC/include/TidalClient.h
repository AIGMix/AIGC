#pragma once
#include "AIGCBase.h"
#include "TidalEnum.h"
#include "TidalModel.h"

class TidalClient
{
public:
    static LoginKey Login(string sUserName, string sPassword, string sToken);
    static LoginKey Login(string sAccessToken);

    static TidalAlbum GetAlbum(LoginKey oKey, string sID);
    static TidalItems GetAlbumItems(LoginKey oKey, string sID);

    static TidalTrack GetTrack(LoginKey oKey, string sID);
    static StreamUrl GetTrackStreamUrl(LoginKey oKey, string sID, eAudioQuality eQuality = eAUDIO_QUALITY_High);

    static TidalVideo GetVideo(LoginKey oKey, string sID);
    static list<VideoStreamUrl> GetVideStreamUrls(LoginKey oKey, string sID);
    static VideoStreamUrl GetVideStreamUrl(LoginKey oKey, string sID, eVideoQuality eReso = eVIDEO_QUALITY_720);

    static TidalPlaylist GetPlaylist(LoginKey oKey, string sID);
    static TidalItems GetPlaylistItems(LoginKey oKey, string sID);

    static TidalArtist GetArtist(LoginKey oKey, string sID, bool bContainEPSingle = true);

    static eTidalType ParseUrl(string sUrl, string& sID);
    static SearchResult Search(LoginKey oKey, string sTex, int iLimit = 10, eTidalType eType = eTIDAL_TYPE_NONE);
};

