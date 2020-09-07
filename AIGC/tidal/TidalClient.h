#pragma once
#include "TidalEnum.h"
#include "TidalModel.h"

class TidalClient
{
public:
    static LoginKey Login(string userName, string password, string token);
    static LoginKey Login(string accessToken);

    static TidalAlbum GetAlbum(LoginKey key, string id);
    static TidalItems GetAlbumItems(LoginKey key, string id);

    static TidalTrack GetTrack(LoginKey key, string id);
    static StreamUrl GetTrackStreamUrl(LoginKey key, string id, AudioQuality quality = eAUDIO_QUALITY_High);

    static TidalVideo GetVideo(LoginKey key, string id);
    static list<VideoStreamUrl> GetVideStreamUrls(LoginKey key, string id);
    static VideoStreamUrl GetVideStreamUrl(LoginKey key, string id, VideoQuality resolution = eVIDEO_QUALITY_720);

    static TidalPlaylist GetPlaylist(LoginKey key, string id);
    static TidalItems GetPlaylistItems(LoginKey key, string id);

    static TidalArtist GetArtist(LoginKey key, string id, bool containEPSingle = true);

    static TidalType ParseUrl(string sUrl, string& id);
    static SearchResult Search(LoginKey key, string text, int limit = 10, TidalType type = eTIDAL_TYPE_NONE);
};

