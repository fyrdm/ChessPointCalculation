#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>


class cTasFactory;
class cOyunTahtasiVisitor;
class cSatrancTahtasiVisitor;
class cTas;
class cVezir;
class cAt;
class cPiyon;

struct MatrixPos2D
{
    MatrixPos2D() : I(0), J(0) {  }
    MatrixPos2D(int i , int j): I(i) , J(j) {  }
    int I = 0;
    int J = 0;

    int linearPos();
    MatrixPos2D operator+(MatrixPos2D right)
    {
        return MatrixPos2D(I + right.I, J + right.J);
    }
};

enum class Takim { SIYAH , BEYAZ };

enum class TasTurleri { ps = 1, as, fs, ks, vs, ss, // SIYAH Taslar
                        NONE ,                      // Bos alanlar icin
                        pb, ab, fb, kb, vb, sb};    // BEYAZ Taslar  

std::map<std::string, TasTurleri> globalTasMap =
{
    {"ps" , TasTurleri::ps},
    {"as" , TasTurleri::as},
    {"fs" , TasTurleri::fs},
    {"ks" , TasTurleri::ks},
    {"vs" , TasTurleri::vs},
    {"ss" , TasTurleri::ss},
            
    {"--" , TasTurleri::NONE},
            
    {"pb" , TasTurleri::pb},
    {"ab" , TasTurleri::ab},
    {"fb" , TasTurleri::fb},
    {"kb" , TasTurleri::kb},
    {"vb" , TasTurleri::vb},
    {"sb" , TasTurleri::sb},
};

class cTas
{
public:
    cTas(Takim tk, MatrixPos2D pos)
    {
        m_team = tk;
        m_position = { pos.I , pos.J };
        m_point = 0;
        m_threatened = false;
    }
    virtual std::vector<MatrixPos2D> accept(cOyunTahtasiVisitor * visitor) = 0;

    void setThreat(bool isThreaned) { m_threatened = isThreaned; }

    MatrixPos2D getPosition() { return m_position; }
    Takim getTeam() { return m_team; }
    bool getThreatened() { return m_threatened; }
    float getPoint() { return m_point; }

protected:
    MatrixPos2D m_position;
    Takim m_team;
    float m_point;
    bool  m_threatened;

};

class cVezir : public cTas
{
public:
    cVezir(Takim tk, MatrixPos2D pos): cTas(tk,pos)
    {
        m_point = 9.0f;
    }

    virtual std::vector<MatrixPos2D> accept(cOyunTahtasiVisitor* visitor);
};

class cAt : public cTas
{
public:
    cAt(Takim tk, MatrixPos2D pos): cTas(tk,pos)
    {
        m_point = 3;
    }

    virtual std::vector<MatrixPos2D> accept(cOyunTahtasiVisitor* visitor);
};

/*TODO: Piyon Class*/
class cPiyon : public cTas
{
public:
    cPiyon(Takim tk, MatrixPos2D pos) : cTas(tk, pos)
    {
        m_point = 1;
    }

    virtual std::vector<MatrixPos2D> accept(cOyunTahtasiVisitor* visitor);
};

class cFil : public cTas
{
public:
    cFil(Takim tk, MatrixPos2D pos) : cTas(tk, pos)
    {
        m_point = 3;
    }

    virtual std::vector<MatrixPos2D> accept(cOyunTahtasiVisitor* visitor);
};

class cKale : public cTas
{
public:
    cKale(Takim tk, MatrixPos2D pos) : cTas(tk, pos)
    {
        m_point = 5;
    }

    virtual std::vector<MatrixPos2D> accept(cOyunTahtasiVisitor* visitor);
};

class cSah : public cTas
{
public:
    cSah(Takim tk, MatrixPos2D pos) : cTas(tk, pos)
    {
        m_point = 100;
    }

    virtual std::vector<MatrixPos2D> accept(cOyunTahtasiVisitor* visitor);
};

