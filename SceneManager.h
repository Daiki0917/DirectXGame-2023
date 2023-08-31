#pragma once
#include"Input.h"

 enum SCENE {
  TITLE,     // 0
  GAMEPLAY,  // 1
  GAMECLEAR, // 2
  GAMEOVER,  //3
 };

class SceneManager {
public:
	SceneManager(const SceneManager& obj) = delete;
	SceneManager& operator=(const SceneManager& obj) = delete;

	static SceneManager* GetInstance();

	void Draw();

	void ChangeScene();

private:
	Input* input_ = nullptr;

	char keys[256] = {0};
	char preKeys[256] = {0};
	int sceneNo = 0;
	const char* sceneString[4] = {"Title", "GamePlay", "GameClear", "GameOver"};
	SceneManager();
	~SceneManager();
};
