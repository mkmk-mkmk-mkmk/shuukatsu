#include <iostream>
#include <fstream>
#include <string>
#include "Vector2.h"

void saveData(const std::string& filename, Vector2 position, float rotate)
{
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << position.x << "," << position.y << std::endl;
        outFile << rotate << std::endl;
        outFile.close();
    }
    else
    {
    }
}

//void loadData(const std::string& filename) {
//    std::ifstream inFile(filename);
//    if (inFile.is_open()) {
//        int score;
//        std::string playerName;
//        inFile >> score;
//        inFile.ignore(); // 改行を無視
//        std::getline(inFile, playerName);
//        inFile.close();
//        std::cout << "スコア: " << score << ", プレイヤー名: " << playerName << std::endl;
//    }
//    else {
//        std::cerr << "ファイルを開けませんでした。" << std::endl;
//    }
//}