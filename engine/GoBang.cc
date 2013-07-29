#include "GoBang.h"

int GoBang::move(const int &i, const int &j)
{
    return move(curr_player, i, j);
}

//��ѵ��RLʱmoveֻ��Ҫ���FIVE�ͽ��ֵ�������� ���ټ�����
int GoBang::move(const int &piece, const int &i, const int &j, const bool &clear_undoStack)
{
    if (winner!=0||!board.place(piece, i, j)) //����ʧ�ܻ�������
        return winner;
    //���ӳɹ�
    move_count++;
    move_stack.push(make_pair(piece, make_pair(i, j)));
    if (clear_undoStack) //stackû�е����� clear�ǿ�������ʵ��
        while(!undo_stack.empty())
            undo_stack.pop();

    changePlayer();
    winner=checkWinner();  //���¼ܹ�һ�� moveӦ��ֻ�����Ӯ ������ʾ����ġ���

//    if (!clear_undoStack) //�Ѿ��Ƶ�redo��ȥ��
//        undo_stack.pop();
//    return checkState();

//    return state; //��ʱ
    return winner;

}



int GoBang::undo()
{
    if (move_stack.empty())
        return winner;
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
        return checkWinner();
    }
}
int GoBang::redo()
{
    if (undo_stack.empty())
        return winner;

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

pair< int, vector<Action> > GoBang::checkLineState(const int &i, const int &j, const char &direct, Board &goBoard, const bool &forbid_move)
{
    int state=0;
    vector<Action> keyPointSet;
    if (goBoard.isPiece(i, j))
    {
        pair<pair< int, vector<Action> >, pair<int, int> > pack=preScan(i, j, direct, goBoard, forbid_move);
        int pre_state=pack.first.first;
        vector<Action> preKeyPointSet=pack.first.second;


        if(pre_state==OVERLINE)//����ǳ���
        {
            if (goBoard.isWhite(i, j)||!forbid_move) //��Ϊ������߽��ֿ��عر� ������Ϊ5
                state=FIVE;
            else
                state=OVERLINE;
        }
        else
        if (pre_state==FIVE||pre_state==0) //����������ƽ��̬
        {
            state=pre_state;
        }
        else
        {
            if (goBoard.isWhite(i, j)||!forbid_move) //��Ϊ������߽��ֿ��عر� ���޽���
            {
                state=pre_state;
                keyPointSet=preKeyPointSet;
            }
                    //��Ϊ�������н����������ּ��
            else     //��������!!!..���ڻ�(�ǻ���) �ǹؼ�λ��(������)�ǽ���(����)Ҳ��Ӱ������״̬  ��������? Ӧ�ò����������� ��Ч���ֵ��ڹؼ���
            {
                int block_begin=pack.second.first>-5?pack.second.first:-5;
                int block_end=pack.second.second<5?pack.second.second:5;

                int try_i=0;
                int try_j=0;
                multimap<int, Action> FM_set;
                int FM_state=0;

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

                for(int n=block_begin+1; n!=block_end; ++n) //�ڷ�Χ��Ѱ�ҽ��ֵ� ���ֽ��ֵ��Ǽ�ʱ�� ��̬�� ����һ����Ǳ�ڵ� С��ͬ��Ľ���
                {
                    try_i=i+n*offset_i;
                    try_j=j+offset_j;

                    goBoard.place(goBoard.at(i, j), try_i, try_j); //��place��Ҫ��remove�������

                    FM_state=isForbidMove(try_i, try_j, goBoard);
                    if (FM_state<0)
                        FM_set.insert(make_pair(FM_state, make_pair(try_i, try_j)));

                    goBoard.remove(try_i, try_j);
                }

                if (FM_set.count(OVERLINE)) //����г���
                {
                    //��������ֱ�ӵ��ɶ������Ӱ�����
                    for(multimap< int, Action >::iterator iter=FM_set.lower_bound(OVERLINE);iter!=FM_set.upper_bound(OVERLINE);++iter)
                    {
                        try_i=(iter->second).first;
                        try_j=(iter->second).second;
                        goBoard.place(-goBoard.at(i, j), try_i, try_j); //�ڳ�������϶�����
                    }
                    //���¼�Ⲣ�һ�ȡ��ⷶΧ
                    pack=preScan(i, j, direct, goBoard, forbid_move);
                    pre_state=pack.first.first;
                    preKeyPointSet=pack.first.second;
                    block_begin=pack.second.first>-5?pack.second.first:-5;
                    block_end=pack.second.second<5?pack.second.second:5;

                    for(multimap< int, Action >::iterator iter=FM_set.lower_bound(OVERLINE);iter!=FM_set.upper_bound(OVERLINE);)
                    {
                        try_i=(iter->second).first;
                        try_j=(iter->second).second;
                        goBoard.remove(try_i, try_j); //�Ƴ�α�ŵ�
                        FM_set.erase(iter++); //��ȥ�Ѵ������
                    }
                }



                //��ֻ�³��� ���ǳ����Ļ����Ժ��Խ���
                if (pre_state==OPEN_FOUR||pre_state==SLEEP_FOUR) //������ִ������� ��������ֵ㼯��Ϊ��
                {
                    state=pre_state;
                    keyPointSet=preKeyPointSet;
                }
                else
                if (pre_state==DOUBLE_FOUR) //��ֻ�³��� ���ǳ����Ļ����Ժ��Խ���
                {
//                    if (goBoard.isWhite(i, j)||!forbid_move)  //��Ϊ������޽��־Ͳ����������
//                        state=pre_state;
//                    else
                    state=F_DOUBLE_FOUR;
                    keyPointSet=preKeyPointSet;
                }
                else //��������˳������ִ������н��� ͨ������˻����� ����һ���������������
                {
                    //��Ҳ�³��� ������һ���ǳ�������ֻ���Ƴ��ؼ��㼯�ϼ���? �ǻ�Ļ����� �������������� ���巨����� ���ķ���Ի�
                    //�����һ���ж�̬����
                }
            }
        }
    }





    return make_pair(state, keyPointSet);
}


int GoBang::isForbidMove(const int &i, const int &j, Board &goBoard)
{
    if (!goBoard.isBlack(i, j))
        return 0;

    pair< int, vector<Action> > direct_state;
    multimap< int, vector<Action> > directStateSet;
    vector<Action> keyPointSet;
//    map<char, vector<Action>> OT_DirectSet;
    map<char, vector<Action> > F_DirectSet;
    int connect_count=0;
    int line_count=0;
    int shortLine_count=0;
    int try_i, try_j;

    pair<int, pair<int, int> > pack;

    for(int n=0; n!=4; ++n)
    {

        direct_state=preScan(i, j, direct_arr[n], goBoard, true).first;
        if (direct_state.first==OVERLINE)
            return OVERLINE;
//        else
//        if (direct_state.first==OPEN_THREE)
//            OT_DirectSet.insert(direct_arr[n]);
//        else
        if (direct_state.first==SLEEP_FOUR||direct_state.first==OPEN_FOUR)
            F_DirectSet.insert(make_pair(direct_arr[n], direct_state.second));

        directStateSet.insert(direct_state);
    }

     if(directStateSet.count(FIVE))
    {
        //����
        return FIVE; //ע������ط� �жϽ���Ҫ��isForbidMove<0��
    }
    else if (directStateSet.count(DOUBLE_FOUR)||directStateSet.count(OPEN_FOUR)+directStateSet.count(SLEEP_FOUR)>1 )
    {

           //ֻ�г������ֵ���Իٵ��� ���õ��������� ֱ����һ���ж� Ҫȫ�����ǳ����Ų�����


            bool OL_flag=false;
            int DOUBLE_FOUR_to_FOUR=0; //֮ǰ��int���bool�ˡ���Ҫ��..


            for(multimap< int, vector<Action> >::iterator iter=directStateSet.lower_bound(DOUBLE_FOUR);iter!=directStateSet.upper_bound(DOUBLE_FOUR);++iter)
            {
                keyPointSet=iter->second;
                for(vector<Action>::iterator ac_iter=keyPointSet.begin(); ac_iter!=keyPointSet.end();)
                {
                    try_i=ac_iter->first;
                    try_j=ac_iter->second;


                    goBoard.place(goBoard.at(i, j), try_i, try_j);
                    OL_flag=false;
                    for(int n=0; n!=4; ++n) //��ʵ������һ��
                    {
                        direct_state=preScan(try_i, try_j, direct_arr[n], goBoard, true).first; //����Ƚ�ʡʱʡ�� ���������� ֻ�ж����� ���ȥ�鿴��������


                        if (direct_state.first==OVERLINE)
                        {
                            OL_flag=true;
                            break;
                        }
                    }

                    goBoard.remove(try_i, try_j);
                    if (OL_flag)                     //ע���������ʧЧ!!!
                        ac_iter=keyPointSet.erase(ac_iter);
                    else
                        ++ac_iter;

                }
                if (keyPointSet.size()==2)
                    return F_DOUBLE_FOUR;
                else
                if (keyPointSet.size()==1)
                    DOUBLE_FOUR_to_FOUR++;


            }

        if (DOUBLE_FOUR_to_FOUR>1)
            return F_DOUBLE_FOUR;


         for(map<char, vector<Action> >::iterator iter=F_DirectSet.begin(); iter!=F_DirectSet.end();)
        {
            keyPointSet=iter->second;
            for(vector<Action>::iterator ac_iter=keyPointSet.begin(); ac_iter<keyPointSet.end();)
            {
                try_i=ac_iter->first;
                try_j=ac_iter->second;
                goBoard.place(goBoard.at(i, j), try_i, try_j);
                OL_flag=false;
                for(int n=0; n!=4; ++n)
                {
                    if (direct_arr[n]!=iter->first)
                    {
                        direct_state=preScan(try_i, try_j, direct_arr[n], goBoard, true).first;
                        if (direct_state.first==OVERLINE)
                        {
                            OL_flag=true;
                            break;
                        }
                    }

                }
                goBoard.remove(try_i, try_j);

               if (OL_flag)                  //����ؼ��� ���������� ������   //ע���������ʧЧ!!!
                    ac_iter=keyPointSet.erase(ac_iter);
                else
                    ++ac_iter;


            }

            if (keyPointSet.size()==0) //������е���㶼���� �������� //ע���������ʧЧ!!!
                F_DirectSet.erase(iter++);
            else
                ++iter;
        }

        if (F_DirectSet.size()+DOUBLE_FOUR_to_FOUR>1)
            return F_DOUBLE_FOUR;


    }
    else
    if (directStateSet.count(OPEN_THREE)>1)
    {
//        bool erase_flag=false;
        int FM_state=0;
        for(multimap< int, vector<Action> >::iterator iter=directStateSet.lower_bound(OPEN_THREE);iter!=directStateSet.upper_bound(OPEN_THREE);)
        {
//            keyPointSet=iter->second;
//            erase_flag=false;

            // //ֻҪ��һ���������ֵ��ȫ�����ǽ��� ���������й� �Ͳ��ǻ��� Oh no���������й�?
            for(vector<Action>::iterator ac_iter=(iter->second).begin(); ac_iter!=(iter->second).end(); )
            {
                try_i=ac_iter->first;
                try_j=ac_iter->second;
                goBoard.place(goBoard.at(i, j), try_i, try_j);
                FM_state=isForbidMove(try_i, try_j, goBoard);
                goBoard.remove(try_i, try_j);

                if (FM_state==OVERLINE)
                    break;
                else if(FM_state<0)
                    ac_iter=(iter->second).erase(ac_iter);
                else
                    ++ac_iter;
            }

            if ((iter->second).size()==0||FM_state==OVERLINE)    //ע���������ʧЧ!!!
                directStateSet.erase(iter++);
            else
                ++iter;

        }


        if (directStateSet.count(OPEN_THREE)>1)
        {
            //������ϻ�Ҫ�ж� ���ǳ���
            //�ٹ�һ�β���
            int offset_i=0;
            int offset_j=0;
            int temp=0;
            char direct;
            bool OL_flag=false;

            for(multimap< int, vector<Action> >::iterator iter=directStateSet.lower_bound(OPEN_THREE);iter!=directStateSet.upper_bound(OPEN_THREE);)
            {
                keyPointSet=iter->second;
                OL_flag=false;

                for(vector<Action>::iterator ac_iter=keyPointSet.begin(); ac_iter!=keyPointSet.end();)
                {
                    offset_i=ac_iter->first-i;
                    offset_j=ac_iter->second-j;
                    temp=offset_i*offset_j;

                    if (temp==0)
                    {
                        if (offset_i==0)
                        {
                            offset_j=1;
                            direct='-';
                        }
                        else
                        {
                            offset_i=1;
                            direct='|';
                        }
                    }
                    else
                    if (temp<0)
                    {
                        offset_i=1;
                        offset_j=-1;
                        direct='\\';
                    }
                    else
                    {
                        offset_i=1;
                        offset_j=1;
                        direct='/';
                    }



                    for(try_i=ac_iter->first+offset_i,try_j=ac_iter->second+offset_j; !goBoard.isEmpty(try_i, try_j); try_i+=offset_i, try_j+=offset_j)
                    {

                    }

                    goBoard.place(goBoard.at(i, j), try_i, try_j);
                    for(int n=0; n!=4; ++n)
                    {
                        if (direct_arr[n]!=direct)
                        {
                            FM_state=preScan(try_i, try_j, direct_arr[n], goBoard, true).first.first;
                            if (FM_state==OVERLINE)
                            {
                                OL_flag=true;
                                break;
                            }
                        }

                    }
                    goBoard.remove(try_i, try_j);

                    if (OL_flag)
                    {
                        ac_iter=keyPointSet.erase(ac_iter);
                        continue;
                    }
                    else
                    {
                        for(try_i=ac_iter->first-offset_i,try_j=ac_iter->second-offset_j; !goBoard.isEmpty(try_i, try_j); try_i-=offset_i, try_j-=offset_j)
                        {

                        }

                        goBoard.place(goBoard.at(i, j), try_i, try_j);

                        for(int n=0; n!=4; ++n)
                        {
                            if (direct_arr[n]!=direct)
                            {
                                FM_state=preScan(try_i, try_j, direct_arr[n], goBoard, true).first.first;
                                if (FM_state==OVERLINE)
                                {
                                    OL_flag=true;
                                    break;
                                }
                            }

                        }
                        goBoard.remove(try_i, try_j);

                        if (OL_flag)
                        {
                            ac_iter=keyPointSet.erase(ac_iter);
                            continue;
                        }
                        else
                            ++ac_iter;


                   }


                }


                if (keyPointSet.size()==0)
                    directStateSet.erase(iter++);
                else
                    ++iter;


            }

            if (directStateSet.count(OPEN_THREE)>1)
                return F_DOUBLE_THREE;
        }

    }

    return 0;
}

//Ԥɨ�� static
pair<pair< int, vector<Action> >, pair<int, int> > GoBang::preScan(const int &i, const int &j, const char &direct, const Board &goBoard, const bool &forbid_move)
{
    vector<Action> keyPointSet;
    int block_begin=-6;
    int block_end=6;

    if (!goBoard.isPiece(i, j)) //�����������
        return make_pair(make_pair(0, keyPointSet), make_pair(block_begin, block_end));

    int connect_count=1;
    int oppo_connect_count=1;
    int offset_i=0;
    int offset_j=0;
    vector<signed char> model(11,0); //11λ�õ�����
    model[5]=goBoard.at(i, j);
    vector<signed char> keyPosSet;
    int state=0;

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
    //˫��ɨ��
    bool countinue_flag=true;
    for(int n=1; n<6; ++n)
    {
        piece=goBoard.at(i+n*offset_i, j+n*offset_j);

        if (piece==ERROR_CODE||piece==-goBoard.at(i, j))
        {
            model[5+n]=-goBoard.at(i, j);
            block_end=n;
                break;
        }
        model[5+n]=piece;

        if (piece==goBoard.at(i, j))
        {
            if (countinue_flag)
                connect_count++;
        }
        else
        {
            countinue_flag=false;
        }
    }

    countinue_flag=true;
    for(int n=-1; n>-6; --n)
    {

        piece=goBoard.at(i+n*offset_i, j+n*offset_j);

        if (piece==ERROR_CODE||piece==-goBoard.at(i, j))
        {
            model[5+n]=-goBoard.at(i, j);
            block_begin=n;
            break;
        }
        model[5+n]=piece;

        if (piece==goBoard.at(i, j))
        {
            if (countinue_flag)
                oppo_connect_count++;
        }
        else
        {
            countinue_flag=false;
        }
    }

    connect_count=connect_count+oppo_connect_count-1;

    if (connect_count==5)
    {
        state=FIVE;
    }
    else
        if (connect_count>5)
    {
        if (goBoard.isWhite(i, j))
            state=FIVE;
        else
            state=OVERLINE;
    }
    else
        if (block_end-block_begin<6)
    {
        state=0;
    }
    else
    {
        if (model[5]==BLACK_STONE&&!forbid_move) //����Ǻ��岢���޽��� ��Ҫ�Ѻ������͵�������������
            for(vector<signed char>::iterator iter=model.begin(); iter!=model.end(); ++iter)
            {
                *iter=-*iter;
            }
        if (model_map.count(model))
        {
            state=model_map[model].first;
            keyPosSet=model_map[model].second;
            for(vector<signed char>::iterator iter=keyPosSet.begin(); iter!=keyPosSet.end(); ++iter)
            {
                keyPointSet.push_back(make_pair(i+(*iter)*offset_i, j+(*iter)*offset_j));
            }
        }
        else
        {
            for(vector<signed char>::iterator iter=model.begin(); iter!=model.end(); ++iter)
            {
                cout<<*iter;
            }
            cout<<endl;
            cout<<"ERROR No Result in Model Map"<<endl;
        }
    }

    return make_pair(make_pair(state, keyPointSet), make_pair(block_begin, block_end));
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

    if (connect_count<5) //�������岻��Ҫ�����ˡ���
    {
//
//                for(int n=block_begin+1; n!=block_end; ++n)
//                {
//                    if (goBoard.isEmpty(i+n*offset_i, j+n*offset_j))
//                        emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
//                }

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

                if (end_index-begin_index+1!=connect_count) //�������ȫ��
                {
                    for(int n=block_begin+1; n!=block_end; ++n)
                    {
                        if (goBoard.isEmpty(i+n*offset_i, j+n*offset_j))
                            emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
                    }
                }
                else //���ȫ�� //Ҳ������checkLineStateWithKeyPoints��������
                {
                            for(int n=1;n!=4;++n)
                            {
//                                cout<<".."<<endl;
                                if (begin_index-n>block_begin&&goBoard.isEmpty(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j))
                                    emptyPointSet.push_back(make_pair(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j));
                                if (end_index+n<block_end&&goBoard.isEmpty(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j))
                                    emptyPointSet.push_back(make_pair(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j));
                            }
                }
//            if(end_index-begin_index==4) //�����β�պ�5 ����begin_index��end_index�м��ҿ�
//            {
//
//
//                for(int n=begin_index+1; n!=end_index; ++n)
//                {
//                    if (goBoard.isEmpty(i+n*offset_i, j+n*offset_j))
//                        emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
//                }
//
//                //������overline�����⡣����
//                if (begin_index==-4)
//                {
//                    for (int n=1; n<=5-connect_count; ++n)
//                    {
//                        if (n>=block_end)
//                            break;
//                        emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
//                    }
//                }
//                else if (end_index==4)
//                {
//                    for (int n=1; n<=5-connect_count; ++n)
//                    {
//                        if (-n<=block_begin)
//                            break;
//                        emptyPointSet.push_back(make_pair(i-n*offset_i, j-n*offset_j));
//                    }
//                }
//
//
//            }
//            else if (end_index-begin_index<4) //����ӳ�С����
//            {
                //��������������� ������ŵ��忪ʼɨ���
//                if (begin_index-block_begin==1)
//                {
//                    for(int n=1; n!=5; ++n)
//                    {
//                        if (goBoard.isEmpty(i+(begin_index+n)*offset_i, j+(begin_index+n)*offset_j))
//                            emptyPointSet.push_back(make_pair(i+(begin_index+n)*offset_i, j+(begin_index+n)*offset_j));
//                    }
//                }
//                else
//                {
//                    int a=end_index-4>block_begin?end_index-4:block_begin;
//                    int b=begin_index+4<block_end?begin_index+4:block_end;
//                    for(int n=a; n<=b; ++n)
//                    {
//                        if (goBoard.isEmpty(i+n*offset_i, j+n*offset_j))
//                            emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
//                    }
//
//                }

//                else if (block_end-end_index==1)
//                {
//                    for(int n=1; n!=5; ++n)
//                    {
//                        if (goBoard.isEmpty(i+(end_index-n)*offset_i, j+(end_index-n)*offset_j))
//                            emptyPointSet.push_back(make_pair(i+(end_index-n)*offset_i, j+(end_index-n)*offset_j));
//                    }
//                }
//                else
//                {
//                    if (end_index-begin_index+1!=connect_count) //�������ȫ��
//                    {
//                        if (end_index-begin_index==2)
//                        {
//                            if (goBoard.isEmpty(i+(begin_index-1)*offset_i, j+(begin_index-1)*offset_j))
//                                emptyPointSet.push_back(make_pair(i+(begin_index-1)*offset_i, j+(begin_index-1)*offset_j));
//                            if (goBoard.isEmpty(i+(begin_index+1)*offset_i, j+(begin_index+1)*offset_j))
//                                emptyPointSet.push_back(make_pair(i+(begin_index+1)*offset_i, j+(begin_index+1)*offset_j));
//                            if (goBoard.isEmpty(i+(end_index+1)*offset_i, j+(end_index+1)*offset_j))
//                                emptyPointSet.push_back(make_pair(i+(end_index+1)*offset_i, j+(end_index+1)*offset_j));
//                        }
//                        else if (end_index-begin_index==3)
//                        {
//                            if (goBoard.isEmpty(i+(begin_index+1)*offset_i, j+(begin_index+1)*offset_j))
//                                emptyPointSet.push_back(make_pair(i+(begin_index+1)*offset_i, j+(begin_index+1)*offset_j));
//                            if (goBoard.isEmpty(i+(end_index-1)*offset_i, j+(end_index-1)*offset_j))
//                                emptyPointSet.push_back(make_pair(i+(end_index-1)*offset_i, j+(end_index-1)*offset_j));
//                        }
//                    }
//                    else //����� X XX XXX XXXX����ȫ����
//                    {
//                        //��������ɨ
//                        if (connect_count==1)
//                            for(int n=1;n!=4;++n)
//                            {
////                                cout<<".."<<endl;
//                                if (begin_index-n>block_begin&&goBoard.isEmpty(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j))
//                                    emptyPointSet.push_back(make_pair(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j));
//                                if (end_index+n<block_end&&goBoard.isEmpty(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j))
//                                    emptyPointSet.push_back(make_pair(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j));
//                            }
//                        else if (connect_count>1&&connect_count<4)
//                            for(int n=1;n!=3;++n)
//                            {
//                                if (goBoard.isEmpty(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j))
//                                    emptyPointSet.push_back(make_pair(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j));
//                                if (goBoard.isEmpty(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j))
//                                    emptyPointSet.push_back(make_pair(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j));
//                            }
//                        else if (connect_count==4)
//                        {
//                            if (goBoard.isEmpty(i+(begin_index-1)*offset_i, j+(begin_index-1)*offset_j))
//                                    emptyPointSet.push_back(make_pair(i+(begin_index-1)*offset_i, j+(begin_index-1)*offset_j));
//                            if (goBoard.isEmpty(i+(end_index+1)*offset_i, j+(end_index+1)*offset_j))
//                                emptyPointSet.push_back(make_pair(i+(end_index+1)*offset_i, j+(end_index+1)*offset_j));
//                        }
//                    }
//                }
//            }
//            else //������
//            {
//                //������ ò�� ȫɨ
//                for(int n=block_begin+1; n!=block_end; ++n)
//                {
//                    if (goBoard.isEmpty(i+n*offset_i, j+n*offset_j))
//                        emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
//                }
//            }
        }
    }


    return make_pair(connect_count, emptyPointSet);
}

