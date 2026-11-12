

vector<int> father;
vector<int> siz;
vector<int> terminalnodes;

struct nodebridge{
    int eid;
    long long dis;
};
bool cmpbridges(const nodebridge &a,nodebridge &b)
{
    return a.dis<b.dis;
}

vector<nodebridge>bridgeedges;
vector<nodebridge>bridgeedges1;
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

    terminalnodes.clear();
    for(int v=1;v<=n;v++){
        if(g.IsTerminal(v))
            terminalnodes.push_back(v);
    }

    bridgeedges1.clear();
    nodebridge nb;
    for(int i=1;i<=m;i++)
    {
        int u,v;
        g.GetEndpoints(i,u,v);
        if(vordata.GetBase(u)!=vordata.GetBase(v)){
            nb.eid=i;
            int fu=vordata.GetBase(u);
            int fv=vordata.GetBase(v);
            if(fu>fv) swap(fu,fv);
            nb.dis=10000000ll*fu+fv;
            bridgeedges1.push_back(nb);
        }
    }
    sort(bridgeedges1.begin(),bridgeedges1.end(),cmpbridges);
    bridgeedges.clear();
    if(bridgeedges1.size())
        bridgeedges.push_back(bridgeedges1[0]);
    for(int i=1;i<bridgeedges1.size();i++)
    {
        if(bridgeedges1[i].dis!=bridgeedges1[i-1].dis)
            bridgeedges.push_back(bridgeedges1[i]);
    }

}



int Partition(Graph &g,SteinerConfig &config,VoronoiData &vordata)
{
    int n=g.VertexCount();
    int m=g.EdgeCount();
    int t=g.TerminalCount();

    int k=config.partnum;
    int d=2;/////
    int curk=t; 
    int bridgenum=bridgeedges.size();

    
    for(int v=0;v<=n;v++){
        father[v]=0;
        siz[v]=0;
    }

    for(int v=1;v<=n;v++){
        int fv=vordata.GetBase(v);
        father[v]=fv;
        siz[fv]++;
    }
    for(int i=t-1;i>=0;i--){
        swap(terminalnodes[i],terminalnodes[rand()%(i+1)]);
    }

    for(int i=bridgenum-1;i>=0;i--){
        swap(bridgeedges[i],bridgeedges[rand()%(i+1)]);
    }
    
    int fbase=config.fbase;
    for(int f=t;f>=k;f=f/2)
    {
        for(int i=0;i<bridgenum;i++)
        {
            int u,v;
            int e=bridgeedges[i].eid;
            g.GetEndpoints(e,u,v);
            int fu=GetFatherforPartition(u);
            int fv=GetFatherforPartition(v);
            if(fu==fv) continue;
            if(siz[fu]+siz[fv]<=n/f*d)
            {
                father[fu]=fv;
                siz[fv]+=siz[fu];
                curk--;
                if(curk==k) 
                    break;
            }
        }
        if(curk==k) break;

        nodebridge nb;
        bridgeedges1.clear();
        for(int i=0;i<bridgenum;i++){
            int u,v;
            int e=bridgeedges[i].eid;
            g.GetEndpoints(e,u,v);
            int fu=GetFatherforPartition(u);
            int fv=GetFatherforPartition(v);
            if(fu==fv) continue;
            
            if(fu>fv) swap(fu,fv);
            nb.dis=10000000ll*fu+fv;
            nb.eid=e;
            bridgeedges1.push_back(nb);
        }
        sort(bridgeedges1.begin(),bridgeedges1.end(),cmpbridges);

        bridgeedges.clear();
        if(bridgeedges1.size())
            bridgeedges.push_back(bridgeedges1[0]);
        for(int i=1;i<bridgeedges1.size();i++)
        {
            if(bridgeedges1[i].dis!=bridgeedges1[i-1].dis)
                bridgeedges.push_back(bridgeedges1[i]);
        }

        bridgenum=bridgeedges.size();

        for(int i=bridgenum-1;i>=0;i--){
            swap(bridgeedges[i],bridgeedges[rand()%(i+1)]);
        }
    }

    for(int i=1;i<=g.VertexCount();i++)
    {
        father[i]=GetFatherforPartition(i);
        g.SetBelongPartition(i,father[i]);
    }


    bool verbox=true;
    if(verbox)
    {
        printf("-----------------initial partition info---------------------------\n");
        int num=0;
        int it=0;
        for(int i=0;i<t;i++)
        {
            int v=terminalnodes[i];
            int fv=GetFatherforPartition(v);
            if(fv!=v) continue;
            int tcount=0;
            for(int j=0;j<t;j++)
            {
                int vv=terminalnodes[j];
                vv=GetFatherforPartition(vv);
                if(vv==v)
                    tcount++;
            }
            printf("%d partition %d   %d   %d\n",++it,v,siz[v],tcount);
            num+=siz[v];
        }
        printf("partition num:  %d\n",curk);
        printf("total vertex num:  %d\n",num);
        printf("-----------------initial partition info---------------------------\n");
    }
    return curk;
}

