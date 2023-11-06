#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

# define max_dis 100000

typedef char vextype[20];
typedef struct {
    int N, E;//N是顶点数，E是边数
    int **matrix;//储存邻接矩阵
    vextype *vertex;//存储节点的名字
} Graph;

Graph createGraph(int n,int e);
int isConnected(Graph g);
void nodeDegree(Graph g, int *node_degree);
double clusteringCoefficient(Graph g);
int Diameter(Graph g);
int Radius(Graph g);
int dijkstra(Graph g, int start, int end, int *path);
void printPath(int d, int *diameter_path, Graph g);
int bfs_graph(Graph g,int node_id,bool* visited);
void dfs_graph(Graph g,int node_id,bool* visited);
int find_closest_node(int total_node_num,bool visited[], int *path_length_to_each);
void Eccentricity(Graph g,int* distance);

/**
 * 创建一个节点数为n的图
 * @param n 节点个数
 * @return 返回这个图
 */
Graph createGraph(int n,int e) {
    int i, j;
    Graph g;
    g.N = n;
    g.E = e;
    g.matrix = (int **) malloc(sizeof(int *) * g.N);
    if(g.matrix == NULL) exit(1);
    for (i = 0; i < n; i++) {
        g.matrix[i] = (int *) malloc(sizeof(int) * g.N);
        if (g.matrix[i]==NULL) exit(1);
    }
    for (i = 0; i < g.N; i++) {
        for (j = 0; j < g.N; j++) {
            g.matrix[i][j] = max_dis;
        }
    }
    for (i = 0; i < g.N; i++) {
        g.matrix[i][i] = 0;
    }
    g.vertex = (vextype *) malloc(sizeof(vextype) * g.N);
    return g;
}

/**
 * 根据距离d和路径数组path输出路径，这样就不需要路径的节点数也能正确输出路径
 * @param d 路径长度
 * @param diameter_path 储存路径的数组
 * @param g 图
 */
void printPath(int d, int *diameter_path, Graph g)
{
    int k = 0;
    int path_length = 0;
    printf("Path: ");
    do {
        printf("%s->", g.vertex[diameter_path[k]]);
        path_length += g.matrix[diameter_path[k]][diameter_path[k + 1]];
        k++;
    } while (path_length < d);
    printf("%s\n", g.vertex[diameter_path[k]]);
}


/**
 * 无向图的广度优先遍历
 * @param g 图
 * @param node_id 遍历起始节点ID
 * @param visited 标记是否已找到的数组
 * @return 连通返回1，否则返回0
 */
int bfs_graph(Graph g,int node_id,bool* visited){
    int nodenum=0; // 已找到节点数目
    int queue_length = g.N+1;
    int* node_queue = (int*)malloc(sizeof(int)*queue_length); // 维护一个队列来存放节点ID, 采用少用一个元素空间方式
    if(node_queue == NULL) exit(1); //分配内存失败, 错误码1
    int first=0,last=0,curr=0; // first是队列头指针(下标), last是队列尾指针(下标), curr是队列当前访问的节点ID(此处设定从0号节点开始)
    node_queue[0]=curr; // 使0号节点先入队
    visited[curr] = true; // 标记: 0号节点已找到
    ++nodenum; // 已找到节点数目+1
    ++last; // 队列尾指针向前移动
    while(first!=last){ // 队列非空时
        curr=node_queue[first]; // 访问队头对应节点
        first = (first+1)%queue_length; // 出队, 头指针向前移动
        for(int i=0;i<g.N;++i){
            if(g.matrix[curr][i]!=max_dis && visited[i]==false){ //某节点与当前节点的距离不为无穷大(有边相连)且未被访问过
                node_queue[last]=i; //此节点入队
                 visited[i]=true; // 找到标记置为true
                 ++nodenum; // 已找到节点数目+1
                 if(nodenum==g.N) return 1; // 如果已经找到了所有节点, 则直接返回1
                last = (last+1)%queue_length; //尾指针向前移动
            }
        }
    }
    free(node_queue);
    free(visited);
    return 0;
}

