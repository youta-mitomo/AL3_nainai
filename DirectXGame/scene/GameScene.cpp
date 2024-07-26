#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "Math/MyMath/MatrixMath.h"
#include <cassert>

GameScene::GameScene(){}

GameScene::~GameScene() {

	delete modelBlock_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;

	
}
	

	void GameScene::Initialize() {

		dxCommon_ = DirectXCommon::GetInstance();
		input_ = Input::GetInstance();
		audio_ = Audio::GetInstance();

		//カメラの生成
		debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
		cameraMarix_ = MatrixMath::MakeAffineMatrix(
			{ 0.0f,0.0f,0.0f },
			debugCamera_->GetViewProjection().rotation_,
			debugCamera_->GetViewProjection().translation_
		);


		AxisIndicator::GetInstance()->SetVisible(true);
		AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

		//要素数
		const uint32_t kNumblockVirtical = 10;  
		const uint32_t kNumBlockHorizontal = 20;

	   /* uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	    uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();*/
	
		//ブロック1個分の横幅
		const float kBlockWidth = 2.0f;
		const float kBlockHeight = 2.0f;

		//要素数を変更する
	    worldTransformBlocks_.resize(kNumblockVirtical);
	    for (uint32_t i = 0; i < kNumblockVirtical; i++) {
		    worldTransformBlocks_[i].resize(kNumBlockHorizontal);
		}

		//	ブロックの生成
	    for (uint32_t row = 0; row < kNumblockVirtical; row++) {
		    for (uint32_t column = 0; column < kNumBlockHorizontal; column++) {
				
					worldTransformBlocks_[row][column] = new WorldTransform();
					worldTransformBlocks_[row][column]->Initialize();
					if ((row + column) % 2 == 0) {
						worldTransformBlocks_[row][column]->translation_.x = kBlockWidth * column;
						worldTransformBlocks_[row][column]->translation_.y = kBlockHeight * row;
					}
			}
		}

		viewProjection_.Initialize();

	    modelBlock_ = Model::CreateFromOBJ("block");

	    modelSkydome_=Model::CreateFromOBJ("sphere", true);

	    skydome_ = new Skydome();
	    skydome_->Initialize(modelSkydome_, &viewProjection_);

	    mapChipField_ = new MapChipField;
	    mapChipField_->LoadMapChipCsv("Resources/map.csv");

			    GenerateBlocks();
	   

	}

	void GameScene::Update() {

		//ブロックの更新

		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
					
				}
				worldTransformBlock->UpdateMatrix();
			}
		}
#ifdef _DEBUG
		
		if (input_->TriggerKey(DIK_SPACE)) {
			isDebugCameraActive_ = true;
		}		
#endif // _DEBUG

		//カメラの処理
		if (isDebugCameraActive_ == true) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		} else {
			//ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}
		
		skydome_->Update();
		
	}

	void GameScene::Draw() {

		// コマンドリストの取得
		ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
		// 背景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに背景スプライトの描画処理を追加できる
		/// </summary>

		// スプライト描画後処理
		Sprite::PostDraw();
		// 深度バッファクリア
		dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
		// 3Dオブジェクト描画前処理
		Model::PreDraw(commandList);


		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
	    
		/// </summary>
	
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			    if (!worldTransformBlock)
				    continue;
				modelBlock_->Draw(*worldTransformBlock, viewProjection_);
			}
		}

		skydome_->Draw();


		// 3Dオブジェクト描画後処理
		Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
		// 前景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>

		// スプライト描画後処理
		Sprite::PostDraw();

#pragma endregion
	}

	void GameScene::GenerateBlocks() {
		//要素数
		uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	    uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

		//要素数を変更する
		//列数を設定（縦方向のブロック数）
		worldTransformBlocks_.resize(numBlockVirtical);
	    for (uint32_t i = 0; i < numBlockVirtical; i++) {
		    worldTransformBlocks_[i].resize(numBlockHorizontal);
	    }		//ブロックの生成
	    for (uint32_t i = 0; i < numBlockVirtical; ++i) {

		    for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
				
				if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {

					WorldTransform* worldTransform = new WorldTransform();
				    worldTransform->Initialize();
				    worldTransformBlocks_[i][j] = worldTransform;

					worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			    }
			}
		}
	}