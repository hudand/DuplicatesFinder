#pragma once
#include <filesystem>
#include <unordered_map>
#include "FileInfo.h"

namespace DuplicateFinder
{
    class FileComparer final
    {
    public:
        static bool AreEqual(const FileInfo& lhs, const FileInfo& rhs);
    };
}