/**
 * 无向图的深度优先遍历
 * @param g 图
 * @param node_id 遍历起始节点ID
 * @param visited 访问标记数组
 * @return 无返回值
 */
void dfs_graph(Graph g,int node_id,bool* visited){
    int i=0;
    visited[node_id] = true; // 访问标记置为true
    for(i=0;i<g.N;++i){ // 对于所有和该节点有边相连且未访问过的节点, 访问它们并以它们为起始作深度优先搜索(递归过程)
         if(g.matrix[node_id][i] != max_dis&&visited[i]==false) dfs_graph(g,i,visited);
    }
    if(i==g.N)return; // 若该节点的所有节点均被遍历, 则退出函数
}
/**
 * 判断图是否连通
 * @param g 图
 * @return 连通返回1，否则返回0
 */
int isConnected(Graph g) {
    bool* visited = (bool*) malloc(sizeof(bool)*g.N); // 访问/找到标记数组
    if(visited == NULL) exit(1); //分配内存失败, 错误码1
    memset(visited,false,sizeof(bool)*g.N); // 置为false(0), 表示所有点均未找到/访问过
    // 广度优先搜索版本
    return bfs_graph(g,0,visited); // 表示从节点0开始进行广度优先遍历
    // 深度优先搜索版本
//    dfs_graph(g,0,visited); // 表示从节点0开始进行深度优先遍历
//    for(int i=0;i<g.N;++i)
//        if (visited[i]==false) return 0; // 检查visited数组, 如有一为假即表示图非连通(某个节点不包含在图的一个连通分量中)
//    free(visited);
//    return 1;
}

/**
 * 计算每个点的度
 * @param g 图
 * @param node_degree 将每个点的度写到这个数组中
 */
void nodeDegree(Graph g, int *node_degree) {
    if(node_degree == NULL) exit(1); //分配内存失败, 错误码1
    memset(node_degree,0,g.N*sizeof(int)); // 将node_degree数组中每个元素均置为0
    for (int i=0;i<g.N;++i){
        for(int j=i+1;j<g.N;++j){ // 因为是无向图, 所以只用遍历邻接矩阵的右上三角即可(不包含主对角线)
                if(g.matrix[i][j]!=max_dis){
                    ++node_degree[i]; // 同时改变节点i和j的度
                    ++node_degree[j];
                }
        }
    }
}

/**
 * 计算图的聚类系数
 * @param g 图
 * @return 返回聚类系数
 */
double clusteringCoefficient(Graph g) {
    double average_clustering_coefficient=0; // 初始化聚类系数变量为0
    for(int i=0;i<g.N;++i){
            int*connect_node = (int *)calloc(g.N,sizeof(int)); // 标记与某节点相连的节点ID
            if(connect_node == NULL) exit(1); //分配内存失败, 错误码1
            int connect_node_num = 0; // 与某节点相连的节点数(节点的度)
            int node_interconnection = 0; // 与某节点相连的节点之间的边数
            double clustering_coefficient_for_node=0; // 某单个节点的聚类系数
        for(int j=0;j<g.N;++j){ // 找到所有与某节点相连的节点
            if(j==i) continue;
            if(g.matrix[i][j]!=max_dis) {
                connect_node[connect_node_num] = j;
                ++connect_node_num;
            }
        }
        if(connect_node_num==0||connect_node_num==1) continue; // 如果节点的度为0或1, 聚类系数为0, 不参与下面计算
        for(int k=0;k<connect_node_num;++k){ // 计算与某节点相连的节点之间的边数
            for(int m=k+1;m<connect_node_num;++m){
                if(g.matrix[connect_node[k]][connect_node[m]] != max_dis) ++node_interconnection;
            }
        }
        clustering_coefficient_for_node = (double)node_interconnection*2/(connect_node_num*(connect_node_num-1)); // 聚类系数公式
        average_clustering_coefficient +=clustering_coefficient_for_node;
        free(connect_node);
    }
    average_clustering_coefficient = average_clustering_coefficient/g.N; // 取平均
    return average_clustering_coefficient;
}

