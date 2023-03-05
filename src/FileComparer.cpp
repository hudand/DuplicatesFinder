#include "FileComparer.h"
#include <fstream>

using namespace DuplicateFinder;

namespace fs = std::filesystem;

namespace
{
    constexpr std::size_t BUFFER_SIZE = 1024 * 1024;

    bool AreFilesEqual(
        const fs::path& lFilePath, const fs::path& rFilePath, std::size_t size)
    {
        const auto sizeToRead = std::min(BUFFER_SIZE, size);

        std::ifstream lFile(lFilePath, std::ios_base::in | std::ios_base::binary);
        std::ifstream rFile(rFilePath, std::ios_base::in | std::ios_base::binary);

        if (!lFile.is_open() || !rFile.is_open())
        {
            return false;
        }

        auto lBuffer = std::make_unique_for_overwrite<char[]>(sizeToRead);
        auto rBuffer = std::make_unique_for_overwrite<char[]>(sizeToRead);

        do
        {
            lFile.read(lBuffer.get(), sizeToRead);
            rFile.read(rBuffer.get(), sizeToRead);

            if (std::memcmp(lBuffer.get(), rBuffer.get(), sizeToRead) != 0)
            {
                return false;
            }
        } while (lFile.good() || rFile.good());

        return true;
    }
}

bool FileComparer::Compare(const FileInfo& lhs, const FileInfo& rhs)
{
    if (lhs.size_ != rhs.size_)
    {
        return false;
    }

    return AreFilesEqual(lhs.path_, rhs.path_, lhs.size_);
}