#ifndef _GOBANGTESTER_H_
#define _GOBANGTESTER_H_

#include "..\\engine\GoBang.h"

class GoBangTester{
public:
    void test()
    {
//        cout<<endl;
//        goBang.undo();
//        cout<<goBang<<endl;
//
//        goBang.move(8,8);
//        goBang.move(8,9);
//        goBang.move(8,10);
//        goBang.move(8,11);
//        goBang.move(7,8);
//        cout<<goBang<<endl;

//        goBang.redo();
//
//        goBang.undo();
//        cout<<goBang<<endl;
//        goBang.undo();
//        goBang.undo();
//        goBang.undo();
//        goBang.undo();
//        goBang.undo();
//        cout<<goBang<<endl;
//
//        goBang.redo();
//        cout<<goBang<<endl;
//        goBang.redo();
//        goBang.redo();
//        goBang.redo();
//        goBang.redo();
//        goBang.redo();
//        cout<<goBang<<endl;
//
//        goBang.undo();
//        goBang.undo();
//        goBang.undo();
//        cout<<goBang<<endl;
//        goBang.move(8,12);
//        goBang.move(8,13);
//        cout<<goBang<<endl;
//        pair< int, vector<Action> > o=goBang.scanLine(8, 8, '|');
//        cout<<"| line-count: "<<o.first<<endl;
//        showActionSet(o.second);
//
//        o=goBang.scanLine(8, 8, '-');
//        cout<<"- line-count: "<<o.first<<endl;
//        showActionSet(o.second);
//
//        o=goBang.scanLine(8, 8, '\\');
//        cout<<"\\ line-count: "<<o.first<<endl;
//        showActionSet(o.second);
//
//        o=goBang.scanLine(8, 8, '/');
//        cout<<"/ line-count: "<<o.first<<endl;
//        showActionSet(o.second);

//        goBang.move(BLACK_STONE,9,8);
//        goBang.move(BLACK_STONE,5,8);
//        goBang.move(BLACK_STONE,5,8);
//        goBang.move(BLACK_STONE,8,7);
//        goBang.move(BLACK_STONE,8,6);
//        goBang.move(BLACK_STONE,8,5);

//        goBang.move(BLACK_STONE,10,8);
//        goBang.move(BLACK_STONE,11,8);
//        goBang.move(BLACK_STONE,12,8);
//        goBang.move(BLACK_STONE,9,9);
//        goBang.move(BLACK_STONE,10,5);
//        goBang.move(WHITE_STONE,8,14);
//        goBang.move(WHITE_STONE,12,11);
//        goBang.move(WHITE_STONE,12,14);
//        goBang.move(WHITE_STONE,10,14);
//
//        goBang.move(BLACK_STONE,1,1);
//        goBang.move(BLACK_STONE,1,3);
//        goBang.move(BLACK_STONE,1,5);
//        goBang.move(BLACK_STONE,1,7);
//        goBang.move(BLACK_STONE,1,9);
//        goBang.move(BLACK_STONE,1,11);
//        goBang.move(BLACK_STONE,1,12);
//        goBang.move(BLACK_STONE,1,13);
//
//        goBang.redo();
//        goBang.redo();
//        cout<<goBang<<endl;
//        showLineState(8, 8, '|');   //感觉有点问题啊
//        showLineState(7, 8, '-');
//        showLineState(10, 8, '|');
//        showLineState(8, 8, '-');
//        showLineState(8, 9, '-');
//        showLineState(8, 10, '\\');
//        showLineState(8, 11, '-');
//        showLineState(9, 9, '\\');
//        showLineState(12, 11, '|');
//        showLineState(10, 14, '|');
//        showLineState(10, 5, '\\');
//        showLineState(1, 13, '-');
//        showLineState(1, 1, '-');
//        showLineState(1, 11, '-');
//        showLineState(1, 7, '-');

        goBang.reset();
//        goBang.move(BLACK_STONE,1,1);
//        goBang.move(BLACK_STONE,1,4);
//        goBang.move(BLACK_STONE,1,5);
//        goBang.move(BLACK_STONE,1,7);
//
//        goBang.move(BLACK_STONE,2,1);
//        goBang.move(BLACK_STONE,2,3);
//        goBang.move(BLACK_STONE,2,4);
//        goBang.move(BLACK_STONE,2,5);
//        goBang.move(BLACK_STONE,2,6);
//        goBang.move(BLACK_STONE,2,8);
//
//        goBang.move(WHITE_STONE,3,1);
//        goBang.move(WHITE_STONE,3,3);
//        goBang.move(WHITE_STONE,3,4);
//        goBang.move(WHITE_STONE,3,5);
//        goBang.move(WHITE_STONE,3,6);
//        goBang.move(WHITE_STONE,3,8);
//
//        goBang.move(WHITE_STONE,4,1);
//        goBang.move(BLACK_STONE,4,3);
//        goBang.move(BLACK_STONE,4,4);
//        goBang.move(BLACK_STONE,4,5);
//        goBang.move(WHITE_STONE,4,8);
//
//        goBang.move(WHITE_STONE,5,1);
//        goBang.move(BLACK_STONE,5,3);
//        goBang.move(BLACK_STONE,5,4);
//        goBang.move(BLACK_STONE,5,5);
//        goBang.move(WHITE_STONE,5,7);
//
//        goBang.move(WHITE_STONE,6,1);
//        goBang.move(BLACK_STONE,6,3);
//        goBang.move(BLACK_STONE,6,6);
//        goBang.move(WHITE_STONE,6,8);
//
//        goBang.move(WHITE_STONE,7,1);
//        goBang.move(BLACK_STONE,7,3);
//        goBang.move(BLACK_STONE,7,5);
//        goBang.move(WHITE_STONE,7,7);
//
//        goBang.move(WHITE_STONE,8,1);
//        goBang.move(BLACK_STONE,8,6);
//
//
//        goBang.move(WHITE_STONE,9,6);
//        goBang.move(BLACK_STONE,9,1);

//        goBang.move(BLACK_STONE,6,4);

        cout<<goBang<<endl;
//        showLineState(1, 1, '-');
//        showLineState(1, 4, '-');
//        showLineState(2, 6, '-');
//        showLineState(3, 4, '-');
//        showLineState(4, 5, '-');
//        showLineState(5, 3, '-');
//        showLineState(6, 3, '-');
//        showLineState(7, 5, '-');
//        showLineState(6, 8, '/');
//        showLineState(6, 8, '|');
//        showLineState(7, 1, '|');
//        showLineState(4, 8, '\\');
//        showLineState(8, 6, '-');
//        showLineState(9, 1, '-');
//        showLineState(6, 8, '\\');
//        showLineState(7, 5, '/');
//        o=goBang.scanLine(6, 8, '/');
//        cout<<"| line-count: "<<o.first<<endl;
//        showActionSet(o.second);

         goBang.move(BLACK_STONE,8,8);
         goBang.move(BLACK_STONE,8,9);
         goBang.move(BLACK_STONE,8,10);
         goBang.move(BLACK_STONE,8,11);
         cout<<goBang<<endl;
         showState(8, 11);

    };

private:
    GoBang goBang;
    inline void showActionSet(const vector<Action> &actionSet) const
    {
        for(vector<Action>::const_iterator iter=actionSet.begin(); iter!=actionSet.end(); iter++)
        {
            cout<<iter->first<<':'<<iter->second<<endl;
        }
    }

