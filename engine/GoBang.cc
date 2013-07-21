#include "GoBang.h"

char direct_arr[4]={'-', '|', '/', '\\'};

int GoBang::move(const int &i, const int &j)
{
    return move(curr_player, i, j);
}

int GoBang::move(const int &piece, const int &i, const int &j, const bool &clear_undoStack)
{
    if (winner!=0||!board.place(piece, i, j)) //����ʧ�ܻ�������
        return ERROR_CODE;
    //���ӳɹ�
    move_count++;
    move_stack.push(make_pair(piece, make_pair(i, j)));
    if (clear_undoStack) //stackû�е����� clear�ǿ�������ʵ��
        while(!undo_stack.empty())
            undo_stack.pop();

    int state=checkState();
    changePlayer();

//    if (!clear_undoStack)
//        undo_stack.pop();
//    return checkState();
    return state; //��ʱ

}



int GoBang::undo()
{
    if (move_stack.empty())
        return 0;
    else
    {
        int i=move_stack.top().second.first;
        int j=move_stack.top().second.second;
        board.remove(i, j);
        move_count--;
        changePlayer(); //����ط���Ҫ�úÿ��ǿ��� ����ʱ������̫��
//        clearWinner();
        winner=EMPTY_POINT;

        undo_stack.push(move_stack.top());
        move_stack.pop();
        return checkState();
    }
}
int GoBang::redo()
{
    if (undo_stack.empty())
        return ERROR_CODE;

    int i=undo_stack.top().second.first;
    int j=undo_stack.top().second.second;
    int piece=undo_stack.top().first;
    undo_stack.pop();
    return move(piece, i, j, false); //��redo���õ�move��Ҫ���undo_stack
}

void GoBang::reset()
{
    board.clear();
    while(!move_stack.empty())
        move_stack.pop();
    while(!undo_stack.empty())
        undo_stack.pop();
    winner=EMPTY_POINT;
    curr_player=BLACK_STONE;
}

