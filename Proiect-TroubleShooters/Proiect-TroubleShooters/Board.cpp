#include "Board.h"

// Constructor
Board::Board(uint16_t boardSize) : size(boardSize), board(boardSize, std::vector<Piece>(boardSize)), pieces(), bridges() {
    if (boardSize != 0)
    {
        std::uniform_int_distribution<int> distribution(1, boardSize - 2);
        std::random_device rd1;
        std::mt19937 x(rd1());
        dozer.first = distribution(x);
        std::random_device rd2;
        std::mt19937 y(rd2());
        dozer.second = distribution(y);
    }
}
// Copy constructor
Board::Board(const Board& other) : size(other.size), board(other.board), pieces(other.pieces), bridges(other.bridges), dozer(other.dozer) {
}

// Copy assignment operator
Board& Board::operator=(const Board& other) {
    if (this != &other) {
        size = other.size;
        board = other.board;
        pieces = other.pieces;
        dozer = other.dozer;
       // bridges = other.bridges;
    }
    return *this;
}

Board::Board(Board&& other) noexcept
    : size{ std::move(other.size) }, board{ std::move(other.board) }, dozer{ std::move(other.dozer) }
{
    // Dup? mutarea resurselor, cel?lalt obiect trebuie s? fie �ntr-o stare valid?
    other.size = 0;
    other.board.clear();
}

Board& Board::operator=(Board&& other) noexcept
{
    if (this != &other)
    {
        size = other.size;
        board = std::move(other.board);
        dozer = other.dozer;

        // Dup? mutarea resurselor, cel?lalt obiect trebuie s? fie �ntr-o stare valid?
        other.size = 0;
        other.board.clear();
    }
    return *this;
}

// Getter pentru dimensiune
uint16_t Board::getSize() const {
    return size;
}

// Getter pentru tabla de joc
const std::vector<std::vector<Piece>>& Board::getBoard() const {
    return board;
}
std::vector<Piece>& Board::getPieces()
{
    return pieces;
}
std::vector<Bridge>& Board::getBridges()
{
    return bridges;
}

Piece& Board::operator()(uint16_t x, uint16_t y)
{
    // Presupun�nd c? x ?i y sunt indici valizi
    return board[x][y];
}

void Board::deletePieces()
{
    pieces.clear();
}

void Board::deleteBridges()
{
    bridges.clear();
}

bool Board::isValidLocation(uint16_t x, uint16_t y) const
{
    if ((x == size - 1 || x == 0) && (y == size - 1 || y == 0))
        return false;
    return x >= 0 && x <= size - 1 && y>=0 && y <= size - 1;
}

bool Board::isOccupied(uint16_t x, uint16_t y) const {
    return board[x][y].getColor() != Color::None || (x == dozer.first && y == dozer.second);
}

void Board::displayBoard() const {
    for (uint16_t row = 0; row < size; row++) {
        if (row == 0 || row == size - 1) {
            std::cout << "  ";
        }
        for (uint16_t col = 0; col < size; col++) {
            if (!((row == size - 1 || row == 0) && (col == size - 1 || col == 0))) {
                if (board[row][col].getColor() == static_cast<int>(Color::Red)) {
                    std::cout << "R ";
                }
                else if (board[row][col].getColor() == static_cast<int>(Color::Black)) {
                    std::cout << "B ";
                }
                else if (row == dozer.first && col == dozer.second)
                    std::cout << "D ";
                else {
                    std::cout << "O ";
                }
            }
        }
        std::cout << '\n';
    }
}

bool Board::placeBridge(Piece& piece1,Piece& piece2) 
{
        if ((abs(piece1.getX() - piece2.getX()) == 1 && abs(piece1.getY() - piece2.getY()) == 2) ||
            (abs(piece1.getX() - piece2.getX()) == 2 && abs(piece1.getY() - piece2.getY()) == 1))
        {
            if (canPlaceBridge(piece1, piece2))
            {
                Bridge newBridge(piece1, piece2);
                bridges.push_back(newBridge);
                return true;
            }
            else
            {
                std::cout << "The bridge can't be placed.";
                return false;
            }
        }
        else
        {
            std::cout << "The bridge can't be placed.";
            return false;
        }
    
}

void Board::deleteBridge(const Piece& p1, const Piece& p2)
{
    if (p1.getColor() != p2.getColor())
        return;
    else
    {
        for (auto it = getBridges().begin(); it !=getBridges().end(); ++it) {
            if ((it->getPiece1()==p1 && it->getPiece2()==p2) ||
                (it->getPiece2() == p1 && it->getPiece1() == p2) ){
              //  bridges.erase(it);
                break;
            }
        }
    }
}

