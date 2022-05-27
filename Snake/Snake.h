#include <Core/Engine.h>
#include <Renderer/QuadRenderer.h>
#include <Renderer/TextRenderer.h>
#include <vector>

class MyGame : public mst::Engine
{
public:
	MyGame(int x, int y) : mst::Engine(x,y) {}
	~MyGame();
	virtual bool UserStartup();
	virtual void UserResize();
	virtual void UserUpdate();
	virtual void UserRender();

	v2f GetRandomPosOnScreen();

	std::vector<v2f> Snake;
	v2f Apple;
	v2f Size;
	v2f Dir;

	float UpdateInterval = 0.2f;
	float CurrentUpdate = 0.0f;

	mst::QuadRenderer* QuadRenderer = nullptr;
	mst::TextRenderer* TextRenderer = nullptr;
};
