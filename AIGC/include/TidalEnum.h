#pragma once

enum eAudioQuality
{
    eAUDIO_QUALITY_Normal,
    eAUDIO_QUALITY_High,
    eAUDIO_QUALITY_HiFi,
    eAUDIO_QUALITY_Master,
};

enum eVideoQuality
{
    eVIDEO_QUALITY_240 = 240,
    eVIDEO_QUALITY_360 = 360,
    eVIDEO_QUALITY_480 = 480,
    eVIDEO_QUALITY_720 = 720,
    eVIDEO_QUALITY_1080 = 1080,
};

enum eTidalType
{
    eTIDAL_TYPE_ALBUM,
    eTIDAL_TYPE_ARTIST,
    eTIDAL_TYPE_PLAYLIST,
    eTIDAL_TYPE_TRACK,
    eTIDAL_TYPE_VIDEO,
    eTIDAL_TYPE_SEARCH,
    eTIDAL_TYPE_NONE,
};