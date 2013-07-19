#include "GoBang.h"

char direct_arr[4]={'-', '|', '/', '\\'};

int GoBang::move(const int &i, const int &j)
{
    return move(curr_player, i, j);
}
int GoBang::move(const int &piece, const int &i, const int &j, const bool &clear_undoStack)
{
    if (winner!=0||!board.place(piece, i, j)) //落子失败或者有了
        return ERROR_CODE;
    //落子成功
    move_count++;
    move_stack.push(make_pair(piece, make_pair(i, j)));
    if (clear_undoStack) //stack没有迭代器 clear是靠迭代器实现
        while(!undo_stack.empty())
            undo_stack.pop();

    int state=checkState();
    changePlayer();

//    if (!clear_undoStack)
//        undo_stack.pop();
//    return checkState();
    return state; //暂时

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
        changePlayer(); //这个地方需要好好考虑考虑 先暂时不考虑太多
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
    return move(piece, i, j, false); //由redo调用的move不要清除undo_stack
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

//对某方向(横竖撇捺) 返回连接点集合和连珠长度
pair< int, vector<Action> > GoBang::scanLine(const int &i, const int &j, const char &direct, const Board &goBoard)
{
    vector<Action> emptyPointSet;
    if (!goBoard.isPiece(i, j)) //如果不是棋子
        return make_pair(0, emptyPointSet);

    int line_count=1;
    int oppo_line_count=1;
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
    default: //横
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
    //双向扫描
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
                line_count++;
        }
        else
        {
            countinue_flag=false;
        }



    }
    if (line_count==5&&goBoard.at(i+5*offset_i, j+5*offset_j)==goBoard.at(i, j))
        line_count++;

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
                oppo_line_count++;
        }
        else
        {
            countinue_flag=false;
        }
    }

    if (oppo_line_count==5&&goBoard.at(i-5*offset_i, j-5*offset_j)==goBoard.at(i, j))
        oppo_line_count++;

    line_count=line_count+oppo_line_count-1;

//    cout<<goBoard<<endl;
//    cout<<"In line_count: "<<line_count<<endl;
//    cout<<"begin-end Index: "<<begin_index<<':'<<end_index<<endl;
//    cout<<"Block begin-end Index: "<<block_begin<<':'<<block_end<<endl;

    if (line_count<5)
    {
        if (block_end-block_begin==6)
        {
            //即刚好留出五格
            //扫描这五格有空则添加
            for(int n=block_begin+1; n!=block_end; ++n)
            {
                if (goBoard.isEmpty(i+n*offset_i, j+n*offset_j))
                    emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
            }
        }
        else
        if (block_end-block_begin>6)
        {
            if(end_index-begin_index==4) //如果首尾刚好5 则在begin_index和end_index中间找空
            {

                for(int n=begin_index+1; n!=end_index; ++n)
                {
                    if (goBoard.isEmpty(i+n*offset_i, j+n*offset_j))
                        emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
                }

            }
            else if (end_index-begin_index<4) //如果延长小于五
            {
                //如果被对手棋贴着 则从贴着的棋开始扫五格
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
                    if (end_index-begin_index+1!=line_count) //如果不是全连
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
                    else //如果是 X XX XXX XXXX这种全连的
                    {
                        //左五右五扫
                        if (line_count==1)
                            for(int n=1;n!=4;++n)
                            {
                                if (goBoard.isEmpty(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j))
                                    emptyPointSet.push_back(make_pair(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j));
                                if (goBoard.isEmpty(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j))
                                    emptyPointSet.push_back(make_pair(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j));
                            }
                        else if (line_count>1&&line_count<4)
                            for(int n=1;n!=3;++n)
                            {
                                if (goBoard.isEmpty(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j))
                                    emptyPointSet.push_back(make_pair(i+(begin_index-n)*offset_i, j+(begin_index-n)*offset_j));
                                if (goBoard.isEmpty(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j))
                                    emptyPointSet.push_back(make_pair(i+(end_index+n)*offset_i, j+(end_index+n)*offset_j));
                            }
                        else if (line_count==4)
                        {
                            if (goBoard.isEmpty(i+(begin_index-1)*offset_i, j+(begin_index-1)*offset_j))
                                    emptyPointSet.push_back(make_pair(i+(begin_index-1)*offset_i, j+(begin_index-1)*offset_j));
                            if (goBoard.isEmpty(i+(end_index+1)*offset_i, j+(end_index+1)*offset_j))
                                emptyPointSet.push_back(make_pair(i+(end_index+1)*offset_i, j+(end_index+1)*offset_j));
                        }
                    }
                }
            }
            else //多于五
            {
                //多余五 貌似 全扫
                for(int n=block_begin+1; n!=block_end; ++n)
                {
                    if (goBoard.isEmpty(i+n*offset_i, j+n*offset_j))
                        emptyPointSet.push_back(make_pair(i+n*offset_i, j+n*offset_j));
                }
            }
        }
    }

