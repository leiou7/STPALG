#include<bits/stdc++.h>
using namespace std;
long long a[33][10],b[33][10];
double p[10]={0,10,6,4,3,2,1,0};
double ans[10];
const int T=7;
int main()
{
    freopen("res.txt","r",stdin);
    freopen("res.out","w",stdout);
    
    /*
    string s;
    for(int i=1;i<=34;i++)
    {
        for(int j=1;j<=8;j++)
        {
            cin>>s;
            cout<<s;
            if(j<8) cout<<"&";
            else cout<<"\\\\"<<endl;
        }
    }
    */
    
    for(int i=1;i<=30;i++)
    {
        for(int j=1;j<=7;j++)
        {
            scanf("%lld",&a[i][j]);
            cout<<a[i][j]<<"  ";
            b[i][j]=a[i][j];
        }
        cout<<endl;
        sort(b[i]+1,b[i]+T+1);
        for(int j=1;j<=T;j++)
        {
            double sum=0;
            int num=0;
            for(int k=1;k<=T;k++)
            {
                if(b[i][k]==a[i][j])
                {
                    num++;
                    sum+=p[k];
                }
            }
            ans[j]+=sum/num;
        }
    }

    for(int i=1;i<=T;i++)
    {
        printf("%.2f  ",ans[i]);
    }
    puts("");
    

    system("pause");
    return 0;
}