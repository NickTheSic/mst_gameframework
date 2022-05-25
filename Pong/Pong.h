#include <Core/Engine.h>
#include <Renderer/QuadRenderer.h>
#include <Renderer/TextRenderer.h>
#include <vector>

class MyGame : public mst::Engine
{
public:
	MyGame(int w, int h) : mst::Engine(w,h) {};
	~MyGame();
	virtual bool UserStartup();
	virtual void UserResize();
	virtual void UserUpdate();
	virtual void UserRender();

	void ClampXPositionUp(v2f& pos, const v2f& size);
	void ClampXPositionDown(v2f& pos);

	float RandomFloat(float low = 0, float high = 1);
	void BounceBall();
	v2f GetNewBallDirection();

	mst::QuadRenderer* QuadRenderer = nullptr;
	mst::TextRenderer* TextRenderer = nullptr;

	v2f Player1Paddle;
	v2f Player2Paddle;
	v2f PaddleSize = v2f(10.f, 40.f);

	v2f Ball;
	v2f BallSize;
	v2f BallMoveDirection;

	float PaddMoveSpeed = 80;
	float PaddleWallDistance = 20;
	float BallSpeed = 80.0f;

	int Player1Score;
	int Player2Score;
};
