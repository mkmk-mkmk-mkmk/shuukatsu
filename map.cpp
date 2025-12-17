#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "map.h"
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "scene.h"

//int g_MapChipList[STAGEBLOCK_HEIGHT][STAGEBLOCK_WIDTH]; //マップチップ配列

void Map::Init(int maptype)
{
	//テキストファイル読み込み
	std::ifstream load_file;
	if (load_file.is_open()) load_file.close();
	load_file.clear();

	switch (maptype)
	{
	case 0:
		load_file.open("map\\title.txt");
		break;

	case 1:
		load_file.open("map\\map1.txt");
		break;

	}

	std::string line;

	//マップチップ配列ー＞テキストファイルに記載されている内容を読み込む
	std::vector<int> g_MapChipList;

	m_MapGetFinish = false;

	while (!m_MapGetFinish)
	{
		if (m_MapTextLineGetFinish)
		{
			m_MapWidthGetFinish = true;
			m_MapTextLineGetFinish = false;
		}

		std::getline(load_file, line);
		std::istringstream stream(line);

		while (!m_MapTextLineGetFinish)
		{
			std::string cell;
			std::getline(stream, cell, ' ');
			if (cell == "n")
			{
				m_MapTextLineGetFinish = true;
				break;
			}
			else if (cell == "e")
			{
				m_MapGetFinish = true;
				break;
			}
			g_MapChipList.push_back(std::stoi(cell));
			if (!m_MapWidthGetFinish)
			{
				STAGEBLOCK_WIDTH++;
			}
		}
		STAGEBLOCK_HEIGHT++;
	}

	//ファイルを閉じる
	load_file.close();

	int total_blocks = g_MapChipList.size();

	for (int idx = 0; idx < total_blocks; idx++)
	{
		int i = idx / STAGEBLOCK_WIDTH; //行番号
		int j = idx % STAGEBLOCK_WIDTH; //列番号

		//マップチップ番号取得
		int chip_id = g_MapChipList.at(idx);

		//値をチェック
		switch (chip_id)
		{
			case 0:	//空白
				break;

			case 1:	//ブロック
				//描画座標を割り出す
				m_Position.x = MAPCHIP_WIDTH * j + MAPCHIP_WIDTH * 0.5f;
				m_Position.y = MAPCHIP_HEIGHT * i + MAPCHIP_HEIGHT * 0.5f;

				//リストに保存
				m_BoxPosList.push_back(m_Position);
				break;

			case 2:	//破壊可能ブロック
				//描画座標を割り出す
				m_Position.x = MAPCHIP_WIDTH * j + MAPCHIP_WIDTH * 0.5f;
				m_Position.y = MAPCHIP_HEIGHT * i + MAPCHIP_HEIGHT * 0.5f;

				//リストに保存
				m_BreakableBoxPosList.push_back(m_Position);
				break;

			case 3: //当たり判定のないブロック
				//描画座標を割り出す
				m_Position.x = MAPCHIP_WIDTH * j + MAPCHIP_WIDTH * 0.5f;
				m_Position.y = MAPCHIP_HEIGHT * i + MAPCHIP_HEIGHT * 0.5f;

				//リストに保存
				m_UntouchableBoxPosList.push_back(m_Position);
				break;

			case 4: //ゴール
				//描画座標を割り出す
				m_Position.x = MAPCHIP_WIDTH * j + MAPCHIP_WIDTH * 0.5f;
				m_Position.y = MAPCHIP_HEIGHT * i + MAPCHIP_HEIGHT * 0.5f;

				//リストに保存
				m_GoalPosList.push_back(m_Position);
				break;

			case 5: //エネミー①
				//描画座標を割り出す
				m_Position.x = MAPCHIP_WIDTH * j + MAPCHIP_WIDTH * 0.5f;
				m_Position.y = MAPCHIP_HEIGHT * i + MAPCHIP_HEIGHT * 0.5f;

				//リストに保存
				m_EnemyPosList.push_back(m_Position);
				break;
			default:
				break;
		}

	}
	//マップチップリストを開放
	g_MapChipList.clear();
}

void Map::Uninit()
{

}

void Map::Update()
{

}

void Map::Draw()
{

}