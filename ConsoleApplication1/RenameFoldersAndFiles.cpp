#include <iostream>
#include <filesystem>
#include <string>
#include <regex>
#include <fstream>
#include <map>
#include "Levenshtein.h"
#include "MapSort.h"

namespace fs = std::filesystem;


// Function to find the most similar entry in the txt file
std::string findMostSimilarTitle(std::string fileName) {
    std::ifstream file("C:\\Users\\L3ttu\\Desktop\\NES\\extracted_game_names.txt");
    std::map<std::string, int> differenceMap;

    // Read the lines in the .txt file
    if (file.is_open()) {
        //std::cout << "FILE IS OPEN" << '\n';
        bool stop = false;

        // Check for stop signs
        // it should stop when the entry has been found
        // or when the first letters don't match anymore
        while (!file.eof()) {
            std::string line;
            std::getline(file, line);

            // Check if the entry and the file name are the same
            if (line[0] == fileName[0]) {
                //std::cout << "STOP NOT TRUE" << '\n';
                int difference = levenshteinDistance(fileName, line);
                //int difference = levenshteinRecursive(fileName, line, fileName.length(), line.length());
                //std::cout << "Difference between " << fileName << " and " << line << " is: " << difference << '\n';
                if (difference == 0)
                {
                    //std::cout << "NO DIFFERENCE ";
                    return fileName;
                }
                else {
                    // Insert the possible solution to the map
                    //differenceMap.insert(std::make_pair(line, difference));
                    differenceMap[line] = difference;
                }
            } 
        }
        file.close();
    }

    if (differenceMap.size() == 0) {
        return fileName;
    }
    else {
        //sortMap(differenceMap);
        pair<string, int> bestMatch = getSortedMapFirstValue(differenceMap);
        return bestMatch.first;
    }
}

// Function to rename files and directories
void renameFolderAndFile(const fs::path& directory) {
    try {
        // Iterate through each folder in the main directory
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (fs::is_directory(entry)) {
                const fs::path& folderPath = entry.path();
                std::string folderName = folderPath.filename().string();

                //std::cout << findMostSimilarTitle(folderName) << '\n';

                // Check if the folder name has "(U)"
                if (folderName.find("(U)") != std::string::npos) {
                    // Rename the folder from "(U)" to "(USA)"
                    std::regex reg("\\(U\\)");
                    std::string newFolderName = std::regex_replace(folderName, reg, "(USA)");
                    newFolderName = findMostSimilarTitle(newFolderName);
                    fs::path newFolderPath = folderPath.parent_path() / newFolderName;

                    // TODO: LOOK THROUGH THE EXTRACTED NAMES AND FIND THE MOST SIMILAR ENTRY
                    //std::cout << newFolderName << " -> " << findMostSimilarTitle(newFolderName) << '\n';

                    try {
                        // Rename the folder
                        fs::rename(folderPath, newFolderPath);
                        std::cout << "Renamed folder: " << folderPath << " -> " << newFolderPath << std::endl;

                        // Iterate through files in the renamed folder
                        for (const auto& fileEntry : fs::directory_iterator(newFolderPath)) {
                            const fs::path& filePath = fileEntry.path();
                            std::string fileName = filePath.filename().string();
                            
                            // Check if the file has "(U)"
                            if (fileName.find("(U)") != std::string::npos) {
                                //// Rename the file
                                //std::string newFileName = std::regex_replace(fileName, reg, "(USA)");
                                //fs::path newFilePath = filePath.parent_path() / newFileName;

                                //fs::rename(filePath, newFilePath);
                                //std::cout << "Renamed file: " << filePath << " -> " << newFilePath << std::endl;

                                // Rename the file 
                                std::string newFileName = newFolderName;
                                
                                // Add the extension
                                newFileName.append(".smc");
                                fs::path newFilePath = filePath.parent_path() / newFileName;

                                fs::rename(filePath, newFilePath);
                                std::cout << "Renamed file: " << filePath << " -> " << newFilePath << std::endl;
                            }
                        }
                    }
                    catch (const fs::filesystem_error& e) {
                        std::cerr << "Error renaming folder or file: " << e.what() << std::endl;
                    }
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}

void renameJustFiles(fs::path& directory) {
    try {

        // Iterate through each file in te main directory
        try {
            for (const auto& fileEntry : fs::directory_iterator(directory)) {
                const fs::path& filePath = fileEntry.path();
                std::string fileName = filePath.filename().string();
                std::regex regUSA("\\(U\\)");
                std::regex regEUR("\\(E\\)");
                std::string newFileName;

                // Check if file has an (U) or (E)
                if (fileName.find("(U)") != std::string::npos) {
                    // Change (U) to (USA)
                    newFileName = std::regex_replace(fileName, regUSA, "(USA)");
                }
                else if (fileName.find("(E)") != std::string::npos) {
                    // Change (E) to (Europe)
                    newFileName = std::regex_replace(fileName, regEUR, "(Europe)");
                }

                newFileName = findMostSimilarTitle(newFileName);
                //std::cout << fileName << " -> " << findMostSimilarTitle(newFileName) << "\n";

                newFileName.append(".nes");

                fs::path newFilePath = filePath.parent_path() / newFileName;
                fs::rename(filePath, newFilePath);
                std::cout << "Renamed file: " << filePath << " -> " << newFilePath << std::endl;
            }
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Error renaming folder or file: " << e.what() << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}

int main() {
    std::string directory;

    // Prompt user for the directory to process
    std::cout << "Enter the path to the main folder: ";
    std::getline(std::cin, directory);

    fs::path dirPath(directory);

    // Check if the directory exists
    if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
        //renameFolderAndFile(dirPath);
        renameJustFiles(dirPath);
    }
    else {
        std::cerr << "Directory does not exist or is not valid!" << std::endl;
    }

    return 0;
}