//    while(goBoard.at(temp_i, temp_j)==goBoard.at(i, j))
//    {
//        line_count++;
//        temp_i=temp_i+offset_i;
//        temp_j=temp_j+offset_j;
//    }
//
//    while(goBoard.at(oppo_temp_i, oppo_temp_j)==goBoard.at(i, j))
//    {
//        line_count++;
//        oppo_temp_i=oppo_temp_i+oppo_offset_i;
//        oppo_temp_j=oppo_temp_j+oppo_offset_j;
//    }
//
//    if (line_count==4)
//    {
//        if (goBoard.isEmpty(temp_i, temp_j))
//            emptyPointSet.push_back(make_pair(temp_i, temp_j));
//        if (goBoard.isEmpty(oppo_temp_i, oppo_temp_j))
//            emptyPointSet.push_back(make_pair(oppo_temp_i, oppo_temp_j));
//    }
//    else if (line_count<4)
//    {
//
//        //        cout<<"check1: "<<temp_i<<':'<<temp_j<<endl;
//        while(goBoard.isEmpty(temp_i, temp_j)) //没边最远探测两格的空点一次?'这个情况有点儿复杂了
//        {
//        //        cout<<"check2"<<temp_i<<':'<<temp_j<<endl;
//            emptyPointSet.push_back(make_pair(temp_i, temp_j));
//
//            temp_i=temp_i+offset_i;
//            temp_j=temp_j+offset_j;
//
//            if (goBoard.isEmpty(temp_i, temp_j))
//            {
//                emptyPointSet.push_back(make_pair(temp_i, temp_j));
//                break;
//            }
//            while(goBoard.isPiece(temp_i, temp_j)&&goBoard.at(temp_i, temp_j)==goBoard.at(i, j))
//            {
//                temp_i=temp_i+offset_i;
//                temp_j=temp_j+offset_j;
//            }
//
//        }
//
//
//         while(goBoard.isEmpty(oppo_temp_i, oppo_temp_j)) //没边最远探测两格的空点一次?'这个情况有点儿复杂了
//        {
//    //            cout<<"check2"<<temp_i<<':'<<temp_j<<endl;
//            emptyPointSet.push_back(make_pair(oppo_temp_i, oppo_temp_j));
//
//            oppo_temp_i=oppo_temp_i+oppo_offset_i;
//            oppo_temp_j=oppo_temp_j+oppo_offset_j;
//
//            if (goBoard.isEmpty(oppo_temp_i, oppo_temp_j))
//            {
//                emptyPointSet.push_back(make_pair(oppo_temp_i, oppo_temp_j));
//                break;
//            }
//            while(goBoard.at(oppo_temp_i, oppo_temp_j)==goBoard.at(i, j))
//            {
//                oppo_temp_i=oppo_temp_i+oppo_offset_i;
//                oppo_temp_j=oppo_temp_j+oppo_offset_j;
//            }
//
//        }
//
//    }



