#ifndef _NEURALNET_H_
#define _NEURALNET_H_

#include <cstdlib>
#include <math.h>
#include <time.h>
#include "tanh_sse.h"

#define LINEAR 0
#define LOGISTIC 1
#define TANH 2
#define approx_tanh(x) (-.67436811832e-5+(.2468149110712040+(.583691066395175e-1+.3357335044280075e-1*x)*x)*x)/(.2464845986383725+(.609347197060491e-1+(.1086202599228572+.2874707922475963e-1*x)*x)*x)

class NeuralNet{
public:
    NeuralNet(const int &l, const std::vector<int> &m, const int &n=1, const double &a=0.1, const double &b=0.5)
    :input_num(l), hidden_num(m), output_num(n), eta(a), momentum(b)
    {
        init();
    };

    NeuralNet(const int &l, const int &m, const int &n=1, const double &a=0.1, const double &b=0.5)
    :input_num(l), hidden_num(1,m), output_num(n), eta(a), momentum(b)
    {
        init();
    };

    ~NeuralNet()
    {
        for(int i=0;i<=unit_num;i++)
        {
            delete w[i];
            w[i]=NULL;
        }

        delete w;
        w=NULL;

        for(int i=0;i<=unit_num;i++)
        {
            delete d_w_old[i];
            d_w_old[i]=NULL;
        }

        delete d_w_old;
        d_w_old=NULL;

        for(int i=0;i<=unit_num;i++)
        {
            delete d_w[i];
            d_w[i]=NULL;
        }

        delete d_w;
        d_w=NULL;

        //记得销毁structure 从0开始
        for(int n=1;n<=layer_num;++n)
        {
            delete structure[n];
            structure[n]=NULL;
        }
        delete structure;
        structure=NULL;

    }

    void init()
    {


        srand((unsigned)time(NULL));
        layer_num=2+hidden_num.size();
        structure=new int*[layer_num+1];
        structure[0]=NULL;

        layer_unit_num.push_back(0);
        //占0位
//        addElement();
        int layer_index=1;
        layer_unit_num.push_back(input_num);
        structure[layer_index]=new int[input_num];
        layer_unit_num[layer_index]=input_num;
        for(int n=1; n<=input_num; ++n)
        {
            structure[layer_index][n-1]=n;
//            addElement();
        }

        int begin_index=input_num+1;

        for(int m=0; m!=hidden_num.size(); ++m)
        {
            ++layer_index;
            layer_unit_num.push_back(hidden_num[m]);
            structure[layer_index]=new int[(hidden_num[m])]; //这个地方有问题

            for(int n=begin_index; n!=begin_index+hidden_num[m]; ++n)
            {
                structure[layer_index][n-begin_index]=n;
            }
            begin_index=begin_index+hidden_num[m];
        }

        ++layer_index;
        structure[layer_index]=new int[output_num];
        layer_unit_num.push_back(output_num);
        for(int n=begin_index; n!=begin_index+output_num; ++n)
        {
            structure[layer_index][n-begin_index]=n;
//            addElement();
            Y.push_back(0.);
            O.push_back(0.);
        }

        unit_num=begin_index+output_num-1;

        w = new double*[unit_num+1];
        for(int i=0;i<=unit_num;i++)
        {
            w[i]=new double[unit_num+1];
            for(int j=0;j<=unit_num;j++)
                w[i][j]=0.;
        }


        d_w_old = new double*[unit_num+1];
        for(int i=0;i<=unit_num;i++)
        {
            d_w_old[i]=new double[unit_num+1];
            for(int j=0;j<=unit_num;j++)
                d_w_old[i][j]=0.;
        }


        d_w = new double*[unit_num+1];
        for(int i=0;i<=unit_num;i++)
        {
            d_w[i]=new double[unit_num+1];
            for(int j=0;j<=unit_num;j++)
                d_w[i][j]=0.;
        }



        //这样就会留空了.. 寻求解决方案
        std::vector<int> empty_int_vec;

        std::vector< std::vector<int> > _FO(unit_num+1, empty_int_vec);
        std::vector< std::vector<int> > _FI(unit_num+1, empty_int_vec);

        std::vector<double> _out_buffer(unit_num+1, 0.0);
        std::vector<double> _err_buffer(unit_num+1, 0.0);

        err_buffer=_err_buffer;
        out_buffer=_out_buffer;
        FO=_FO;
        FI=_FI;

        //bias为1
        out_buffer[0]=1.; //debug

    }

//    inline void addElement()
//    {
//        vector<int> empty_int_vec;
//        vector<double> empty_double_vec;
//        err_buffer.push_back(double(0)); //这个地方还留有一个棘手问题
//        out_buffer.push_back(double(0));
//        FO.push_back(empty_int_vec);
//        FI.push_back(empty_int_vec);
//    }

    inline void full_connect()
    {
        int high_layer_index;
        int low_layer_index;

        for(int layer_index=1; layer_index!=layer_num; ++layer_index)
        {
            high_layer_index=layer_index;
            low_layer_index=layer_index+1;

                for(int high_layer_unit_index=0;high_layer_unit_index!=layer_unit_num[layer_index];++high_layer_unit_index)
                    for(int low_layer_unit_index=0;low_layer_unit_index!=layer_unit_num[layer_index+1];++low_layer_unit_index)
                {

                    connect(structure[high_layer_index][high_layer_unit_index], structure[low_layer_index][low_layer_unit_index]);
                }
        }


        for(int layer_index=2; layer_index<=layer_num; ++layer_index)
        {
            for(int unit_index=0;unit_index!=layer_unit_num[layer_index];++unit_index)
                connect(0,  structure[layer_index][unit_index]); //连接bias
        }




    }

