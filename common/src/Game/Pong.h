#pragma once

inline constexpr float GameSizeX = 1600.f;
inline constexpr float GameSizeY = 900.f;
inline constexpr float BallRadius = 5.f;
inline constexpr float PaddleDistFromBorders = 10.f;
inline constexpr float PaddleHeight = 100.f;

enum class PlayerSide
{
	No,
	Left,
	Right

};

enum class GameStateInfo
{
	Ongoing = 0,
	LeftWins,
	RightWins
};

enum class PaddlesBehaviour
{
	None = 0,

	LeftUp = 1 << 0,
	LeftDown = 1 << 1,
	RightUp = 1 << 2,
	RightDown = 1 << 3,

	Left = LeftUp | LeftDown,
	Right = RightUp | RightDown,
};

PaddlesBehaviour operator|(PaddlesBehaviour lhs, PaddlesBehaviour rhs);
PaddlesBehaviour operator|=(PaddlesBehaviour& lhs, PaddlesBehaviour rhs);
PaddlesBehaviour operator&(PaddlesBehaviour lhs, PaddlesBehaviour rhs);
PaddlesBehaviour operator&=(PaddlesBehaviour& lhs, PaddlesBehaviour rhs);
PaddlesBehaviour operator~(PaddlesBehaviour rhs);

struct Pong
{
	void Reset();
	void Update(float dt);
	GameStateInfo GameStateInfos() const;
	const PlayerSide& GamePlayerSide();

	float BallX, BallY;
	float BallDx, BallDy;
	float LeftPaddle, RightPaddle;
	bool isFirstPlayer;
	PaddlesBehaviour Behaviours;
};
