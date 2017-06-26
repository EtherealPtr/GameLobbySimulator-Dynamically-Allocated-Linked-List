#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <string>

class Player
{
public:
	Player() { m_pNext = nullptr; };

	inline Player* Player::GetNextPlayer() const { return m_pNext; };
	inline void Player::SetNextPlayer(Player* NextPlayer) { m_pNext = NextPlayer; };

private:
	Player* m_pNext;
};

#endif // !__PLAYER_H__
