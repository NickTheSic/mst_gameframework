#include <Core/Engine.h>
#include <Renderer/SpriteSheetRenderer.h>
#include <Renderer/TextRenderer.h>

// I guess it will be easiest if the dice value starts at 1

constexpr int MAX_GRID_PIECES = 8;
constexpr int PLAYER_START = 8;
constexpr int LEVEL_WALL = 7;

struct Dice
{
	int CurrentValue = 1;
	int CurrentGridSpace = 0;

	int Top, Bottom, Left, Right, Flip;

	void Reset(int GridSpace = 0)
	{
		CurrentValue = 1;
		CurrentGridSpace = GridSpace;

		Top = 3;
		Bottom = 4;
		Left = 5;
		Right = 2;
		Flip = 6;
	}
};

struct Grid
{
	v2i GridSize;
	int Count;
	int GoalSpace;
	int* Pieces;
	v2f SpaceSize = {64.f,64.f};
	v2f WorldOffset = {100.f, 100.f};

	Grid();
	void InitGrid(const v2i& Size, int *Pieces);

	v2i GetCoordAtIndex(int idx);
	v2f GetWorldCoordAtIndex(int idx);
	int GetIndexAtCoor(const v2i& Coord);

	int& operator[](int idx)
	{
		return Pieces[idx];
	}

	int operator[](int idx) const
	{
		return Pieces[idx];
	}
};

void SaveGrid(const Grid& Grid);
void LoadGrid(Grid& Grid, int LevelIndex);

class MyGame : public mst::Engine
{
public:
	MyGame(int x, int y) : mst::Engine(x,y) {}
	~MyGame();
	virtual bool UserStartup();
	virtual void UserResize();
	virtual void UserUpdate();
	virtual void UserRender();

	#ifndef PLATFORM_WEB
	void EditorUpdate();
	int HoveredSquare = -1;
	bool EditorMode = false;
	#endif

	void GameUpdate();
	void ResetLevel();
	void SetLevel(int level);

	Grid LoadedGrid = {};
	int LevelMax;
	int LevelCurrent;
	int CurrentMoves = 0;

	Dice Die = {};

	bool OnTitleScreen = true;
	bool CompletedCurrentLevel = false;

	mst::TextRenderer* TextRenderer = nullptr;
	mst::SpriteSheetGeneratorRenderer* SpriteRenderer = nullptr;
	mst::SpriteSheetGeneratorRenderer* TitleScreenRenderer = nullptr;
};
