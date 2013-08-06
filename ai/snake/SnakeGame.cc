
#include "SnakeMap.h"
#include "..\\ann\NeuralNet.h"

#include <iostream>
#include <set>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32
  #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#define LEN 8
#define WID 8

using namespace std;

int length=LEN;
int width=WID;
bool over=false;
int score=0;
int offset_x=LEN/2;
int offset_y=WID/2;
SnakeMap snakeMap(LEN,WID);
NeuralNet net(LEN*WID*4+6*LEN, LEN*WID+10, 1, 0.01, 0);

vector<double> policy(3,0.);
vector<double> feature(LEN*WID*4+6*LEN,0.);
vector<double> temp_feature(LEN*WID*4+6*LEN,0.);
vector<double> prev_feature(LEN*WID*4+6*LEN,0.);
vector<double> prediction(3,0.);
int action=0;
int state=0;
double reward=0.;
double epsilon=0.2;
double gamma=0.95;
//double d_reward;
int interval=100;
int oper_num=0;
set< pair<char, char> > record;
bool begin_flag=true;


    void specialKeys(int key, int x, int y )
{
    switch (key) {
    case GLUT_KEY_LEFT:
          {
//              cout<<"GO LEFT"<<endl;
              snakeMap.move_abs(WEST);
          }
      break;
    case GLUT_KEY_RIGHT:
         {
//             cout<<"GO RIGHT"<<endl;
            snakeMap.move_abs(EAST);
         }
      break;
    case GLUT_KEY_UP:
        {
//            cout<<"GO UP"<<endl;
            snakeMap.move_abs(NORTH);
        }
      break;
    case GLUT_KEY_DOWN:
        {
//            cout<<"GO DOWN"<<endl;
            snakeMap.move_abs(SOUTH);
        }
      break;
    }
    glutPostRedisplay();
}

    inline void keyboard(unsigned char key, int x, int y)
    {
        switch (key)
        {
        case 'r':
              {
                  snakeMap.reset();
              }
          break;
        case 'f':
              {
                  interval=400;
              }
          break;
        case 'd':
              {
                  interval=100;
              }
          break;
        case 's':
              {
                  interval=10;
              }
          break;
        case 'a':
              {
                  interval=1;
              }
          break;
        case '_':
              {
                  epsilon=epsilon-0.1;
                  if (epsilon<0.)
                    epsilon=0.000001;
                    cout<<endl;
                  cout<<"EPSILON is "<<epsilon<<endl;
                cout<<endl;
              }
          break;
        case '+':
              {
                  epsilon=epsilon+0.1;

                  if (epsilon>1.)
                    epsilon=0.99999999;
                cout<<endl;
                  cout<<"EPSILON is "<<epsilon<<endl;
                  cout<<endl;
              }
         break;
         case '-':
              {
                  epsilon=epsilon-0.1;
                  if (epsilon<0.)
                    epsilon=0.00000001;
                    cout<<endl;
                  cout<<"EPSILON is "<<epsilon<<endl;
                  cout<<endl;
              }
          break;
        case '=':
              {
                  epsilon=epsilon+0.1;

                  if (epsilon>1.)
                    epsilon=0.99999999;
                    cout<<endl;
                  cout<<"EPSILON is "<<epsilon<<endl;
                  cout<<endl;
              }
         break;
        }
        glutPostRedisplay();
    }


    inline void parHead(float x1, float x2, float y1, float y2, float z1, float z2, double co)
    {
        glColor3f(1.0, 1.0*co, 1.0);
        glBegin(GL_QUADS);

        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y1, z1);
        glVertex3f(x2, y2, z1);
        glVertex3f(x1, y2, z1);

        glEnd();
    }

    inline void par(float x1, float x2, float y1, float y2, float z1, float z2)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_QUADS);

        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y1, z1);
        glVertex3f(x2, y2, z1);
        glVertex3f(x1, y2, z1);

        glEnd();
    }

    void draw_solid_circle(float x, float y, float radius)
    {
        glColor3f(1.0, 1.0, 1.0);
        int count;
        int sections=200;

        GLfloat TWOPI=2.0f * 3.14159f;

        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(x, y);

            for(count=0; count<=sections; count++)
            {
                glVertex2f(x+radius*cos(count*TWOPI/sections), y+radius*sin(count*TWOPI/sections));
            }
        glEnd();
    }

    inline void display(void)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity ();

        glTranslatef(0.0, 0.0, -22.0);
        int i;

        par(-offset_x/2.-0.3-0.2,  (WID-1-offset_x)/2.+0.3+0.2,  (LEN-1-offset_y)/2.+0.27,  (LEN-1-offset_y)/2.+0.47, 0.0, 0.0);
        par(-offset_x/2.-0.3-0.2,  (WID-1-offset_x)/2.+0.3+0.2, -offset_y/2.-0.47, -offset_y/2.-0.27, 0.0, 0.0);
        par(-offset_x/2.-0.3, -offset_x/2.-0.5, -offset_y/2.-0.27,  (LEN-1-offset_y)/2.+0.27, 0.0, 0.0);
        par( (WID-1-offset_x)/2.+0.5,  (WID-1-offset_x)/2.+0.3, -offset_y/2.-0.27,  (LEN-1-offset_y)/2.+0.27, 0.0, 0.0);