//��ĳ����(����Ʋ��) �������ӵ㼯�Ϻ����鳤��
pair< int, vector<Action> > GoBang::scanLine(const int &i, const int &j, const char &direct, const Board &goBoard)
{
    vector<Action> emptyPointSet;
    if (!goBoard.isPiece(i, j)) //�����������
        return make_pair(0, emptyPointSet);

    int connect_count=1;
    int oppo_connect_count=1;
    int offset_i=0;
    int offset_j=0;



    switch(direct)
    {
    case '-':
        {
            offset_i=0;
            offset_j=1;
        }
        break;
    case '|':
        {
            offset_i=1;
            offset_j=0;
        }
        break;
    case '/':
        {
            offset_i=1;
            offset_j=1;
        }
        break;
    case '\\':
        {
            offset_i=1;
            offset_j=-1;
        }
        break;
    default: //��
        {
            offset_i=0;
            offset_j=1;
        }
        break;
    }


    int block_begin=-5;
    int block_end=5;
    int end_index=0;
    int begin_index=0;
    int piece=0;
    //˫��ɨ��
    bool countinue_flag=true;
    for(int n=1; n<5; ++n)
    {
        piece=goBoard.at(i+n*offset_i, j+n*offset_j);

        if (piece==ERROR_CODE||piece==-goBoard.at(i, j))
        {
            block_end=n;
                break;
        }
        if (piece==goBoard.at(i, j))
        {
            end_index=n;
            if (countinue_flag)
                connect_count++;
        }
        else
        {
            countinue_flag=false;
        }



    }
    if (connect_count==5&&goBoard.at(i+5*offset_i, j+5*offset_j)==goBoard.at(i, j))
        connect_count++;

    countinue_flag=true;
    for(int n=-1; n>-5; --n)
    {

        piece=goBoard.at(i+n*offset_i, j+n*offset_j);

        if (piece==ERROR_CODE||piece==-goBoard.at(i, j))
        {
            block_begin=n;
            break;
        }

        if (piece==goBoard.at(i, j))
        {
            begin_index=n;
            if (countinue_flag)
                oppo_connect_count++;
        }
        else
        {
            countinue_flag=false;
        }
    }

    if (oppo_connect_count==5&&goBoard.at(i-5*offset_i, j-5*offset_j)==goBoard.at(i, j))
        oppo_connect_count++;

    connect_count=connect_count+oppo_connect_count-1;

//    cout<<goBoard<<endl;
//    cout<<"In connect_count: "<<connect_count<<endl;
//    cout<<"begin-end Index: "<<begin_index<<':'<<end_index<<endl;
//    cout<<"Block begin-end Index: "<<block_begin<<':'<<block_end<<endl;

    if (connect_count<5)
    {
        if (block_end-block_begin==6)
        {
            //���պ��������
            //ɨ��������п������
            for(int n=block_begin+1; n!=block_end; ++n)
            {
                if (goBoard.isEmpty(i+n*offset_i, j+n*offset_j))
                    emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
            }
        }
        else
        if (block_end-block_begin>6)
        {
            if(end_index-begin_index==4) //�����β�պ�5 ����begin_index��end_index�м��ҿ�
            {


                for(int n=begin_index+1; n!=end_index; ++n)
                {
                    if (goBoard.isEmpty(i+n*offset_i, j+n*offset_j))
                        emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
                }

                //������overline�����⡣����
                if (begin_index==-4)
                {
                    for (int n=1; n<=5-connect_count; ++n)
                    {
                        emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
                    }
                }
                else if (end_index==4)
                {
                    for (int n=1; n<=5-connect_count; ++n)
                    {
                        emptyPointSet.push_back(make_pair(i-n*offset_i, j-n*offset_j));
                    }
                }


            }
            else if (end_index-begin_index<4) //����ӳ�С����
            {
                //��������������� ������ŵ��忪ʼɨ���
                if (begin_index-block_begin==1)
                {
                    for(int n=1; n!=5; ++n)
                    {
                        if (goBoard.isEmpty(i+(begin_index+n)*offset_i, j+(begin_index+n)*offset_j))
                            emptyPointSet.push_back(make_pair(i+(begin_index+n)*offset_i, j+(begin_index+n)*offset_j));
                    }
                }
                else if (block_end-end_index==1)
                {
                    for(int n=1; n!=5; ++n)
                    {
                        if (goBoard.isEmpty(i+(end_index-n)*offset_i, j+(end_index-n)*offset_j))
                            emptyPointSet.push_back(make_pair(i+(end_index-n)*offset_i, j+(end_index-n)*offset_j));
                    }
                }
                else
                {
                    if (end_index-begin_index+1!=connect_count) //�������ȫ��
                    {
                        if (end_index-begin_index==2)
                        {
                            if (goBoard.isEmpty(i+(begin_index-1)*offset_i, j+(begin_index-1)*offset_j))
                                emptyPointSet.push_back(make_pair(i+(begin_index-1)*offset_i, j+(begin_index-1)*offset_j));
                            if (goBoard.isEmpty(i+(begin_index+1)*offset_i, j+(begin_index+1)*offset_j))
                                emptyPointSet.push_back(make_pair(i+(begin_index+1)*offset_i, j+(begin_index+1)*offset_j));
                            if (goBoard.isEmpty(i+(end_index+1)*offset_i, j+(end_index+1)*offset_j))
                                emptyPointSet.push_back(make_pair(i+(end_index+1)*offset_i, j+(end_index+1)*offset_j));
                        }
                        else if (end_index-begin_index==3)
                        {
                            if (goBoard.isEmpty(i+(begin_index+1)*offset_i, j+(begin_index+1)*offset_j))
                                emptyPointSet.push_back(make_pair(i+(begin_index+1)*offset_i, j+(begin_index+1)*offset_j));
                            if (goBoard.isEmpty(i+(end_index-1)*offset_i, j+(end_index-1)*offset_j))
                                emptyPointSet.push_back(make_pair(i+(end_index-1)*offset_i, j+(end_index-1)*offset_j));
                        }
                    }
                    else //����� X XX XXX XXXX����ȫ����
                    {
                        //��������ɨ
                        if (connect_count==1)
                            for(int n=1;n!=4;++n)
                            {
                                if (goBoard.isEmpty(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j))
                                    emptyPointSet.push_back(make_pair(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j));
                                if (goBoard.isEmpty(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j))
                                    emptyPointSet.push_back(make_pair(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j));
                            }
                        else if (connect_count>1&&connect_count<4)
                            for(int n=1;n!=3;++n)
                            {
                                if (goBoard.isEmpty(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j))
                                    emptyPointSet.push_back(make_pair(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j));
                                if (goBoard.isEmpty(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j))
                                    emptyPointSet.push_back(make_pair(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j));
                            }
                        else if (connect_count==4)
                        {
                            if (goBoard.isEmpty(i+(begin_index-1)*offset_i, j+(begin_index-1)*offset_j))
                                    emptyPointSet.push_back(make_pair(i+(begin_index-1)*offset_i, j+(begin_index-1)*offset_j));
                            if (goBoard.isEmpty(i+(end_index+1)*offset_i, j+(end_index+1)*offset_j))
                                emptyPointSet.push_back(make_pair(i+(end_index+1)*offset_i, j+(end_index+1)*offset_j));
                        }
                    }
                }
            }
            else //������
            {
                //������ ò�� ȫɨ
                for(int n=block_begin+1; n!=block_end; ++n)
                {
                    if (goBoard.isEmpty(i+n*offset_i, j+n*offset_j))
                        emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
                }
            }
        }
    }


    return make_pair(connect_count, emptyPointSet);
}

