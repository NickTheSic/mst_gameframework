#include "DiceGame.h"
#include <iostream>
#include <fstream>
#include <Utils.h>
#include <UI/imUI.h>

#ifndef PLATFORM_WEB
#include <filesystem>
#endif

#define dbgval(val) std::cout << #val << ": " << val << std::endl;
#define dbglog(msg) std::cout << msg << std::endl;

static int GridSaveCount = 0;

static const char* GridPath = "Data/Grids/";

static int GetSavedGrids()
{
	int count = 0;
	#ifndef PLATFORM_WEB
	for (const auto& entry : std::filesystem::directory_iterator(GridPath))
	{
		dbgval(entry);
		count++;
	}
	#else
	count = 21;
	#endif
	return count;
}

std::ostream& operator<<(std::ostream& os, const v2i& v)
{
	os << "X: " << v.x << " Y: " << v.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, const v2f& v)
{
	os << "X: " << v.x << " Y: " << v.y;
	return os;
}

mst::Engine* mst::Engine::CreateEngine()
{
	MyGame* g = new MyGame(600, 600);

	//AudioPlayer::Get();

	GridSaveCount = GetSavedGrids();

	g->LevelReached = GridSaveCount;

	return g;
}

MyGame::~MyGame()
{
	if (TextRenderer)
	{
		delete TextRenderer;
	}

	if (SpriteRenderer)
	{
		delete SpriteRenderer;
	}

	if (TitleScreenRenderer)
	{
		delete TitleScreenRenderer;
	}
}

bool MyGame::UserStartup()
{
	glClearDepth(1.0f);

	glClearColor(0.1f,0.1f,0.1f,1.f);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	MainCamera.Position = ScreenSize / 2;

	LevelCurrent = 0;
	
	TextRenderer = new mst::TextRenderer();
	TextRenderer->Init(100, "Data/caviardreamsbold.ttf");
	TextRenderer->UseProgram();
	TextRenderer->SetUniform("u_CameraPos", MainCamera.Position);

	SpriteRenderer = new mst::SpriteSheetGeneratorRenderer(100, 
				{"Data/Dice0.png","Data/Dice1.png",   // 0 is empty
				 "Data/Dice2.png","Data/Dice3.png",
				 "Data/Dice4.png","Data/Dice5.png",
				 "Data/Dice6.png","Data/Dice7.png",   // 7 is empty but darker for walls
				 #ifndef PLATFORM_WEB
				 "Data/Dice8.png"});                  // 8 is for the starting spot, possibly editor only? 
				 #else
				 "Data/Dice0.png"});
				 #endif
	SpriteRenderer->UseProgram();
	SpriteRenderer->SetUniform("u_CameraPos", MainCamera.Position);

	TitleScreenRenderer = new mst::SpriteSheetGeneratorRenderer(2,
				{"Data/gmtk_jam.png", "Data/roll_of_the_dice.png"});
	TitleScreenRenderer->UseProgram();
	TitleScreenRenderer->SetUniform("u_CameraPos", MainCamera.Position);

	UserResize();

	ui::InitUI();

	if (GridSaveCount > 0)
	{
		LoadGrid(LoadedGrid,0);
	}
	else
	{ 
		v2f GridSize(8,1);
		int arr[] = {0,1,2,3,4,5,6,7,8};

		LoadedGrid.InitGrid(GridSize, arr);
	}

	Die.Reset();

	SetLevel(LevelCurrent);

	int b = 0;

	srand(timer.startoffset);

	return true;
}

void MyGame::UserResize()
{
	if (TextRenderer != nullptr)
	{
		TextRenderer->UseProgram();
		TextRenderer->SetUniform("u_WorldSize", ScreenSize);
	}

	if (SpriteRenderer != nullptr)
	{
		SpriteRenderer->UseProgram();
		SpriteRenderer->SetUniform("u_WorldSize", ScreenSize);
	}

	if (TitleScreenRenderer != nullptr)
	{
		TitleScreenRenderer->UseProgram();
		TitleScreenRenderer->SetUniform("u_WorldSize", ScreenSize);
	}
}

