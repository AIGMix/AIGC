#pragma once
#include <list>
#include <vector>
#include <string>
using namespace std;

class TidalResult
{
public:
    bool   bSuccess;
    string sErrmessage;
};

class LoginKey : public TidalResult
{
public:
    string UserName;
    string Password;
    string UserID;
    string CountryCode;
    string SessionID;
    string AccessToken;
};

class TidalArtist : public TidalResult
{
public:
    string   ID;
    string   Name;
    string   Type;
    string   Url;
    string   Picture;
    int      Popularity;
    vector<string> ArtistTypes;
};

class TidalAlbum : public TidalResult
{
public:
    string   ID;
    string   Title;
    int      Duration;
    bool     StreamReady;
    string   StreamStartDate;
    bool     AllowStreaming;
    bool     PremiumStreamingOnly;
    int      NumberOfTracks;
    int      NumberOfVideos;
    int      NumberOfVolumes;
    string   ReleaseDate;
    string   Copyright;
    string   Type;
    string   Version;
    string   Url;
    string   Cover;
    string   VideoCover;
    bool     Explicit;
    string   Upc;
    int      Popularity;
    string   AudioQuality;
    TidalArtist   Artist;
    vector<string> AudioModes;
    vector<TidalArtist> Artists;
};

class TidalTrack : public TidalResult
{
public:
    string   ID;
    string   Title;
    int      Duration;
    string   ReplayGain;
    string   Peak;
    bool     AllowStreaming;
    bool     StreamReady;
    string   StreamStartDate;
    bool     PremiumStreamingOnly;
    int      TrackNumber;
    int      VolumeNumber;
    string   Version;
    int      Popularity;
    string   Copyright;
    string   Url;
    string   Isrc;
    bool     Editable;
    bool     Explicit;
    string   AudioQuality;
    TidalArtist   Artist;
    TidalAlbum    Album;
    vector<string> AudioModes;
    vector<TidalArtist> Artists;
};

class StreamUrl : public TidalResult
{
public:
    string TrackID;
    string Url;
    string Codec;
    string EncryptionKey;
    int    PlayTimeLeftInMinutes;
    string SoundQuality;
};

class VideoStreamUrl : public TidalResult
{
public:
    string   Codec;
    string   Resolution;
    vector<string> ResolutionArray;
    string   M3u8Url;
};

class TidalVideo : public TidalResult
{
public:
    string ID;
    string Title;
    int    Duration;
    string ImageID;
    int    TrackNumber;
    string ReleaseDate;
    string Version;
    string Copyright;
    string Quality;
    bool   Explicit;
    TidalArtist Artist;
    TidalAlbum  Album;
    vector<TidalArtist> Artists;
};

class TidalPlaylist : public TidalResult
{
public:
    string UUID;
    string Title;
    int    NumberOfTracks;
    int    NumberOfVideos;
    string Description;
    int    Duration;
    string LastUpdated;
    string Created;
    string Type;
    string Url;
    string Image;
    string SquareImage;
    bool   PublicPlaylist;
    int    Popularity;
};

class SearchResult : public TidalResult
{
public:
    vector<TidalArtist>   Artists;
    vector<TidalAlbum>    Albums;
    vector<TidalTrack>    Tracks;
    vector<TidalVideo>    Videos;
    vector<TidalPlaylist> Playlists;
};

class TidalItems : public TidalResult
{
public:
    vector<TidalTrack> Tracks;
    vector<TidalVideo> Videos;
};