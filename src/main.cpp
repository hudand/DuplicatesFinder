#include <filesystem>
#include <iostream>
#include <unordered_map>

int main()
{
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    std::unordered_map<std::filesystem::path, std::size_t> fileSizes;

    for (const auto& dirEntry : recursive_directory_iterator("c:\\Work\\D3"))
    {
        if (dirEntry.is_regular_file())
        {
            std::cout << "file: " << dirEntry << ", size: " << dirEntry.file_size() << std::endl;
        }
        else
        {
            std::cout << "dir: " << dirEntry << std::endl;
        }
    }

    for (const auto& dirEntry : recursive_directory_iterator("c:\\Work\\D3"))
    {
        if (dirEntry.is_regular_file())
        {
            std::cout << "file: " << dirEntry << ", size: " << dirEntry.file_size() << std::endl;
        }
        else
        {
            std::cout << "dir: " << dirEntry << std::endl;
        }
    }

    std::string my_string;
    std::vector<std::string> vect;
    std::cout << sizeof vect << std::endl;
}