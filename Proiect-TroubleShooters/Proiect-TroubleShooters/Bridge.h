#pragma once
#include "Piece.h"
//#include "Player.h"

//class Player;
class Bridge
{
    const  Piece& m_piece1; 
    const Piece& m_piece2;
public:
	Bridge(const Piece& piece1,const Piece& piece2);
	Bridge(const Bridge& other)=default;
	Bridge& operator=(const Bridge& other)noexcept=default;
	Bridge(Bridge&& other)=default;
	Bridge& operator=(Bridge&& other)=default;
	const Piece& getPiece1() const;
	const Piece& getPiece2() const;
};