class cTasFactory
{
public:
    cTas* createTas(TasTurleri type, MatrixPos2D pos)
    {
        cTas* tas = new cAt(Takim::BEYAZ, MatrixPos2D(0, 0));

        switch (type)
        {
        case TasTurleri::ps:
            tas = new cPiyon(Takim::SIYAH, pos);
            break;
        case TasTurleri::as:
            tas = new cAt(Takim::SIYAH, pos);
            break;
        case TasTurleri::fs:
            tas = new cFil(Takim::SIYAH, pos);
            break;
        case TasTurleri::ks:
            tas = new cKale(Takim::SIYAH, pos);
            break;
        case TasTurleri::vs:
            tas = new cVezir(Takim::SIYAH, pos);
            break;
        case TasTurleri::ss:
            tas = new cSah(Takim::SIYAH, pos);
            break;
        case TasTurleri::NONE:
            break;
        case TasTurleri::pb:
            tas = new cPiyon(Takim::BEYAZ, pos);
            break;
        case TasTurleri::ab:
            tas = new cAt(Takim::BEYAZ, pos);
            break;
        case TasTurleri::fb:
            tas = new cFil(Takim::BEYAZ, pos);
            break;
        case TasTurleri::kb:
            tas = new cKale(Takim::BEYAZ, pos);
            break;
        case TasTurleri::vb:
            tas = new cVezir(Takim::BEYAZ, pos);
            break;
        case TasTurleri::sb:
            tas = new cSah(Takim::BEYAZ, pos);
            break;
        default:
            break;
        }

        return tas;
    }

private:

};

class cOyunTahtasiVisitor
{
public:
    virtual std::vector<MatrixPos2D> visit(cAt* at) = 0;
    virtual std::vector<MatrixPos2D> visit(cVezir* vezir) = 0;
    virtual std::vector<MatrixPos2D> visit(cPiyon* piyon) = 0;
    virtual std::vector<MatrixPos2D> visit(cFil* fil) = 0;
    virtual std::vector<MatrixPos2D> visit(cKale* kale) = 0;
    virtual std::vector<MatrixPos2D> visit(cSah* sah) = 0;

    virtual void printElements() = 0;
    virtual TasTurleri getTasTurleri(int i, int j) = 0;
};

class cSatrancTahtasiVisitor : public cOyunTahtasiVisitor
{
public:
    cSatrancTahtasiVisitor(const char* fileName)
    {
        // TODO: Load context from file (.TXT)
        std::string strTas;
        std::ifstream inFile(fileName);
        std::map<std::string, TasTurleri>::iterator it;

        if (!inFile)
            std::cout << "ERROR::cSatrancTahtasiVisitor --- An error occured while opening "
            << fileName << std::endl;

        for (size_t i = 0; i < m_rowSize; i++)
        {
            for (size_t j = 0; j < m_colSize; j++)
            {
                inFile >> strTas;
                it = globalTasMap.find(strTas);
                if (it != globalTasMap.end())
                    m_tahtaMatrisi[i][j] = it->second;
            }
        }

        if (inFile.is_open())
            inFile.close();
    }