void MyGame::UserUpdate()
{
	if (OnTitleScreen)
	{
		if (IsKeyPressed(mst::Key::SPACE))
		{
			OnTitleScreen = false;
		}
		return;
	}

#ifndef PLATFORM_WEB
	if (IsKeyPressed(mst::Key::ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}

	if (IsKeyPressed(mst::Key::T))
	{
		EditorMode = !EditorMode;
		if (!EditorMode)
		{
			HoveredSquare = -1;
			Die.Reset();
			for (int i = 0; i < LoadedGrid.Count; i++)
			{
				if (LoadedGrid[i] == PLAYER_START)
				{
					Die.CurrentGridSpace = i;
					break;
				}
			}
		}
	}

	if (EditorMode)
	{
		EditorUpdate();
	}
	else
#endif
	GameUpdate();
}

void MyGame::UserRender()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (OnTitleScreen)
	{
		TitleScreenRenderer->StartRender();
		TitleScreenRenderer->RenderScaledSpriteAtIndexCenter(1, {ScreenCenter.x, ScreenCenter.y + 160}, 0.1);
		TitleScreenRenderer->RenderScaledSpriteAtIndexCenter(0, {50,50}, 0.05);
		TitleScreenRenderer->EndRender();

		TextRenderer->StartRender();

		TextRenderer->RenderText("A Sokoban Game", { 170, ScreenCenter.y+20 }, 2);

		TextRenderer->RenderText("Press Space to Start the Game!", {180, 100});

		TextRenderer->RenderTextFromRight("Controls:", {ScreenSize.x-170, 220});
		TextRenderer->RenderTextFromRight("Move: W/A/S/D", { ScreenSize.x-170, 190 });
		TextRenderer->RenderTextFromRight("Reset: R", { ScreenSize.x - 170, 160 });
		TextRenderer->RenderTextFromRight("Menu: Q", { ScreenSize.x - 170, 130 });

		TextRenderer->EndRender();

		
		v2f initialPos = {ScreenSize.x - 80, ScreenSize.y - 30};

		for (int i = 0; i < LevelReached; i++)
		{ 
			std::string label = "Level: " + std::to_string(i);
			if (ui::AddButton(label.c_str(), initialPos, {60,120,100}))
			{
				SetLevel(i);
				OnTitleScreen = false;
			}
			initialPos.y -= 18;
		}

		ui::Present();

		return;
	}

	SpriteRenderer->StartRender();

	for (int i = 0; i < LoadedGrid.Count; i++)
	{
		#ifndef PLATFORM_WEB
		if (i == HoveredSquare)
			SpriteRenderer->RenderSpriteAtIndex(LoadedGrid.Pieces[i], LoadedGrid.GetWorldCoordAtIndex(i), { 100,150,100 });
		else
		#endif
			SpriteRenderer->RenderSpriteAtIndex(LoadedGrid.Pieces[i], LoadedGrid.GetWorldCoordAtIndex(i), { 80, 120, 200 });
	}

	// Render Die;
	#ifndef PLATFORM_WEB
	if (!EditorMode)
	#endif
	SpriteRenderer->RenderSpriteAtIndex(Die.CurrentValue, LoadedGrid.GetWorldCoordAtIndex(Die.CurrentGridSpace));

	SpriteRenderer->RenderScaledSpriteAtIndexCenter(Die.Top, { 100, 50 }, 0.5f );
	SpriteRenderer->RenderScaledSpriteAtIndexCenter(Die.CurrentValue, {100, 90}, 0.5f);
	SpriteRenderer->RenderScaledSpriteAtIndexCenter(Die.Bottom, { 100, 130 }, 0.5f);
	SpriteRenderer->RenderScaledSpriteAtIndexCenter(Die.Left, {60,90}, 0.5f);
	SpriteRenderer->RenderScaledSpriteAtIndexCenter(Die.Right, {140, 90}, 0.5f);

	SpriteRenderer->EndRender();

	TextRenderer->StartRender();
	std::string CurrLevel = "Level: " + std::to_string(LevelCurrent);
	TextRenderer->RenderText(CurrLevel, v2f{10, ScreenSize.y - 60 });
	std::string CurrMoves = "Moves: " + std::to_string(CurrentMoves);
	TextRenderer->RenderText(CurrMoves, v2f{10, ScreenSize.y - 80 });

	if (CompletedCurrentLevel)
	{
		if (LevelCurrent != GridSaveCount-1)
		{ 
			TextRenderer->RenderText("Level Complete!", v2f(ScreenCenter.x-200,ScreenCenter.y+200), 2);
			TextRenderer->RenderText("Press Space To Continue", v2f(ScreenCenter.x-200, ScreenCenter.y+170),1);
		}
		else
		{
			TextRenderer->RenderText("Game Complete!", v2f(ScreenCenter.x - 200, ScreenCenter.y), 2);
		}
	}

	#ifndef PLATFORM_WEB
	if (EditorMode)
	{ 
		std::string values;
		for (int i = 0; i < LoadedGrid.Count; i++)
		{
			values.append(std::to_string(LoadedGrid.Pieces[i]) + ", ");
		}
		TextRenderer->RenderText(values, {0,0});
	}
	#endif

	TextRenderer->EndRender();
}