/**
 * 寻找距离起始点最近的点
 * @param total_node_num 节点总数
 * @param visited访问标记
 * @param path_length_to_each 路径长度数组
 * @return 最近节点ID
 */
int find_closest_node(int total_node_num,bool visited[], int *path_length_to_each){
    int min_weight = max_dis, ret=-1;
    for(int i=0;i<total_node_num;++i){
        if(visited[i]==false && path_length_to_each[i]<min_weight) { // 假如此节点未被访问过且源点到此节点的距离更小
                min_weight = path_length_to_each[i];
                ret = i; // 返回值设为这个节点ID
        }
    }
    return ret;
}

/**
 * 使用dijkstra算法计算单源最短路径
 * @param g 图
 * @param start 起点
 * @param end 终点
 * @param path 从start到end的路径, [start,...,end]
 * @return 路径长度
 */
int dijkstra(Graph g, int start, int end, int *path)
{
    int path_length=max_dis,closest_node; // path_length 路径长度, closest_node离源点最近的节点ID
    if(start<0||start>g.N||end<0||end>g.N) return path_length; // 要求寻找不存在的结点路径, 直接返回
    bool* visited = (bool *)calloc(g.N,sizeof(bool)); // 利用calloc申请内存并初始化访问标记数组为false(0)
    if(visited == NULL) exit(1); //分配内存失败, 错误码1
    int* path_length_to_each = (int *)calloc(g.N,sizeof(int)); // 从源点到每个节点的最短路径长度数组
    if(path_length_to_each == NULL) exit(1); //分配内存失败, 错误码1
    memcpy(path_length_to_each,g.matrix[start],g.N*sizeof(int)); // 初始化路径长度数组为邻接矩阵中对应值
    int* last_node = (int *)calloc(g.N,sizeof(int)); // 标记到达某一个节点上一步所到达的节点
    if(last_node == NULL) exit(1); //分配内存失败, 错误码1
    memset(last_node,-1,sizeof(int)*g.N); // 将标记数组各元素置为-1(表示没有从源点到该点的路径)
    for(int i=0;i<g.N;++i)
        if(path_length_to_each[i]!=max_dis&&i!=start)
            last_node[i]=start; // 将源点可到达的节点(除自身外)的"上一节点标记"记为源点ID
    visited[start]=true; // 源点处于已访问节点集合中
    closest_node = find_closest_node(g.N,visited,path_length_to_each); // 更新最近节点
    while(closest_node!=-1){
            for(int i=0;i<g.N;++i){
                    if (i==closest_node) continue; // 自己到自己的路径不参与计算
                    if(path_length_to_each[i]>(path_length_to_each[closest_node]+g.matrix[closest_node][i])){ // 假如从现在的节点到某节点的距离比从目前已知到这个节点的距离更短
                        path_length_to_each[i] = (path_length_to_each[closest_node]+g.matrix[closest_node][i]); // 更新到达这个节点的距离
                        last_node[i] = closest_node; // 更新到达这个节点之前经过的上一个节点ID
                    }
            }
            visited[closest_node] = true; // 节点访问标记置为true
        closest_node=find_closest_node(g.N,visited,path_length_to_each); // 更新最近节点
    }
    if(last_node[end]!=-1) { // 从当前节点向前找
            int last_node_id = end, reverse_signal = g.N;
            while(last_node_id!=-1){ // 只要向前找到的节点不是-1(说明源点还未加入路径中)
                path[--reverse_signal] = last_node_id; // 更新路径
                last_node_id = last_node[last_node_id]; // 再往前找一个节点
            }
           memmove(path,path+reverse_signal,sizeof(int)*(g.N-reverse_signal)); // 路径是从后往前存储的, 源点ID所在内存单元与path有偏移, 需要移动对齐
    }
    path_length = path_length_to_each[end]; // 待返回路径长为路径长度数组中end下标对应的元素
    free(path_length_to_each);
    free(visited);
    return path_length;
}

