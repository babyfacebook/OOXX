#ifndef _BOARDTESTER_H_
#define _BOARDTESTER_H_

#include "..\\engine\Board.h"

class BoardTester{
public:
    BoardTester():board(){};
    void test()
    {
        cout<<"Side Length: "<<board.size()<<endl;
        cout<<"Side of Board: "<<sizeof(board)<<endl;
        cout<<board<<endl;

        for(int i=1;i<=board.size();++i)
            for(int j=1;j<=board.size();++j)
                if(board.remove(i, j))
                    cout<<"ERROR"<<endl;
        cout<<board<<endl;

        for(int i=1;i<=board.size();++i)
            for(int j=1;j<=board.size();++j)
                if (!board.isEmpty(i, j))
                    cout<<"ERROR"<<endl;

        for(int i=1;i<=board.size();++i)
            for(int j=1;j<=board.size();++j)
                board.place(BLACK_STONE, i, j);
        cout<<board<<endl;

        for(int i=1;i<=board.size();++i)
            for(int j=1;j<=board.size();++j)
                if (!board.isBlack(i, j))
                    cout<<"ERROR"<<endl;

        for(int i=1;i<=board.size();++i)
            for(int j=1;j<=board.size();++j)
                if(board.place(WHITE_STONE, i, j))
                    cout<<"ERROR"<<endl;
        cout<<board<<endl;

        for(int i=1;i<=board.size();++i)
            for(int j=1;j<=board.size();++j)
                board.remove(i, j);
        cout<<board<<endl;

        for(int i=1;i<=board.size();++i)
            for(int j=1;j<=board.size();++j)
                board.place(WHITE_STONE, i, j);
        cout<<board<<endl;

        for(int i=1;i<=board.size();++i)
            for(int j=1;j<=board.size();++j)
                if (!board.isWhite(i, j))
                    cout<<"ERROR- "<<i<<':'<<j<<endl;


        board.clear();
        if (!board.isCleared())
                    cout<<"ERROR"<<endl;

        cout<<board<<endl;

        board.place(BLACK_STONE, 8, 8);
        board.place(WHITE_STONE, 15, 3);
        board.place(WHITE_STONE, 2, 15);
        board.place(BLACK_STONE, 12, 1);
        board.place(BLACK_STONE, 1, 5);

        board.place(WHITE_STONE, 10, 5);
        board.place(WHITE_STONE, 14, 9);
        board.place(BLACK_STONE, 8, 11);
        board.place(BLACK_STONE, 6, 2);

        cout<<board<<endl;

        board.rotate();
        cout<<board<<endl;

        board.rotate();
        board.rotate();
        board.rotate();
        cout<<board<<endl;

        board.rotate(-1);
        board.rotate(1);
        cout<<board<<endl;

        board.rotate(-2);
        board.rotate(2);
        cout<<board<<endl;
        cout<<"Side Length: "<<board.size()<<endl;
        cout<<"Side of Board: "<<sizeof(board)<<endl;
        cout<<"is cleared?: "<<board.isCleared()<<endl;


    }
private:
    Board board;

};

#endif // _BOARDTESTER_H_