//checkLineStateWithKeyPoints ��ĳ������scanLine����״̬�ж�:��һ/��һ/�߶�/���/����/����/����/����/��/����
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
pair< int, vector<Action> > GoBang::checkLineStateWithKeyPoints(const int &i, const int &j, const char &direct, Board &goBoard, const bool &forbid_move, int depth) //��ΧӦ��������i,j���������
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

        char ch; //debug
        for(vector<Action>::iterator iter=emptyPointSet.begin(); iter!=emptyPointSet.end(); ++iter)
        {
            try_i=iter->first;
            try_j=iter->second;
//
//            if (try_i==8&&try_j==9)
//            {
//                cout<<goBoard<<endl;//debug
//                cout<<i<<':'<<j<<endl;
//                cin>>ch;
//            }

            //����������
            goBoard.place(goBoard.at(i, j), try_i, try_j); //����ط��������ֹ��������� ������Ҫ���Board ��Ҫ��֤һ��place�Ƿ�ɹ� ��������

//            cout<<"check1"<<endl;//debug

//            cout<<"try_pos: "<<try_i<<':'<<try_j<<endl;//debug
//            cout<<goBoard<<endl;//debug


            //����ط����иĽ��Ŀռ� ����  X..X->XX.X->XXXX  X..X->X.XX->XXXX  X..X��Ҫ����λ�õ���XXXX  ��Խ�ٳ��ֵ��ظ�Խ��
            try_state=checkLineStateWithKeyPoints(i, j, direct, goBoard, forbid_move, depth).first; //��switch�������ж�? �������ж���������?
            //����ǽ��ֵ�������
//
//            cout<<"depth:"<<depth<<" try: "<<try_i<<':'<<try_j<<" state: "<<try_state<<endl;//debug
//            cout<<"i: "<<i<<" j: "<<j<<endl;


            if (forbid_move&&goBoard.isBlack(i,j))
                                                                        //�ǽ�������㲻ͣ�ر�̽��
                if (try_state==OVERLINE||try_state==DOUBLE_FOUR||(try_state!=FIVE&&isForbidMove_Old(try_i, try_j, goBoard))
                    ||(try_state==FIVE&&isForbidMove_Old(try_i, try_j, goBoard)==OVERLINE)) //Ҫ��bug �жϽ���ֻ��������� �����жϽ��ֽ��� ����2 ����1 ����0 ����Ҳ�����
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
        return make_pair(0, make_pair(0, 0));

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

int GoBang::isForbidMove_Old(const int &i, const int &j, Board &goBoard) //bool
{
    // ���ڵ�����������checkLineStateWithKeyPoints������ ��ʱA���Ѳ����ܳ�Ϊ���� �亯����Ȼ������A�����������Ƿ�Ϊ���� ���ºľ�
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
//            cout<<"FM cheking..."<<endl;
            direct_state=checkLineStateWithKeyPoints(i, j, direct_arr[n], goBoard, false, 2);  //��û��ӹؼ��� //debug Ҫ��Ӵ
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
            direct_state=checkLineStateWithKeyPoints(i, j, *iter, goBoard, true, 1);
            if (direct_state.first==DOUBLE_FOUR)   //checkLineStateWithKeyPoints�ǲ���������Ľ��ֵ�
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
            direct_state=checkLineStateWithKeyPoints(i, j, *iter, goBoard, true, 2);
            if (direct_state.first!=OPEN_THREE)
                OT_DirectSet.erase(iter);
        }
        if (OT_DirectSet.size()>1)
            return F_DOUBLE_THREE;

    }

    return 0;
}

