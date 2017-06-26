#include "Lobby.h"
#include "Utils.h"

// --------------------------------------
// Author: Rony Hanna
// Description: Function that adds dynamically allocates a player node and adds it to the back of the list
// --------------------------------------
void Lobby::AddPlayer()
{
	Player* NewPlayer = new Player();

	if (m_pHead == nullptr)
		m_pHead = NewPlayer;
	else
	{
		Player* pIter = m_pHead;

		while (pIter->GetNextPlayer() != nullptr)
			pIter = pIter->GetNextPlayer();
		
		if (pIter->GetNextPlayer() == nullptr)
			pIter->SetNextPlayer(NewPlayer);
	}
	return;
}

// --------------------------------------
// Author: Rony Hanna
// Description: Function that kicks the first player on the linked list
// --------------------------------------
void Lobby::KickPlayer()
{
	if (m_pHead == nullptr)
		return;
	else
	{
		Player* pTemp = m_pHead;
		m_pHead = m_pHead->GetNextPlayer();		
		delete pTemp;
		pTemp = nullptr;

		m_bKickFirstPlayer = true;
	}
}

// --------------------------------------
// Author: Rony Hanna
// Description: Function that clears the entire lobby
// --------------------------------------
void Lobby::ClearLobby()
{
	while (m_pHead != nullptr)
	{
		Player* pTemp = m_pHead;
		m_pHead = m_pHead->GetNextPlayer();
		delete pTemp;
		pTemp = nullptr;
		m_PlayerVec.erase(m_PlayerVec.begin());
	}

	return;
}

// --------------------------------------
// Author: Rony Hanna
// Description: Function that renders the players the user places 
// --------------------------------------
void Lobby::RenderPlayer(const int X, const int Y, HDC BackbufferDC)
{
	if (m_pHead == nullptr)
	{
		DisplayText(BackbufferDC);
		return;
	}
	else
	{
		Player* pIter = m_pHead;
		while (pIter != nullptr)
		{
			HBRUSH Redbrush;
			HPEN NullPen, OldPen, LinkingPen;

			Redbrush = CreateSolidBrush(RGB(255, 0, 0));
			LinkingPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

			NullPen = (HPEN)GetStockObject(NULL_PEN);
			OldPen = (HPEN)SelectObject(BackbufferDC, NullPen);
			
			int left = 70;
			int right = 10;
			int top = 70;
			int bottom = 10;
			
			// Check if first player is to be kicked
			if (m_bKickFirstPlayer)
			{
				// If so erase from the vector
				m_PlayerVec.erase(m_PlayerVec.begin());
				m_bKickFirstPlayer = false;
			}

			for (unsigned int i = 0; i < m_PlayerVec.size(); ++i)
			{
				SelectObject(BackbufferDC, Redbrush);
				Rectangle(BackbufferDC, m_PlayerVec[i].m_X + left, m_PlayerVec[i].m_Y + top, m_PlayerVec[i].m_X + right, m_PlayerVec[i].m_Y + bottom);
			}

			// If there are at least 2 players or more in the lobby
			if (m_PlayerVec.size() >= 2)
			{
				// Draw the link
				for (unsigned int i = 0; i < m_PlayerVec.size() - 1; ++i)
				{
					SelectObject(BackbufferDC, LinkingPen);
					MoveToEx(BackbufferDC, m_PlayerVec[i].m_X + left / 2, m_PlayerVec[i].m_Y + top / 2, 0);
					LineTo(BackbufferDC, m_PlayerVec[i + 1].m_X + left / 2, m_PlayerVec[i + 1].m_Y + top / 2);
					Ellipse(BackbufferDC, m_PlayerVec[i + 1].m_X + 16 + left / 2, m_PlayerVec[i + 1].m_Y + 16 + top / 2, m_PlayerVec[i + 1].m_X + 16 + right, m_PlayerVec[i + 1].m_Y + 16 + bottom);
				}
			}

			DisplayText(BackbufferDC);

			// Free memory
			DeleteObject(Redbrush);
			DeleteObject(LinkingPen);

			pIter = pIter->GetNextPlayer();
		}
	}
}

// --------------------------------------
// Author: Rony Hanna
// Description: Helper method that gets the mouse position
// --------------------------------------
void Lobby::GetMouseCoord(double _mouseX, double _mouseY)
{
	m_MouseX = _mouseX;
	m_MouseY = _mouseY;
}

// --------------------------------------
// Author: Rony Hanna
// Description: Function that displays text to the screen 
// --------------------------------------
void Lobby::DisplayText(HDC BackbufferDC)
{
	for (unsigned int i = 0; i < m_PlayerVec.size(); ++i)
	{
		std::string Text = "Player " + ToString(i);
		TextOutA(BackbufferDC, m_PlayerVec[i].m_X, m_PlayerVec[i].m_Y, Text.c_str(), Text.size());
	}

	std::string NumOfPlayerTextLabel = "Number Of Players: " + ToString(m_PlayerVec.size());

	TextOutA(BackbufferDC, 5, 500, NumOfPlayerTextLabel.c_str(), NumOfPlayerTextLabel.size());
}