bool Board::placePiece(const Piece & newPiece)
{

    if (isValidLocation(newPiece.getX(),newPiece.getY()) && !isOccupied(newPiece.getX(), newPiece.getY()))
    {
        for (int i = 0; i < mines.size();i++) {
            if (newPiece.getX() == std::get<0>(mines[i]) && newPiece.getY() == std::get<1>(mines[i])) {
                explode(mines[i]);
                mines.erase(mines.begin() + i);
                return true;
            }
        }
      board[newPiece.getX()][newPiece.getY()] = newPiece;
      pieces.push_back(newPiece);
      return true;
    }
    return false;
}

bool Board::isBridgeBetween(const uint16_t& x1, const uint16_t& y1, const uint16_t& x2, const uint16_t& y2)
{
    Piece piece1, piece2;
    for (const Piece& piece :pieces)
    {
        if (piece.getX() == x1 && piece.getY() == y1) { piece1 = piece; }
        if (piece.getX() == x2 && piece.getY() == y2) { piece2 = piece; }
    }
    if (piece1.getColor()== piece2.getColor())
        for (const auto& bridge : bridges)
        {
            if ((bridge.getPiece1() == piece1 && bridge.getPiece2() == piece2) || (bridge.getPiece2() == piece1 && bridge.getPiece1() == piece2))
                return true;
        }
    return false;
}


bool Board::availableWay(const uint16_t& x, const uint16_t& y, const uint16_t& sign, const bool& vertical)
{
    if(vertical)
    {
        if (isBridgeBetween(x + 1, y, x - 1, y - 1 * sign))
            return false;
        if (isBridgeBetween(x + 1, y, x, y - 2 * sign))
            return false;
        if (isBridgeBetween(x + 1, y, x + 2, y - 2 * sign))
            return false;
        if (isBridgeBetween(x + 1, y - 1 * sign, x, y + 1 * sign))
            return false;
        if (isBridgeBetween(x + 1, y - 1 * sign, x + 2, y + 1 * sign))
            return false;
        if (isBridgeBetween(x + 1, y - 1 * sign, x + 3, y))
            return false;
        if (isBridgeBetween(x + 1, y + 1 * sign, x, y - 1 * sign))
            return false;
        if (isBridgeBetween(x, y - 1 * sign, x + 2, y))
            return false;
        if (isBridgeBetween(x + 2, y, x + 1, y - 2 * sign))
            return false;
    }
    else
    {
        if (isBridgeBetween(x, y + 1 * sign, x + 1, y - 1 * sign))
            return false;
        if (isBridgeBetween(x, y + 1 * sign, x + 2, y))
            return false;
        if (isBridgeBetween(x, y + 1 * sign, x + 2, y + 2 * sign))
            return false;
        if (isBridgeBetween(x + 1, y + 1 * sign, x - 1, y))
            return false;
        if (isBridgeBetween(x + 1, y + 1 * sign, x - 1, y + 2 * sign))
            return false;
        if (isBridgeBetween(x + 1, y + 1 * sign, x, y + 3 * sign))
            return false;
        if (isBridgeBetween(x - 1, y + 1 * sign, x + 1, y))
            return false;
        if (isBridgeBetween(x + 1, y, x, y + 2 * sign))
            return false;
        if (isBridgeBetween(x, y + 2 * sign, x + 2, y + 1 * sign))
            return false;
    }
    return true;
}

bool Board::canPlaceBridge(const Piece& piece1, const Piece& piece2)
{
    bool condX = piece1.getX() > piece2.getX();
    bool condY = piece1.getY() > piece2.getY();
    if (abs(piece1.getX() - piece2.getX()) > abs(piece1.getY() - piece2.getY()))
    {
        uint16_t x, y;
        if (piece1.getX() < piece2.getX())
            x = piece1.getX(), y = piece1.getY();
        else
            x = piece2.getX(), y = piece2.getY();
        if (condX && condY || !condX && !condY)
        {
            if (!availableWay(x, y, -1, true))
                return false;
        }
        else //if (condX && !condY || !condX && condY)
        {
            if (!availableWay(x, y, 1, true))
                return false;
        }
    }
    else //if (abs(piece1.getX() - piece2.getX()) < abs(piece1.getY() - piece2.getY()))
    {
        if (condX && condY || !condX && !condY)
        {
            if (piece1.getY() < piece2.getY())
            {
                if (!availableWay(piece1.getX(), piece1.getY(), 1, false))
                    return false;
            }
            else
            {
                if (!availableWay(piece2.getX(), piece2.getY(), 1, false))
                    return false;
            }
        }
        else //if (condX && !condY || !condX && condY)
        {
            if (piece1.getY() > piece2.getY())
            {
                if (!availableWay(piece1.getX(), piece1.getY(), -1, false))
                    return false;
            }
            else
            {
                if (!availableWay(piece2.getX(), piece2.getY(), -1, false))
                    return false;
            }
        }
    }
    return true;
}

