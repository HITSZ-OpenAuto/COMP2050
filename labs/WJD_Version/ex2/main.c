#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct TreeNode {
    int id;
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode, *TreeNodePtr;

typedef struct ListNode {
    struct TreeNode *node; // 队列的值的类型是树结点指针
    struct ListNode *next;
} ListNode, *ListNodePtr;

typedef struct Queue {
    ListNodePtr dummyHead;
    ListNodePtr tail;
    int size;
} *QueuePtr;

// 创建链表的结点
ListNodePtr createListNode(TreeNodePtr node, ListNodePtr next) {
    ListNodePtr curr = (ListNodePtr) (malloc(sizeof(ListNode)));
    curr->node = node;
    curr->next = next;
    return curr;
}

// 创建树的结点
int TreeId = 0;

TreeNodePtr createTreeNode(int val, TreeNodePtr left, TreeNodePtr right) {
    TreeNodePtr curr = (TreeNodePtr) (malloc(sizeof(TreeNode)));
    curr->id = TreeId++;
    curr->val = val;
    curr->left = left;
    curr->right = right;
    return curr;
}

// 单链表队列初始化
QueuePtr InitQueue() {
    QueuePtr queue = (QueuePtr) malloc(sizeof(struct Queue));
    TreeNodePtr dummyTreeNode = createTreeNode(-1, NULL, NULL);
    queue->size = 0;
    queue->dummyHead = createListNode(dummyTreeNode, NULL);
    queue->tail = queue->dummyHead;
    return queue;
}

// 在 queue 的尾部添加一个元素的副本
void EnQueue(QueuePtr queue, TreeNodePtr node) {
    ListNodePtr curr = createListNode(node, NULL);
    queue->tail->next = curr;
    queue->tail = queue->tail->next;
    queue->size++;
}

// 删除 queue 中的第一个元素
void DeQueue(QueuePtr queue) {
    if (queue->size == 0) {
        perror("error! the size of queue is zero when call DeQueue().");
        return;
    }
    ListNodePtr head = queue->dummyHead->next;
    queue->dummyHead->next = head->next;
    queue->size--;
    if (queue->size == 0) queue->tail = queue->dummyHead;
    free(head);
}

// 如果 queue 中没有元素, 返回 true
bool QueueEmpty(QueuePtr queue) {
    return queue->size == 0;
}

// 返回 queue 中第一个元素的引用
TreeNodePtr GetHead(QueuePtr queue) {
    if (QueueEmpty(queue)) {
        perror("error! the size of queue is zero when call front().");
        return NULL;
    } else {
        return queue->dummyHead->next->node;
    }
}

int max(int a, int b) {
    return (a >= b) ? a : b;
}

// 将输入转换为数组
void getDigits(char *buff, int *data) {
    int len = strlen(buff);
    int index = 0;
    for (int i = 0; i < len; i++) {
        int num = 0;
        if (buff[i] == '#') {
            num = -1;
            i++;
        } else {
            while (buff[i] != ' ' && buff[i] != '\0') {
                num = num * 10 + (buff[i++] - '0');
            }
        }
        data[index++] = num;
    }
}

// 创建 dot 可视化文件
void createDotFile(const char *filename, TreeNodePtr root, int MaxSize) {
    FILE *fp = fopen(filename, "w");    // 文件指针
    if (fp == NULL) {   // 为NULL则返回
        printf("File cannot open!");
        exit(0);
    }
    fprintf(fp, "digraph G {\n");   // 开头
    // 利用层次遍历构造
    QueuePtr queue = InitQueue();
    EnQueue(queue, root);
    int id = 1;
    while (!QueueEmpty(queue)) { // 若队列不空，继续遍历。否则，遍历结束
        TreeNodePtr curr = GetHead(queue);
        DeQueue(queue);
        if (curr == NULL) continue;
        fprintf(fp, "%d [shape=circle, label=\"%d\"];\n", curr->id, curr->val);
        if (curr->left != NULL) { // 如果有左孩子，左孩子入队
            EnQueue(queue, curr->left);
            fprintf(fp, "%d->%d;\n", curr->id, curr->left->id);
        }
        id++;
        // 中间虚拟节点
        fprintf(fp, "_n%d [shape=circle, label=\"#\", style=invis];\n", id);
        fprintf(fp, "%d->_n%d [style=invis, weight=10];\n", curr->id, id);
        if (curr->right != NULL) { // 如果有右孩子，右孩子入队
            EnQueue(queue, curr->right);
            fprintf(fp, "%d->%d;\n", curr->id, curr->right->id);
        }
        id++;
    }
    fprintf(fp, "}\n"); // 结尾
    fclose(fp); // 关闭IO
}


