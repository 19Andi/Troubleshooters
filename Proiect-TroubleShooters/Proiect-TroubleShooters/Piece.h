#pragma once
#include <cstdint>
#include<iostream>
#include <QColor>
enum Color
{
    None = 0,
    Red = 1,
    Black = 2
};
class Piece
{
    QColor m_color;// Color of the piece
    uint16_t x, y; // Position on the board
public:
    // Constructor
    Piece(QColor color = Qt::gray, uint16_t x = 0, uint16_t y = 0);
    ~Piece() = default;
    Piece(const Piece& other) = default;
    Piece& operator=(const Piece& other) = default;
    Piece(Piece&& other) noexcept = default;
    Piece& operator=(Piece&& other) noexcept = default;
    QColor getColor() const;
    void setColor(QColor color);
    const uint16_t& getX()const;
    const uint16_t& getY()const;
    bool operator==(const Piece& other) const;
    friend std::ostream& operator<<(std::ostream& out, const Piece& piece);
    friend std::istream& operator>>(std::istream& in, Piece& piece);
};