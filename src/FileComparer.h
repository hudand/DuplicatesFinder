#pragma once
#include <filesystem>
#include <unordered_map>
#include "FileInfo.h"

namespace DuplicateFinder
{
    class FileComparer final
    {
    public:
        static bool Compare(const FileInfo& lhs, const FileInfo& rhs);
    };
}
