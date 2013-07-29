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
    NeuralNet(const int &l, const vector<int> &m, const int &n=1, const double &a=0.1):input_num(l), hidden_num(m), output_num(n), eta(a)
    {
        init();
    };

    NeuralNet(const int &l, const int &m, const int &n=1, const double &a=0.1):input_num(l), hidden_num(1,m), output_num(n), eta(a)
    {
        init();
    };

    void init()
    {
        srand((unsigned)time(NULL));
        vector<int> layer;
        for(int n=1; n<=input_num; ++n)
        {
            layer.push_back(n);
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
                FO[0].insert(n);
                FI[n].insert(0);
                w[0][n]=rand()/(double)(RAND_MAX)-0.5;
            }
            if (layer.size())
                structure.push_back(layer);
            begin_index=begin_index+hidden_num[m];
        }

        layer.clear();
        for(int n=begin_index; n!=begin_index+output_num; ++n)
        {
            layer.push_back(n);
            FO[0].insert(n);
            FI[n].insert(0);
            w[0][n]=rand()/(double)(RAND_MAX)-0.5;
        }
        structure.push_back(layer);
        //bias¹Ì¶¨µÄ1
        out_buffer[0]=1.;

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

    }

    inline void connect(const int &i, const int &j, double weight=0)
    {
        if (weight==0)
            weight=rand()/(double)(RAND_MAX)-0.5;

        w[i][j]=weight;

        FO[i].insert(j);
        FI[j].insert(i);

    }

    inline double& learning_rate()
    {
        return eta;
    }

//    inline void backprop(const string& X, const string& Y)
//    {
//
//    }

    inline double backprop(const vector<double> &X, const vector<double> &T)
    {
        vector<double> O=predict(X);
        double error=0;
        vector<int> layer=structure.back();
        int i,j;
        set<int> out_set;
        int n=0;
        for(vector<int>::iterator iter=layer.begin(); iter!=layer.end(); ++iter)
        {
            j=*iter;
            err_buffer[j]=derivate(j)*(T[n]-O[n]);
            error+=(T[n]-O[n])*(T[n]-O[n]);
            ++n;
        }

        for(vector< vector<int> >::reverse_iterator riter=structure.rbegin()+1; riter!=structure.rend(); ++riter)
        {
            layer=*riter;
            for(vector<int>::iterator iter=layer.begin(); iter!=layer.end(); ++iter)
            {
                i=*iter;
                out_set=FO[i];
                err_buffer[i]=0;
                for(set<int>::iterator s_iter=out_set.begin(); s_iter!=out_set.end(); ++s_iter)
                {
                    j=*s_iter;
//                    err_buffer[i]+=err_buffer[j]*w[i][j];
                    w[i][j]+=eta*err_buffer[j]*out_buffer[i];
                    w[0][j]+=eta*err_buffer[j];//
                    err_buffer[i]+=err_buffer[j]*w[i][j];
                }
                err_buffer[i]*=derivate(i);

            }
        }
        return error*0.5;
    }

    inline double derivate(int j, const int &ac_fun=LOGISTIC)
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
        layer=structure[0];
        for(vector<int>::iterator iter=layer.begin(); iter!=layer.end(); ++iter)
        {
            if (*iter>X.size())
                out_buffer[*iter]=0.0;
            else
                out_buffer[*iter]=X[(*iter)-1];
        }

        int i,j;
        set<int> in_set;
        for(vector< vector<int> >::iterator iter=structure.begin()+1; iter!=structure.end(); ++iter)
        {
            layer=*iter;
            for(vector<int>::iterator l_iter=layer.begin(); l_iter!=layer.end(); ++l_iter)
            {
                j=*l_iter;
                in_set=FI[j];
                out_buffer[j]=0;
                for(set<int>::iterator s_iter=in_set.begin(); s_iter!=in_set.end(); ++s_iter)
                {
                    i=*s_iter;
                    out_buffer[j]=out_buffer[j]+out_buffer[i]*w[i][j];
                }
                out_buffer[j]=active_func(out_buffer[j]);
            }
        }

        layer=structure.back();
        vector<double> Y;
        for(vector<int>::iterator l_iter=layer.begin(); l_iter!=layer.end(); ++l_iter)
        {
            Y.push_back(out_buffer[*l_iter]);
        }

        return Y;
    }

private:
    int input_num;
    vector<int> hidden_num;
    int output_num;
    vector< vector<int> > structure;
//    int layer_num;
    double eta;

    map< int, map<int, double> > w;
    map< int, set<int> > FO;
    map< int, set<int> > FI;

    map<int, double> out_buffer;
    map<int, double> err_buffer;

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
