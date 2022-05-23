#include <Core/Engine.h>
#include <Renderer/QuadRenderer.h>
#include <Renderer/TextRenderer.h>
#include <vector>

class MyGame : public mst::Engine
{
public:
	~MyGame();
	virtual bool UserStartup();
	virtual void UserResize();
	virtual void UserUpdate();
	virtual void UserRender();

	mst::QuadRenderer* QuadRenderer = nullptr;
	mst::TextRenderer* TextRenderer = nullptr;
};
