#pragma once
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <future>
#include "FileInfo.h"

namespace DuplicateFinder
{
    using Duplicates = std::unordered_map<
        std::filesystem::path,
        std::unordered_set<std::filesystem::path>>;

    class Finder final
    {
    public:
        Finder(std::filesystem::path path, std::size_t threadsCount);

        Duplicates Process();
    private:
        Duplicates ProcessByOneThread();
        void CompareFiles(
            std::size_t lhsIndex, std::size_t rhsIndex, Duplicates& result);
        void RunProcessing();
        Duplicates CombineResults();

        std::filesystem::path path_;
        std::size_t taskCount_;
        std::atomic_uint64_t counter_;
        std::size_t recordCount_;
        std::size_t maxNumber_;
        std::vector<FileInfo> fileList_;
        std::list<std::future<Duplicates>> tasks_;
    };
}
