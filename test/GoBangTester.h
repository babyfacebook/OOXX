#ifndef _GOBANGTESTER_H_
#define _GOBANGTESTER_H_

#include "..\\engine\GoBang.h"
#include <sstream>
#include <map>
#include <fstream>

class GoBangTester{
public:
    GoBangTester():goBang(true){};
    void test()
    {
        pair< int, vector<Action> > o;
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

//        goBang.reset();
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

//        cout<<goBang<<endl;
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

//         goBang.move(BLACK_STONE,8,8);
//         goBang.move(BLACK_STONE,8,9);
//         goBang.move(BLACK_STONE,8,10);
//         goBang.move(BLACK_STONE,8,11);
//         cout<<goBang<<endl;
//         showState(8, 11);

//         goBang.reset();
//         goBang.move_debug(BLACK_STONE,8,7);
//         goBang.move_debug(WHITE_STONE,8,8);
//         goBang.move_debug(BLACK_STONE,8,9);
////        goBang.move_debug(WHITE_STONE,7,9);
//         goBang.move_debug(WHITE_STONE,6,9);
//         goBang.move_debug(WHITE_STONE,5,9);
//         goBang.move_debug(WHITE_STONE,4,9);
//         goBang.move_debug(WHITE_STONE,2,9);
//         cout<<goBang<<endl;
////         showLineState(6, 9, '|');
//        o=goBang.scanLine(6, 9, '|');
//        cout<<"| 6:9 line-count: "<<o.first<<endl;
//        showActionSet(o.second);
//         cout<<goBang<<endl;

//         goBang.reset();
//         goBang.move_debug(BLACK_STONE,9,8);
//         goBang.move_debug(BLACK_STONE,10,8);
//         goBang.move_debug(BLACK_STONE,11,8);
//
//         goBang.move_debug(BLACK_STONE,7,8);
//         goBang.move_debug(BLACK_STONE,6,8);
//         goBang.move_debug(BLACK_STONE,5,8);
//
//         goBang.move_debug(BLACK_STONE,8,7);
//         goBang.move_debug(BLACK_STONE,8,9);
//         cout<<goBang<<endl;
//
//        o=goBang.scanLine(8, 9, '-');
//        cout<<"| 8:9 line-count: "<<o.first<<endl;
//        showActionSet(o.second);
//        showLineState(8, 9, '-');
//
//
//        goBang.move_debug(BLACK_STONE,8,10);
//
//        cout<<goBang<<endl;
//
//        o=goBang.scanLine(8, 9, '-');
//        cout<<"| 8:9 line-count: "<<o.first<<endl;
//        showActionSet(o.second);
//        showLineState(8, 9, '-');
//
//
//        goBang.move_debug(BLACK_STONE,8,11);
//
//        cout<<goBang<<endl;
//
//        o=goBang.scanLine(8, 9, '-');
//        cout<<"| 8:9 line-count: "<<o.first<<endl;
//        showActionSet(o.second);
//        showLineState(8, 9, '-');
//            goBang.reset();
//            goBang.move_debug(BLACK_STONE,9,9);
//            goBang.move_debug(BLACK_STONE,9,8);
//            goBang.move_debug(BLACK_STONE,8,10);
//            goBang.move_debug(BLACK_STONE,7,10);
//            goBang.move_debug(BLACK_STONE,6,8);
//            goBang.move_debug(BLACK_STONE,6,9);
//            goBang.move_debug(BLACK_STONE,7,7);
//            cout<<goBang<<endl;
//            o=goBang.scanLine(7, 7, '-');
//            cout<<"| 7:7 line-count: "<<o.first<<endl;
//            showActionSet(o.second);
//            showLineState(7, 7, '|');

            goBang.reset();
//            goBang.move_debug(BLACK_STONE,9,9);
//            goBang.move_debug(BLACK_STONE,9,8);
//            goBang.move_debug(BLACK_STONE,8,10);
//            goBang.move_debug(BLACK_STONE,7,10);
//            goBang.move_debug(BLACK_STONE,6,8);
//            goBang.move_debug(BLACK_STONE,6,9);
//            goBang.move_debug(BLACK_STONE,7,7);
//            cout<<goBang<<endl;
//            showPreScan(7, 7, '|');

            goBang.move_debug(WHITE_STONE,8,6);
            goBang.move_debug(BLACK_STONE,8,7);
            goBang.move_debug(BLACK_STONE,8,8);
            goBang.move_debug(BLACK_STONE,8,9);
            goBang.move_debug(BLACK_STONE,8,10);
            goBang.move_debug(BLACK_STONE,8,12);
            goBang.move_debug(WHITE_STONE,8,13);
            cout<<goBang<<endl;
            showLineState(8, 8, '-');




    };