// 绘制二叉树图片,调用 createDotFile 并使用 system 执行命令
void plot(TreeNodePtr tree_root, int i, int size, char *name) {
    char tree_filename[50], paint_tree[100];
    sprintf(tree_filename, "./%s_%d.dot", name, i);
    createDotFile(tree_filename, tree_root, size);
    sprintf(paint_tree, "dot -Tpng %s -o ./%s_%d.png", tree_filename, name, i);
    // puts(paint_tree);
    system(paint_tree);
}


// 删除二叉树
void destoryTree(TreeNodePtr root) {
    if (!root) return;
    if (root->left) {
        destoryTree(root->left);
        root->left = NULL;
    }
    if (root->right) {
        destoryTree(root->right);
        root->right = NULL;
    }
    free(root);
}

/** TODO:  任务一：请你通过队列来实现层次遍历构建二叉树，并返回二叉树的头结点 */
TreeNodePtr createTreeWithLevelOrder(int *data, int size) {
    if (*data==-1) return NULL; // 如果第一个字符即为#, 则说明没有根节点, 返回空
    QueuePtr data_queue;
    data_queue = InitQueue(); // 创建并初始化用于存储树结点的队列
    int cnt =0; // 下标引用data数组中的数据, 用于给树结点赋值
    bool is_root = true; // 标记变量, 是否为根节点(需要保存此时的父结点指针来传出)
    TreeNodePtr child_node,father_node,root_node;
    father_node = createTreeNode(*data,NULL,NULL);
    EnQueue(data_queue,father_node); // 先创建根节点并入队
    while(!QueueEmpty(data_queue)&& cnt<size){
        father_node = GetHead(data_queue);
        DeQueue(data_queue); // 第一个结点出队(此处用获得队列中第一个结点保存的树节点指针+删除队列中第一个结点的方法来实现)
        // 以下创建上述结点的左右孩子
        // 创建左孩子
        ++cnt;
        if(cnt<size && data[cnt]!=-1 && father_node!=NULL){ // 如果下标不越界, 且读到的数据不代表空节点, 且此父结点非空
            child_node= createTreeNode (data[cnt],NULL,NULL); // 创建左孩子结点
            father_node->left = child_node; // 将左孩子结点指针赋给上述父结点指针
            EnQueue(data_queue,child_node); // 左孩子结点入队, 当轮到它出队时, 正好就到了创建它的子节点的时候
        }
        else { // 如果下标越界或读到的数据代表空节点, 则将其子节点设为空, 并将其入队
            if(father_node!=NULL) father_node->left = NULL;
            EnQueue(data_queue,NULL);
        }
        // 创建右孩子, 操作和上述创建左孩子时相同
        ++cnt;
        if(cnt<size && data[cnt]!=-1 && father_node!=NULL){
            child_node = createTreeNode (data[cnt],NULL,NULL);
            father_node->right = child_node;
            EnQueue(data_queue,child_node);
        }
        else {
            if(father_node!=NULL)   father_node->right = NULL;
            EnQueue(data_queue,NULL);
        }
        // 如果此父节点是根节点, 则将father_node传给root_node(根节点指针), 在函数结束时传出. 该保存只会进行一次.
        // 此处采用cnt是否为2判断也可
        if(is_root) {
            root_node = father_node;
            is_root = false;
        }
    }
    return root_node;
}

/**
 * ================================================
 * ||                  前序遍历                   ||
 * ================================================
 */
void preOrderTraverse(TreeNodePtr root) {
    if(root == NULL) return;
    printf("%d ",root->val);
    preOrderTraverse(root->left);
    preOrderTraverse(root->right);
}

