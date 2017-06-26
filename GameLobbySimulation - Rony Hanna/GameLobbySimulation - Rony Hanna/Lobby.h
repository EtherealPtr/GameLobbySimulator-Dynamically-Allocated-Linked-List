#pragma once
#ifndef __LOBBY_H__
#define __LOBBY_H__

#include "Player.h"
#include <Windows.h>
#include <vector>

using std::cin;
using std::getline;
using std::vector;

struct Vector2D
{
	Vector2D(double X, double Y)
	{
		m_X = X;
		m_Y = Y;
	}

	double m_X;
	double m_Y;
};

class Lobby
{
public:
	Lobby() { m_pHead = nullptr; };
	~Lobby() { ClearLobby(); };

	void AddPlayer();
	void KickPlayer();
	void ClearLobby();
	void RenderPlayer(const int, const int, HDC);
	void AddPlayerVector(const Vector2D& aPlayer) { m_PlayerVec.push_back(aPlayer); };
	void GetMouseCoord(double, double);
	void DisplayText(HDC);

private:
	double m_MouseX, m_MouseY;
	Player* m_pHead;
	bool m_bKickFirstPlayer;
	std::vector<Vector2D> m_PlayerVec;
};

#endif // !__LOBBY_H__