    inline void hashModel()
    {
            goBang.reset();

            int A[11];
            vector<int> B(11,0);
            int k=0;
            int ch;
            pair<int, vector<Action> > o;
            bool flag=false;

            int block_begin, block_end;

//            pair<int, vector<Action> > o;
            int state;
            vector<Action> keyPointSet;
            vector<int> keyPosSet;
            stringstream stream;
            int x=0;
            for(A[0]=-1; A[0]!=2; ++A[0])
                for(A[1]=-1; A[1]!=2; ++A[1])
                    for(A[2]=-1; A[2]!=2; ++A[2])
                        for(A[3]=-1; A[3]!=2; ++A[3])
                            for(A[4]=-1; A[4]!=2; ++A[4])
                                for(A[5]=-1; A[5]<2; A[5]=A[5]+2)
                                for(A[6]=-1; A[6]!=2; ++A[6])
                                    for(A[7]=-1; A[7]!=2; ++A[7])
                                        for(A[8]=-1; A[8]!=2; ++A[8])
                                            for(A[9]=-1; A[9]!=2; ++A[9])
                                                for(A[10]=-1; A[10]!=2; ++A[10])
                                                {
//                                                    x++;
//                                                    cout<<x<<" th Try"<<endl;


                                                    goBang.reset();

                                                    block_begin=-6;
                                                    block_end=6;

                                                    flag=false;
                                                    B[5]=A[5];

                                                    //修剪排除冗余
                                                    for(int i=1; i<=5;++i)
                                                    {
                                                        if (flag)
                                                            B[5+i]=EMPTY_POINT;
                                                        else
                                                            B[5+i]=A[5+i];
                                                        if (A[5+i]==-A[5]&&!flag)
                                                        {
                                                            block_end=i;
                                                            flag=true;
                                                        }

                                                    }
                                                    flag=false;
                                                    for(int i=1; i<=5;++i)
                                                    {
                                                        if (flag)
                                                            B[5-i]=EMPTY_POINT;
                                                        else
                                                            B[5-i]=A[5-i];
                                                        if (A[5-i]==-A[5]&&!flag)
                                                        {
                                                            block_begin=-i;
                                                            flag=true;
                                                        }

                                                    }


                                                    stream.clear();
                                                    stream.str("");



                                                    for(int n=0; n!=11; ++n)
                                                    {
//                                                        cout<<A[n]<<' ';
                                                        goBang.board.place(B[n],8, 3+n);
//                                                        cout<<goBang.board.mark(8, 3+n);
                                                        stream<<goBang.board.mark(8, 3+n);
                                                    }
//                                                    cout<<endl;
//
                                                    o=goBang.checkLineStateWithKeyPoints(8, 8, '-');
                                                    state=o.first;

                                                    keyPointSet=o.second;


                                                    if (!(state==FIVE||state==OVERLINE||block_end-block_begin<6))
                                                    {

                                                        if (model_map.count(B)) //如果找到
                                                        {
                                                            cout<<"BINGO!"<<endl;
//                                                            cin>>ch;

                                                            cout<<stream.str()<<endl;
                                                            keyPosSet=model_map[B].second;
                                                            for(vector<int>::iterator iter=keyPosSet.begin(); iter!=keyPosSet.end(); ++iter)
                                                            {
                                                                cout<<*iter<<endl;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            //没找到则反一下再找
                                                            int temp;
                                                            for(int i=1; i<=5;++i)
                                                            {
                                                                temp=B[5+i];
                                                                B[5+i]=B[5-i];
                                                                B[5-i]=temp;
                                                            }

                                                            if (model_map.count(B))
                                                            {
                                                                cout<<"R BINGO!"<<endl;

                                                                cout<<stream.str()<<endl;
                                                                keyPosSet=model_map[B].second;
                                                                for(vector<int>::iterator iter=keyPosSet.begin(); iter!=keyPosSet.end(); ++iter)
                                                                {
                                                                    cout<<*iter<<endl;
                                                                }
                                                            }
                                                            else
                                                            {   //反回来
                                                                for(int i=1; i<=5;++i)
                                                                {
                                                                    temp=B[5+i];
                                                                    B[5+i]=B[5-i];
                                                                    B[5-i]=temp;
                                                                }

                                                                k++;
                                                                cout<<k<<"th Example: "<<stream.str()<<endl;
                                                                keyPosSet.clear();
                                                                for(vector<Action>::iterator iter=keyPointSet.begin(); iter!=keyPointSet.end(); ++iter)
                                                                {
                                                                    keyPosSet.push_back(iter->second-8);
                                                                }
                                                                model_map[B]=make_pair(state, keyPosSet);
                                                                showLineState(8, 8, '-');
                                                            }



                                                        }

                                                    }

                                                    cout<<endl;
                                                }

                                                map_serialization(model_map);
    }


private:
    GoBang goBang;
    map< vector<int>, pair<int, vector<int> > > model_map;

    inline void showActionSet(const vector<Action> &actionSet) const
    {
        for(vector<Action>::const_iterator iter=actionSet.begin(); iter!=actionSet.end(); iter++)
        {
            cout<<iter->first<<':'<<iter->second<<endl;
        }
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


//    inline void map_serialization(map<vector<int>, pair<int, vector<int> > > &m)
//    {
//        const char * file="model_map.dat";
//
//        vector<int> keyPosSet;
//        vector<int> model;
//        int keyPosSet_size;
//        ofstream fout(file, ios::binary);//以二进制模式打开以写入只追加打开文件
//        int k=0;
//        int map_size=m.size();
//
//        fout.write(reinterpret_cast <char *>(&(map_size)),sizeof (map_size));
//        for(map<vector<int>, pair<int, vector<int> > >::iterator iter=m.begin(); iter!=m.end(); ++iter)
//        {
//            k++;
//            cout<<k<<" th serialization completed."<<endl;
//            model=iter->first;
//            for(vector<int>::iterator model_iter=model.begin(); model_iter!=model.end(); ++model_iter)
//            {
//                fout.write(reinterpret_cast <char *>(&(*model_iter)),sizeof (*model_iter));
//            }
//
//            fout.write(reinterpret_cast <char *>(&((iter->second).first)), sizeof ((iter->second).first));
//            keyPosSet=(iter->second).second;
//            keyPosSet_size=keyPosSet.size();
//            fout.write(reinterpret_cast <char *>(&(keyPosSet_size)),sizeof(keyPosSet_size));
//            for(vector<int>::iterator pos_iter=keyPosSet.begin(); pos_iter!=keyPosSet.end();++pos_iter)
//            {
//                    fout.write(reinterpret_cast <char *>(&(*pos_iter)),sizeof(*pos_iter));
//            }
//        }
//
//
////    fout.flush();
//        fout.close();
//    }
        //还有压缩空间 黑白相同棋型 状态和着法都一样的可以合并变成 0->0 2->棋子 3->对手
    inline void map_serialization(map<vector<int>, pair<int, vector<int> > > &m)
    {
        const char * file="model_map.dat";

        vector<int> keyPosSet;
        vector<int> model;
        int keyPosSet_size;
        ofstream fout(file, ios::binary|ios::out);//以二进制模式打开以写入只追加打开文件
        int k=0;
        int map_size=m.size();
        char state=0;
        char buffer=0;
        vector<char> B(11,0);
        fout.write(reinterpret_cast <char *>(&(map_size)),sizeof (map_size));

        for(map<vector<int>, pair<int, vector<int> > >::iterator iter=m.begin(); iter!=m.end(); ++iter)
        {
            k++;
            cout<<k<<" th serialization completed."<<endl;
            model=iter->first;

//            for(vector<int>::iterator model_iter=model.begin(); model_iter!=model.end(); ++model_iter)
//            {
//                fout.write(reinterpret_cast <char *>(&(*model_iter)),sizeof (*model_iter));
//            }
            for(int n=0; n!=11; ++n)
            {
                B[n]=model[n]+1; //避免有符号数的麻烦 原-1 0 1现在对应0 1 2 反序列化时记得减一变回来
            }

            buffer=(B[0]&0x03)|(B[1]<<2&0x0C)|(B[2]<<4&0x30)|(B[3]<<6&0xC0);
            fout.write(&buffer,sizeof (buffer));
//            fout<<buffer;
            buffer=(B[4]&0x03)|(B[5]<<2&0x0C)|(B[6]<<4&0x30)|(B[7]<<6&0xC0);
            fout.write(&buffer,sizeof (buffer));
//            fout<<buffer;
            buffer=(B[8]&0x03)|(B[9]<<2&0x0C)|(B[10]<<4&0x30);
            fout.write(&buffer,sizeof (buffer));
//            fout<<buffer;

//            fout.write(reinterpret_cast <char *>(&((iter->second).first)), sizeof ((iter->second).first));
//
            keyPosSet=(iter->second).second;
            keyPosSet_size=keyPosSet.size();
            state=(iter->second).first;
            buffer=(state<<4&0xF0)|(keyPosSet_size&0x0F);
            fout.write(&buffer,sizeof(buffer));
//            fout<<buffer;

//            fout.write(reinterpret_cast <char *>(&(keyPosSet_size)),sizeof(keyPosSet_size));

            bool low=true;
            int temp;
            for(vector<int>::iterator pos_iter=keyPosSet.begin(); pos_iter!=keyPosSet.end();++pos_iter) //这个地方也要移位-5到5移到0到10
            {
//                    fout.write(reinterpret_cast <char *>(&(*pos_iter)),sizeof(*pos_iter));
                    temp=*pos_iter+5;
                    if (low)
                    {
                        buffer=0x00|(temp&0x0F);
                        low=false;
                    }
                    else
                    {
                        buffer|=(temp<<4&0xF0);
                        fout.write(&buffer,sizeof (buffer));
                        low=true;

                    }
            }
            if (keyPosSet_size%2!=0)
            {
                fout.write(&buffer,sizeof (buffer));
            }
        }

//        fout.flush();
        fout.close();
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


        for(vector<Action>::iterator iter=keyPointSet.begin(); iter!=keyPointSet.end(); ++iter)
        {
            cout<<iter->first<<':'<<iter->second<<endl;
        }
    }

    inline void showPreScan(const int &i, const int &j, const char &direct)
    {
        pair<pair< int, vector<Action> >, pair<int, int> > o=goBang.preScan(i, j, direct);
        int state=o.first.first;
        vector<Action> keyPointSet=o.first.second;
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

    inline void showLineState(const int &i, const int &j, const char &direct)
    {
        pair<int, vector<Action> > o=goBang.checkLineStateWithKeyPoints(i, j, direct);
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
