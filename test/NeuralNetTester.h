#ifndef _NEURALNETTESTER_H_
#define _NEURALNETTESTER_H_

#include "..\\ai\ANN\NeuralNet.h"
#include "..\\utils\timer.h"
//using namespace std;

class NeuralNetTester{

public:

    void test()
    {
        NeuralNet net(2,2,1,0.1,0);



//        int** struc=net.structure;
//        vector<int> layer;
//        for(vector< vector<int> >::iterator iter=struc.begin(); iter!=struc.end(); ++iter)
//        {
//            layer=*iter;
//            for(vector<int>::iterator l_iter=layer.begin(); l_iter!=layer.end(); ++l_iter)
//            {
//                cout<<*l_iter<<' ';
//            }
//            cout<<endl;
//        }

        for(int n=1;n<=net.layer_num;++n)
        {
            for(int m=0; m!=net.layer_unit_num[n];++m)
            {
                cout<<net.structure[n][m]<<' ';
            }
            cout<<endl;
        }

        net.full_connect();
//        net.connect(1,4);
//        net.connect(2,4);

        net.w[0][3]=-0.5;  //17125
        net.w[0][4]=0.05;  //21638
        net.w[0][5]=-0.3;
        net.w[1][3]=0.3;
        net.w[1][4]=0.1;
        net.w[2][3]=0.2;
        net.w[2][4]=0.15;
        net.w[3][5]=0.4;
        net.w[4][5]=0.35;

        //0.3²»ÐÐ
//        net.w[0][3]=0.0786309;
//        net.w[0][4]=0.434233;
//        net.w[0][5]=0.480468;
//        net.w[1][3]=-0.370785;
//        net.w[1][4]=-0.0590381;
//        net.w[2][3]=0.369778;
//        net.w[2][4]=-0.423063;
//        net.w[3][5]=0.218253;
//        net.w[4][5]=-0.225272;


//        net.w[0][3]=0.;
//        net.w[0][4]=0.;
////        net.w[0][5]=0.;
//        net.w[1][3]=0.;
//        net.w[1][4]=0.;
//        net.w[2][3]=0.;
//        net.w[2][4]=0.;
////        net.w[3][5]=0.;
////        net.w[4][5]=0.;
//        net.w[3][4]=0.;

        showWeight(net);


        vector<double> X0;
        X0.push_back(0);
        X0.push_back(0);

        vector<double> X1;
        X1.push_back(0);
        X1.push_back(1);
        vector<double> X2;
        X2.push_back(1);
        X2.push_back(0);
        vector<double> X3;
        X3.push_back(1);
        X3.push_back(1);

        cout<<"P"<<net.predict(X2)[0]<<endl;
        showOutBuffer(net);

    //predict测时
//        tic;
//        for (int i = 0 ; i < 171250 ; i++ )
//        {
//            net.predict(X0);
//            net.predict(X1);
//            net.predict(X2);
//            net.predict(X3);
//        }
//        toc;
//        tictoc(stdout);

//        vector<double> Y=net.predict(X);
//        for(vector<double>::iterator iter=Y.begin(); iter!=Y.end(); ++iter)
//        {
//            cout<<*iter<<' ';
//        }

        vector<double> Y0;
        Y0.push_back(0);
        vector<double> Y1;
        Y1.push_back(1);
        vector<double> Y2;
        Y2.push_back(1);
        vector<double> Y3;
        Y3.push_back(0);

        cout<<endl;





//        net.backprop(X2, Y2);
//        cout<<endl;
//        showOutBuffer(net);
//        cout<<endl;
//        showErrBuffer(net);
//        cout<<endl;
//        showWeight(net);
//        cout<<endl;
//        cout<<net.predict(X2)[0]<<endl;




        double error;

        tic;
        for(int n=0; n!=50000; ++n)
        {
            error=0;

            error+=net.backprop(X2, Y2);
            error+=net.backprop(X0, Y0);
            error+=net.backprop(X1, Y1);
            error+=net.backprop(X3, Y3);

            error=error/4;
            if (error<0.001)
            {
                cout<<n+1<<" Iterations."<<endl;
                break;
            }

        }
        toc;
        tictoc(stdout);
//        showWeight(net);

        cout<<"ERROR:"<<0.5*error<<endl;
        vector<double> X;
        X.push_back(0);
        X.push_back(0);
        cout<<net.predict(X)[0]<<endl;

        X.clear();
        X.push_back(1);
        X.push_back(1);
        cout<<net.predict(X)[0]<<endl;

        X.clear();
        X.push_back(1);
        X.push_back(0);
        cout<<net.predict(X)[0]<<endl;

        X.clear();
        X.push_back(0);
        X.push_back(1);
        cout<<net.predict(X)[0]<<endl;



    }

private:
//    NeuralNet net;
    void showWeight(const NeuralNet &net)
    {
        double **w=net.w;
        int unit_num=net.unit_num;
        for(int i=0;i<=unit_num;++i)
        {
            for(int j=0;j<=unit_num;++j)
            {
                if (fabs(w[i][j]-0.)>1e-6)
                    cout<<i<<'-'<<j<<':'<<w[i][j]<<endl;
            }
        }
    }

    void showOutBuffer(const NeuralNet &net)
    {
        vector<double> out_buffer=net.out_buffer;
        int n=0;
        for(vector<double>::iterator iter=out_buffer.begin(); iter!=out_buffer.end(); ++iter)
        {
            cout<<"Out "<<n<<':'<<*iter<<endl;
            ++n;
        }
    }

    void showErrBuffer(const NeuralNet &net)
    {
        vector<double> err_buffer=net.err_buffer;
        int n=0;
        for(vector<double>::iterator iter=err_buffer.begin(); iter!=err_buffer.end(); ++iter)
        {
            cout<<"Delta "<<n<<':'<<*iter<<endl;
            ++n;
        }
    }
};

#endif // _NEURALNETTESTER_H_
