#include "Finder.h"
#include "FileInfo.h"

using namespace DuplicateFinder;

namespace fs = std::filesystem;

namespace
{
    using directory_iterator = fs::recursive_directory_iterator;

    std::vector<FileInfo> CreateFileList(const fs::path& folderPath)
    {
        std::vector<FileInfo> result;

        for (const auto& dirEntry : directory_iterator(folderPath))
        {
            if (dirEntry.is_regular_file())
            {
                const auto& path = dirEntry.path();
                result.push_back(FileInfo{ path, fs::file_size(path) });
            }
        }

        return result;
    }
}

Finder::Finder(fs::path path)
    : path_(std::move(path))
{
}

duplicates Finder::Process()
{
    duplicates result;

    const auto fileList = CreateFileList(path_);

    // TODO: implement processing files

    return result;
}
