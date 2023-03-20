#include "Finder.h"
#include "FileInfo.h"
#include "FileComparer.h"
#include <chrono>

using namespace DuplicateFinder;
using namespace std::chrono_literals;

namespace fs = std::filesystem;

namespace
{
    using directory_iterator = fs::recursive_directory_iterator;

    using Duplicate = std::pair<const fs::path, std::unordered_set<fs::path>>;

    constexpr auto Delay = std::chrono::milliseconds(10ms);

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

    std::pair<std::size_t, std::size_t> GetIndexes(
        std::size_t number, std::size_t recordCount)
    {
        return std::make_pair(number / recordCount, number % recordCount);
    }

    bool TryMergeToRecordWithTheSameKey(
        Duplicates& allDuplicates, Duplicate& duplicate)
    {
        auto iter = allDuplicates.find(duplicate.first);

        if (iter == allDuplicates.end())
        {
            return false;
        }

        iter->second.merge(duplicate.second);

        return true;
    }

    bool TryMergeToRecordWithKeyEqualedToOneOfValues(
        Duplicates& allDuplicates, Duplicate& duplicate)
    {
        for (auto& rhsDuplicate : duplicate.second)
        {
            auto iter = allDuplicates.find(rhsDuplicate);

            if (iter != allDuplicates.end())
            {
                iter->second.emplace(duplicate.first);
                iter->second.merge(duplicate.second);
                iter->second.erase(iter->first);
                return true;
            }
        }

        return false;
    }

    void AddRecord(Duplicates& allDuplicates, Duplicate& duplicate)
    {
        allDuplicates.insert(std::move(duplicate));
    }

    void Merge(Duplicates& allDuplicates, Duplicates&& newDuplicates)
    {
        for (auto& duplicate : newDuplicates)
        {
            if (TryMergeToRecordWithTheSameKey(allDuplicates, duplicate) ||
                TryMergeToRecordWithKeyEqualedToOneOfValues(allDuplicates, duplicate))
            {
                continue;
            }

            AddRecord(allDuplicates, duplicate);
        }
    }
}

Finder::Finder(fs::path path, std::size_t taskCount)
    : path_(std::move(path))
    , taskCount_(taskCount)
    , counter_(0)
    , recordCount_(0)
    , maxNumber_(0)
    , fileList_()
{
}

Duplicates Finder::Process()
{
    fileList_ = CreateFileList(path_);
    recordCount_ = fileList_.size();
    maxNumber_ = recordCount_ * (recordCount_ - 1) - 1;

    RunProcessing();

    return CombineResults();
}

void Finder::RunProcessing()
{
    for (std::size_t index = 0; index < taskCount_; ++index)
    {
        tasks_.emplace_back(std::async(
            std::launch::async, &Finder::ProcessByOneThread, this));
    }
}

Duplicates Finder::CombineResults()
{
    Duplicates result;

    while (!tasks_.empty())
    {
        for (auto task = tasks_.begin(); task != tasks_.end();)
        {
            if (task->wait_for(Delay) == std::future_status::ready)
            {
                Merge(result, task->get());
                task = tasks_.erase(task);
            }
            else
            {
                ++task;
            }
        }
    }

    return result;
}

Duplicates Finder::ProcessByOneThread()
{
    Duplicates result;

    while (true)
    {
        const std::size_t number = ++counter_;

        if (number > maxNumber_)
        {
            break;
        }

        const auto [lhsIndex, rhsIndex] = GetIndexes(number, recordCount_);

        if (lhsIndex + 1 > rhsIndex)
        {
            continue;
        }

        CompareFiles(lhsIndex, rhsIndex, result);
    }

    return result;
}

void Finder::CompareFiles(
    std::size_t lhsIndex, std::size_t rhsIndex, Duplicates& result)
{
    const auto& lhsRecord = fileList_[lhsIndex];
    const auto& rhsRecord = fileList_[rhsIndex];

    if (FileComparer::AreEqual(lhsRecord, rhsRecord))
    {
        const auto iter = result.find(lhsRecord.path_);

        if (iter != result.cend())
        {
            iter->second.insert(rhsRecord.path_);
        }
        else
        {
            result.emplace(
                lhsRecord.path_,
                std::unordered_set<fs::path>{ rhsRecord.path_ });
        }
    }
}