    inline void showState(const int &i, const int &j)
    {
        pair<int, vector<Action> > o=goBang.checkState(i, j);
        int state=o.first;
        vector<Action> keyPointSet=o.second;
        switch(state)
        {
        case FIVE:
            {
                cout<<i<<':'<<j<<" Five"<<endl;
            }
            break;
        case OVERLINE:
            {
                cout<<i<<':'<<j<<" Overline"<<endl;
            }
            break;
        case OPEN_FOUR:
            {
                cout<<i<<':'<<j<<" Open Four"<<endl;
            }
            break;
        case SLEEP_FOUR:
            {
                cout<<i<<':'<<j<<" Sleep Four"<<endl;
            }
            break;
        case OPEN_THREE:
            {
                cout<<i<<':'<<j<<" Open Three"<<endl;
            }
            break;
        case SLEEP_THREE:
            {
                cout<<i<<':'<<j<<" Sleep Three"<<endl;
            }
            break;
        case OPEN_TWO:
            {
                cout<<i<<':'<<j<<" Open Two"<<endl;
            }
            break;
        case SLEEP_TWO:
            {
                cout<<i<<':'<<j<<" Sleep Two"<<endl;
            }
            break;
        case OPEN_ONE:
            {
                cout<<i<<':'<<j<<" Open One"<<endl;
            }
            break;
        case SLEEP_ONE:
            {
                cout<<i<<':'<<j<<" Sleep One"<<endl;
            }
            break;
        case DOUBLE_FOUR:
            {
                cout<<i<<':'<<j<<" Double Four"<<endl;
            }
            break;
        case DOUBLE_THREE:
            {
                cout<<i<<':'<<j<<" Double THree"<<endl;
            }
            break;
        case F_DOUBLE_FOUR:
            {
                cout<<i<<':'<<j<<" F Double Four"<<endl;
            }
            break;
        case F_DOUBLE_THREE:
            {
                cout<<i<<':'<<j<<" F Double THree"<<endl;
            }
            break;
        default: cout<<i<<':'<<j<<" Nothing"<<endl;
            break;
        }
        cout<<"Key Point Set:"<<endl;
        for(vector<Action>::iterator iter=keyPointSet.begin(); iter!=keyPointSet.end(); ++iter)
        {
            cout<<iter->first<<':'<<iter->second<<endl;
        }
    }

