#include <iostream>
#include <fstream>
#include <filesystem>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

#define VERSION "0.1.0"

#include <windows.h>
namespace fs = std::filesystem; // C++17

// To build this program, you need to use this command:
// g++ -O3 -std=c++17 -o dictool dictool.cpp

void mergeDictionaries(const std::string &directory, const std::string &outputFileName);
void deduplicateFile(const std::string &inputFileName, const std::string &outputFileName);
void mergeAndDeduplicate(const std::string &directory, const std::string &outputFileName);
void removeEmptyLinesAndTrim(const std::string &inputFileName, const std::string &outputFileName);
void displayMenu();

int main()
{
    SetConsoleOutputCP(CP_UTF8); // 显示中文

    std::string option, directory, outputFileName, inputFileName;
    std::string outputDirectory;

    displayMenu();

    std::cout << "请选择操作: ";
    std::cin >> option;

    switch (std::stoi(option))
    {
    case 1:
    {
        std::cout << "请输入包含字典文件的目录路径: ";
        std::cin >> directory;
        std::cout << "请输入输出文件名: ";
        std::cin >> outputFileName;
        std::cout << "请输入输出目录路径(留空则为当前目录): ";
        std::cin.ignore();
        std::getline(std::cin, outputDirectory);
        if (outputDirectory.empty())
        {
            outputDirectory = fs::current_path().string();
        }
        if (outputDirectory.back() != '/' && outputDirectory.back() != '\\')
        {
            outputDirectory += "/";
        }
        outputFileName = outputDirectory + outputFileName;
        mergeDictionaries(directory, outputFileName);
        break;
    }
    case 2:
    {
        std::cout << "请输入要处理的文件路径: ";
        std::cin >> inputFileName;
        std::cout << "请输入输出文件名: ";
        std::cin >> outputFileName;
        std::cout << "请输入输出目录路径(留空则为当前目录): ";
        std::cin.ignore();
        std::getline(std::cin, outputDirectory);
        if (outputDirectory.empty())
        {
            outputDirectory = fs::current_path().string();
        }
        if (outputDirectory.back() != '/' && outputDirectory.back() != '\\')
        {
            outputDirectory += "/";
        }
        outputFileName = outputDirectory + outputFileName;
        deduplicateFile(inputFileName, outputFileName);
        break;
    }
    case 3:
    {
        std::cout << "请输入包含字典文件的目录路径: ";
        std::cin >> directory;
        std::cout << "请输入输出文件名: ";
        std::cin >> outputFileName;
        std::cout << "请输入输出目录路径(留空则为当前目录): ";
        std::cin.ignore();
        std::getline(std::cin, outputDirectory);
        if (outputDirectory.empty())
        {
            outputDirectory = fs::current_path().string();
        }
        if (outputDirectory.back() != '/' && outputDirectory.back() != '\\')
        {
            outputDirectory += "/";
        }
        outputFileName = outputDirectory + outputFileName;
        mergeAndDeduplicate(directory, outputFileName);
        break;
    }
    case 4:
    {
        std::cout << "请输入要处理的文件路径: ";
        std::cin >> inputFileName;
        std::cout << "请输入输出文件名: ";
        std::cin >> outputFileName;
        std::cout << "请输入输出目录路径(留空则为当前目录): ";
        std::cin.ignore();
        std::getline(std::cin, outputDirectory);
        if (outputDirectory.empty())
            outputDirectory = fs::current_path().string();
        if (outputDirectory.back() != '/' && outputDirectory.back() != '\\')
            outputDirectory += "/";
        outputFileName = outputDirectory + outputFileName;
        removeEmptyLinesAndTrim(inputFileName, outputFileName);
        break;
    }
    default:
        std::cout << "无效选项，请重新运行程序并选择有效选项" << std::endl;
    }
    system("pause");
    return 0;
}

void displayMenu()
{
    std::cout << "dictool " << VERSION << std::endl;
    std::cout << "==================" << std::endl;
    std::cout << "1. 合并目录下的所有字典文件" << std::endl;
    std::cout << "2. 对指定字典文件进行去重" << std::endl;
    std::cout << "3. 合并目录下的所有字典文件并去重" << std::endl;
    std::cout << "4. 去除空行并清理空白字符" << std::endl;
    std::cout << std::endl;
}