//checkLineState ��ĳ������scanLine����״̬�ж�:��һ/��һ/�߶�/���/����/����/����/����/��/����
//���� ����connect_count>5
//�� ����connect_count==5 ����connect_count>5
//���� ����ɳ�����connect_count==4
//���� һ��ɳ���(connect_count<=4)
//���� �е�ɳɻ���(�е�ɳ�����Ϊ�����O.OOO..)(connect_count<=3)
//���� �е�ɳ�����(�е�ɳɻ�����Ϊ��������O.XXX..)
//��� �е�ɳɻ��� (connect_count<=2)
//�߶� �е�ɳ�����(�е�ɳɻ�����Ϊ���) (connect_count<=2)
//��һ �е�ɳɻ�� (connect_count==1)
//��һ �е�ɳ��߶� (connect_count==1)
//δ���ÿռ任ʱ��
pair< int, vector<Action> > GoBang::checkLineState(const int &i, const int &j, const char &direct, Board goBoard, const bool &forbid_move, int depth) //��ΧӦ��������i,j���������
{
    int state=0;
    vector<Action> keyPointSet;

    if (!goBoard.isPiece(i, j))
        return make_pair(state, keyPointSet);



//    cout<<"check1"<<endl;//debug
    pair< int, vector<Action> > data=scanLine(i, j, direct, goBoard);
//    cout<<"check2"<<endl;//debug
    int connect_count=data.first;
    vector<Action> emptyPointSet(data.second);
//    int state=0;
    int try_state=0;
    int try_BestState=0;
    int try_i=0;
    int try_j=0;

//    cout<<"line-count:"<<connect_count<<endl; //debug
//    cout<<"EmptyPointSize: "<<emptyPointSet.size()<<endl; //debug
//    cout<<"depth:"<<depth<<endl; //debug

    if (connect_count>5)
    {
        if (forbid_move&&goBoard.isBlack(i, j))
        {
                state=OVERLINE;
        }
        else
            state=FIVE;
    }
    else if (connect_count==5)
    {
        state=FIVE;
    }
    else //connect_count<4
    {
        if (depth==0)
            return make_pair(0, keyPointSet);
        if (depth<0)
        {
//            cout<<"check"<<endl; //debug
            depth=5-connect_count;
        }
        else
            depth--;

//        char ch; //debug
        for(vector<Action>::iterator iter=emptyPointSet.begin(); iter!=emptyPointSet.end(); ++iter)
        {
            try_i=iter->first;
            try_j=iter->second;

            //����������
            goBoard.place(goBoard.at(i, j), try_i, try_j); //��ʱ���޸����� ���ᷢ��ʲô

//            cout<<"check1"<<endl;//debug

//            cout<<"try_pos: "<<try_i<<':'<<try_j<<endl;//debug
//            cout<<goBoard<<endl;//debug


            //����ط����иĽ��Ŀռ� ����  X..X->XX.X->XXXX  X..X->X.XX->XXXX  X..X��Ҫ����λ�õ���XXXX  ��Խ�ٳ��ֵ��ظ�Խ��
            try_state=checkLineState(i, j, direct, goBoard, forbid_move, depth).first; //��switch�������ж�? �������ж���������?
            //����ǽ��ֵ�������

//            cout<<"depth:"<<depth<<" try: "<<try_i<<':'<<try_j<<" state: "<<try_state<<endl;//debug



            if (forbid_move&&goBoard.isBlack(i,j))
                                                                        //�ǽ�������㲻ͣ�ر�̽��
                if (try_state==OVERLINE||try_state==DOUBLE_FOUR||(try_state!=FIVE&&isForbidMove(try_i, try_j, goBoard))
                    ||(try_state==FIVE&&isForbidMove(try_i, try_j, goBoard)==OVERLINE)) //Ҫ��bug �жϽ���ֻ��������� �����жϽ��ֽ��� ����2 ����1 ����0 ����Ҳ�����
                {
//                    cout<<"try: "<<try_i<<try_j<<endl;
//                    cout<<goBoard<<endl; //debug
                    goBoard.remove(try_i, try_j);


                    continue;  //��break�Ĺ���
                }

            goBoard.remove(try_i, try_j);


//            cin>>ch; //debug


            if (try_state>try_BestState)
            {
                try_BestState=try_state;
                keyPointSet.clear();
                keyPointSet.push_back(*iter);  //���ֵ��Ƿ�Ҫ�ռ�?
            }
            else if (try_state==try_BestState&&try_state!=0)
            {
                keyPointSet.push_back(*iter);
            }

        }

//        cout<<try_BestState<<' '<<connect_count<<endl;//debug

        switch(try_BestState)
        {
        case FIVE:
            {
                if(keyPointSet.size()==2) //ע��ƽ���������Ľ���
                {
                    if (connect_count==4)
                        state=OPEN_FOUR;
                    else
                        state=DOUBLE_FOUR;
                }
                else
                    state=SLEEP_FOUR;
            }
            break;
        case OPEN_FOUR:
            {
                state=OPEN_THREE;
            }
            break;
        case SLEEP_FOUR:
            {
                state=SLEEP_THREE;
            }
            break;
        case OPEN_THREE:    //Ŀǰ��û���ǽ��� ���ڻ����⿼�ǽ��ּ��� ����ط���Ҫ˼��
            {
                state=OPEN_TWO;
            }
            break;
        case SLEEP_THREE:
            {
                state=SLEEP_TWO;
            }
            break;
        case OPEN_TWO:
            {
                state=OPEN_ONE;
            }
            break;
        case SLEEP_TWO:
            {
                state=SLEEP_ONE;
            }
            break;
        default: state=0;
            break;
        }
    }

    return make_pair(state, keyPointSet);
}

