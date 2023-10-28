#pragma once
class Board
{
public:
    int size; // Dimensiunea tablei
    int** board; // Matrice de �ntregi pentru reprezentarea tablei

    // Constructor
    Board(int boardSize);
    ~Board();
    Board(const Board& other);
    Board& operator=(const Board& other);

};