//        for(vector< pair<char, char> >::const_iterator iter=snakeMap.snake.begin(); iter!=snakeMap.snake.begin()+1; ++iter)
//        {
//
//            parHead(((iter -> first-offset_x)/2.0)-0.2,((iter -> first-offset_x))/2.0 + 0.2,(iter -> second-offset_y)/2.0-0.21,(iter -> second-offset_y)/2.0 + 0.21, 0.0, 0.0);
//        }
        double co=0.;
        int n=0;
        for(vector< pair<char, char> >::const_iterator iter=snakeMap.snake.begin(); iter!=snakeMap.snake.end(); ++iter)
        {
            n++;
            co=double(n)/(snakeMap.snake.size()+1);
            parHead(((iter -> first-offset_x)/2.0)-0.2,((iter -> first-offset_x))/2.0 + 0.2,(iter -> second-offset_y)/2.0-0.21,(iter -> second-offset_y)/2.0 + 0.21, 0.0, 0.0, co);
        }

//        par((snakeMap.food_pos.first-offset_x)/2.0-0.2, (snakeMap.food_pos.first-offset_x)/2.0 + 0.2 , (snakeMap.food_pos.second-offset_y)/2.0-0.2 , (snakeMap.food_pos.second-offset_y)/2.0 + 0.2, 0.0 , 0.0);
        draw_solid_circle((snakeMap.food_pos.first-offset_x)/2.0, (snakeMap.food_pos.second-offset_y)/2.0,0.23);
        glutSwapBuffers();
    }

    inline void Reshape(int w, int h)
    {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
//        gluPerspective(45.0, (float)w/(float)h, 0.1, 200.0);
        gluPerspective(45.0, (float)w/(float)h, 0.01, 5000.0);

    }


