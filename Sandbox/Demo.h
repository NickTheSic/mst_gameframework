#include <Core/Engine.h>
#include <Renderer/QuadRenderer.h>
#include <Renderer/TextRenderer.h>
#include <Renderer/SpriteSheetRenderer.h>
#include <vector>

class MyGame : public mst::Engine
{
public:
	~MyGame();
	virtual bool UserStartup();
	virtual void UserResize();
	virtual void UserUpdate();
	virtual void UserRender();
	v2i WorldSpaceToIndex(const v2f& WorldCoord);

	int SquareSizes = 20;
	float Tick = 0.0f;

	mst::QuadRenderer* QuadRenderer = nullptr;
	mst::TextRenderer* TextRenderer = nullptr;
	mst::TextRenderer* TextRenderer2 = nullptr;
	mst::SpriteSheetGeneratorRenderer* FirstSpriteSheetGenerator = nullptr;

	std::vector<v2f> MousePositions;
	std::vector<v2f> GridRectPositions;
	std::vector<Color> RandomColours;
	v2i GridSize;

	v2f* MoveableGridPiece = nullptr;
};
