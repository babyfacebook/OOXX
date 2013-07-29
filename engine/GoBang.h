#ifndef _GOBANG_H_
#define _GOBANG_H_

#include "Board.h"

#include <iostream>
#include <fstream>
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
const char direct_arr[4]={'-', '|', '/', '\\'};

class GoBang{
public:

    int curr_player;
    int winner;
    int move_count;
    Board board;

    GoBang(const bool &flag=true):curr_player(BLACK_STONE), winner(EMPTY_POINT), move_count(0), forbid_move(flag)
    {
        readModelMap(); //û�о�����һ��?
    };

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

    pair<pair< int, vector<Action> >, pair<int, int> > preScan(const int &i, const int &j, const char &direct, const Board &goBoard, const bool &forbid_move=true);

    inline pair<pair< int, vector<Action> >, pair<int, int> > preScan(const int &i, const int &j, const char &direct)
    {
        return preScan(i, j, direct, board, forbid_move);
    }

    //��ĳ����(����Ʋ��) �������ӵ㼯�Ϻ����鳤��
    static pair< int, vector<Action> > scanLine(const int &i, const int &j, const char &direct, const Board &goBoard);

    inline pair< int, vector<Action> > scanLine(const int &i, const int &j, const char &direct) const
    {
        return scanLine(i, j, direct, board);
    };

    static pair<int, pair<int, int> > countLine(const int &i, const int &j, const char &direct, const Board &goBoard);



    int isForbidMove(const int &i, const int &j, Board &goBoard);

    inline int isForbidMove(const int &i, const int &j)
    {
        return isForbidMove(i, j, board);
    }

    static int isForbidMove_Old(const int &i, const int &j, Board &goBoard);

    pair< int, vector<Action> > checkLineState(const int &i, const int &j, const char &direct, Board &goBoard, const bool &forbid_move=true);


    static pair< int, vector<Action> > checkLineStateWithKeyPoints(const int &i, const int &j, const char &direct, Board &goBoard, const bool &forbid_move=true, int depth=-1);

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
    inline pair< int, vector<Action> > checkLineStateWithKeyPoints(const int &i, const int &j, const char &direct)
    {
        return checkLineStateWithKeyPoints(i, j, direct, board, forbid_move);
    };
    //�����з�������checkState���״̬ ������Ӯ״̬ Ҫ�������йؼ����ӵ�
    static pair< int, vector<Action> > checkState(const int &i, const int &j, Board &goBoard, const bool &forbid_move=true);

    inline pair< int, vector<Action> > checkState(const int &i, const int &j)
    {
        return checkState(i, j, board, forbid_move);
    }

//    inline pair< int, vector<Action> > checkState()
//    {
//        if (move_stack.empty())
//            return make_pair(0, ac_vec);
//        pair< int, vector<Action> > state;
//        state=checkState(move_stack.top().second.first, move_stack.top().second.second); //��ʱ��ȥ
//        if (state.first==FIVE)
//            winner=board.at(move_stack.top().second.first, move_stack.top().second.second);
//        else
//        if (state.first<0)
//        {
//            winner=WHITE_STONE;
//        }
//
//        return state;
//    }
    inline pair< int, vector<Action> > checkState()
    {
        if (move_stack.empty())
            return make_pair(0, ac_vec);

//        pair< int, vector<Action> > state;
//        state=preScan(move_stack.top().second.first, move_stack.top().second.second, '-').first; //��ʱ��ȥ
//        if (state.first==FIVE)
//            winner=board.at(move_stack.top().second.first, move_stack.top().second.second);
//        else
//        if (state.first<0)
//        {
//            winner=WHITE_STONE;
//        }
//        return state;

        int state=0;
        state=isForbidMove(move_stack.top().second.first, move_stack.top().second.second);
        if (state==FIVE)
            winner=board.at(move_stack.top().second.first, move_stack.top().second.second);
        else
        if (state<0)
        {
            winner=WHITE_STONE;
        }

        return make_pair(state, ac_vec);
    }

    int checkWinner(const int &i, const int &j, Board &goBoard, const bool &forbid_move=true);
    inline int checkWinner(const int &i, const int &j)
    {
        return checkWinner(i, j, board, forbid_move);
    }

    inline int checkWinner()
    {
        if (move_stack.empty())
            return 0;
        return checkWinner(move_stack.top().second.first, move_stack.top().second.second);
    }


private:
    stack< pair<int, Action> > move_stack;
    stack< pair<int, Action> > undo_stack;
    bool forbid_move; //���ֿ���  ����ɾ�����ˡ��� �鷳
    map< vector<signed char>, pair<signed char, vector<signed char> > > model_map; //���static���и���ֵĶ���

    inline void changePlayer()
    {
        curr_player=-curr_player;
    }

