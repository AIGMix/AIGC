#pragma once
#include <list>
#include <vector>
#include <string>
using namespace std;

class TidalResult
{
public:
    bool   success;
    string errMessage;
};

class LoginKey : public TidalResult
{
public:
    string userName;
    string password;
    string userID;
    string countryCode;
    string sessionID;
    string accestoken;
};

class TidalArtist : public TidalResult
{
public:
    string   id;
    string   name;
    string   type;
    string   url;
    string   picture;
    int      popularity;
    vector<string> artistTypes;
};

class TidalAlbum : public TidalResult
{
public:
    string   id;
    string   title;
    int      duration;
    bool     streamReady;
    string   streamStartDate;
    bool     allowStreaming;
    bool     premiumStreamingOnly;
    int      numberOfTracks;
    int      numberOfVideos;
    int      numberOfVolumes;
    string   releaseDate;
    string   copyright;
    string   type;
    string   version;
    string   url;
    string   cover;
    string   videoCover;
    bool     Explicit;
    string   upc;
    int      popularity;
    string   audioQuality;
    TidalArtist   artist;
    vector<string> audioModes;
    vector<TidalArtist> artists;
};

class TidalTrack : public TidalResult
{
public:
    string   id;
    string   title;
    int      duration;
    string   replayGain;
    string   peak;
    bool     allowStreaming;
    bool     streamReady;
    string   streamStartDate;
    bool     premiumStreamingOnly;
    int      trackNumber;
    int      volumeNumber;
    string   version;
    int      popularity;
    string   copyright;
    string   url;
    string   isrc;
    bool     editable;
    bool     Explicit;
    string   audioQuality;
    TidalArtist   artist;
    TidalAlbum    album;
    vector<string> audioModes;
    vector<TidalArtist> artists;
};

class StreamUrl : public TidalResult
{
public:
    string trackID;
    string url;
    string codec;
    string encryptionKey;
    int    playTimeLeftInMinutes;
    string soundQuality;
};

class VideoStreamUrl : public TidalResult
{
public:
    string   codec;
    string   resolution;
    vector<string> resolutionArray;
    string   m3u8Url;
};

class TidalVideo : public TidalResult
{
public:
    string id;
    string title;
    int    duration;
    string ImageID;
    int    trackNumber;
    string releaseDate;
    string version;
    string copyright;
    string quality;
    bool   Explicit;
    TidalArtist artist;
    TidalAlbum  album;
    vector<TidalArtist> artists;
};

class TidalPlaylist : public TidalResult
{
public:
    string uuid;
    string title;
    int    numberOfTracks;
    int    numberOfVideos;
    string description;
    int    duration;
    string lastUpdated;
    string created;
    string type;
    string url;
    string image;
    string squareImage;
    bool   publicPlaylist;
    int    popularity;
};

class SearchResult : public TidalResult
{
public:
    vector<TidalArtist>   artists;
    vector<TidalAlbum>    albums;
    vector<TidalTrack>    tracks;
    vector<TidalVideo>    videos;
    vector<TidalPlaylist> playlists;
};

class TidalItems : public TidalResult
{
public:
    vector<TidalTrack> tracks;
    vector<TidalVideo> videos;
};