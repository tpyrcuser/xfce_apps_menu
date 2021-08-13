#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>

#include "File.h"

void File::clearFileContent(const std::string &fileName) {
    std::ofstream fileStream(fileName, std::ios::out | std::ios::trunc);
}

void File::writeFile(const std::string &fileName, const std::string &content) {
    std::ofstream fileStream{fileName, std::ios::out | std::ios::trunc};

    if (!fileStream.good()) throw std::runtime_error("Error: file not good (need sudo / not found)");

    fileStream << content;
    fileStream.close();
}

std::vector<std::string> File::getFilesInDirectory(const std::string &directory) {
    std::vector<std::string> result;
    DIR *dir;

    class dirent *ent;
    class stat st{};

    dir = opendir(directory.c_str());

    while ((ent = readdir(dir)) != nullptr) {
        const std::string file_name = ent->d_name;
        const std::string full_file_name = directory + "/" + file_name;

        if (file_name[0] == '.' || stat(full_file_name.c_str(), &st) == -1 || (st.st_mode & S_IFDIR) != 0) {
            continue;
        }

        result.push_back(file_name);
    }

    return result;
}

std::string File::getFileContent(const std::string &fileName) {
    std::ifstream ifs(fileName);

    if (!ifs.good()) throw std::runtime_error("Error: file not good (need sudo / not found)");

    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    return content;
}

