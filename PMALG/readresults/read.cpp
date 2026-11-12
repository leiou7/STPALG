#include<cstdio>
#include<iostream>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <ctime>
using namespace std;
char file[10000][100];
int main()
{
    const int BUFSIZE = 2048;
    char buffer [BUFSIZE];
    char sol[1000];
    double soll;
    int t,n,m;
    double sdoubletime;

    // freopen("./instances/Rfilenames.txt","r",stdin);
    // for(int i=1;i<=45;i++)
    // {
    //     scanf("%s",file[i]);
    // }
    // freopen("sol.txt ","w",stdout);
    // printf("Input_File_Name=(");
    // for(int i=1;i<=45;i++)
    // {
    //     if(i==1)
    //     printf("\"./instances/%s\"",file[i]);
    //     else 
    //     printf(" \"./instances/%s\"",file[i]);
    // }
    // printf(")\n\n");

    // printf("Output_File_Name=(");
    // for(int i=1;i<=45;i++)
    // {
    //     if(i==1)
    //     printf("\"./results/%s\"",file[i]);
    //     else 
    //     printf(" \"./results/%s\"",file[i]);
    // }
    // printf(")\n\n");

    n=114;
    int N=114;
    srand(time(0));
    
    freopen("./results/Rfilenames.txt","r",stdin);
    for(int i=1;i<=N;i++)
    {
        scanf("%s",file[i]);
    }
    freopen("sol.txt ","w",stdout);

    for(int i=1;i<=N;i++)
    {
        //if(i%n==1) cout<<file[i]<<endl<<endl;
        ifstream FIC;
        FIC.open(file[i]);
        if (FIC.fail())
        {
            printf("Fail to open the input file %s\n", file[i]);
        } 
        //printf("%s\n",file[i]);
        while (FIC.getline(buffer,BUFSIZE)) 
        {
            if (sscanf (buffer, "Primal  %s", &sol)>0) 
            {
                printf("%s \n",sol);
                //printf("%.2f\n",soll);
                break;
            }
             else if (sscanf (buffer, "Terminals %d", &t)>0) {
			 	printf("%d\n",t);
			 } else if (sscanf (buffer, "Nodes %d", &n)>0) {
			 	printf("%d ",n);
			 } else if (sscanf (buffer, "Edges %d", &m)>0) {
			 	printf("%d ",m);
			 } 

            
             if(sscanf (buffer, "Solution %lf %s", &soll, &sol)>0)
             {
                 //printf("%.2f\n",sdoubletime);
             }
        }
        //if(i%n==0) puts("");
        FIC.close();
    }

    system("pause");
	return 0;

}