//���������������Ӹ���
pair<int, pair<int, int> > GoBang::countLine(const int &i, const int &j, const char &direct, const Board &goBoard)
{
    vector<Action> emptyPointSet;
    if (!goBoard.isPiece(i, j)) //�����������
        return make_pair(0, make_pair(0,0));

    int connect_count=1;
    int oppo_connect_count=1;
    int shortLine_count=1;
    int line_count=1;
    int oppo_line_count=1;
    int offset_i=0;
    int offset_j=0;
    bool countinue_flag;

    switch(direct)
    {
    case '-':
        {
            offset_i=0;
            offset_j=1;
        }
        break;
    case '|':
        {
            offset_i=1;
            offset_j=0;
        }
        break;
    case '/':
        {
            offset_i=1;
            offset_j=1;
        }
        break;
    case '\\':
        {
            offset_i=1;
            offset_j=-1;
        }
        break;
    default: //��
        {
            offset_i=0;
            offset_j=1;
        }
        break;
    }


    int piece=0;
    countinue_flag=true;
    //˫��ɨ��
    for(int n=1; n<5; ++n)
    {
        piece=goBoard.at(i+n*offset_i, j+n*offset_j);

        if (piece==goBoard.at(i, j))
        {
                line_count++;
                if (n<3)
                    shortLine_count++;
                if (countinue_flag)
                    connect_count++;
        }
        else

        {
            countinue_flag=false;
            if (piece!=EMPTY_POINT)
            {
                break;
            }
        }


    }
    if (connect_count==5&&goBoard.at(i+5*offset_i, j+5*offset_j)==goBoard.at(i, j))
        connect_count++;

    countinue_flag=true;
    for(int n=1; n<5; ++n)
    {

        piece=goBoard.at(i-n*offset_i, j-n*offset_j);

        if (piece==goBoard.at(i, j))
        {
            oppo_line_count++;
            if (n<3)
                    shortLine_count++;
            if (countinue_flag)
                    oppo_connect_count++;
        }
        else
        {
            countinue_flag=false;
            if (piece!=EMPTY_POINT)
            {
                break;
            }
        }

    }
    if (oppo_connect_count==5&&goBoard.at(i-5*offset_i, j-5*offset_j)==goBoard.at(i, j))
        oppo_connect_count++;

    line_count=line_count+oppo_line_count-1;
    connect_count=connect_count+oppo_connect_count-1;
    return make_pair(line_count, make_pair(shortLine_count, connect_count));
}

