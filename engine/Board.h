#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <bitset>
//#include <stdexcept>


using namespace std;


const int MAX_BOARD_SIZE=15;
const int MIN_BOARD_SIZE=5;
const int BLACK_STONE=1;
const int WHITE_STONE=-1;
const int EMPTY_POINT=0;
const int ERROR_CODE=-404;
const char BLACK_MARK='X';
const char WHITE_MARK='O';
const char EMPTY_MARK='.';
const char INVALID_MARK='$';
const char STAR_MARK='+';
const int TIE=2;

class Board{

public:

    Board(int l=MAX_BOARD_SIZE):black_pieces(0), white_pieces(0)
    {
        l=2*(l/2)+1;
        l=l<5? MIN_BOARD_SIZE:l;
        l=l>MAX_BOARD_SIZE? MAX_BOARD_SIZE:l;
        setSize(l);
    };

    inline int at(const int &i, const int &j) const
    {
        if (!validateIndex(i, j))
            return ERROR_CODE;
        int index=pos2index(i, j);
        if (black_pieces.test(index))
        {
//            if (white_pieces[index]==1)
//                throw std::runtime_error("error board!"); //��֤�б�Ҫô Ŀǰû��
            return BLACK_STONE;
        }
        else if (white_pieces.test(index))
            return WHITE_STONE;
        else
            return EMPTY_POINT;
    }

    inline bool place(const int &piece, const int &i, const int &j)
    {
        if (!validateIndex(i, j))
            return false;
        int index=pos2index(i, j);
        if (isPiece(index))
            return false;

        if (piece>0)
        {
            black_pieces[index]=1;
            return true;
        }
        else if (piece<0)
        {
            white_pieces[index]=1;
            return true;
        }
        else
            return false;

    }

    inline bool replace(const int &piece, const int &i, const int &j)
    {
        remove(i, j);
        return place(piece, i, j);
    }

    inline bool remove(const int &i, const int &j)
    {
        if (!validateIndex(i, j))
            return false;
        int index=pos2index(i, j);
        if (isEmpty(index))
            return false;
        black_pieces[index]=0;
        white_pieces[index]=0;
        return true;
    }


    inline int size() const
    {
        return 8*black_pieces[255]+4*black_pieces[254]+2*white_pieces[255]+white_pieces[254];
    }

    inline void clear()
    {
        //��ת�Ʊ߳�
        int l=size();
        black_pieces.reset();
        white_pieces.reset();
        setSize(l);

    }

    inline void flip(const char&direction='-'); //Ҳ������map��

    inline void rotate(const int &right_angle_num=1) //δ��������map�� map������Ǳ����չ����ô ò��û��
    {
        int len=size();
        int temp;
        for(int m=0;m<len/2;++m)
        {
            for(int j=1;j<len-2*m;++j)
            {
                switch (right_angle_num)
                {
                case -2:
                    {
                        temp=at(len-m, m+j);
                        replace(at(m+1, len-m-j+1), len-m, m+j);
                        replace(temp, m+1, len-m-j+1);

                        temp=at(m+j, m+1);
                        replace(at(len-m-j+1, len-m), m+j, m+1);
                        replace(temp, len-m-j+1, len-m);
                    }
                    break;
                case -1:
                    {
                        temp=at(len-m, m+j);
                        replace(at(len-m-j+1, len-m), len-m, m+j);
                        replace(at(m+1, len-m-j+1),len-m-j+1, len-m);
                        replace(at(m+j, m+1), m+1, len-m-j+1);
                        replace(temp, m+j, m+1);
                    }
                    break;
                case 1:
                    {
                        temp=at(len-m, m+j);
                        replace(at(m+j, m+1), len-m, m+j);
                        replace(at(m+1, len-m-j+1), m+j, m+1);
                        replace(at(len-m-j+1, len-m), m+1, len-m-j+1);
                        replace(temp, len-m-j+1, len-m);
                    }
                    break;
                case 2:
                    {
                        temp=at(len-m, m+j);
                        replace(at(m+1, len-m-j+1), len-m, m+j);
                        replace(temp, m+1, len-m-j+1);

                        temp=at(m+j, m+1);
                        replace(at(len-m-j+1, len-m), m+j, m+1);
                        replace(temp, len-m-j+1, len-m);
                    }
                    break;
                default: //do nothing
                    break;
                }

            }
        }
    }



