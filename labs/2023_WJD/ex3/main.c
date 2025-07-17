#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

# define max_dis 100000

typedef char vextype[20];
typedef struct {
    int N, E;//N�Ƕ�������E�Ǳ���
    int **matrix;//�����ڽӾ���
    vextype *vertex;//�洢�ڵ������
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
 * ����һ���ڵ���Ϊn��ͼ
 * @param n �ڵ����
 * @return �������ͼ
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
 * ���ݾ���d��·������path���·���������Ͳ���Ҫ·���Ľڵ���Ҳ����ȷ���·��
 * @param d ·������
 * @param diameter_path ����·��������
 * @param g ͼ
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
 * ����ͼ�Ĺ�����ȱ���
 * @param g ͼ
 * @param node_id ������ʼ�ڵ�ID
 * @param visited ����Ƿ����ҵ�������
 * @return ��ͨ����1�����򷵻�0
 */
int bfs_graph(Graph g,int node_id,bool* visited){
    int nodenum=0; // ���ҵ��ڵ���Ŀ
    int queue_length = g.N+1;
    int* node_queue = (int*)malloc(sizeof(int)*queue_length); // ά��һ����������Žڵ�ID, ��������һ��Ԫ�ؿռ䷽ʽ
    if(node_queue == NULL) exit(1); //�����ڴ�ʧ��, ������1
    int first=0,last=0,curr=0; // first�Ƕ���ͷָ��(�±�), last�Ƕ���βָ��(�±�), curr�Ƕ��е�ǰ���ʵĽڵ�ID(�˴��趨��0�Žڵ㿪ʼ)
    node_queue[0]=curr; // ʹ0�Žڵ������
    visited[curr] = true; // ���: 0�Žڵ����ҵ�
    ++nodenum; // ���ҵ��ڵ���Ŀ+1
    ++last; // ����βָ����ǰ�ƶ�
    while(first!=last){ // ���зǿ�ʱ
        curr=node_queue[first]; // ���ʶ�ͷ��Ӧ�ڵ�
        first = (first+1)%queue_length; // ����, ͷָ����ǰ�ƶ�
        for(int i=0;i<g.N;++i){
            if(g.matrix[curr][i]!=max_dis && visited[i]==false){ //ĳ�ڵ��뵱ǰ�ڵ�ľ��벻Ϊ�����(�б�����)��δ�����ʹ�
                node_queue[last]=i; //�˽ڵ����
                 visited[i]=true; // �ҵ������Ϊtrue
                 ++nodenum; // ���ҵ��ڵ���Ŀ+1
                 if(nodenum==g.N) return 1; // ����Ѿ��ҵ������нڵ�, ��ֱ�ӷ���1
                last = (last+1)%queue_length; //βָ����ǰ�ƶ�
            }
        }
    }
    free(node_queue);
    free(visited);
    return 0;
}

/**
 * ����ͼ��������ȱ���
 * @param g ͼ
 * @param node_id ������ʼ�ڵ�ID
 * @param visited ���ʱ������
 * @return �޷���ֵ
 */
void dfs_graph(Graph g,int node_id,bool* visited){
    int i=0;
    visited[node_id] = true; // ���ʱ����Ϊtrue
    for(i=0;i<g.N;++i){ // �������к͸ýڵ��б�������δ���ʹ��Ľڵ�, �������ǲ�������Ϊ��ʼ�������������(�ݹ����)
         if(g.matrix[node_id][i] != max_dis&&visited[i]==false) dfs_graph(g,i,visited);
    }
    if(i==g.N)return; // ���ýڵ�����нڵ��������, ���˳�����
}
/**
 * �ж�ͼ�Ƿ���ͨ
 * @param g ͼ
 * @return ��ͨ����1�����򷵻�0
 */
int isConnected(Graph g) {
    bool* visited = (bool*) malloc(sizeof(bool)*g.N); // ����/�ҵ��������
    if(visited == NULL) exit(1); //�����ڴ�ʧ��, ������1
    memset(visited,false,sizeof(bool)*g.N); // ��Ϊfalse(0), ��ʾ���е��δ�ҵ�/���ʹ�
    // ������������汾
    return bfs_graph(g,0,visited); // ��ʾ�ӽڵ�0��ʼ���й�����ȱ���
    // ������������汾
//    dfs_graph(g,0,visited); // ��ʾ�ӽڵ�0��ʼ����������ȱ���
//    for(int i=0;i<g.N;++i)
//        if (visited[i]==false) return 0; // ���visited����, ����һΪ�ټ���ʾͼ����ͨ(ĳ���ڵ㲻������ͼ��һ����ͨ������)
//    free(visited);
//    return 1;
}

/**
 * ����ÿ����Ķ�
 * @param g ͼ
 * @param node_degree ��ÿ����Ķ�д�����������
 */
void nodeDegree(Graph g, int *node_degree) {
    if(node_degree == NULL) exit(1); //�����ڴ�ʧ��, ������1
    memset(node_degree,0,g.N*sizeof(int)); // ��node_degree������ÿ��Ԫ�ؾ���Ϊ0
    for (int i=0;i<g.N;++i){
        for(int j=i+1;j<g.N;++j){ // ��Ϊ������ͼ, ����ֻ�ñ����ڽӾ�����������Ǽ���(���������Խ���)
                if(g.matrix[i][j]!=max_dis){
                    ++node_degree[i]; // ͬʱ�ı�ڵ�i��j�Ķ�
                    ++node_degree[j];
                }
        }
    }
}

/**
 * ����ͼ�ľ���ϵ��
 * @param g ͼ
 * @return ���ؾ���ϵ��
 */
double clusteringCoefficient(Graph g) {
    double average_clustering_coefficient=0; // ��ʼ������ϵ������Ϊ0
    for(int i=0;i<g.N;++i){
            int*connect_node = (int *)calloc(g.N,sizeof(int)); // �����ĳ�ڵ������Ľڵ�ID
            if(connect_node == NULL) exit(1); //�����ڴ�ʧ��, ������1
            int connect_node_num = 0; // ��ĳ�ڵ������Ľڵ���(�ڵ�Ķ�)
            int node_interconnection = 0; // ��ĳ�ڵ������Ľڵ�֮��ı���
            double clustering_coefficient_for_node=0; // ĳ�����ڵ�ľ���ϵ��
        for(int j=0;j<g.N;++j){ // �ҵ�������ĳ�ڵ������Ľڵ�
            if(j==i) continue;
            if(g.matrix[i][j]!=max_dis) {
                connect_node[connect_node_num] = j;
                ++connect_node_num;
            }
        }
        if(connect_node_num==0||connect_node_num==1) continue; // ����ڵ�Ķ�Ϊ0��1, ����ϵ��Ϊ0, �������������
        for(int k=0;k<connect_node_num;++k){ // ������ĳ�ڵ������Ľڵ�֮��ı���
            for(int m=k+1;m<connect_node_num;++m){
                if(g.matrix[connect_node[k]][connect_node[m]] != max_dis) ++node_interconnection;
            }
        }
        clustering_coefficient_for_node = (double)node_interconnection*2/(connect_node_num*(connect_node_num-1)); // ����ϵ����ʽ
        average_clustering_coefficient +=clustering_coefficient_for_node;
        free(connect_node);
    }
    average_clustering_coefficient = average_clustering_coefficient/g.N; // ȡƽ��
    return average_clustering_coefficient;
}

/**
 * Ѱ�Ҿ�����ʼ������ĵ�
 * @param total_node_num �ڵ�����
 * @param visited���ʱ��
 * @param path_length_to_each ·����������
 * @return ����ڵ�ID
 */
int find_closest_node(int total_node_num,bool visited[], int *path_length_to_each){
    int min_weight = max_dis, ret=-1;
    for(int i=0;i<total_node_num;++i){
        if(visited[i]==false && path_length_to_each[i]<min_weight) { // ����˽ڵ�δ�����ʹ���Դ�㵽�˽ڵ�ľ����С
                min_weight = path_length_to_each[i];
                ret = i; // ����ֵ��Ϊ����ڵ�ID
        }
    }
    return ret;
}

/**
 * ʹ��dijkstra�㷨���㵥Դ���·��
 * @param g ͼ
 * @param start ���
 * @param end �յ�
 * @param path ��start��end��·��, [start,...,end]
 * @return ·������
 */
int dijkstra(Graph g, int start, int end, int *path)
{
    int path_length=max_dis,closest_node; // path_length ·������, closest_node��Դ������Ľڵ�ID
    if(start<0||start>g.N||end<0||end>g.N) return path_length; // Ҫ��Ѱ�Ҳ����ڵĽ��·��, ֱ�ӷ���
    bool* visited = (bool *)calloc(g.N,sizeof(bool)); // ����calloc�����ڴ沢��ʼ�����ʱ������Ϊfalse(0)
    if(visited == NULL) exit(1); //�����ڴ�ʧ��, ������1
    int* path_length_to_each = (int *)calloc(g.N,sizeof(int)); // ��Դ�㵽ÿ���ڵ�����·����������
    if(path_length_to_each == NULL) exit(1); //�����ڴ�ʧ��, ������1
    memcpy(path_length_to_each,g.matrix[start],g.N*sizeof(int)); // ��ʼ��·����������Ϊ�ڽӾ����ж�Ӧֵ
    int* last_node = (int *)calloc(g.N,sizeof(int)); // ��ǵ���ĳһ���ڵ���һ��������Ľڵ�
    if(last_node == NULL) exit(1); //�����ڴ�ʧ��, ������1
    memset(last_node,-1,sizeof(int)*g.N); // ����������Ԫ����Ϊ-1(��ʾû�д�Դ�㵽�õ��·��)
    for(int i=0;i<g.N;++i)
        if(path_length_to_each[i]!=max_dis&&i!=start)
            last_node[i]=start; // ��Դ��ɵ���Ľڵ�(��������)��"��һ�ڵ���"��ΪԴ��ID
    visited[start]=true; // Դ�㴦���ѷ��ʽڵ㼯����
    closest_node = find_closest_node(g.N,visited,path_length_to_each); // ��������ڵ�
    while(closest_node!=-1){
            for(int i=0;i<g.N;++i){
                    if (i==closest_node) continue; // �Լ����Լ���·�����������
                    if(path_length_to_each[i]>(path_length_to_each[closest_node]+g.matrix[closest_node][i])){ // ��������ڵĽڵ㵽ĳ�ڵ�ľ���ȴ�Ŀǰ��֪������ڵ�ľ������
                        path_length_to_each[i] = (path_length_to_each[closest_node]+g.matrix[closest_node][i]); // ���µ�������ڵ�ľ���
                        last_node[i] = closest_node; // ���µ�������ڵ�֮ǰ��������һ���ڵ�ID
                    }
            }
            visited[closest_node] = true; // �ڵ���ʱ����Ϊtrue
        closest_node=find_closest_node(g.N,visited,path_length_to_each); // ��������ڵ�
    }
    if(last_node[end]!=-1) { // �ӵ�ǰ�ڵ���ǰ��
            int last_node_id = end, reverse_signal = g.N;
            while(last_node_id!=-1){ // ֻҪ��ǰ�ҵ��Ľڵ㲻��-1(˵��Դ�㻹δ����·����)
                path[--reverse_signal] = last_node_id; // ����·��
                last_node_id = last_node[last_node_id]; // ����ǰ��һ���ڵ�
            }
           memmove(path,path+reverse_signal,sizeof(int)*(g.N-reverse_signal)); // ·���ǴӺ���ǰ�洢��, Դ��ID�����ڴ浥Ԫ��path��ƫ��, ��Ҫ�ƶ�����
    }
    path_length = path_length_to_each[end]; // ������·����Ϊ·������������end�±��Ӧ��Ԫ��
    free(path_length_to_each);
    free(visited);
    return path_length;
}

/**������
 * ��Floyd�㷨����ͼ�и��ڵ�������ʡ�
 * @param g ͼ
 * @param distance ���ڴ洢ͼ��ÿ���ڵ������ʵ�һά����
 * @return �޷���ֵ
 */
 void Eccentricity(Graph g,int* distance){
     int i,j,k;
     // ά��һ����ά�������洢������������̾���, ��ʼֵΪ�ڽӾ����е�ֵ
     int** distance_matrix;
     distance_matrix = (int **) malloc(sizeof(int *) * g.N);
    for (i = 0; i < g.N; i++) {
        distance_matrix[i] = (int *) malloc(sizeof(int) * g.N);
        distance_matrix[i] = memcpy(distance_matrix[i],g.matrix[i],g.N*sizeof(int));
    }
      for(i=0;i<g.N;++i){ // ÿ�ν��ڵ�i�����ѷ��ʼ���
        for(j=0;j<g.N;++j){
            for(k=0;k<g.N;++k){
                    // ����j, k�ڵ����̾���
                    distance_matrix[j][k] = distance_matrix[j][k]<(distance_matrix[j][i]+distance_matrix[i][k])?distance_matrix[j][k]:(distance_matrix[j][i]+distance_matrix[i][k]);
            }
        }
      }
      // ���ÿ���ڵ��������
      for(i=0;i<g.N;++i){
        for(j=0;j<g.N;++j) distance[i] = distance_matrix[i][j]>distance[i]?distance_matrix[i][j]:distance[i];
      }
       for (i = 0; i < g.N; i++) {
        free(distance_matrix[i]);
    }
    free(distance_matrix); // �ͷ��ڴ�
 }

/**������
 * ����ͼ��ֱ������ʾ��Floyd�㷨
 * @param g ͼ
 * @return ֱ���ĳ���
 */
int Diameter(Graph g) {
    int* distance = (int*) calloc(g.N,sizeof(int));
    Eccentricity(g,distance);
    int maxnum = 0;
    for(int i=0;i<g.N;++i){
        maxnum = maxnum>distance[i]?maxnum:distance[i]; // �����(���������)��Ϊͼ��ֱ��
    }
    free(distance);
    return maxnum;
}


/**������
 * ����ͼ�İ뾶
 * @param g ͼ
 * @return �뾶����
 */
int Radius(Graph g) {
    int* distance = (int*) calloc(g.N,sizeof(int));
    Eccentricity(g,distance);
    int minnum = max_dis; // ��С��(��С������)��Ϊͼ�İ뾶
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