    inline void showLineState(const int &i, const int &j, const char &direct)
    {
        pair<int, vector<Action> > o=goBang.checkLineState(i, j, direct);
        int state=o.first;
        vector<Action> keyPointSet=o.second;
        switch(state)
        {
        case FIVE:
            {
                cout<<i<<':'<<j<<direct<<"Five"<<endl;
            }
            break;
        case OVERLINE:
            {
                cout<<i<<':'<<j<<direct<<"Overline"<<endl;
            }
            break;
        case OPEN_FOUR:
            {
                cout<<i<<':'<<j<<direct<<"Open Four"<<endl;
            }
            break;
        case SLEEP_FOUR:
            {
                cout<<i<<':'<<j<<direct<<"Sleep Four"<<endl;
            }
            break;
        case OPEN_THREE:
            {
                cout<<i<<':'<<j<<direct<<"Open Three"<<endl;
            }
            break;
        case SLEEP_THREE:
            {
                cout<<i<<':'<<j<<direct<<"Sleep Three"<<endl;
            }
            break;
        case OPEN_TWO:
            {
                cout<<i<<':'<<j<<direct<<"Open Two"<<endl;
            }
            break;
        case SLEEP_TWO:
            {
                cout<<i<<':'<<j<<direct<<"Sleep Two"<<endl;
            }
            break;
        case OPEN_ONE:
            {
                cout<<i<<':'<<j<<direct<<"Open One"<<endl;
            }
            break;
        case SLEEP_ONE:
            {
                cout<<i<<':'<<j<<direct<<"Sleep One"<<endl;
            }
            break;
        default: cout<<i<<':'<<j<<direct<<"Nothing"<<endl;
            break;
        }
        cout<<"Key Point Set:"<<endl;
        for(vector<Action>::iterator iter=keyPointSet.begin(); iter!=keyPointSet.end(); ++iter)
        {
            cout<<iter->first<<':'<<iter->second<<endl;
        }
    }

};

#endif // _GOBANGTESTER_H_
