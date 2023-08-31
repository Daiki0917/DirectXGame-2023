#pragma once

enum SCENE
{
	TITLE,//0
	GAMEPLAY,//1
	GAMECLEAR,//2
	GAMEOVER,//3
};

class Scene {
public:
	Scene(const Scene& obj) = delete;
	Scene& operator=(const Scene& obj) = delete;

	static Scene* GetInstance();

	void Draw();

	void ChangeScene();

private:
	char keys[256] = {0};
	char preKeys[256] = {0};
	int sceneNo = 0;
	const char* sceneString[4] = {"Title", "GamePlay", "GameClaer", "GameOver"};
	Scene();
	~Scene();

};
