#pragma once

enum class AudioQuality
{
    Normal,
    High,
    HiFi,
    Master,
};

enum class VideoQuality
{
    e240 = 240,
    e360 = 360,
    e480 = 480,
    e720 = 720,
    e1080 = 1080,
};

enum class TidalType
{
    Album,
    Artist,
    Playlist,
    Track,
    Video,
    Search,
    None,
};