int GoBang::isForbidMove(const int &i, const int &j, Board &goBoard) //bool
{
    // ���ڵ�����������checkLineState������ ��ʱA���Ѳ����ܳ�Ϊ���� �亯����Ȼ������A�����������Ƿ�Ϊ���� ���ºľ�
    if (!goBoard.isBlack(i, j))
        return 0;

//    int state=0;
//    vector<Action> keyPointSet; //��ʱ����������

//    if (!goBoard.isPiece(i, j))
//        return make_pair(state, keyPointSet);

    pair< int, vector<Action> > direct_state;
    multimap< int, vector<Action> > directStateSet;
    set<char> OT_DirectSet;
    set<char> F_DirectSet;
//    set<char> DF_DirectSet;
    int connect_count=0;
    int line_count=0;
    int shortLine_count=0;

    pair<int, pair<int, int> > pack;

    for(int n=0; n!=4; ++n)
    {
        //ʹ�ü����̾��������ľ�����
        pack=countLine(i, j, direct_arr[n], goBoard);
        line_count=pack.first;
        shortLine_count=pack.second.first;
        connect_count=pack.second.second;

        if (line_count>2&&shortLine_count>1) //���ڵ��������п����н��� �����������ڵ��ڶ����п��� to be done
        {
            if (connect_count>5)
            {
//                cout<<connect_count<<':'<<line_count;
                return OVERLINE;
            }

            direct_state=checkLineState(i, j, direct_arr[n], goBoard, false, 2);  //��û��ӹؼ��� //debug Ҫ��Ӵ
            if (direct_state.first==OPEN_THREE)
                OT_DirectSet.insert(direct_arr[n]);

            if (direct_state.first==SLEEP_FOUR||direct_state.first==OPEN_FOUR||direct_state.first==DOUBLE_FOUR)
                F_DirectSet.insert(direct_arr[n]);

            directStateSet.insert(direct_state);
        }

    }

//    if (directStateSet.count(OVERLINE))
//    {
//        //����
//        return OVERLINE;
//    }
//    else
    if(directStateSet.count(FIVE))
    {
        //����
        return 0;
    }
    else if (directStateSet.count(DOUBLE_FOUR)||directStateSet.count(OPEN_FOUR)+directStateSet.count(SLEEP_FOUR)>1 )
    {
         for(set<char>::iterator iter=F_DirectSet.begin(); iter!=F_DirectSet.end(); ++iter)
        {
            direct_state=checkLineState(i, j, *iter, goBoard, true, 1);
            if (direct_state.first==DOUBLE_FOUR)   //checkLineState�ǲ���������Ľ��ֵ�
            {
                return F_DOUBLE_FOUR;
            }
            else
            if (direct_state.first!=OPEN_FOUR&&direct_state.first!=SLEEP_FOUR)
                F_DirectSet.erase(iter);


        }
        if (F_DirectSet.size()>1)
            return F_DOUBLE_FOUR;
    }

    else
    if (directStateSet.count(OPEN_THREE)>1)
    {
        for(set<char>::iterator iter=OT_DirectSet.begin(); iter!=OT_DirectSet.end(); ++iter)
        {
            direct_state=checkLineState(i, j, *iter, goBoard, true, 2);
            if (direct_state.first!=OPEN_THREE)
                OT_DirectSet.erase(iter);
        }
        if (OT_DirectSet.size()>1)
            return F_DOUBLE_THREE;

    }

    return 0;
}