    inline void setweight(const int &i, const int &j, const double &weight=0)
    {
//        if(i==2&&j==5)
//            cout<<"ERROR!"<<endl;
        if (fabs(weight-0.)<1e-6)
            w[i][j]=(rand()/(double)(RAND_MAX)-0.5)/10.;
        else
            w[i][j]=weight;

//        d_w_old[i][j]=0.;
        d_w[i][j]=0.;
    }

    inline void connect(const int &i, const int &j, const double &weight=0)
    {
        //这回要检测重复了
        FO[i].push_back(j);
        FI[j].push_back(i);
        setweight(i,j, weight);

    }

    inline double& learning_rate()
    {
        return eta;
    }

//    inline double backprop(const string& X, const string& Y)
//    {
//
//    }

    inline double backprop(const std::vector<double> &X, const std::vector<double> &T)
    {
        O=predict(X);
        //之前用vector接收O 性能低下
        int i,j;
        double error=0.;
        double residual;

        for(int unit_index=0; unit_index!=layer_unit_num[layer_num]; ++unit_index)
        {
            j=structure[layer_num][unit_index];

            residual=T[unit_index]-O[unit_index];
            err_buffer[j]=derivate(j)*residual;
            error+=residual*residual;
            d_w[0][j]=eta*err_buffer[j]*out_buffer[0]+momentum*d_w[0][j];
            w[0][j]+=d_w[0][j];
        }

        for(int layer_index=layer_num-1; layer_index!=0; --layer_index)
        {
            for(int unit_index=0; unit_index!=layer_unit_num[layer_index]; ++unit_index)
            {
                i=structure[layer_index][unit_index];
                //之前用vector接收FO[i] 性能低下
                err_buffer[i]=0.;
                for(std::vector<int>::iterator s_iter=FO[i].begin(); s_iter!=FO[i].end(); ++s_iter)
                {
                    j=*s_iter;

                    err_buffer[i]+=err_buffer[j]*w[i][j];

                    d_w[i][j]=eta*err_buffer[j]*out_buffer[i]+momentum*d_w[i][j];  //注意cache optimisation 二维矩阵换成一维 时间上相邻最好空间也相邻

                    w[i][j]+=d_w[i][j];

//                    err_buffer[i]+=err_buffer[j]*w[i][j];  //这个位置对于小的学习速率加速收敛 快那么一点儿 但是对于大学习率会出严重问题


                }
                err_buffer[i]*=derivate(i);
                d_w[0][i]=eta*err_buffer[i]*out_buffer[0]+momentum*d_w[0][i];
                w[0][i]+=d_w[0][i];

            }
        }



//        dd_pointer=d_w_old;
//        d_w_old=d_w;
//        d_w=dd_pointer;

        return 0.5*error;

    }

    inline double derivate(const int &j, const int &ac_fun=TANH)
    {
        switch(ac_fun)
        {
        case LOGISTIC:
            return out_buffer[j]*(1-out_buffer[j]);
            break;
        case TANH:
            return (1+out_buffer[j])*(1-out_buffer[j]);
            break;
        default:
            return out_buffer[j];
            break;
        }
    }

    inline std::vector<double> predict(const std::vector<double> &X)
    {
        for(int unit_index=0;unit_index!=layer_unit_num[1];++unit_index)
        {
            if (unit_index+1>X.size())
            {
                out_buffer[structure[1][unit_index]]=0.0;
            }
            else
            {
                out_buffer[structure[1][unit_index]]=X[unit_index];

            }
        }

        int i,j;

//        cout<<"Layer NUM:"<<layer_num;

        for(int layer_index=2; layer_index<=layer_num; ++layer_index)
        {
            for(int unit_index=0;unit_index!=layer_unit_num[layer_index]; ++unit_index)
            {
                j=structure[layer_index][unit_index];
                out_buffer[j]=0;
//                cout<<"FI_NUM("<<j<<"):"<<FI[j].size()<<endl;
                for(std::vector<int>::iterator s_iter=FI[j].begin(); s_iter!=FI[j].end(); ++s_iter)
                {
                    i=*s_iter;
                    out_buffer[j]=out_buffer[j]+out_buffer[i]*w[i][j];

                }
                out_buffer[j]=activation(out_buffer[j]);
//                cout<<"out "<<j<<" :"<<out_buffer[j]<<endl;
            }
        }


        for(int m=0;m!=layer_unit_num[layer_num];++m)
        {
            Y[m]=out_buffer[structure[layer_num][m]];
        }


        return Y;
    }

private:
    int input_num;
    std::vector<int> hidden_num;
    int output_num;
//    vector< vector<int> > structure;
    int **structure;

    double eta;
    double momentum;
    int unit_num;
    int layer_num;
    std::vector<int> layer_unit_num;
//    vector< vector<double> > w;
    double **w;
//    vector< vector<double> > d_w;
    double **d_w;

//    vector< vector<double> > d_w_old;
    double **d_w_old;

    std::vector< std::vector<int> > FO;
    std::vector< std::vector<int> > FI;

    std::vector<double> out_buffer;
    std::vector<double> err_buffer;
    std::vector<double> Y;
    std::vector<double> O;

//    double **dd_pointer;

    inline double activation(double y)
    {
        return tanh(y);
//        return fast_tanh(y);
    }

    inline double linear(double y)
    {
        return y;
    }

    inline double logistic(double y)
    {
        return 1./(1.+exp(-y));
    }

    inline double tanh(double y)
    {
        if (y<3.1&&y>-3.1)
        {
            if (y>=0.)
                return approx_tanh(y);
            else
                return -approx_tanh(-y);
        }

        return (2./(1.+exp(-2.*y))-1);
    }

    friend class NeuralNetTester;
};


#endif // _NEURALNET_H_