    inline void readModelMap()
    {
           const char * file="model_map.dat";
            vector<signed char> keyPosSet;
            vector<signed char> model;
            int piece;
            int pos;
            signed char state=0;
            int keyPosSet_size;
            int map_size;
            char buffer=0;
           ifstream fin(file,ios::binary);
//            int k=0;
              fin.read(reinterpret_cast <char*>(&map_size),sizeof (map_size));
              for(int n=0;n<map_size;++n)
              {
                    model.clear();
//                    for(int i=0;i!=11;++i)
//                    {
//                        fin.read(reinterpret_cast <char*>(&piece),sizeof (piece));
//                        model.push_back(piece);
//                    }
                    fin.read(&buffer,sizeof (buffer));
                    model.push_back(buffer&0x03);
                    model.push_back((buffer&0x0C)>>2);
                    model.push_back((buffer&0x30)>>4);
                    model.push_back((buffer&0xC0)>>6); //�����ֿ����Ƴ����⡣��
                    fin.read(&buffer,sizeof (buffer));
                    model.push_back(buffer&0x03);
                    model.push_back((buffer&0x0C)>>2);
                    model.push_back((buffer&0x30)>>4);
                    model.push_back((buffer&0xC0)>>6); //�����ֿ����Ƴ����⡣��
                    fin.read(&buffer,sizeof (buffer));
                    model.push_back(buffer&0x03);
                    model.push_back((buffer&0x0C)>>2);
                    model.push_back((buffer&0x30)>>4);

                    //��һ��ԭ����

                    for(vector<signed char>::iterator iter=model.begin(); iter!=model.end(); ++iter)
                    {
                        *iter=*iter-1;

                    }

//                    fin.read(reinterpret_cast <char*>(&state),sizeof (state));
//
//                    fin.read(reinterpret_cast <char*>(&keyPosSet_size),sizeof (keyPosSet_size));

                    fin.read(&buffer,sizeof (buffer));
                    state=(buffer&0xF0)>>4;
                    keyPosSet_size=buffer&0x0F;

                    keyPosSet.clear();

                    bool low=true;

                    for(int i=0; i!=keyPosSet_size;++i)
                    {
//                        fin.read(reinterpret_cast <char*>(&pos),sizeof (pos));

                        if (low)
                        {
                            fin.read(&buffer,sizeof (buffer));
                            pos=buffer&0x0F;
                            low=false;
                        }
                        else
                        {
                            pos=(buffer&0xF0)>>4;
                            low=true;

                        }
                        //�ָ�ԭֵ
                        pos=pos-5;
                        keyPosSet.push_back(pos);
                    }

                    model_map[model]=make_pair(state, keyPosSet);


              }

            fin.close();
//            showMap(model_map);


            map< vector<signed char>, pair<signed char, vector<signed char> > > temp_model_map=model_map;
            for(map< vector<signed char>, pair<signed char, vector<signed char> > >::iterator iter=model_map.begin(); iter!=model_map.end(); ++iter)
            {
                for(int n=0;n<11;++n)
                {
                    model[n]=(iter->first)[10-n];
                }
                if (model_map.count(model)==0)
                {
                    keyPosSet=(iter->second).second;
                    for(vector<signed char>::iterator pos_iter=keyPosSet.begin(); pos_iter!=keyPosSet.end(); ++pos_iter)
                    {
                        *pos_iter=-*pos_iter;
                    }

                    temp_model_map[model]=make_pair((iter->second).first, keyPosSet);
                }

            }
            model_map=temp_model_map;


    }

    inline void showMap(map<vector<int>, pair<int, vector<int> > > &m)
    {
        vector<int> keyPosSet;
        vector<int> model;
        int piece;
        int pos;
        int state=0;
        for(map<vector<int>, pair<int, vector<int> > >::iterator iter=m.begin(); iter!=m.end(); ++iter)
        {
            model=iter->first;
            for(vector<int>::iterator model_iter=model.begin(); model_iter!=model.end(); ++model_iter)
            {
                piece=*model_iter;
                switch(piece)
                {
                case BLACK_STONE: cout<<BLACK_MARK;
                    break;
                case WHITE_STONE: cout<<WHITE_MARK;
                    break;
                case EMPTY_POINT: cout<<EMPTY_MARK;
                    break;
                default: cout<<INVALID_MARK;
                    break;
                }

            }
            cout<<endl;
            state=(iter->second).first;

            switch(state)
        {
        case FIVE:
            {
                cout<<"state: "<<" Five"<<endl;
            }
            break;
        case OVERLINE:
            {
                cout<<"state: "<<" Overline"<<endl;
            }
            break;
        case OPEN_FOUR:
            {
                cout<<"state: "<<" Open Four"<<endl;
            }
            break;
        case SLEEP_FOUR:
            {
                cout<<"state: "<<" Sleep Four"<<endl;
            }
            break;
        case OPEN_THREE:
            {
                cout<<"state: "<<" Open Three"<<endl;
            }
            break;
        case SLEEP_THREE:
            {
                cout<<"state: "<<" Sleep Three"<<endl;
            }
            break;
        case OPEN_TWO:
            {
                cout<<"state: "<<" Open Two"<<endl;
            }
            break;
        case SLEEP_TWO:
            {
                cout<<"state: "<<" Sleep Two"<<endl;
            }
            break;
        case OPEN_ONE:
            {
                cout<<"state: "<<" Open One"<<endl;
            }
            break;
        case SLEEP_ONE:
            {
                cout<<"state: "<<" Sleep One"<<endl;
            }
            break;
        case DOUBLE_FOUR:
            {
                cout<<"state: "<<" Double Four"<<endl;
            }
            break;
        case DOUBLE_THREE:
            {
                cout<<"state: "<<" Double THree"<<endl;
            }
            break;
        case F_DOUBLE_FOUR:
            {
                cout<<"state: "<<" F Double Four"<<endl;
            }
            break;
        case F_DOUBLE_THREE:
            {
                cout<<"state: "<<" F Double THree"<<endl;
            }
            break;
        default: cout<<"state: "<<" Nothing"<<endl;
            break;
        }





            keyPosSet=(iter->second).second;
            for(vector<int>::iterator pos_iter=keyPosSet.begin(); pos_iter!=keyPosSet.end(); ++pos_iter)
            {
                cout<<"pos: "<<*pos_iter<<endl;
            }
            cout<<endl;

        }

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
