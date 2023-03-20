#include <filesystem>
#include <iostream>
#include "Finder.h"

using namespace DuplicateFinder;

namespace fs = std::filesystem;

int main()
{
    const fs::path folder{ "c:\\Work\\D3" };
    constexpr std::size_t taskCount = 8;

    Finder finder(folder, taskCount);
    const auto duplicates = finder.Process();

    return EXIT_SUCCESS;
}