pair< int, vector<Action> > GoBang::checkState(const int &i, const int &j, Board &goBoard, const bool &forbid_move)
{

    int state=0;
    vector<Action> keyPointSet; //��ʱ����������

    if (!goBoard.isPiece(i, j))
        return make_pair(state, keyPointSet);

    pair< int, vector<Action> > direct_state;
    multimap< int, vector<Action> > directStateSet;


    for(int n=0; n!=4; ++n)
    {
//        cout<<"check: "<<i<<':'<<j<<" Direct: "<<direct_arr[n]<<endl;//debug

        direct_state=checkLineState(i, j, direct_arr[n], goBoard, forbid_move);  //��û��ӹؼ��� //debug Ҫ��Ӵ

//        cout<<"Direct: "<<direct_arr[n]<<" state: "<<direct_state.first<<endl;//debug
//        cout<<"-----------------------------------"<<endl; //debug

        if (direct_state.first>state)
        {
            state=direct_state.first;
        }
        directStateSet.insert(direct_state);
    }

    if (directStateSet.count(OVERLINE)) //��������ͬʱ���������
    {
        //����
        state=OVERLINE;
    }
    else
    if(directStateSet.count(FIVE))
    {
        //����
        state=FIVE;
    }
    else
    if( directStateSet.count(DOUBLE_FOUR)||directStateSet.count(OPEN_FOUR)+directStateSet.count(SLEEP_FOUR)>1 )
    {
        //���� ����԰������ĺͺ���������������
        if (forbid_move&&goBoard.isBlack(i, j))
            state=F_DOUBLE_FOUR;
        else
            state=DOUBLE_FOUR;
    }
    else
    if (directStateSet.count(OPEN_THREE)==1&&directStateSet.count(OPEN_FOUR)+directStateSet.count(SLEEP_FOUR)==1)
    {
        //����
        state=FOUR_THREE;
    }
    else
    if (directStateSet.count(OPEN_THREE)>1)
    {   //����
        if (forbid_move&&goBoard.isBlack(i, j))
            state=F_DOUBLE_THREE;
        else
        if (directStateSet.count(OPEN_FOUR)+directStateSet.count(SLEEP_FOUR)==1) //������ �н���ʱΪ�������� �޽���ʱΪ����
        {
            state=FOUR_THREE;
        }
        else
            state=DOUBLE_THREE;
    }
    else
    {

    }

    return make_pair(state, keyPointSet);
}
