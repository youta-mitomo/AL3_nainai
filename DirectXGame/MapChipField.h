#pragma once

#include"Vector3.h"
#include<cstdint>
#include<string>
#include<vector>


///<sumary>
///マップチップフィールド
///</sumary>



class MapChipField {
public:
	//1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);


	private:
	//ブロックの個数
	static inline const float uint32_t kNumBlockVirtical = 20;
	static inline const float uint32_t kNumBlockHorizontal = 100;


	MapChipData mapChipData_;
};

enum class MapChipType {
	kBlank,//空白
	kBlock,//ブロック
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};