#ifndef PLATFORM_WEB
void MyGame::EditorUpdate()
{
	if (IsKeyPressed(mst::Key::S) && IsKeyDown(mst::Key::CONTROL_LEFT))
	{
		SaveGrid(LoadedGrid);
	}

	if (IsKeyPressed(mst::Key::L) && IsKeyDown(mst::Key::CONTROL_LEFT))
	{
		LoadGrid(LoadedGrid, LevelCurrent);
	}

	if (IsKeyPressed(mst::Key::H))
	{
		++LevelCurrent;
		if (LevelCurrent >= GridSaveCount)
		{
			LevelCurrent = 0;
		}
	}

	if (IsKeyPressed(mst::Key::G))
	{
		--LevelCurrent;
		if (LevelCurrent < 0)
		{
			LevelCurrent = GridSaveCount;
		}
	}

	HoveredSquare = -1;
	for (int i = 0; i < LoadedGrid.Count; i++)
	{
		if (Utils::IsPointInRect(GetMousePosition(), LoadedGrid.GetWorldCoordAtIndex(i), LoadedGrid.SpaceSize))
		{
			HoveredSquare = i;
			if (IsMouseButtonPressed(0))
			{
				LoadedGrid[i]++;
				if (LoadedGrid[i] > MAX_GRID_PIECES) LoadedGrid[i] = 0;
			}
			else if (IsMouseButtonPressed(1))
			{
				LoadedGrid[i]--;
				if (LoadedGrid[i] < 0) LoadedGrid[i] = MAX_GRID_PIECES;
			}
			break;
		}
	}

	v2i PreviousGridSize = LoadedGrid.GridSize;

	if (IsKeyPressed(mst::Key::J))
	{
		LoadedGrid.GridSize.x--;
		if (LoadedGrid.GridSize.x <= 0) LoadedGrid.GridSize.x = 1;
	}
	if (IsKeyPressed(mst::Key::K))
	{
		LoadedGrid.GridSize.x++;
	}

	if (IsKeyPressed(mst::Key::N))
	{
		LoadedGrid.GridSize.y--;
		if (LoadedGrid.GridSize.y <= 0) LoadedGrid.GridSize.y = 1;
	}
	if (IsKeyPressed(mst::Key::M))
	{
		LoadedGrid.GridSize.y++;
	}
	
	if (PreviousGridSize != LoadedGrid.GridSize)
	{
		LoadedGrid.InitGrid(LoadedGrid.GridSize, nullptr);
	}
}
#endif