int GoBang::checkWinner(const int &i, const int &j, Board &goBoard, const bool &forbid_move)
{
    if (!goBoard.isPiece(i, j))
        return 0;

    int state;
    int FM_state=0;
    if (forbid_move&&goBoard.isBlack(i, j))
    {
        FM_state=isForbidMove(i, j, goBoard);
        if (FM_state<0)
            return WHITE_STONE;
        else if (FM_state==FIVE)
            return BLACK_STONE;
    }
    else
    {
        for(int n=0; n!=4; ++n)
        {
            //������иĽ��ռ� ���ж��Ƿ����
            state=preScan(i, j, direct_arr[n], goBoard, forbid_move).first.first;
            if (state==FIVE)
                return goBoard.at(i, j);

        }
    }

    if (goBoard.isFull())
        return TIE;

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

        direct_state=checkLineStateWithKeyPoints(i, j, direct_arr[n], goBoard, forbid_move);  //��û��ӹؼ��� //debug Ҫ��Ӵ

//        cout<<"Direct: "<<direct_arr[n]<<" state: "<<direct_state.first<<endl;//debug
//        cout<<"-----------------------------------"<<endl; //debug

        if (direct_state.first>state)
        {
            state=direct_state.first;
            keyPointSet=direct_state.second;
        }
        directStateSet.insert(direct_state);
    }

    if (directStateSet.count(OVERLINE)) //��������ͬʱ���������
    {
        //����
        state=OVERLINE;
        keyPointSet.clear();
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
        {
            state=F_DOUBLE_FOUR;
            keyPointSet.clear();
        }

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
        {
            state=F_DOUBLE_THREE;
            keyPointSet.clear();
        }
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
