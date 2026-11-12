partition和puw结合
进行partition，根据pnbase将图分为t/pnbase个子图  (进行多次)
对子图跑puw算法得到子树，将子树覆盖到边在原图上给与一定的权值，作为容易被选择的子结构，然后进行构造初始解的时候利用
最后puw对初始解进行搜索
参数：
"args": ["./instances1/R100K02EFSTINT.stp","-puw","1","-partn","0","-pnbase","1000","-parttimes","1","-prep","1","-mstype","1","-msit","65536","-timelimit","3600","-logfilename","./results/R100K02EFSTINT.out"]


     