void MyGame::GameUpdate()
{
	if (IsKeyDown(mst::Key::R))
	{
		CompletedCurrentLevel = false;
		ResetLevel();
		return;
	}

	if (IsKeyDown(mst::Key::Q))
	{
		OnTitleScreen = true;
		return;
	}

	if (IsKeyDown(mst::Key::Y))
	{
		CompletedCurrentLevel = false;
		Die.Reset();
		for (int i = 0; i < LoadedGrid.Count; i++)
		{
			if (LoadedGrid[i] == PLAYER_START)
			{
				Die.CurrentGridSpace = i;
				break;
			}
		}
		return;
	}

	if (CompletedCurrentLevel)
	{
		if (IsKeyPressed(mst::Key::SPACE))
		{ 
			CompletedCurrentLevel = false;
			LevelCurrent++;
			if (LevelCurrent >= GridSaveCount)
			{	
				LevelCurrent = 0;
				OnTitleScreen = true;
			}
			SetLevel(LevelCurrent);
		}
		return;
	}

	int Temp = Die.CurrentValue;
	int Index;
	int ExpectedSpot;

	bool MovedThisFrame = false;

	if (IsKeyPressed(mst::Key::W))
	{
		Index = Die.CurrentGridSpace + LoadedGrid.GridSize.x;
		ExpectedSpot = LoadedGrid[Index];
		if (ExpectedSpot != LEVEL_WALL && Index <= LoadedGrid.Count-1)
		{ 
			// Roll Dice Up
			Die.CurrentGridSpace += LoadedGrid.GridSize.x;
			Die.CurrentValue = Die.Bottom;
			Die.Bottom = Die.Flip;
			Die.Flip = Die.Top;
			Die.Top = Temp;
			MovedThisFrame = true;
		}
	}

	if (IsKeyPressed(mst::Key::S))
	{
		Index = Die.CurrentGridSpace - LoadedGrid.GridSize.x;
		ExpectedSpot = LoadedGrid[Index];
		if (ExpectedSpot != LEVEL_WALL && Index >= 0)
		{
			// Roll Dice Down
			Die.CurrentGridSpace -= LoadedGrid.GridSize.x;
			Die.CurrentValue = Die.Top;
			Die.Top = Die.Flip;
			Die.Flip = Die.Bottom;
			Die.Bottom = Temp;
			MovedThisFrame = true;
		}
	}

	if (IsKeyPressed(mst::Key::A))
	{
		Index = Die.CurrentGridSpace - 1;
		ExpectedSpot = LoadedGrid[Index];
		if (ExpectedSpot != LEVEL_WALL && (Index+1)%LoadedGrid.GridSize.x != 0)
		{
			// Roll Dice Left
			Die.CurrentGridSpace--;
			Die.CurrentValue = Die.Left;
			Die.Left = Die.Flip;
			Die.Flip = Die.Right;
			Die.Right = Temp;
			MovedThisFrame = true;
		}
	}

	if (IsKeyPressed(mst::Key::D))
	{
		Index = Die.CurrentGridSpace + 1;
		ExpectedSpot = LoadedGrid[Index];
		int Comp = ((Index -1) / LoadedGrid.GridSize.x) + 1;
		if (ExpectedSpot != LEVEL_WALL && Index < (Comp * LoadedGrid.GridSize.x))
		{
			Die.CurrentGridSpace++;
			Die.CurrentValue = Die.Right;
			Die.Right = Die.Flip;
			Die.Flip = Die.Left;
			Die.Left = Temp;
			MovedThisFrame = true;
		}
	}

	if (MovedThisFrame)
	{
		CurrentMoves++;
		if (Die.CurrentGridSpace != LoadedGrid.GoalSpace)
		{
			return;
		}
		
		if (Die.CurrentValue == LoadedGrid[LoadedGrid.GoalSpace])
		{
			CompletedCurrentLevel = true;
		}
	}
}