void MultiLevelSearch(Graph &g,SteinerConfig &config,int partnum,vector<int>&zeroedge)
{
    int n=g.VertexCount();
    int m=g.EdgeCount();
    int t=g.TerminalCount();
    int k=partnum;
    int d=2;

    double delttimelimit=config.delttime;
    //delttimelimit=1;
    EdgeCost sumofpartcost;
    int partitionindex=0;
    config.TIME_LIMIT=GetTime()-begin_time;
    for(int i=0;i<terminalnodes.size();i++)
    {
        int v=terminalnodes[i];
        int fv=g.GetBelong_to_Patition(v);
        if(fv!=v) continue;
        printf("---------------------begin to solve partition %d------------------------\n",++partitionindex);
        double beginsolvepartitiontime=GetTime();
        Graph g1;
        g1.ReadSubSTP(g,v);

        ExecutionLog executionLog(&g1, beginsolvepartitiontime, config.TIME_LIMIT);
        SteinerSolution bestSolution(&g1);

        config.TIME_LIMIT+=delttimelimit;
        EdgeCost partcost=RunMultistartforPartition(g1, config, executionLog, &bestSolution);
        sumofpartcost+=partcost;

        if (bestSolution.EdgeCount()!=0) {
            int v,w;
            for (size_t e = 1; e <= g1.EdgeCount(); ++e) 
            if (bestSolution.Contains(e))
            {
                zeroedge[oldedgeid[e]]=1;
            }
        }
        printf("-----------------------finished partition %d---------------------------\n\n",partitionindex);
    }


    vector<nodebridge>bridgeedgesML;
    bridgeedgesML.clear();
    for(int i=0;i<bridgeedges.size();i++)
    {
        int e=bridgeedges[i].eid;
        int u,v,fu,fv;
        g.GetEndpoints(e,u,v);
        fu=GetFatherforPartition(u);
        fv=GetFatherforPartition(v);
        if(fu!=fv){
            bridgeedgesML.push_back(bridgeedges[i]);
        }
    }
    int bridgeedgesMLnum=bridgeedgesML.size();
    for(int i=bridgeedgesMLnum-1;i>=0;i--){
        swap(bridgeedgesML[i],bridgeedgesML[rand()%(i+1)]);
    }

    cout<<"bridgeML  num:  "<<bridgeedgesMLnum<<endl;

    vector<int>zeroedgesub;
    zeroedgesub.resize(m,0);

    int curk=partnum;
    vector<bool>ifcombined;
    ifcombined.resize(n,false);
    int fbase=config.fbase;
    for(int f=k-1;f>=1;f=f/fbase)
    {
        for(int i=0;i<bridgeedgesMLnum;i++)
        {
            int u,v;
            int e=bridgeedgesML[i].eid;
            g.GetEndpoints(e,u,v);
            int fu=GetFatherforPartition(u);
            int fv=GetFatherforPartition(v);
            if(fu==fv) continue;
            if(siz[fu]+siz[fv]<=n/f*d)
            {
                father[fu]=fv;
                ifcombined[fv]=true;
                siz[fv]+=siz[fu];
                curk--;
                if(curk==1) 
                    break;
            }
        }
        if(curk==1) break;

        ////SEARCH PART
        cout<<"hhhhhhhhhhhhhhhhhhhhhhhhhhhh"<<curk<<endl;

        for(int i=1;i<=g.VertexCount();i++)
        {
            father[i]=GetFatherforPartition(i);
            g.SetBelongPartition(i,father[i]);
        }

        int partitionindex=0;
        config.TIME_LIMIT=GetTime()-begin_time;
        for(int i=0;i<terminalnodes.size();i++)
        {
            int v=terminalnodes[i];
            int fv=g.GetBelong_to_Patition(v);
            if(fv!=v||!ifcombined[fv]) continue;
            ifcombined[fv]=false;
            
            printf("---------------------begin to solve partition %d------------------------\n",++partitionindex);
            double beginsolvepartitiontime=GetTime();
            Graph g1;
            g1.ReadSubSTP(g,v);
            for(int e=1;e<=g1.EdgeCount();e++)
            {
                zeroedgesub[e]=zeroedge[oldedgeid[e]];
            }

            config.TIME_LIMIT+=delttimelimit;
            ExecutionLog executionLog(&g1, beginsolvepartitiontime, config.TIME_LIMIT);
            SteinerSolution bestSolution(&g1);
            
            //EdgeCost partcost=RunMultistartforPartition(g1, config, executionLog, &bestSolution);
            EdgeCost partcost=RunMultistart(g1, config, executionLog, &bestSolution,zeroedgesub);
            sumofpartcost+=partcost;

            if (bestSolution.EdgeCount()!=0) {
                int v,w;
                for (size_t e = 1; e <= g1.EdgeCount(); ++e) 
                if (bestSolution.Contains(e)){
                    zeroedge[oldedgeid[e]]=1;
                }
                else zeroedge[oldedgeid[e]]=0;
            }
            printf("-----------------------finished partition %d---------------------------\n\n",partitionindex);
        }
        nodebridge nb;

        bridgeedges1.clear();
        for(int i=0;i<bridgeedgesMLnum;i++){
            int u,v;
            int e=bridgeedgesML[i].eid;
            g.GetEndpoints(e,u,v);
            int fu=GetFatherforPartition(u);
            int fv=GetFatherforPartition(v);
            if(fu==fv) continue;
            
            if(fu>fv) swap(fu,fv);
            nb.dis=10000000ll*fu+fv;
            nb.eid=e;
            bridgeedges1.push_back(nb);
        }
        sort(bridgeedges1.begin(),bridgeedges1.end(),cmpbridges);
        
        bridgeedgesML.clear();
        if(bridgeedges1.size())
            bridgeedgesML.push_back(bridgeedges1[0]);
        for(int i=1;i<bridgeedges1.size();i++)
        {
            if(bridgeedges1[i].dis!=bridgeedges1[i-1].dis)
                bridgeedgesML.push_back(bridgeedges1[i]);
        }

        bridgeedgesMLnum=bridgeedgesML.size();

        for(int i=bridgeedgesMLnum-1;i>=0;i--){
            swap(bridgeedgesML[i],bridgeedgesML[rand()%(i+1)]);
        }
    }
}

