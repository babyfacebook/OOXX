#ifndef _NEURALNET_H_
#define _NEURALNET_H_

#include <cstdlib>
#include <math.h>
#include <time.h>


#define LINEAR 0
#define LOGISTIC 1
#define TANH 2

class NeuralNet{
public:
    NeuralNet(const int &l, const vector<int> &m, const int &n=1, const double &a=0.1, const double &b=0.5)
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
            delete deltaw_old[i];
            deltaw_old[i]=NULL;
        }

        delete deltaw_old;
        deltaw_old=NULL;

        for(int i=0;i<=unit_num;i++)
        {
            delete deltaw[i];
            deltaw[i]=NULL;
        }

        delete deltaw;
        deltaw=NULL;
    }

    void init()
    {


        srand((unsigned)time(NULL));
        vector<int> layer;

        //占0位
        addElement();

        for(int n=1; n<=input_num; ++n)
        {
            layer.push_back(n);
            addElement();
        }
        structure.push_back(layer);
        int begin_index=input_num+1;

        for(int m=0; m!=hidden_num.size(); ++m)
        {
            layer.clear();
            for(int n=begin_index; n!=begin_index+hidden_num[m]; ++n)
            {
                //bias
                layer.push_back(n);
                addElement();



            }
            if (layer.size())
                structure.push_back(layer);
            begin_index=begin_index+hidden_num[m];
        }


        layer.clear();
        for(int n=begin_index; n!=begin_index+output_num; ++n)
        {
            layer.push_back(n);
            addElement();
            Y.push_back(0.);
        }
        structure.push_back(layer);

        unit_num=begin_index+output_num-1;

        w = new double*[unit_num+1];
        for(int i=0;i<=unit_num;i++)
            w[i]=new double[unit_num+1];

        deltaw_old = new double*[unit_num+1];
        for(int i=0;i<=unit_num;i++)
            deltaw_old[i]=new double[unit_num+1];

        deltaw = new double*[unit_num+1];
        for(int i=0;i<=unit_num;i++)
            deltaw[i]=new double[unit_num+1];

        //bias为1
        out_buffer[0]=1.; //debug
        //这样就会留空了.. 寻求解决方案

//        for(vector< vector<double> >::iterator iter=deltaw_old.begin(); iter!=deltaw_old.end(); ++iter)
//        {
//            for(int n=0;n!=deltaw_old.size();++n)
//                (*iter).push_back(0.);
//        }
//
//        for(vector< vector<double> >::iterator iter=deltaw.begin(); iter!=deltaw.end(); ++iter)
//        {
//            for(int n=0;n!=deltaw.size();++n)
//                (*iter).push_back(0.);
//        }

//        for(vector< vector<double> >::iterator iter=w.begin(); iter!=w.end(); ++iter)
//        {
//            for(int n=0;n!=w.size();++n)
//                (*iter).push_back(0.);
//        }

    }

    inline void addElement()
    {
        vector<int> empty_int_vec;
        vector<double> empty_double_vec;
        out_buffer.push_back(0.);
        err_buffer.push_back(0.);
        FO.push_back(empty_int_vec);
        FI.push_back(empty_int_vec);
//        deltaw_old.push_back(empty_double_vec);
//        deltaw.push_back(empty_double_vec);
//        w.push_back(empty_double_vec);
    }

    inline void full_connect()
    {
        vector<int> high_layer;
        vector<int> low_layer;
        for(vector< vector<int> >::iterator iter=structure.begin(); iter+1!=structure.end(); ++iter)
        {
            high_layer=*iter;
            low_layer=*(iter+1);
            for(vector<int>::iterator h_iter=high_layer.begin(); h_iter!=high_layer.end(); ++h_iter)
                for(vector<int>::iterator l_iter=low_layer.begin(); l_iter!=low_layer.end(); ++l_iter)
                {

                    connect(*h_iter, *l_iter);
                }
        }
        for(vector< vector<int> >::iterator iter=structure.begin()+1; iter!=structure.end(); ++iter)
        {
            high_layer=*iter;
            for(vector<int>::iterator h_iter=high_layer.begin(); h_iter!=high_layer.end(); ++h_iter)
                connect(0, *h_iter);
        }




    }

    inline void setweight(const int &i, const int &j, const double &weight=0)
    {
        if (fabs(weight-0.)<1e-6)
            w[i][j]=rand()/(double)(RAND_MAX)-0.5;
        else
            w[i][j]=weight;

        deltaw_old[i][j]=0.;
        deltaw[i][j]=0.;
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

    inline double backprop(const vector<double> &X, const vector<double> &T)
    {
        vector<double> O=predict(X);

        vector<int> layer=structure.back();
        int i,j;
        vector<int> out_set;
        int n=0;
        double error=0.;
        double residual;
        for(vector<int>::iterator iter=layer.begin(); iter!=layer.end(); ++iter)
        {
            j=*iter;
            residual=T[n]-O[n];
            err_buffer[j]=derivate(j)*residual;
            error+=residual*residual;
            ++n;
            deltaw[0][j]=eta*err_buffer[j]*out_buffer[0]+momentum*deltaw_old[0][j];
            w[0][j]+=deltaw[0][j];
        }

        for(vector< vector<int> >::reverse_iterator riter=structure.rbegin()+1; riter!=structure.rend(); ++riter)
        {
            layer=*riter;
            for(vector<int>::iterator iter=layer.begin(); iter!=layer.end(); ++iter)
            {
                i=*iter;
                out_set=FO[i];
                err_buffer[i]=0.;
                for(vector<int>::iterator s_iter=out_set.begin(); s_iter!=out_set.end(); ++s_iter)
                {
                    j=*s_iter;

//                    err_buffer[i]+=err_buffer[j]*w[i][j];
                    deltaw[i][j]=eta*err_buffer[j]*out_buffer[i]+momentum*deltaw_old[i][j];

                    w[i][j]+=deltaw[i][j];

                    err_buffer[i]+=err_buffer[j]*w[i][j];  //这个位置反而加速收敛


                }
                err_buffer[i]*=derivate(i);
                deltaw[0][i]=eta*err_buffer[i]*out_buffer[0]+momentum*deltaw_old[0][i];
                w[0][i]+=deltaw[0][i];

            }
        }
        double **temp;
        temp=deltaw_old;
        deltaw_old=deltaw;
        deltaw=temp;

        return 0.5*error;

    }

    inline double derivate(const int &j, const int &ac_fun=LOGISTIC)
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

    inline vector<double> predict(const vector<double> &X)
    {
        vector<int> layer;
        layer=structure.front();
        for(vector<int>::iterator iter=layer.begin(); iter!=layer.end(); ++iter)
        {
            if (*iter>X.size())
                out_buffer[*iter]=0.0;
            else
                out_buffer[*iter]=X[(*iter)-1];
        }

        int i,j;
        vector<int> in_set;
        for(vector< vector<int> >::iterator iter=structure.begin()+1; iter!=structure.end(); ++iter)
        {
            layer=*iter;
            for(vector<int>::iterator l_iter=layer.begin(); l_iter!=layer.end(); ++l_iter)
            {
                j=*l_iter;
                in_set=FI[j];
                out_buffer[j]=0;
                for(vector<int>::iterator s_iter=in_set.begin(); s_iter!=in_set.end(); ++s_iter)
                {
                    i=*s_iter;
                    out_buffer[j]=out_buffer[j]+out_buffer[i]*w[i][j];
                }
                out_buffer[j]=active_func(out_buffer[j]);
            }
        }

        layer=structure.back();

        int n=0;
        for(vector<int>::iterator l_iter=layer.begin(); l_iter!=layer.end(); ++l_iter)  //这个地方也是要耗时间的
        {
            Y[n]=out_buffer[*l_iter];
            ++n;
        }


        return Y;
    }

private:
    int input_num;
    vector<int> hidden_num;
    int output_num;
    vector< vector<int> > structure;

    double eta;
    double momentum;
    int unit_num;
    int layer_num;
//    vector< vector<double> > w;
    double **w=NULL;
//    vector< vector<double> > deltaw;
    double **deltaw=NULL;

//    vector< vector<double> > deltaw_old;
    double **deltaw_old=NULL;

    vector< vector<int> > FO;
    vector< vector<int> > FI;

    vector<double> out_buffer;
    vector<double> err_buffer;
    vector<double> Y;


    inline double active_func(double y)
    {
        return logistic(y);
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
        return (2./(1.+exp(-2.*y))-1);
    }

    friend class NeuralNetTester;
};


#endif // _NEURALNET_H_
