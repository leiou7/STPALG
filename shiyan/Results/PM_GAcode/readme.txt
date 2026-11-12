MultiLevel
1.进行初始划分（划分到每个分区有一定的大小，太小则没有必要进行搜索，太大则搜索的复杂度太大，这里选取一个分区大小的参数）
计算voronoi diagram，每个区域初始时有一个terminal
迭代进行初始合并，遍历所有的边e(u,v)（这条边是两个子图之间的桥梁边）,判断两个顶点所属的子图点数相加是否<=n/f (f=f/2每次迭代指数减少，f参数怎么减少？)
得到一个初始的分区。

2.进行多级合并
继续进行迭代，每次f=f/2:
遍历e(u,v),判断两个顶点所属的子图点数相加是否<=n/f,如果可以则将两个分区进行合并。
对当前的k个新的子图进行搜索(搜索时间参数)得到k个解，对解的边进行标记，然后继续向上一层传递。

3.合并完成，进行最终搜索
当合并的只剩下一个分区的时候，跳出multilevel，然后进行搜索，直至达到时间限制。


参数：
合并规则(connect)：karger's min-cut algorithm，terminal shortest path, randomly
初始划分的子图数量(partn)
合并的时候每次减少的幅度，n/f是合并后区域内点的数量的上限，当前f每次变为之前的1/(fbase)
每个子图的搜索时间(delttime)


./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -connect $connect -partn $partn -fbase $fbase -delttime $delttime -logfilename ${Output_File_Name[i]} -seed 0 -prep 1