void Board::dozerTurn()
{
    std::cout << '\n';
    int dozer_action;
    {
        std::uniform_int_distribution<int> distribution(1, 100);
        std::random_device rd;
        std::mt19937 engine(rd());
        dozer_action = distribution(engine);
    }
    if (dozer_action <= 40)
    {
        std::uniform_int_distribution<int> distribution(0, pieces.size() - 1);
        std::random_device rd;
        std::mt19937 engine(rd());
        int piece_location = distribution(engine);
        Piece& chosen_piece = pieces[piece_location];
        for (int i=0; i<bridges.size(); i++)
        {
            if (bridges[i].getPiece1() == chosen_piece || bridges[i].getPiece2() == chosen_piece)
                bridges.erase(bridges.begin() + i);
        }
        dozer = { chosen_piece.getX(),chosen_piece.getY() };
        Piece empty;
        board[dozer.first][dozer.second] = empty;
        pieces.erase(pieces.begin() + piece_location);
        std::cout << "The dozer detroyed the piece at: " << dozer.first << " " << dozer.second << '\n';
    }
    else
    {
        uint16_t x, y;
        do
        {
            std::uniform_int_distribution<int> distribution(1, size - 2);
            std::random_device rd1;
            std::mt19937 eng1(rd1());
            x = distribution(eng1);
            std::random_device rd2;
            std::mt19937 eng2(rd2());
            y = distribution(eng2);
        } while (isOccupied(x, y));
        dozer = { x,y };
        std::cout << "The dozer moved to an empty position.\n";
    }
    displayBoard();
}

void Board::generateMines(const uint16_t& mines_nr)
{
    for (int i{ 0 }; i < mines_nr; i++) {
        uint16_t x, y, type;
        std::uniform_int_distribution<int> distribution1(1, size - 2);
        std::random_device rd1;
        std::mt19937 eng1(rd1());
        x = distribution1(eng1);

        std::random_device rd2;
        std::mt19937 eng2(rd2());
        y = distribution1(eng2);

        std::uniform_int_distribution<int> distribution2(1, 3);
        std::random_device rd3;
        std::mt19937 eng3(rd3());
        type = distribution2(eng3);

        mines.push_back(std::make_tuple(x, y, type));
    }
}

void Board::explode(const std::tuple<uint16_t, uint16_t, uint16_t>& mine)
{
    switch (std::get<2>(mine))
    {
    case 1:
        for (int i{ std::get<0>(mine) - 1 }; i < std::get<0>(mine) + 1; i++)
            for (int j{ std::get<1>(mine) - 1 }; j < std::get<1>(mine) + 1; j++) {
                if (board[i][j].getColor() != None) {
                    for (int t = 0; t < bridges.size(); t++)
                    {
                        if (bridges[t].getPiece1() == board[i][j] || bridges[t].getPiece2() == board[i][j])
                            bridges.erase(bridges.begin() + t);
                    }
                }
                for (int t = 0; t < pieces.size(); t++)
                    if (pieces[t] == board[i][j])
                        pieces.erase(pieces.begin()+t);
                Piece empty;
                board[i][j] = empty;
            }
        break;
    case 2:
        for (int j{ 0 }; j < size - 1; j++)
        {
            if (board[std::get<0>(mine)][j].getColor() != None) {
                for (int t = 0; t < bridges.size(); t++)
                    if (bridges[t].getPiece1() == board[std::get<0>(mine)][j] || bridges[t].getPiece2() == board[std::get<0>(mine)][j])
                       bridges.erase(bridges.begin() + t);
            }
            for (int t = 0; t < pieces.size(); t++)
                if (pieces[t] == board[std::get<0>(mine)][j])
                    pieces.erase(pieces.begin() + t);
            Piece empty;
            board[std::get<0>(mine)][j] = empty;
        }
        break;
    case 3:
        for (int i{ 0 }; i < size - 1; i++)
        {
            if (board[i][std::get<1>(mine)].getColor() != None) {
                for (int t = 0; t < bridges.size(); t++)
                    if (bridges[t].getPiece1() == board[i][std::get<1>(mine)] || bridges[t].getPiece2() == board[i][std::get<1>(mine)])
                        bridges.erase(bridges.begin() + t);
            }
            for (int t = 0; t < pieces.size(); t++)
                if (pieces[t] == board[i][std::get<1>(mine)])
                    pieces.erase(pieces.begin() + t);
            Piece empty;
            board[i][std::get<1>(mine)] = empty;
        }
        break;
    default:
        break;
    }
}

void Board::reset()
{
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            board[row][col] = Color::None;
        }
    }
}



