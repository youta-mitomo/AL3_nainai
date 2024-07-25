#pragma once

#include"Vector3.h"
#include<cstdint>
#include<string>
#include<vector>
#include<stdint.h>

enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};



struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

///<sumary>
///マップチップフィールド
///</sumary>

class MapChipField {
public:
	
	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }
	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }


	private:

	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	//ブロックの個数
	static inline const  uint32_t kNumBlockVirtical = 20;
	static inline const  uint32_t kNumBlockHorizontal = 100;


	MapChipData mapChipData_;
};


