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

const vector<Action> ac_vec;

class GoBang{
public:

    int curr_player;
    int winner;
    int move_count;
    Board board;

    GoBang(const bool &flag=true):curr_player(BLACK_STONE), winner(EMPTY_POINT), move_count(0), forbid_move(flag){};

    inline int move_debug(const int &piece, const int &i, const int &j)
    {
        if (winner!=0||!board.place(piece, i, j)) //落子失败或者有了
            return ERROR_CODE;
        return 0; //暂时

    }

    pair< int, vector<Action> > move(const int &i, const int &j);
    pair< int, vector<Action> > move(const int &piece, const int &i, const int &j, const bool &clear_undoStack=true);
    pair< int, vector<Action> > undo();
    pair< int, vector<Action> > redo();

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

    //对某方向(横竖撇捺) 返回连接点集合和连珠长度
    static pair< int, vector<Action> > scanLine(const int &i, const int &j, const char &direct, const Board &goBoard);

    inline pair< int, vector<Action> > scanLine(const int &i, const int &j, const char &direct) const
    {
        return scanLine(i, j, direct, board);
    };

    static pair<int, pair<int, int> > countLine(const int &i, const int &j, const char &direct, const Board &goBoard);

    static int isForbidMove(const int &i, const int &j, Board &goBoard);

    static pair< int, vector<Action> > checkLineState(const int &i, const int &j, const char &direct, Board &goBoard, const bool &forbid_move=true, int depth=-1);

    //checkLineState 在某方向用scanLine进行状态判断:眠一/活一/眠二/活二/眠三/活三/眠四/活四/五/长连 这个地方的难点在于还要将所有的关键点检测出来
    //长连 黑棋line_count>5
    //五 黑棋line_count==5 白棋line_count>5
    //活四 两点可成五且line_count==4
    //眠四 一点可成五(line_count<=4)
    //活三 有点可成活四(有点可成五则为五比如O.OOO..)(line_count<=3)
    //眠三 有点可成眠四(有点可成活四则为活三比如O.XXX..)
    //活二 有点可成活三 (line_count<=2)
    //眠二 有点可成眠三(有点可成活三则为活二) (line_count<=2)  X...X
    //活一 有点可成活二 (line_count==1)
    //眠一 有点可成眠二 (line_count==1)
    inline pair< int, vector<Action> > checkLineState(const int &i, const int &j, const char &direct)
    {
        return checkLineState(i, j, direct, board, forbid_move);
    };
    //在所有方向上用checkState检查状态 除了输赢状态 要返回所有关键落子点
    static pair< int, vector<Action> > checkState(const int &i, const int &j, Board &goBoard, const bool &forbid_move=true);

    inline pair< int, vector<Action> > checkState(const int &i, const int &j)
    {
        return checkState(i, j, board, forbid_move);
    }

    inline pair< int, vector<Action> > checkState()
    {
        if (move_stack.empty())
            return make_pair(0, ac_vec);
        pair< int, vector<Action> > state;
        state=checkState(move_stack.top().second.first, move_stack.top().second.second); //暂时隐去
        if (state.first==FIVE)
            winner=board.at(move_stack.top().second.first, move_stack.top().second.second);
        else
        if (state.first<0)
        {
            winner=WHITE_STONE;
        }

        return state;
    }


private:
    stack< pair<int, Action> > move_stack;
    stack< pair<int, Action> > undo_stack;
    bool forbid_move; //禁手开关

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