    virtual std::vector<MatrixPos2D> visit(cAt* at) override
    {
        // TODO: Threat Analyze
        /***** Knight moves *** |
        /     Offset Grid       |
        /
        /      -2 -1 0 1 2      |
        /       . . . . .       |
        /-2.....o X o X o       |
        /-1.....X o o o X       |
        /.0.....o o K o o       |
        /.1.....X o o o X       |
        /.2.....o X o X o       |
        /       . . . . .       |
        /***********************/

        std::vector<MatrixPos2D> moveOffsets{ {-2,-1}, {-2,1},
                                              {-1,-2}, {-1,2},
                                              { 1,-2}, { 1,2},
                                              { 2,-1}, { 2,1} };

        MatrixPos2D aPos = at->getPosition();
        MatrixPos2D attackedPos;
        std::vector<MatrixPos2D> threatenedPositions;


        for (size_t i = 0; i < moveOffsets.size(); i++)
        {
            attackedPos = aPos + moveOffsets[i];
            // indices must be inside the matrix border (i.e. 8x8)
            bool validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            bool validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                //SIYAH or BEYAZ
                if (at->getTeam() == Takim::SIYAH) // IF SIYAH : attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        threatenedPositions.push_back(attackedPos);
                }
                else if (at->getTeam() == Takim::BEYAZ) // IF BEYAZ : attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        threatenedPositions.push_back(attackedPos);
                }
            }
        }

        return threatenedPositions;

    }

    virtual std::vector<MatrixPos2D> visit(cVezir* vezir) override
    {
        // TODO: Threat Analyze

        std::vector<MatrixPos2D> threatenedPositions;
        MatrixPos2D aPos = vezir->getPosition();

        MatrixPos2D attackedPos;
        bool validRow, validCol;

        for (int i = 1; i < m_rowSize; i++)
        {
            // NorthWest-SouthEast AND NorthEast-SouthWest 
            // Vertical            AND Horizontal move
            // offsets : DIAGONAL <-i,-i> & <+i,+i> AND <-i,+i> & <+i,-i>
            //           HOR-VER  <i,0> & <-i,0> AND <0,i> & <0,-i>

            // 1- posVector(-i,-i)
            attackedPos = MatrixPos2D(-i, -i) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (vezir->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (vezir->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break;
                }
            }
        }

        for (int i = 1; i < m_rowSize; i++)
        {
            // 2- posVector(+i,+i)
            attackedPos = MatrixPos2D(i, i) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (vezir->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (vezir->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break;
                }
            }
        }

        for (int i = 1; i < m_rowSize; i++)
        {
            // 3- posVector(-i,+i)
            attackedPos = MatrixPos2D(-i, i) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (vezir->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (vezir->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break;
                }
            }
        }

        for (int i = 1; i < m_rowSize; i++)
        {
            // 4- posVector(+i,-i)
            attackedPos = MatrixPos2D(+i, -i) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (vezir->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (vezir->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break;
                }
            }
        }

        for (int i = 1; i < m_rowSize; i++)
        {
            // 5- posVector(+i,0)
            attackedPos = MatrixPos2D(i, 0) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (vezir->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (vezir->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break;
                }
            }
        }

        for (int i = 1; i < m_rowSize; i++)
        {
            // 6- posVector(0,i)
            attackedPos = MatrixPos2D(0, i) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (vezir->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (vezir->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break;
                }
            }
        }

        for (int i = 1; i < m_rowSize; i++)
        {
            // 7- posVector(-i,0)
            attackedPos = MatrixPos2D(-i, 0) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (vezir->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (vezir->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break;
                }
            }
        }

        for (int i = 1; i < m_rowSize; i++)
        {
            // 8- posVector(0,-i)
            attackedPos = MatrixPos2D(0, -i) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (vezir->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (vezir->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break;
                }
            }
        }

        return threatenedPositions;
    }

    virtual std::vector<MatrixPos2D> visit(cPiyon* piyon) override
    {
        // TODO: Threat Analyze
        std::vector<MatrixPos2D> threatenedPositions;
        MatrixPos2D aPos = piyon->getPosition();
        //SIYAH or BEYAZ

        // indices must be inside the matrix border (i.e. 8x8)
        bool validRow = 0 <= (aPos.I - 1) && (aPos.I + 1) < m_rowSize;
        bool validCol = 0 <= (aPos.J - 1) && (aPos.J + 1) < m_colSize;
        if (validRow && validCol)
        {
            if (piyon->getTeam() == Takim::SIYAH) // IF SIYAH : attacks downward 
            {
                // threaten i+1 ---- j-1
                //          i+1 ---- j+1
                if (m_tahtaMatrisi[aPos.I + 1][aPos.J - 1] > TasTurleri::NONE) // opponent analysis
                    threatenedPositions.push_back(MatrixPos2D(aPos.I + 1, aPos.J - 1));
                if (m_tahtaMatrisi[aPos.I + 1][aPos.J + 1] > TasTurleri::NONE) // opponent analysis
                    threatenedPositions.push_back(MatrixPos2D(aPos.I + 1, aPos.J + 1));
            }
            else if (piyon->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks upward
            {
                // threaten i-1 ---- j-1
                //          i-1 ---- j+1
                if (m_tahtaMatrisi[aPos.I - 1][aPos.J - 1] < TasTurleri::NONE) // opponent analysis
                    threatenedPositions.push_back(MatrixPos2D(aPos.I - 1, aPos.J - 1));
                if (m_tahtaMatrisi[aPos.I - 1][aPos.J + 1] < TasTurleri::NONE) // opponent analysis
                    threatenedPositions.push_back(MatrixPos2D(aPos.I - 1, aPos.J + 1));
            }
        }

        return threatenedPositions;
    }

    virtual std::vector<MatrixPos2D> visit(cFil* fil) override
    {
        // TODO: Threat Analyze

        std::vector<MatrixPos2D> threatenedPositions;
        MatrixPos2D aPos = fil->getPosition();

        MatrixPos2D attackedPos;
        bool validRow, validCol;

        for (int i = 1; i < m_rowSize; i++)
        {
            // NorthWest-SouthEast AND NorthEast-SouthWest move
            // offsets : both <-i,-i> & <+i,+i> AND <-i,+i> & <+i,-i>
            // 1- posVector(-i,-i)
            attackedPos = MatrixPos2D(-i, -i) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (fil->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (fil->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break;
                }
            }
        }

        for (int i = 1; i < m_rowSize; i++)
        {
            // 2- posVector(+i,+i)
            attackedPos = MatrixPos2D(i, i) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (fil->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (fil->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break;
                }
            }
        }

        for (int i = 1; i < m_rowSize; i++)
        {
            // 3- posVector(-i,+i)
            attackedPos = MatrixPos2D(-i, i) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (fil->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (fil->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break;
                }
            }
        }

        for (int i = 1; i < m_rowSize; i++)
        {
            // 4- posVector(+i,-i)
            attackedPos = MatrixPos2D(+i, -i) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (fil->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (fil->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break;
                }
            }
        }

        return threatenedPositions;
    }


    virtual std::vector<MatrixPos2D> visit(cKale* kale) override
    {
        // TODO: Threat Analyze
        /***** King moves *** |
        /     Offset Grid     |
        /      .  .  .        |
        /     -1  o  1        |
        /-1.....  o  .        |
        /.0..o o  Rk o o o o o|
        /.1.....  o  .        |
        /      .  o  .        |
        /***********************/
        std::vector<MatrixPos2D> threatenedPositions;
        MatrixPos2D aPos = kale->getPosition();

        MatrixPos2D attackedPos(0, 0);
        bool validRow, validCol;

        for (int i = 1; i < m_rowSize; i++)
        {
            // vertical move
            // offsets : <i,0> & <-i,0> 
            // 1- posVector(i,0)
            attackedPos = MatrixPos2D(i, 0) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            if (validRow)
            {
                if (kale->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break; // BLOCKED 
                }
                else if (kale->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break; // BLOCKED
                }
            }
        }

        for (int i = 1; i < m_rowSize; i++)
        {
            // 2- posVector(-i,0)
            attackedPos = MatrixPos2D(-i, 0) + aPos;
            validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            if (validRow)
            {
                if (kale->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break;
                }
                else if (kale->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break; // BLOCKED
                }
            }
        }
        
        for (int i = 1; i < m_rowSize; i++)
        {
            // 3- posVector(0,i)
            attackedPos = MatrixPos2D(0, i) + aPos;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validCol)
            {
                if (kale->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break; // BLOCKED
                }
                else if (kale->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break; // BLOCKED
                }
            }
        }

        for (int i = 1; i < m_rowSize; i++)
        {
            // 4- posVector(0,-i)
            attackedPos = MatrixPos2D(0, -i) + aPos;
            validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validCol)
            {
                if (kale->getTeam() == Takim::SIYAH) // IF SIYAH: attacks BEYAZ
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        break; // BLOCKED
                }
                else if (kale->getTeam() == Takim::BEYAZ) // IF BEYAZ: attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                    {
                        threatenedPositions.push_back(attackedPos);
                        break;
                    }
                    else if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        break; // BLOCKED
                }
            }
        }

        return threatenedPositions;
    }

    virtual std::vector<MatrixPos2D> visit(cSah* sah) override
    {
        // TODO: Threat Analyze
        /***** King moves *** |
        /     Offset Grid     |
        /      .  .  .        |
        /     -1  0  1        |
        /-1....o  o  o        |
        /.0....o Kng o        |
        /.1....o  o  o        |
        /      . . . .        |
        /***********************/
        std::vector<MatrixPos2D> threatenedPositions;
        MatrixPos2D aPos = sah->getPosition();

        std::vector<MatrixPos2D> moveOffsets{ {-1,-1}, {-1,0},{-1, 1},
                                              { 0,-1},/*Sah*/ {0, 1},
                                              { 1,-1},{ 1,0}, { 1,1} };
        MatrixPos2D attackedPos;


        for (size_t i = 0; i < moveOffsets.size(); i++)
        {
            attackedPos = aPos + moveOffsets[i];
            // indices must be inside the matrix border (i.e. 8x8)
            bool validRow = 0 <= attackedPos.I && attackedPos.I < m_rowSize;
            bool validCol = 0 <= attackedPos.J && attackedPos.J < m_colSize;
            if (validRow && validCol)
            {
                if (sah->getTeam() == Takim::SIYAH) // IF SIYAH : attacks BEYAZ 
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] > TasTurleri::NONE)
                        threatenedPositions.push_back(attackedPos);
                }
                else if (sah->getTeam() == Takim::BEYAZ) // IF BEYAZ : attacks SIYAH
                {
                    if (m_tahtaMatrisi[attackedPos.I][attackedPos.J] < TasTurleri::NONE)
                        threatenedPositions.push_back(attackedPos);
                }
            }
        }

        return threatenedPositions;
    }

    TasTurleri getTasTurleri(int i, int j) {
        return m_tahtaMatrisi[i][j];
    }


    void printElements()
    {
        MatrixPos2D pos = { 1,2 };
        std::cout << pos.I << " " << pos.J << std::endl;
        std::cout << MatrixPos2D().I << " " << MatrixPos2D().J << std::endl;
        std::cout << std::endl;

        for (size_t i = 0; i < m_rowSize; i++)
        {
            for (size_t j = 0; j < m_colSize; j++)
            {
                std::cout << (int)m_tahtaMatrisi[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    static const int m_colSize = 8;
    static const int m_rowSize = 8;
private:

    /* 8x8 position matrix.*/
    TasTurleri m_tahtaMatrisi[m_rowSize][m_colSize] = { TasTurleri::NONE };

};

class SatrancManager
{
public:
    SatrancManager(const char* fileName)
    {
        OTvisitor = new cSatrancTahtasiVisitor(fileName);
        Load();
        m_whitePoint = 0.0f;
        m_blackPoint = 0.0f;
    }

    void Load()
    {
        // Load mapTaslar
        // TODO: Load context from m_TahtaMatrisi

        cTasFactory fabrika;
        TasTurleri currentType;

        for (int i = 0; i < cSatrancTahtasiVisitor::m_rowSize; i++)
        {
            for (int j = 0; j < cSatrancTahtasiVisitor::m_colSize; j++)
            {
                currentType = OTvisitor->getTasTurleri(i, j);
                
                if (currentType != TasTurleri::NONE )
                {
                    cTas* yeniTas = fabrika.createTas(currentType, MatrixPos2D(i, j));

                    std::pair<int, cTas*> pairPosTas( yeniTas->getPosition().linearPos() ,
                                                      yeniTas);

                    mapTaslar.insert(pairPosTas);
                }                    
            }
        }
    }

    void ThreatAnalyze()
    {
        std::vector<MatrixPos2D> threatenedPositions;

        std::map<int, cTas*>::iterator itr;
        for (itr = mapTaslar.begin(); itr != mapTaslar.end(); itr++)
        {
            threatenedPositions = itr->second->accept(OTvisitor);

            if (threatenedPositions.size() > 0)
            for (size_t idx = 0; idx < threatenedPositions.size(); idx++)
            {
                mapTaslar.find(threatenedPositions[idx].linearPos())->second->setThreat(true);
            }
        }
    }

    void CalculatePoints()
    {
        ThreatAnalyze();

        m_whitePoint = 0.0f;
        m_blackPoint = 0.0f;

        std::map<int, cTas*>::iterator itr;
        for (itr = mapTaslar.begin(); itr != mapTaslar.end(); itr++)
        {
            if (itr->second->getTeam() == Takim::BEYAZ)
            {
                m_whitePoint += (itr->second->getThreatened()) ?  // TERNARY IF
                                itr->second->getPoint() * 0.5f : itr->second->getPoint();
            }

            else if (itr->second->getTeam() == Takim::SIYAH)
            {
                m_blackPoint += (itr->second->getThreatened()) ?  // TERNARY IF
                    itr->second->getPoint() * 0.5f : itr->second->getPoint();
            }
        }

        OTvisitor->printElements();

        std::cout   << "--------PUANLAR----------" << std::endl
                    << "WHITE: " << m_whitePoint << std::endl
                    << "BLACK: " << m_blackPoint << std::endl;

    }

private:
    std::map<int, cTas*> mapTaslar;
    cOyunTahtasiVisitor* OTvisitor;
    float m_whitePoint;
    float m_blackPoint;
};


int MatrixPos2D::linearPos() { return I * cSatrancTahtasiVisitor::m_rowSize + J; }


/* accept method definitions: */
std::vector<MatrixPos2D> cVezir::accept(cOyunTahtasiVisitor* visitor)
{
    // TODO: accept visitor
    return visitor->visit(this);
}

std::vector<MatrixPos2D> cAt::accept(cOyunTahtasiVisitor* visitor)
{
    // TODO: accept visitor
    return visitor->visit(this);
}

std::vector<MatrixPos2D> cPiyon::accept(cOyunTahtasiVisitor* visitor)
{
    // TODO: accept visitor
    return visitor->visit(this);
}

std::vector<MatrixPos2D> cKale::accept(cOyunTahtasiVisitor* visitor)
{
    // TODO: accept visitor
    return visitor->visit(this);
}

std::vector<MatrixPos2D> cSah::accept(cOyunTahtasiVisitor* visitor)
{
    // TODO: accept visitor
    return visitor->visit(this);
}

std::vector<MatrixPos2D> cFil::accept(cOyunTahtasiVisitor* visitor)
{
    // TODO: accept visitor
    return visitor->visit(this);
}

int main()
{
    //std::vector<cTas*> vTaslar;
    //cOyunTahtasiVisitor* OTvisitor = new cSatrancTahtasiVisitor("board1.txt");
    SatrancManager* sm = new SatrancManager("board3.txt");
    sm->CalculatePoints();

    delete sm;
}
