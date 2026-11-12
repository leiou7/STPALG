
#include<queue>
#include<algorithm>
vector<int> father;
vector<int> siz;
vector<int> terminalnodes;
vector<int>ifselected;
vector<int>bridgeedges[10000005];
int GetFatherforPartition(int v)
{
    return v==father[v]?v:father[v]=GetFatherforPartition(father[v]);
}

void PartitionPreProcess(Graph &g,SteinerConfig &config,VoronoiData &vordata, UniverseSet &baselist, BinaryHeap<EdgeCost> &binheap)
{
    int n=g.VertexCount();
    int m=g.EdgeCount();
    int t=g.TerminalCount();
    config.partnumbase=min(config.partnumbase,t);

    if(config.partnum==0)
        config.partnum=t/config.partnumbase;

    Basics::ComputeVoronoi(g,vordata,baselist,binheap,NULL); 

    father.resize(n+1,0); 
    siz.resize(n+1,0);
    ifselected.resize(n+1,false);

    terminalnodes.clear();
    for(int v=1;v<=n;v++){
        if(g.IsTerminal(v))
            terminalnodes.push_back(v);
        bridgeedges[v].clear();
    }

    for(int i=1;i<=m;i++)
    {
        int u,v;
        g.GetEndpoints(i,u,v);
        if(vordata.GetBase(u)!=vordata.GetBase(v)){
            bridgeedges[vordata.GetBase(u)].push_back(i);
            bridgeedges[vordata.GetBase(v)].push_back(i);
        }
    }
}

struct nodebridge{
    int e;
    int weight;
    bool operator < (const nodebridge &a)const{
        return a.weight<weight;
    }
};


void Partition(Graph &g,SteinerConfig &config,VoronoiData &vordata)
{
    int n=g.VertexCount();
    int m=g.EdgeCount();
    int t=g.TerminalCount();

    int k=config.partnum;
    int d=2;/////
    int curk=t; 

    
    for(int v=0;v<=n;v++){
        father[v]=0;
        siz[v]=0;
    }

    for(int v=1;v<=n;v++){
        int fv=vordata.GetBase(v);
        father[v]=fv;
        siz[fv]++;
        g.SetBelongPartition(v,0);
    }

    int rt=terminalnodes[rand()%t];
    while(ifselected[rt])
        rt=terminalnodes[rand()%t];

    priority_queue<nodebridge>q;
    while(!q.empty()) q.pop();
    for(int i=0;i<bridgeedges[rt].size();i++)
    {
        int e=bridgeedges[rt][i];
        nodebridge nb;
        nb.e=e;
        int u,v;
        g.GetEndpoints(e,u,v);
        int fu=vordata.GetBase(u);
        int fv=vordata.GetBase(v);
        if(ifselected[fu]||ifselected[fv])
            nb.weight=100;
        else 
            nb.weight=rand()%100;
            
        q.push(nb);
    }
    int numt=config.partnumbase;

    while(!q.empty())
    {
        nodebridge nb=q.top();
        q.pop();
        int e=nb.e;
        int u,v,fu,fv;
        g.GetEndpoints(e,u,v);
        fu=GetFatherforPartition(u);
        fv=GetFatherforPartition(v);
        if(fu==fv) continue;
        int node=fv;
        if(fu!=rt)
            node=fu;

        father[node]=rt;
        numt--;
        if(numt==0) break;
        for(int i=0;i<bridgeedges[node].size();i++)
        {
            nb.e=bridgeedges[node][i];
            g.GetEndpoints(nb.e,u,v);
            int fu=vordata.GetBase(u);
            int fv=vordata.GetBase(v);
            if(ifselected[fu]||ifselected[fv])
                nb.weight=100;
            else 
                nb.weight=rand()%100;
            q.push(nb);
        }
    }



    for(int i=1;i<=n;i++)
    {
        if(GetFatherforPartition(i)==rt)
        {
            g.SetBelongPartition(i,1);
            ifselected[i]=true;
        }
    }


    bool verbox=true;
    if(verbox)
    {
        printf("-----------------partition info---------------------------\n");
        int num=0;
        int tcount=0;
        for(int v=1;v<=n;v++)
        if(g.GetBelong_to_Patition(v)==1)
        {
            num++;
            if(g.IsTerminal(v))
                tcount++;
        }
        printf("subgraph terminal num:  %d\n",tcount);
        printf("subgraph vertex num:  %d\n",num);
        printf("-----------------partition info---------------------------\n");
    }
}

