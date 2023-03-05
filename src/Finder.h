#pragma once
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

namespace DuplicateFinder
{
    using Duplicates = std::unordered_map<
        std::filesystem::path,
        std::unordered_set<std::filesystem::path>>;

    class Finder final
    {
    public:
        explicit Finder(std::filesystem::path path);

        Duplicates Process();
    private:
        std::filesystem::path path_;
    };
}
