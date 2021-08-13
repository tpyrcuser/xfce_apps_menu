#ifndef XFCE_APPS_MENU_FILE_H
#define XFCE_APPS_MENU_FILE_H

#include <iostream>
#include <vector>

class File {
public:
    static std::string getFileContent(const std::string &fileName);
    static void clearFileContent(const std::string &fileName);
    static void writeFile(const std::string &fileName, const std::string &content);
    static std::vector<std::string> getFilesInDirectory(const std::string &directory);
};

#endif