void PartitionSearch( Graph &g, SteinerConfig &config, char *filename)
{
    int subgraph_flag=(config.puw+1)%2;
    int n=g.VertexCount();
    int m=g.EdgeCount();
    int t=g.TerminalCount();
    double TIME_LIMIT=config.TIME_LIMIT;
    vector<int>zeroedge;
    zeroedge.resize(m+1,0);

    if(subgraph_flag==1)
    {
        printf("\nBEGIN Randomly Subgraph SEARCH !\n\n");
        UniverseSet baselist(n);
        for (int v=1; v<=n; v++) {
            if (g.IsTerminal(v)) baselist.Insert(v);
        }
        BinaryHeap <EdgeCost> binheap(n); // = new BinaryHeap<ArcCost> (n);
        VoronoiData vordata(n);// = new VoronoiData(n);
        PartitionPreProcess(g,config,vordata,baselist,binheap);

        int times=config.partnum*4/3;
        zeroedge[0]=times;
        double delttimelimit=15;////need to change!!!!!!!!!!!!!!!!
    
        for(int time=1;time<=times;time++)
        {
            printf("---------------------begin to solve partition %d------------------------\n",time);

            double begin_partition_time=GetTime();
            Partition(g,config,vordata);
            fprintf (stdout, "partition ptime %.6f\n",GetTime()-begin_partition_time);

            EdgeCost sumofpartcost=0;

            double beginsolvepartitiontime=GetTime();
            Graph g1;
            g1.ReadSubSTP(g,1);

            ExecutionLog executionLog(&g1, beginsolvepartitiontime, config.TIME_LIMIT);
            SteinerSolution bestSolution(&g1);

            config.TIME_LIMIT=GetTime()-begin_time+delttimelimit;
            EdgeCost partcost=RunMultistartforPartition(g1, config, executionLog, &bestSolution);
            sumofpartcost+=partcost;

            if (bestSolution.EdgeCount()!=0) {
                int v,w;
                for (size_t e = 1; e <= g1.EdgeCount(); ++e)
                if (bestSolution.Contains(e))
                {
                    zeroedge[oldedgeid[e]]++;
                }
            }

            printf("-----------------------finished partition %d---------------------------\n\n",time);
        }
        config.TIME_LIMIT=TIME_LIMIT;
        printf("\nConnect partitions , BEGIN COMPLETE SEARCH !\n\n");
    }
    else printf("\nBEGIN PUW SEARCH !\n\n");

    ExecutionLog executionLog(&g, begin_time, config.TIME_LIMIT);
    SteinerSolution bestSolution(&g);
    
    EdgeCost answercost=RunMultistart(g, config, executionLog, &bestSolution, zeroedge);
    //cout<<"partition"<<endl;
    //return;

    if (!config.LOG_FILENAME.empty()) {
		ofstream logFile(config.LOG_FILENAME.c_str());
		if (logFile.is_open()) {
			logFile << "SECTION Comment" << endl
				<< "Name \"" << filename << "\"" << endl
				<< "Problem \"SPG\"" << endl
				<< "Program \"pbpuw\"" << endl
				<< "End" << endl
				<< endl
				<< "SECTION Solutions" << endl;
			for (size_t i = 0; i < executionLog.solCost.size(); ++i) {
				logFile << "Solution " << fixed << executionLog.solCost[i].second << " " << fixed << executionLog.solCost[i].first << endl;
			}
			logFile << "End" << endl
				<< endl
				<< "SECTION Run" << endl
				<< "Threads 1" << endl
				<< "Time " << GetTime()-begin_time << endl
				<< "Dual 0" << endl;
			logFile << "Primal " << fixed << answercost << endl;
			logFile << "End" << endl
				<< endl;

			if (!executionLog.solCost.empty()) {
				logFile << "SECTION Finalsolution" << endl;

				size_t numVertices = 0;
				int sum=0;
				for (int v = 1; v <= g.VertexCount(); ++v) {
					sum++;
					if (executionLog.bestSolution.GetDegree(v) > 0 || g.IsTerminal(v))
						++numVertices;
				}
				logFile << "Vertices " << numVertices << endl;
				for (int v = 1; v <= g.VertexCount(); ++v) {
					if (executionLog.bestSolution.GetDegree(v) > 0 || g.IsTerminal(v))
						logFile << "V " << v << endl;
				}
				logFile << "Edges " << executionLog.bestSolution.EdgeCount() << endl;
				for (size_t e = 1; e <= g.EdgeCount(); ++e) {
					if (executionLog.bestSolution.Contains(e))
						logFile << "E " << g.GetFirstEndpoint(e) << " " << g.GetSecondEndpoint(e) << endl;
				}
				logFile << "End" << endl;
			}
			logFile.close();
		}
	}
}