/**
 * ================================================
 * ||                  中序遍历                   ||
 * ================================================
 */
void inOrderTraverse(TreeNodePtr root) {
    if(root == NULL) return;
    inOrderTraverse(root->left);
    printf("%d ",root->val);
    inOrderTraverse(root->right);
}

/**
 * ================================================
 * ||                  后序遍历                   ||
 * ================================================
 */
void postOrderTraverse(TreeNodePtr root) {
    if(root == NULL) return;
    postOrderTraverse(root->left);
    postOrderTraverse(root->right);
    printf("%d ",root->val);
}


/** ToDO: 任务二：请你通过递归求取该二叉树的所有左子叶权重之和 */
int sumOfLeftLeaves(TreeNodePtr root) {
    if(root == NULL) return 0;
    if(root->left && root->left->left == NULL && root->left->right == NULL)
        return root->left->val+sumOfLeftLeaves(root->right);
    return sumOfLeftLeaves(root->left)+sumOfLeftLeaves(root->right);
}

/** TODO: 任务三：请你通过递归求取该树的镜像，即翻转该二叉树 */
TreeNodePtr invertTree(TreeNodePtr root) {
    if(root == NULL) return NULL;
    invertTree(root->left);
    TreeNodePtr temp = NULL;
    temp = root->left;
    root->left = root->right;
    root ->right = temp;
    invertTree(root->left);
    return root;
}


int main() {

    int SIZE = 128;
    int MAX_NUM = 10;
    char buff[SIZE];
    char num[MAX_NUM];
    bool use_graphviz = true;//可视化开关
    /**
     * ===============================================================
     * ||                   End Configuration                       ||
     * ===============================================================
     */

    // printf("Read data...\n");
    FILE *fp = fopen("./test.txt", "r");
    if (!fp) {
        perror("打开文件时发生错误");
        return -1;
    } else {
        int i = 0;
        // printf("success!\n");
        /**
         * ===============================================================
         * ||                   Read data here                          ||
         * ===============================================================
         */
        while (fgets(num, MAX_NUM, fp) && fgets(buff, SIZE, fp)) {
            char *nextline = strchr(buff, '\n');          //查找换行符
            if (nextline)                            //如果find不为空指针
                *nextline = '\0';                    //就把一个空字符放在这里
			*nextline = strchr(buff, '\r'); 		 //查找回车符
			if (nextline)                            //如果find不为空指针
            *nextline = '\0';                    //就把一个空字符放在这里
            printf("Case %d, data: %s, nodes number: %s", i, buff, num);
            int size = atoi(num);
            int data[size];
            getDigits(buff, data);

            /**
             * ===============================================================
             * ||       你的任务在这里，当然你也可以以任意方式修改函数的原型          ||
             * ===============================================================
             */

            /** 任务一 */
            TreeNodePtr tree_root = createTreeWithLevelOrder(data, size);
            printf("Answer for task 1 is: \n");
            printf("preOrderTraverse is:");
            preOrderTraverse(tree_root);
            printf("\n");
            printf("inOrderTraverse is:");
            inOrderTraverse(tree_root);
            printf("\n");
            printf("postOrderTraverse is:");
            postOrderTraverse(tree_root);
            printf("\n");

            /** 通过 graphviz 可视化，附加题，勿删，助教测试使用 */
            if (use_graphviz) {
                plot(tree_root, i, size, "tree");
            }

            /** 任务二 */
            int weight_sum = sumOfLeftLeaves(tree_root);
            printf("Answer for task 2 is : %d \n", weight_sum);

            /** 任务三 */
            TreeNodePtr invert_tree_root = invertTree(tree_root);
            printf("inOrderTraverse for task 3 is:");
            inOrderTraverse(invert_tree_root);
            printf("\n\n");

            /** 通过 graphviz 可视化，附加题，勿删，助教测试使用 */
            if (use_graphviz) {
                plot(invert_tree_root, i, size, "invert_tree");
            }

            destoryTree(invert_tree_root);
            tree_root = invert_tree_root = NULL;
            i++;

        }

    }

    fclose(fp);

}