void PartitionSearch( Graph &g, SteinerConfig &config, char *filename)
{
    int partition_flag=(config.puw+1)%2;
    int n=g.VertexCount();
    int m=g.EdgeCount();
    int t=g.TerminalCount();
    double TIME_LIMIT=config.TIME_LIMIT;
    vector<int>zeroedge;
    zeroedge.resize(m+1,0);

    if(partition_flag==1)
    {
        printf("\nBEGIN PARTITION SEARCH !\n\n");
        UniverseSet baselist(n);
        for (int v=1; v<=n; v++) {
            if (g.IsTerminal(v)) baselist.Insert(v);
        }
        BinaryHeap <EdgeCost> binheap(n); // = new BinaryHeap<ArcCost> (n);
        VoronoiData vordata(n);// = new VoronoiData(n);
        PartitionPreProcess(g,config,vordata,baselist,binheap);


        double begin_partition_time=GetTime();
        int partnum=Partition(g,config,vordata);

        MultiLevelSearch(g,config,partnum,zeroedge);

        fprintf (stdout, "partition ptime %.6f\n",GetTime()-begin_partition_time);
        
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
				logFile << "Solution " << fixed << executionLog.solCost[i].second << " " << fixed <<setprecision(12)<< executionLog.solCost[i].first << endl;
			}
			logFile << "End" << endl
				<< endl
				<< "SECTION Run" << endl
				<< "Threads 1" << endl
				<< "Time " << GetTime()-begin_time << endl
				<< "Dual 0" << endl;
			logFile << "Primal " << fixed <<setprecision(12)<< answercost << endl;
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