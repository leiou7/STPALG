partition和puw结合
1、随机抠取p个子图，每个子图包含pnbase个terminal节点
2、对子图跑puw算法得到子树（覆盖子图上的所有terminal节点） 时间限制暂时设置在15s左右
3、将子树覆盖到的边在原图上给与一定的权值(放大或者缩小)，作为容易(困难)被选择的子结构，然后进行构造初始解的时候利用
4、最后puw对初始解进行搜索（puw运行时间要）


参数：
"args": ["./instances1/R100K02EFSTINT.stp","-puw","1","-partn","0","-pnbase","1000","-parttimes","1","-prep","1","-mstype","1","-msit","65536","-timelimit","3600","-logfilename","./results/R100K02EFSTINT.out"]


     