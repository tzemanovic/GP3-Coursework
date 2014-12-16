#pragma once

// convert multibyte string to unicode widechar string
std::wstring s2ws( const std::string& s );

// returns base path from a file path
std::string basePath( const std::string& path );