vector<double> snake2feature(const SnakeMap &s, const int& action, const set< pair<char, char> > &record)
{
    for(vector<double>::iterator iter=temp_feature.begin();iter!=temp_feature.end();++iter)
    {
        *iter=-1.;
    }
    for(vector< pair<char, char> >::const_iterator iter=s.snake.begin(); iter!=s.snake.end(); ++iter) //占位
    {
        temp_feature[iter->first+iter->second*s.width]=1;
    }
    temp_feature[LEN*WID+s.food_pos.first+s.food_pos.second*s.width]=1.; //食物位置
    temp_feature[2*LEN*WID+s.snake[0].first+s.snake[0].second*s.width]=1.; //头位置

    for(set< pair<char, char> >::const_iterator iter=record.begin(); iter!=record.end(); ++iter) //头经历过的位置
    {
        temp_feature[3*LEN*WID+iter->first+iter->second*s.width]=1;
    }

    int offset_i,offset_j;
    bool find_flag=false;
    for(int i=1;i<LEN;i++) //搜索食物
    {
        for(int dir=-1;dir!=2; ++dir)
        {
            snakeMap.get_offset(dir, offset_i, offset_j);
            if (snakeMap.food_pos.first==snakeMap.snake[0].first+i*offset_i&&snakeMap.food_pos.second==snakeMap.snake[0].second+i*offset_j)
            {
                temp_feature[4*LEN*WID+3*(i-1)+dir+1]=1;
                find_flag=true;
                break;
            }

        }
        if(find_flag)
                break;

    }
    for(int i=1;i<LEN+1;i++)
    {
        for(int dir=-1;dir!=2; ++dir)
        {
            snakeMap.get_offset(dir, offset_i, offset_j);
            if (snakeMap.collision(snakeMap.snake[0].first+i*offset_i, snakeMap.snake[0].second+i*offset_j))
            {
                temp_feature[4*LEN*WID+3*(LEN-1)+3*(i-1)+dir+1]=1;
            }
        }

    }

//    cout<<"ac:"<<action<<endl;
    if (action<0)
        temp_feature[temp_feature.size()-3]=1;
    else
    if (action==0)
        temp_feature[temp_feature.size()-2]=1;
    else
        temp_feature[temp_feature.size()-1]=1;

//    for(int i=0;i<25;i++)
//    {
//        cout<<feature[i];
//    }
//    cout<<endl;
//    for(int i=25;i<53;i++)
//    {
//        cout<<feature[i];
//    }
//    int ch;
//    cin>>ch;
    return temp_feature;
}

vector<double> updatePolicy(const SnakeMap &snakeMap, double &max_Q)
{
    prediction[0]=net.predict(snake2feature(snakeMap, -1, record))[0];
    prediction[1]=net.predict(snake2feature(snakeMap, 0, record))[0];
    prediction[2]=net.predict(snake2feature(snakeMap, 1, record))[0];

    double good_pred=-1.;
    int good_index=-1;
    for(int i=0; i!=3; ++i)
    {
        if (prediction[i]>good_pred)
        {
            good_pred=prediction[i];
            good_index=i;
        }


    }
    max_Q=good_pred;
        for(int i=0; i!=3; ++i)
        {
            if (i==good_index)
                policy[i]=(1-epsilon)+epsilon/3;
            else
                policy[i]=epsilon/3;

        }

    return policy;
}

pair<int, double> sampleFromPolicy(const vector<double> &policy)
{





    double ran_numf = rand() / (double)(RAND_MAX);
    int action=-2;
    for(vector<double>::const_iterator iter=policy.begin(); iter!=policy.end();++iter)
    {
        action++;
        ran_numf=ran_numf-(*iter);
        if (ran_numf<=.0)
        {
            break;
        }

    }

    return make_pair(action, prediction[action+1]);
}