/**附加题
 * 用Floyd算法计算图中各节点的离心率。
 * @param g 图
 * @param distance 用于存储图中每个节点离心率的一维数组
 * @return 无返回值
 */
 void Eccentricity(Graph g,int* distance){
     int i,j,k;
     // 维护一个二维数组来存储任意两点间的最短距离, 初始值为邻接矩阵中的值
     int** distance_matrix;
     distance_matrix = (int **) malloc(sizeof(int *) * g.N);
    for (i = 0; i < g.N; i++) {
        distance_matrix[i] = (int *) malloc(sizeof(int) * g.N);
        distance_matrix[i] = memcpy(distance_matrix[i],g.matrix[i],g.N*sizeof(int));
    }
      for(i=0;i<g.N;++i){ // 每次将节点i加入已访问集合
        for(j=0;j<g.N;++j){
            for(k=0;k<g.N;++k){
                    // 更新j, k节点间最短距离
                    distance_matrix[j][k] = distance_matrix[j][k]<(distance_matrix[j][i]+distance_matrix[i][k])?distance_matrix[j][k]:(distance_matrix[j][i]+distance_matrix[i][k]);
            }
        }
      }
      // 求解每个节点的离心率
      for(i=0;i<g.N;++i){
        for(j=0;j<g.N;++j) distance[i] = distance_matrix[i][j]>distance[i]?distance_matrix[i][j]:distance[i];
      }
       for (i = 0; i < g.N; i++) {
        free(distance_matrix[i]);
    }
    free(distance_matrix); // 释放内存
 }

/**附加题
 * 计算图的直径。提示：Floyd算法
 * @param g 图
 * @return 直径的长度
 */
int Diameter(Graph g) {
    int* distance = (int*) calloc(g.N,sizeof(int));
    Eccentricity(g,distance);
    int maxnum = 0;
    for(int i=0;i<g.N;++i){
        maxnum = maxnum>distance[i]?maxnum:distance[i]; // 最大数(最大离心率)即为图的直径
    }
    free(distance);
    return maxnum;
}


/**附加题
 * 计算图的半径
 * @param g 图
 * @return 半径长度
 */
int Radius(Graph g) {
    int* distance = (int*) calloc(g.N,sizeof(int));
    Eccentricity(g,distance);
    int minnum = max_dis; // 最小数(最小离心率)即为图的半径
    for(int i=0;i<g.N;++i){
        minnum = minnum<distance[i]?minnum:distance[i];
    }
    free(distance);
    return minnum;
}


int main() {
    int node_num;
    int edge_num;
    int ca = 1;
    if (freopen("stu.in", "r", stdin) == NULL) {
        printf("There is an error in reading file stu.in");
    }
    while (scanf("%d %d\n", &node_num, &edge_num) != EOF) {
        printf("\ncase %d:\n", ca++);
        int start_idx, end_idx, weight;
        Graph g = createGraph(node_num,edge_num);
        for(int i = 0; i < node_num; i++) {
            sprintf(g.vertex[i], "%d", i);
        }
        for (int i = 0; i < edge_num; i++) {
            scanf("%d %d %d\n", &start_idx, &end_idx, &weight);
            g.matrix[start_idx][end_idx] = weight;
            g.matrix[end_idx][start_idx] = weight;
        }

        printf("connected: %d\n", isConnected(g));

        int *degree = (int *)malloc(sizeof(int) * g.N);
        nodeDegree(g, degree);
        printf("degree distribution:\n");
        for(int i=0; i<g.N; i++)
        {
            printf("node%s:%d,", g.vertex[i], degree[i]);
        }
        printf("\n");
        free(degree);

        double c = clusteringCoefficient(g);
        printf("clustering coefficient:%f\n", c);

       // if(isConnected(g))
        {
            int *short_path = (int *)malloc(sizeof(int) * g.N);
            int start=1,end=3;
            int dis = dijkstra(g, start, end, short_path);
            if(dis!=max_dis){
                 printf("the shortest path between %d and %d: %d\n", start,end, dis);
                printPath(dis, short_path, g);
            }
            else printf("Path from %d to %d is not found!\n",start,end);
            free(short_path);

            int d = Diameter(g);
            printf("diameter:%d\n", d);

            int r = Radius(g);
            printf("radius:%d\n", r);

        }
    }
    return 0;
}
