#ifndef _GOBANG_H_
#define _GOBANG_H_

#include "Board.h"

#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

#define F_DOUBLE_THREE -6
#define F_DOUBLE_FOUR -8
#define OVERLINE -9
#define FIVE 12
#define DOUBLE_FOUR 11
#define FOUR_THREE 10
#define DOUBLE_THREE 9
#define OPEN_FOUR 8
#define SLEEP_FOUR 7
#define OPEN_THREE 6
#define SLEEP_THREE 5
#define OPEN_TWO 4
#define SLEEP_TWO 3
#define OPEN_ONE 2
#define SLEEP_ONE 1

typedef pair<int, int> Action;



class GoBang{
public:

    int curr_player;
    int winner;
    int move_count;
    Board board;

    GoBang(const bool &flag=true):curr_player(BLACK_STONE), winner(EMPTY_POINT), move_count(0), forbid_move(flag){};

    inline int move_debug(const int &piece, const int &i, const int &j)
    {
        if (winner!=0||!board.place(piece, i, j)) //����ʧ�ܻ�������
            return ERROR_CODE;
        return 0; //��ʱ

    }

    int move(const int &i, const int &j);
    int move(const int &piece, const int &i, const int &j, const bool &clear_undoStack=true);
    int undo();
    int redo();

    inline Action last_move() const
    {
        if (move_stack.empty())
            return make_pair(0, 0);

        return move_stack.top().second;
    };

    inline int last_piece() const
    {
        if (move_stack.empty())
            return EMPTY_POINT;

        return move_stack.top().first;
    };

    void reset();

    //��ĳ����(����Ʋ��) �������ӵ㼯�Ϻ����鳤��
    static pair< int, vector<Action> > scanLine(const int &i, const int &j, const char &direct, const Board &goBoard);

    inline pair< int, vector<Action> > scanLine(const int &i, const int &j, const char &direct) const
    {
        return scanLine(i, j, direct, board);
    };

    static pair<int, pair<int, int> > countLine(const int &i, const int &j, const char &direct, const Board &goBoard);

    static int isForbidMove(const int &i, const int &j, Board &goBoard);

    static pair< int, vector<Action> > checkLineState(const int &i, const int &j, const char &direct, Board goBoard, const bool &forbid_move=true, int depth=-1);

    //checkLineState ��ĳ������scanLine����״̬�ж�:��һ/��һ/�߶�/���/����/����/����/����/��/���� ����ط����ѵ����ڻ�Ҫ�����еĹؼ��������
    //���� ����line_count>5
    //�� ����line_count==5 ����line_count>5
    //���� ����ɳ�����line_count==4
    //���� һ��ɳ���(line_count<=4)
    //���� �е�ɳɻ���(�е�ɳ�����Ϊ�����O.OOO..)(line_count<=3)
    //���� �е�ɳ�����(�е�ɳɻ�����Ϊ��������O.XXX..)
    //��� �е�ɳɻ��� (line_count<=2)
    //�߶� �е�ɳ�����(�е�ɳɻ�����Ϊ���) (line_count<=2)  X...X
    //��һ �е�ɳɻ�� (line_count==1)
    //��һ �е�ɳ��߶� (line_count==1)
    inline pair< int, vector<Action> > checkLineState(const int &i, const int &j, const char &direct)
    {
        return checkLineState(i, j, direct, board, forbid_move);
    };
    //�����з�������checkState���״̬ ������Ӯ״̬ Ҫ�������йؼ����ӵ�
    static pair< int, vector<Action> > checkState(const int &i, const int &j, Board &goBoard, const bool &forbid_move=true);

    inline pair< int, vector<Action> > checkState(const int &i, const int &j)
    {
        return checkState(i, j, board, forbid_move);
    }

    inline int checkState()
    {
        if (move_stack.empty())
            return 0;
        int state=0;
        state=checkState(move_stack.top().second.first, move_stack.top().second.second).first; //��ʱ��ȥ
        if (state==FIVE)
            winner=board.at(move_stack.top().second.first, move_stack.top().second.second);
        else
        if (state<0)
        {
            winner=WHITE_STONE;
        }

        return state;
    }


private:
    stack< pair<int, Action> > move_stack;
    stack< pair<int, Action> > undo_stack;
    bool forbid_move; //���ֿ���

    inline void changePlayer()
    {
        curr_player=-curr_player;
    }
    friend ostream& operator<< (ostream& os, const GoBang &goBang);
};

inline ostream& operator<< (ostream& os, const GoBang &goBang)
{
    os<<"   ";
    for(int j=0;j!=goBang.board.size();j++)
        os<<char('A'+j)<<' ';
    os<<endl;

    for(int i=goBang.board.size();i!=0;i--)
    {
        os<<setw(2)<<i<<' ';

        for(int j=1;j<=goBang.board.size();j++)
        {
            os<<goBang.board.mark(i, j)<<' ';

        }
        os<<i<<endl;
    }

    os<<"   ";
    for(int j=0;j!=goBang.board.size();j++)
        os<<char('A'+j)<<' ';

    return os;
}

#endif // _GOBANG_H_
