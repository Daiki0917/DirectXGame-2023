#include "Scene.h"

SceneManager::SceneManager() { sceneNo = TITLE; }

SceneManager::~SceneManager() {}

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;

	return &instance;
}

void SceneManager::ChangeScene() {
	memcpy(preKeys, keys, 256);
	Novice::GetHitKeyStateAll(keys);

	switch (sceneNo) {
	case TITLE:
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			sceneNo = 1;
		}
		break;

	case NEWGAME:
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			sceneNo = 2;
		}
		break;

	case GAMEPLAY:
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			sceneNo = 3;
		}
		break;

	case GAMECLEAR:
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			sceneNo = 0;
		}
		break;
	}
}

void SceneManager::Draw() {
	switch (sceneNo) {
	case TITLE:
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, RED, kFillModeSolid);
		break;
	case NEWGAME:
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, BLUE, kFillModeSolid);
		break;
	case GAMEPLAY:
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, GREEN, kFillModeSolid);
		break;
	case GAMECLEAR:
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, BLACK, kFillModeSolid);
		break;
	}

	Novice::ScreenPrintf(100, 200, "SceneNo : %d", sceneNo);
	Novice::ScreenPrintf(100, 220, "Press Space to Scene Change");
}