#ifndef _SNAKEMAP_H_
#define _SNAKEMAP_H_

#include <cstdlib>
#include <time.h>
#include <vector>
#include <iostream>

#define NORTH 0
#define SOUTH 2
#define WEST 1
#define EAST 3



//using namespace std;

class SnakeMap{

public:

    std::vector< std::pair<char, char> > snake;
    std::pair<char, char> food_pos;
    int length;
    int width;

    SnakeMap(const int &l=20, const int &m=20):length(l),width(m),gameover_flag(false)
    {
        init();
    }

    inline int move_abs(const int &abs_direct=0)
    {
        if (gameover_flag)
            return -1;

        int offset_i=snake[0].first-snake[1].first;
        int offset_j=snake[0].second-snake[1].second;
        if (offset_i<0)
        {
            switch(abs_direct)
            {
            case NORTH:return move(1);
                break;
            case SOUTH:return move(-1);
                break;
            case WEST:return move();
                break;
            case EAST:return move();
                break;
            default:
                break;
            }
        }
        else
        if (offset_i>0)
        {
            switch(abs_direct)
            {
            case NORTH:return move(-1);
                break;
            case SOUTH:return move(1);
                break;
            case WEST:return move();
                break;
            case EAST:return move();
                break;
            default:
                break;
            }
        }
        else
        {
            if (offset_j>0)
            {
                switch(abs_direct)
                {
                case NORTH:return move();
                    break;
                case SOUTH:return move();
                    break;
                case WEST:return move(-1);
                    break;
                case EAST:return move(1);
                    break;
                default:
                    break;
                }
            }
            else
            {
                switch(abs_direct)
                {
                case NORTH:return move();
                    break;
                case SOUTH:return move();
                    break;
                case WEST:return move(1);
                    break;
                case EAST:return move(-1);
                    break;
                default:
                    break;
                }
            }
        }

        return 0;
    }

    inline void reset()
    {
        gameover_flag=false;
        snake.clear();
        init();
    }

    inline int move(const int &rel_direct=0)
    {
        if (gameover_flag)
            return -1;
        int state=0;
        int offset_i=snake[0].first-snake[1].first;
        int offset_j=snake[0].second-snake[1].second;

        if (rel_direct<0) //左转
        {
            if (offset_i<0)
            {
                offset_i=0;
                offset_j=-1;
            }
            else
            if (offset_i>0)
            {
                offset_i=0;
                offset_j=1;
            }
            else
            {
                if (offset_j>0)
                {
                    offset_i=-1;
                    offset_j=0;
                }
                else
                {
                    offset_i=1;
                    offset_j=0;
                }
            }
        }
        else
        if (rel_direct>0) //右转
        {
            if (offset_i<0)
            {
                offset_i=0;
                offset_j=1;
            }
            else
            if (offset_i>0)
            {
                offset_i=0;
                offset_j=-1;
            }
            else
            {
                if (offset_j>0)
                {
                    offset_i=1;
                    offset_j=0;
                }
                else
                {
                    offset_i=-1;
                    offset_j=0;
                }
            }
        }

        head_pos.first=snake[0].first+offset_i;
        head_pos.second=snake[0].second+offset_j;

        if (collision(head_pos.first, head_pos.second))
        {
            state=-1;
            gameover_flag=true;
        }
        else
        if (get_food())
        {

            snake.push_back(snake.back());
//            for(vector< pair<char, char> >::reverse_iterator iter=snake.rbegin()+1; iter+1!=snake.rend(); ++iter)
//            {
//                *iter=*(iter+1);
//            }
            state=1;

            if (snake.size()>(width*length-1))
            {
                gameover_flag=true;
                std::cout<<"WIN"<<std::endl;
                state= 2;
            }
            else
            {
                place_food();
            }

//            cout<<"FOOD "<<int(food_pos.first)<<':'<<int(food_pos.second)<<endl;
//            cout<<"HEAD "<<int(head_pos.first)<<':'<<int(head_pos.second)<<endl;

        }

        for(std::vector< std::pair<char, char> >::reverse_iterator iter=snake.rbegin(); iter+1!=snake.rend(); ++iter)
        {
            *iter=*(iter+1);
        }
        snake[0]=head_pos;

        return state;
    }

    void place_food()
    {
        //难道这个有问题 可能出现在蛇身上?
        food_pos.first=rand()%width;
        food_pos.second=rand()%length;

        while(!check_foodPos())
        {
            food_pos.first=rand()%width;
            food_pos.second=rand()%length;
        }

    }

    void get_offset(const int &rel_direct, int &offset_i, int &offset_j) const
    {
        offset_i=snake[0].first-snake[1].first;
        offset_j=snake[0].second-snake[1].second;

        if (rel_direct<0) //左转
        {
            if (offset_i<0)
            {
                offset_i=0;
                offset_j=-1;
            }
            else
            if (offset_i>0)
            {
                offset_i=0;
                offset_j=1;
            }
            else
            {
                if (offset_j>0)
                {
                    offset_i=-1;
                    offset_j=0;
                }
                else
                {
                    offset_i=1;
                    offset_j=0;
                }
            }
        }
        else
        if (rel_direct>0) //右转
        {
            if (offset_i<0)
            {
                offset_i=0;
                offset_j=1;
            }
            else
            if (offset_i>0)
            {
                offset_i=0;
                offset_j=-1;
            }
            else
            {
                if (offset_j>0)
                {
                    offset_i=1;
                    offset_j=0;
                }
                else
                {
                    offset_i=-1;
                    offset_j=0;
                }
            }
        }
    }
    bool illegal_pos(const int &x, const int &y)
    {
        return (x<0||x>=width||y<0||y>=length);
    }

    inline bool collision(const char &head_x, const char &head_y)
    {
        //检查碰撞
        //撞墙
//        if (head_pos.first<0||head_pos.first>=width||head_pos.second<0||head_pos.second>=length)
            if (illegal_pos(head_x, head_y))
            return true;
        //撞自己
        for(std::vector< std::pair<char, char> >::iterator iter=snake.begin()+2; iter!=snake.end()-1; ++iter)
        {
            if (head_x==iter->first&&head_y==iter->second)
                return true;
        }
        return false;
    }

private:

    std::pair<char, char> head_pos;

    bool gameover_flag;

    inline void init()
    {
        snake.push_back(std::make_pair(2, 0));
        snake.push_back(std::make_pair(1, 0));
        snake.push_back(std::make_pair(0, 0));
//
//        snake.push_back(make_pair(7, 0));
//        snake.push_back(make_pair(6, 0));
//        snake.push_back(make_pair(5, 0));


        head_pos=snake[0];
        srand((unsigned)time(NULL));
//        srand(1);
        place_food();
    }

    inline bool check_foodPos() const
    {
        for(std::vector< std::pair<char, char> >::const_iterator iter=snake.begin(); iter!=snake.end(); ++iter)
        {
            if (iter->first==food_pos.first&&iter->second==food_pos.second)
                return false;
        }
        if (head_pos==food_pos)
            return false;
        return true;
    }

    inline bool get_food() const
    {
        return (head_pos==food_pos);
    }



};

#endif // _SNAKEMAP_H_