void MyGame::ResetLevel()
{
	SetLevel(LevelCurrent);
}

Grid::Grid(): GridSize(0,0), Count(0), Pieces(nullptr){}

void Grid::InitGrid(const v2i& Size, int* Arr)
{
	this->GridSize= Size;
	Count = Size.x * Size.y;

	v2f basedWorldOffset = v2f((Size.x/2.f)*this->SpaceSize.x, (Size.y/2.f)*this->SpaceSize.y);
	WorldOffset = mst::Engine::Get()->GetScreenCenter() - basedWorldOffset;

	if (Pieces != nullptr)
	{
		delete[] Pieces;
	}

	Pieces = new int[Count];

	if (Arr != nullptr)
	{ 
		for (int i = 0; i < Count; i++)
		{
			Pieces[i] = Arr[i];

			if (Pieces[i] > 0 && Pieces[i] < 7)
			{
				GoalSpace = i;
			}
		}
	}
	else
	{
		for (int i = 0; i < Count; i++)
		{
			Pieces[i] = 0;
		}
	}
}

v2i Grid::GetCoordAtIndex(int idx)
{
	return std::move(v2i(idx%GridSize.x, idx/GridSize.x));
}

v2f Grid::GetWorldCoordAtIndex(int idx)
{
	return std::move((v2f)GetCoordAtIndex(idx) * SpaceSize + WorldOffset);
}

int Grid::GetIndexAtCoor(const v2i& Coord)
{
	return -1;
}

void SaveGrid(const Grid& Grid)
{
	std::string filename = GridPath;
	filename.append("Grid_");
	filename.append(std::to_string(GridSaveCount));
	filename.append(".bin");

	std::ofstream Output;
	Output.open(filename.c_str(), std::ios::out | std::ios::binary);

	if (Output.is_open())
	{
		Output.write(reinterpret_cast<const char*>(&Grid.GridSize.x), sizeof(Grid.GridSize.x));
		Output.write(reinterpret_cast<const char*>(&Grid.GridSize.y), sizeof(Grid.GridSize.y));

		for (int i = 0; i < Grid.Count; i++)
		{
			Output.write(reinterpret_cast<const char*>(&Grid.Pieces[i]), sizeof(int));
		}

		++GridSaveCount;

		Output.close();
	}
}

void LoadGrid(Grid& InGrid, int LevelIndex)
{
	std::string filename = GridPath;
	filename.append("Grid_");
	filename.append(std::to_string(LevelIndex));
	filename.append(".bin");

	v2i Size;
	int* Pieces = nullptr;
	
	std::ifstream Input;
	Input.open(filename.c_str(), std::ios::in | std::ios::binary);

	if (Input.is_open())
	{
		Input.read(reinterpret_cast<char*> (&Size.x), sizeof(int));
		Input.read(reinterpret_cast<char*> (&Size.y), sizeof(int));
		
		size_t Count = Size.x * Size.y;
		
		Pieces = new int[Count];
		
		for (int i = 0; i < Count; i++)
		{ 
			Input.read(reinterpret_cast<char*>(&Pieces[i]), sizeof(int));
		}

		Input.close();

		InGrid.InitGrid(Size, Pieces);

		delete[] Pieces;
	}
}

void MyGame::SetLevel(int level)
{
	CompletedCurrentLevel = false;
	CurrentMoves = 0;
	LevelCurrent = level;

	if (level > LevelReached)
	{
		LevelReached = level;
	}

	LoadGrid(LoadedGrid, level);

	Die.Reset();

	for (int i = 0; i < LoadedGrid.Count; i++)
	{
		if (LoadedGrid[i] == PLAYER_START)
		{
			Die.CurrentGridSpace = i;
			break;
		}
	}
}