//    if(line_count>4)
//        emptyPointSet.clear();
//    char ch;
//    for(vector<Action>::iterator iter=emptyPointSet.begin(); iter!=emptyPointSet.end(); ++iter) //debug
//        cout<<iter->first<<':'<<iter->second<<endl;
//    cin>>ch;

    return make_pair(line_count, emptyPointSet);
}

//checkLineState 在某方向用scanLine进行状态判断:眠一/活一/眠二/活二/眠三/活三/眠四/活四/五/长连
//长连 黑棋line_count>5
//五 黑棋line_count==5 白棋line_count>5
//活四 两点可成五且line_count==4
//眠四 一点可成五(line_count<=4)
//活三 有点可成活四(有点可成五则为五比如O.OOO..)(line_count<=3)
//眠三 有点可成眠四(有点可成活四则为活三比如O.XXX..)
//活二 有点可成活三 (line_count<=2)
//眠二 有点可成眠三(有点可成活三则为活二) (line_count<=2)
//活一 有点可成活二 (line_count==1)
//眠一 有点可成眠二 (line_count==1)
//未来用空间换时间
pair< int, vector<Action> > GoBang::checkLineState(const int &i, const int &j, const char &direct, Board goBoard, const bool &forbid_move, int depth) //范围应该限制在i,j左右五格内
{
    int state=0;
    vector<Action> keyPointSet;

    if (!goBoard.isPiece(i, j))
        return make_pair(state, keyPointSet);



//    cout<<"check1"<<endl;//debug
    pair< int, vector<Action> > data=scanLine(i, j, direct, goBoard);
//    cout<<"check2"<<endl;//debug
    int line_count=data.first;
    vector<Action> emptyPointSet(data.second);
//    int state=0;
    int try_state=0;
    int try_BestState=0;
    int try_i=0;
    int try_j=0;

//    cout<<"line-count:"<<line_count<<endl; //debug
//    cout<<"EmptyPointSize: "<<emptyPointSet.size()<<endl; //debug
//    cout<<"depth:"<<depth<<endl; //debug

    if (line_count>5)
    {
        if (forbid_move&&goBoard.isBlack(i, j))
        {
                state=OVERLINE;
        }
        else
            state=FIVE;
    }
    else if (line_count==5)
    {
        state=FIVE;
    }
    else //line_count<4
    {
        if (depth==0)
            return make_pair(0, keyPointSet);
        if (depth<0)
        {
//            cout<<"check"<<endl; //debug
            depth=5-line_count;
        }
        else
            depth--;

//        char ch; //debug
        for(vector<Action>::iterator iter=emptyPointSet.begin(); iter!=emptyPointSet.end(); ++iter)
        {
            try_i=iter->first;
            try_j=iter->second;

            //假落子推算
            goBoard.place(goBoard.at(i, j), try_i, try_j); //暂时不修改引用 看会发生什么

//            cout<<"check1"<<endl;//debug

//            cout<<"try_pos: "<<try_i<<':'<<try_j<<endl;//debug
//            cout<<goBoard<<endl;//debug


            //这个地方还有改进的空间 比如  X..X->XX.X->XXXX  X..X->X.XX->XXXX  X..X需要两个位置到达XXXX  子越少出现的重复越多
            try_state=checkLineState(i, j, direct, goBoard, forbid_move, depth).first; //在switch作禁手判断? 在这里判断真三假三?
            //如果是禁手点则跳过

//            cout<<"depth:"<<depth<<" try: "<<try_i<<':'<<try_j<<" state: "<<try_state<<endl;//debug



            if (forbid_move&&goBoard.isBlack(i,j))
                                                                        //非禁手这个点不停地被探究
                if (try_state==DOUBLE_FOUR||(try_state!=FIVE&&isForbidMove(try_i, try_j, goBoard))) //要排bug 判断禁手只给两层深度 这里判断禁手仅仅 三三2 四四1 长连0 这里也有深度
                    break;
            goBoard.remove(try_i, try_j);


//            cin>>ch; //debug


            if (try_state>try_BestState)
            {
                try_BestState=try_state;
                keyPointSet.clear();
                keyPointSet.push_back(*iter);  //禁手点是否要收集?
            }
            else if (try_state==try_BestState&&try_state!=0)
            {
                keyPointSet.push_back(*iter);
            }

        }

//        cout<<try_BestState<<' '<<line_count<<endl;//debug

        switch(try_BestState)
        {
        case FIVE:
            {
                if(keyPointSet.size()==2) //注意平行三种四四禁手
                {
                    if (line_count==4)
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
        case OPEN_THREE:    //目前还没考虑禁手 仅在活三这考虑禁手即可 这个地方还要思考
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

//用来数区域内落子个数
int GoBang::countLine(const int &i, const int &j, const char &direct, const Board &goBoard)
{
    vector<Action> emptyPointSet;
    if (!goBoard.isPiece(i, j)) //如果不是棋子
        return 0;

    int line_count=1;
    int oppo_line_count=1;
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
    default: //横
        {
            offset_i=0;
            offset_j=1;
        }
        break;
    }


    int piece=0;
    //双向扫描
    for(int n=1; n<5; ++n)
    {
        piece=goBoard.at(i+n*offset_i, j+n*offset_j);

        if (piece==goBoard.at(i, j))
        {
                line_count++;
        }
        else if (piece!=EMPTY_POINT)
        {
            break;
        }


    }
    if (line_count==5&&goBoard.at(i+5*offset_i, j+5*offset_j)==goBoard.at(i, j))
        line_count++;

    for(int n=-1; n>-5; --n)
    {

        piece=goBoard.at(i+n*offset_i, j+n*offset_j);

        if (piece==goBoard.at(i, j))
        {
            oppo_line_count++;
        }
        else if (piece!=EMPTY_POINT)
        {
            break;
        }
    }

    line_count=line_count+oppo_line_count-1;
    return line_count;
}

bool GoBang::isForbidMove(const int &i, const int &j, Board &goBoard) //bool
{
    // 现在的问题是由于checkLineState的问题 即时A点已不可能成为禁手 其函数仍然会搜索A点的配套落点是否为禁手 导致耗尽
    if (!goBoard.isBlack(i, j))
        return false;

//    int state=0;
//    vector<Action> keyPointSet; //暂时先让它空着

//    if (!goBoard.isPiece(i, j))
//        return make_pair(state, keyPointSet);

    pair< int, vector<Action> > direct_state;
    multimap< int, vector<Action> > directStateSet;
    set<char> OT_DirectSet;
    set<char> F_DirectSet;
//    set<char> DF_DirectSet;

    for(int n=0; n!=4; ++n)
    {
        //使用计数短距计数避免耗尽计算
        if (countLine(i, j, direct_arr[n], goBoard)>2) //大于三才有可能有禁手
        {
            direct_state=checkLineState(i, j, direct_arr[n], goBoard, false, 2);  //还没添加关键点 //debug 要改哟
            if (direct_state.first==OPEN_THREE)
                OT_DirectSet.insert(direct_arr[n]);

            if (direct_state.first==SLEEP_FOUR||direct_state.first==OPEN_FOUR||direct_state.first==DOUBLE_FOUR)
                F_DirectSet.insert(direct_arr[n]);
//            if (direct_state.first==SLEEP_FOUR||direct_state.first==OPEN_FOUR)
//                F_DirectSet.insert(direct_arr[n]);
//            if (direct_state.first==DOUBLE_FOUR) //如果是特殊四四 立即进行验证 可能退化到眠四
//            {
//                direct_state=checkLineState(i, j, direct_arr[n], goBoard, true, 2);
//                if (direct_state.first==SLEEP_FOUR||direct_state.first==OPEN_FOUR)
//                    F_DirectSet.insert(direct_arr[n]);
//            }

            directStateSet.insert(direct_state);
        }

    }

    if (directStateSet.count(OVERLINE))
    {
        //长连
        return true;
    }
    else
    if(directStateSet.count(FIVE))
    {
        //五连
        return false;
    }
    else if (directStateSet.count(DOUBLE_FOUR)||directStateSet.count(OPEN_FOUR)+directStateSet.count(SLEEP_FOUR)>1 )
    {
         for(set<char>::iterator iter=F_DirectSet.begin(); iter!=F_DirectSet.end(); ++iter)
        {
            direct_state=checkLineState(i, j, *iter, goBoard, true, 1);
            if (direct_state.first==DOUBLE_FOUR)   //checkLineState是不会给出四四禁手的
            {
                return true;
            }
            else
            if (direct_state.first!=OPEN_FOUR&&direct_state.first!=SLEEP_FOUR)
                F_DirectSet.erase(iter);


        }
        if (F_DirectSet.size()>1)
            return true;
    }
//    else
//    if( directStateSet.count(DOUBLE_FOUR))
//    {
//            return true;
//    }
//    else if (directStateSet.count(OPEN_FOUR)+directStateSet.count(SLEEP_FOUR)>1 )
//    {
//        //四四 这里对白棋四四和黑棋四四作出区别
//        for(set<char>::iterator iter=F_DirectSet.begin(); iter!=F_DirectSet.end(); ++iter)
//        {
//            direct_state=checkLineState(i, j, *iter, goBoard, true, 1);
//            if (direct_state.first!=OPEN_FOUR&&direct_state.first!=SLEEP_FOUR)
//                F_DirectSet.erase(iter);
//        }
//        if (F_DirectSet.size()>1)
//            return true;
//    }
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
            return true;

    }

    return false;
}

pair< int, vector<Action> > GoBang::checkState(const int &i, const int &j, Board &goBoard, const bool &forbid_move)
{

    int state=0;
    vector<Action> keyPointSet; //暂时先让它空着

    if (!goBoard.isPiece(i, j))
        return make_pair(state, keyPointSet);

    pair< int, vector<Action> > direct_state;
    multimap< int, vector<Action> > directStateSet;


    for(int n=0; n!=4; ++n)
    {
//        cout<<"check: "<<i<<':'<<j<<" Direct: "<<direct_arr[n]<<endl;//debug

        direct_state=checkLineState(i, j, direct_arr[n], goBoard, forbid_move);  //还没添加关键点 //debug 要改哟

//        cout<<"Direct: "<<direct_arr[n]<<" state: "<<direct_state.first<<endl;//debug
//        cout<<"-----------------------------------"<<endl; //debug

        if (direct_state.first>state)
        {
            state=direct_state.first;
        }
        directStateSet.insert(direct_state);
    }

    if (directStateSet.count(OVERLINE))
    {
        //长连
        state=OVERLINE;
    }
    else
    if(directStateSet.count(FIVE))
    {
        //五连
        state=FIVE;
    }
    else
    if( directStateSet.count(DOUBLE_FOUR)||directStateSet.count(OPEN_FOUR)+directStateSet.count(SLEEP_FOUR)>1 )
    {
        //四四 这里对白棋四四和黑棋四四作出区别
        if (forbid_move&&goBoard.isBlack(i, j))
            state=F_DOUBLE_FOUR;
        else
            state=DOUBLE_FOUR;
    }
    else
    if (directStateSet.count(OPEN_THREE)==1&&directStateSet.count(OPEN_FOUR)+directStateSet.count(SLEEP_FOUR)==1)
    {
        //四三
        state=FOUR_THREE;
    }
    else
    if (directStateSet.count(OPEN_THREE)>1)
    {   //三三
        if (forbid_move&&goBoard.isBlack(i, j))
            state=F_DOUBLE_THREE;
        else
        if (directStateSet.count(OPEN_FOUR)+directStateSet.count(SLEEP_FOUR)==1) //四三三 有禁手时为三三禁手 无禁手时为四三
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