void mergeDictionaries(const std::string &directory, const std::string &outputFileName)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    std::ofstream outFile(outputFileName);
    if (!outFile)
    {
        std::cerr << "无法创建输出文件: " << outputFileName << std::endl;
        return;
    }
    std::cout << "开始合并字典文件..." << std::endl;
    std::vector<std::string> filesToMerge;
    for (const auto &entry : fs::recursive_directory_iterator(directory))
        if (entry.path().extension() == ".txt")
            filesToMerge.push_back(entry.path().string());

    if (filesToMerge.empty())
    {
        std::cerr << "目录中没有找到任何txt文件!" << std::endl;
        outFile.close();
        return;
    }

    std::cout << "找到 " << filesToMerge.size() << " 个txt文件" << std::endl;

    for (const auto &file : filesToMerge)
    {
        std::ifstream inFile(file);
        if (!inFile)
        {
            std::cerr << "无法打开文件: " << file << std::endl;
            continue;
        }

        std::cout << "正在合并文件: " << file << std::endl;

        std::string line;
        while (std::getline(inFile, line))
            outFile << line << std::endl;

        inFile.close();
    }

    outFile.close();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "合并完成，结果已保存到: " << outputFileName << std::endl;
    std::cout << "合并耗时: " << duration.count() << " ms" << std::endl;
}

void deduplicateFile(const std::string &inputFileName, const std::string &outputFileName)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    std::ifstream inFile(inputFileName);
    if (!inFile)
    {
        std::cerr << "无法打开输入文件: " << inputFileName << std::endl;
        return;
    }
    std::set<std::string> uniqueLines;
    std::string line;
    size_t totalLines = 0;
    std::cout << "正在读取文件并去重..." << std::endl;
    while (std::getline(inFile, line))
    {
        totalLines++;
        if (!line.empty())
            uniqueLines.insert(line);
    }

    inFile.close();

    std::cout << "原始文件 " << inputFileName << " 中有 " << totalLines << " 行，去重后保留 " << uniqueLines.size() << " 行" << std::endl;

    std::ofstream outFile(outputFileName);
    if (!outFile)
    {
        std::cerr << "无法创建输出文件: " << outputFileName << std::endl;
        return;
    }

    for (const auto &line : uniqueLines)
        outFile << line << std::endl;

    outFile.close();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "去重完成，结果已保存到: " << outputFileName << std::endl;
    std::cout << "去重耗时: " << duration.count() << " ms" << std::endl;
}

void mergeAndDeduplicate(const std::string &directory, const std::string &outputFileName)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    std::cout << "开始合并并去重..." << std::endl;
    std::string tempFileName = "temp_combined.txt";
    auto merge_start = std::chrono::high_resolution_clock::now();
    mergeDictionaries(directory, tempFileName);
    auto merge_end = std::chrono::high_resolution_clock::now();

    auto deduplicate_start = std::chrono::high_resolution_clock::now();
    deduplicateFile(tempFileName, outputFileName);
    auto deduplicate_end = std::chrono::high_resolution_clock::now();

    fs::remove(tempFileName);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    auto merge_duration = std::chrono::duration_cast<std::chrono::milliseconds>(merge_end - merge_start);
    auto deduplicate_duration = std::chrono::duration_cast<std::chrono::milliseconds>(deduplicate_end - deduplicate_start);

    std::cout << "合并并去重完成，结果已保存到: " << outputFileName << std::endl;
    std::cout << "合并耗时: " << merge_duration.count() << " ms" << std::endl;
    std::cout << "去重耗时: " << deduplicate_duration.count() << " ms" << std::endl;
    std::cout << "总耗时: " << total_duration.count() << " ms" << std::endl;
}

void removeEmptyLinesAndTrim(const std::string &inputFileName, const std::string &outputFileName)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    std::ifstream inFile(inputFileName);
    if (!inFile)
    {
        std::cerr << "无法打开输入文件: " << inputFileName << std::endl;
        return;
    }
    std::ofstream outFile(outputFileName);
    if (!outFile)
    {
        std::cerr << "无法创建输出文件: " << outputFileName << std::endl;
        inFile.close();
        return;
    }

    size_t totalLines = 0;
    size_t nonEmptyLines = 0;
    std::string line;
    std::cout << "正在处理文件并去除空行..." << std::endl;

    while (std::getline(inFile, line))
    {
        totalLines++;
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);
        if (!line.empty())
        {
            outFile << line << std::endl;
            nonEmptyLines++;
        }
    }
    inFile.close();
    outFile.close();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "处理完成，结果已保存到: " << outputFileName << std::endl;
    std::cout << "原始文件有 " << totalLines << " 行，处理后保留 " << nonEmptyLines << " 行" << std::endl;
    std::cout << "处理耗时: " << duration.count() << " ms" << std::endl;
}
