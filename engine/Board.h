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

class Board{

public:

    Board(int l=MAX_BOARD_SIZE):black_pos_state(0), white_pos_state(0)
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
        if (black_pos_state.test(index))
        {
//            if (white_pos_state[index]==1)
//                throw std::runtime_error("error board!"); //验证有必要么 目前没有
            return BLACK_STONE;
        }
        else if (white_pos_state.test(index))
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
            black_pos_state[index]=1;
            return true;
        }
        else if (piece<0)
        {
            white_pos_state[index]=1;
            return true;
        }
        else
            return false;

    }

    inline bool remove(const int &i, const int &j)
    {
        if (!validateIndex(i, j))
            return false;
        int index=pos2index(i, j);
        if (isEmpty(index))
            return false;
        black_pos_state[index]=0;
        white_pos_state[index]=0;
        return true;
    }


    inline int size() const
    {
        return 8*black_pos_state[255]+4*black_pos_state[254]+2*white_pos_state[255]+white_pos_state[254];
    }

    inline void clear()
    {
        //先转移边长
        int l=size();
        black_pos_state.reset();
        white_pos_state.reset();
        setSize(l);

    }

    inline void rotate(const int &right_angle_num=1) //to be done
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
                        remove(len-m, m+j);
                        place(at(m+1, len-m-j+1), len-m, m+j);
                        remove(m+1, len-m-j+1);
                        place(temp, m+1, len-m-j+1);

                        temp=at(m+j, m+1);
                        remove(m+j, m+1);
                        place(at(len-m-j+1, len-m), m+j, m+1);
                        remove(len-m-j+1, len-m);
                        place(temp, len-m-j+1, len-m);
                    }
                    break;
                case -1:
                    {
                        temp=at(len-m, m+j);
                        remove(len-m, m+j);
                        place(at(len-m-j+1, len-m), len-m, m+j);
                        remove(len-m-j+1, len-m);
                        place(at(m+1, len-m-j+1),len-m-j+1, len-m);
                        remove(m+1, len-m-j+1);
                        place(at(m+j, m+1), m+1, len-m-j+1);
                        remove(m+j, m+1);
                        place(temp, m+j, m+1);
                    }
                    break;
                case 1:
                    {
                        temp=at(len-m, m+j);
                        remove(len-m, m+j);
                        place(at(m+j, m+1), len-m, m+j);
                        remove(m+j, m+1);
                        place(at(m+1, len-m-j+1), m+j, m+1);
                        remove(m+1, len-m-j+1);
                        place(at(len-m-j+1, len-m), m+1, len-m-j+1);
                        remove(len-m-j+1, len-m);
                        place(temp, len-m-j+1, len-m);
                    }
                    break;
                case 2:
                    {
                        temp=at(len-m, m+j);
                        remove(len-m, m+j);
                        place(at(m+1, len-m-j+1), len-m, m+j);
                        remove(m+1, len-m-j+1);
                        place(temp, m+1, len-m-j+1);

                        temp=at(m+j, m+1);
                        remove(m+j, m+1);
                        place(at(len-m-j+1, len-m), m+j, m+1);
                        remove(len-m-j+1, len-m);
                        place(temp, len-m-j+1, len-m);
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
        return (black_pos_state.test(index)&&!white_pos_state.test(index));
    }

    inline bool isWhite(const int &i, const int &j) const
    {
        if (!validateIndex(i, j))
            return false;
        int index=pos2index(i, j);
        return (!black_pos_state.test(index)&&white_pos_state.test(index));
    }

    inline bool isCleared()
    {
        int l=size();
        black_pos_state.reset(255);
        black_pos_state.reset(254);
        white_pos_state.reset(255);
        white_pos_state.reset(254);
        bool flag=black_pos_state.none()&&white_pos_state.none();
        setSize(l);
        return flag;
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

    bitset<256> black_pos_state; //256+256位的棋盘 前1位不要 中间15*15表示黑棋 15*15表示白棋 从左到右从上至下索引 black+white各后两位放棋盘边长 最大15位
    bitset<256> white_pos_state; //任意时候 black_pos_state&white_pos_state==0 位0表示对应的位置没有棋子 1表示有相应棋子

    inline void setSize(const int &l)
    {
        bitset<4> lenB(l);
        black_pos_state[255]=lenB[3];
        black_pos_state[254]=lenB[2];
        white_pos_state[255]=lenB[1];
        white_pos_state[254]=lenB[0];
    }

    inline int pos2index(const int &i, const int &j) const
    {
        return (i-1)*size()+j;
    }

    inline bool isEmpty(const int &index) const //无检查
    {
        return (black_pos_state[index]|white_pos_state[index])==0;
    }

    inline bool isPiece(const int &index) const
    {
        return (black_pos_state[index]|white_pos_state[index])==1;
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