    inline bool isEmpty(const int &i, const int &j) const
    {
        if (!validateIndex(i, j))
            return false;
        return isEmpty(pos2index(i, j));
    }

    inline bool isPiece(const int &i, const int &j) const
    {
        if (!validateIndex(i, j))
            return false;
        return isPiece(pos2index(i, j));
    }

    inline bool isBlack(const int &i, const int &j) const
    {
        if (!validateIndex(i, j))
            return false;
        int index=pos2index(i, j);
        return (black_pieces.test(index)&&!white_pieces.test(index));
    }

    inline bool isWhite(const int &i, const int &j) const
    {
        if (!validateIndex(i, j))
            return false;
        int index=pos2index(i, j);
        return (!black_pieces.test(index)&&white_pieces.test(index));
    }

    inline bool isCleared()
    {
        int l=size();
        black_pieces.reset(255);
        black_pieces.reset(254);
        white_pieces.reset(255);
        white_pieces.reset(254);
        bool flag=black_pieces.none()&&white_pieces.none();
        setSize(l);
        return flag;
    }

    inline bool isFull()
    {
        int len=size();
        for(int m=0;m<len/2;++m)
        {
            for(int j=1;j<len-2*m;++j)
            {
                if(at(len-m, m+j)==0)
                    return false;
                if(at(len-m-j+1, len-m)==0)
                    return false;
                if(at(m+1, len-m-j+1)==0)
                    return false;
                if(at(m+j, m+1)==0)
                    return false;
            }
        }
        return true;
    }

    inline char mark(const int &i, const int &j) const
    {
        char sign=INVALID_MARK;
        if (isEmpty(i, j))
            sign=EMPTY_MARK;
        else
        if (isBlack(i, j))
                sign=BLACK_MARK;
        else
        if (isWhite(i, j))
                sign=WHITE_MARK;
        return sign;
    }

private:

    bitset<256> black_pieces; //256+256λ������ ǰ1λ��Ҫ �м�15*15��ʾ���� 15*15��ʾ���� �����Ҵ����������� black+white������λ�����̱߳� ���15
    bitset<256> white_pieces; //����ʱ�� black_pieces&white_pieces==0(����߳��Ļ�) λ0��ʾ��Ӧ��λ��û������ 1��ʾ����Ӧ����

    inline void setSize(const int &l)
    {
        bitset<4> length(l);
        black_pieces[255]=length[3];
        black_pieces[254]=length[2];
        white_pieces[255]=length[1];
        white_pieces[254]=length[0];
    }

    inline int pos2index(const int &i, const int &j) const
    {
        return (i-1)*size()+j;
    }

    inline bool isEmpty(const int &index) const //�޼��
    {
        return (black_pieces[index]|white_pieces[index])==0;
    }

    inline bool isPiece(const int &index) const
    {
        return (black_pieces[index]|white_pieces[index])==1;
    }

    inline bool validateIndex(const int &i, const int &j) const
    {
        return i>0&&i<=size()&&j>0&&j<=size();
    }

    friend ostream& operator<< (ostream& os, const Board &board);
};

inline ostream& operator<< (ostream& os, const Board &board)
{
    for(int i=board.size();i!=0;i--)
    {
        for(int j=1;j<=board.size();j++)
        {
            os<<board.mark(i, j)<<' ';

        }
        os<<endl;
    }

    return os;
}

#endif // BOARD