//int _distance,prev_distance;

 void showFeature();

    inline void TD_train_iter()
    {
            double max_Q;

            policy=updatePolicy(snakeMap, max_Q);



            pair<int, double> a_p=sampleFromPolicy(policy);

            action=a_p.first;

//            if (action!=0)
//            {
//                reward--;
//                oper_num++;
//            }
                if (!begin_flag)
                {
                    vector<double> target(1,0.);
                    cout<<"reward is "<<reward<<endl;
//                    target[0]=reward+gamma*a_p.second;
                    target[0]=reward+gamma*max_Q;
                    net.backprop(prev_feature, target);

                }
                else
                {
                    begin_flag=false;

                }


            feature=snake2feature(snakeMap, action, record);


//                showFeature();
                cout<<"<-:"<<net.predict(snake2feature(snakeMap, -1, record))[0]<<" V:"<<net.predict(snake2feature(snakeMap, 0, record))[0]<<" ->:"<<net.predict(snake2feature(snakeMap, 1, record))[0]<<endl;
                cout<<"action:"<<action<<endl;



            state=snakeMap.move(action);

//            _distance=(snakeMap.snake[0].first-snakeMap.food_pos.first)*(snakeMap.snake[0].first-snakeMap.food_pos.first)
//            +(snakeMap.snake[0].second-snakeMap.food_pos.second)*(snakeMap.snake[0].second-snakeMap.food_pos.second);

//            cout<<"Sate "<<state<<endl;
            if (state<0)
            {
                reward=-1.;
                over=true;
            }
            else if (state==1)
            {
                cout<<"EAT!!!"<<endl;
                reward=1.;
                oper_num=0;
                record.clear();
//                over=true;//暂时
            }
            else if (state==2)
            {
                reward=10.;
                over=true;
                record.clear();
            }
            else
            {
                if (record.count(snakeMap.snake[0]))
                {
                    cout<<"Oh TWICE"<<endl;  //没有记忆 反而会confuse agent
                    reward=-0.2;
                }
                else
                {

                    record.insert(snakeMap.snake[0]);
                    reward=0.2;
//                    reward=0;
                }

//                if(_distance<prev_distance)
//                    reward=0.1;
//                else
//                    reward=-0.02;
            }

            if(over)
            {

                vector<double> target(1,0.);
                target[0]=reward;

                cout<<"reward is "<<reward<<endl;

                net.backprop(feature, target);

            }

        prev_feature=feature;
//        prev_distance=_distance;
    }



    void myIdleFunc(int a)
    {
//        if(!pus){
//            itera();
//        }
//        cout << "iterations : " << iterations << " score : " << sc << endl;
        TD_train_iter();
//        glutPostRedisplay();
        display();

        if (over)
        {
//            cout<<"over!"<<endl;
           over=false;
           snakeMap.reset();
           reward=0;
           begin_flag=true;
           record.clear();
           record.insert(snakeMap.snake[0]);
        }
        glutTimerFunc(interval, myIdleFunc, 0);
    }

    void showFeature()
    {
        for(int i=0;i!=WID*LEN;++i)
        {
            cout<<feature[i];
        }
        cout<<endl;
        for(int i=WID*LEN;i!=2*WID*LEN; ++i)
        {
            cout<<feature[i];
        }
        cout<<endl;
        for(int i=2*WID*LEN;i!=3*WID*LEN; ++i)
        {
            cout<<feature[i];
        }
        cout<<endl;
        for(int i=3*WID*LEN;i!=4*WID*LEN; ++i)
        {
            cout<<feature[i];
        }
        cout<<endl;

        for(int i=4*WID*LEN;i!=4*WID*LEN+3*(LEN-1); ++i)
        {
            cout<<feature[i];
        }
        cout<<endl;


        for(int i=4*WID*LEN+3*(LEN-1); i!=4*WID*LEN+3*(LEN-1)+3*LEN; ++i)
        {
            cout<<feature[i];
        }
        cout<<endl;

        for(int i=4*WID*LEN+3*(LEN-1)+3*LEN; i!=4*WID*LEN+6*LEN; ++i)
        {
            cout<<feature[i];
        }
        cout<<endl;
    }




int main(int argc, char** argv)
{
//    length=LEN;
//    width=WID;
//    over=false;
//    score=0;
//    offset_x=width/2;
//    offset_y=length/2;
//    SnakeMap snakeMap(LEN, WID);
    net.full_connect();
    glutInit(&argc,argv);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400,400);
    glutInitWindowPosition(500,0);
    glutCreateWindow("snake game - Q learning with Neural Network");
//        init();
    glutTimerFunc(interval, myIdleFunc, 0);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc( keyboard );
    glutDisplayFunc(display);
//    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}
