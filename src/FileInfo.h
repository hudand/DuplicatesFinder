#pragma once

#include <filesystem>

struct FileInfo final
{
    std::filesystem::path path_;
    std::size_t size_;
};
