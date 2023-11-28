#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node{
    char StuID[11];
    int Grade;
    struct node *next;
}StudentLinkedListNode; 

/* 创建相交链表 */
void createCrossLink(StudentLinkedListNode* a, StudentLinkedListNode* b, int beforeCross1, int beforeCross2){
    // a和b后面若干结点值相同
    // beforeCross1为跳过的a中的个数，从第beforeCross1 + 1个结点开始相交
    // beforeCross2为跳过的b中的个数，从第beforeCross2 + 1个结点开始相交
    // 相交方法是将b中的前一结点指向a中的首个相交结点
    StudentLinkedListNode *p, *q;
    while(beforeCross1--)a=a->next;
    while(--beforeCross2)b=b->next;
    p = b->next;
    b->next = a;
    //销毁野指针结点
    while(p){
        q = p->next;
        free(p);
        p = q;
    }
}

void destroyCrossLink(StudentLinkedListNode* a, StudentLinkedListNode* b, StudentLinkedListNode* crossNode){
    StudentLinkedListNode* p = crossNode->next, *q;
    while(p){
        q = p->next;
        free(p);
        p = q;
    }
    while(a != crossNode){
        q = a->next;
        free(a);
        a = q;
    }
    while(b != crossNode){
        q = b->next;
        free(b);
        b = q;
    }
    free(crossNode);
}

/* 打印单个节点 */
void printLinkedListNode(StudentLinkedListNode * node){
    printf("{ID:%s, Grade:%d}", node->StuID, node->Grade);
    if(node->next!=NULL){
        printf("->");
    }else{
        printf("\n");
    }

}

/** 输出该表的成绩情况 */
void outputStudentLinkedList(StudentLinkedListNode* head){
	// 指针m指向链表头部 
	StudentLinkedListNode *m = head;
	while (m){
		// 打印m指向的节点数据 
		printLinkedListNode(m);
		// m移向下一个节点 
		m=m->next;
	}
}


/** 新建一个链表node并返回 */
StudentLinkedListNode* studentLinkedListCreate(char student_id[], int grade) {
    // 利用malloc函数创建新节点并分配内存
	StudentLinkedListNode *NewNode = (StudentLinkedListNode*)malloc(sizeof(StudentLinkedListNode));
	// 若分配内存不成功则输出错误信息并结束程序 
	if(NewNode==NULL){
		printf("Fail to create new node!");
		exit(2); 
	} 
	// 节点学生ID数据, 为字符串型量, 使用strcpy函数赋值 
	strcpy(NewNode->StuID,student_id);
	// 节点学生成绩数据, 为整型量, 直接赋值 
	NewNode-> Grade = grade;
	return NewNode; 
}


/** 按照降序插入学生的成绩情况,并返回链表头指针 */
StudentLinkedListNode* studentLinkedListAdd(StudentLinkedListNode* head, StudentLinkedListNode* node) {
    // 采用头插法, 首先将新节点的下一节点连接到现在头节点
	node->next = head;
	// 然后将头节点设置为新节点
	head = node;
	return head;
}


/** 反转链表 */
StudentLinkedListNode* reverseLinkedList(StudentLinkedListNode*head){
	// 定义两个指针作为标记, p指向头节点, q置空 
    StudentLinkedListNode *p =head,*q = NULL;
    head = NULL;
    while(p){
    	// 先找到当前节点的下一个节点并以q标记 
    	q = p->next;
    	// 然后将当前节点的下一个节点设置为原来的(上一步循环结束后的)头 
    	p->next = head;
    	// 然后将新的头节点设置为当前的节点, 即完成了头节点向链表尾部移动一个节点 
    	head = p;
    	// 将p移到下一个节点，下一个循环即处理这个节点
    	p = q;
	}
	// 最后会将head移动到目前链表的最后一个节点, 返回即可 
	return head;
}

/** 找到相交的第一个结点 */
StudentLinkedListNode* findCrossBeginNode(StudentLinkedListNode* class1, StudentLinkedListNode* class2) {
    // 定义两个指针作为标记, p指向链表a的头节点, q置空, 日后用于指向链表b的节点 
    StudentLinkedListNode *p =class1,*q = NULL;
    // 使用双重循环查找第一个重合节点  
    while(p){
    	// 对于指向链表a中的每一个节点的p, 都从链表b的第一个节点开始遍历
    	q = class2;
    	while(q){
    		// 若找到内存地址相同的两个节点, 则返回节点内存地址, 跳出函数体 
    		if(p==q) return p;
    		// 若未找到, 则q移向链表b的下一个节点, 重复比较过程 
        	q = q->next;
		}
		// 若遍历一轮链表b仍未找到, 则p移向链表a的下一个节点, 重复比较过程 
		p = p->next;
	}
	// 若将链表a,b中的所有节点都两两比较后, 仍未找到, 则输出提示信息并结束程序
	printf("Not found!");
	exit(1);
}

int main(){
    freopen("./gradeImport.in","r",stdin);

    StudentLinkedListNode *class1=NULL, *class2=NULL;
    int num1, num2, i;
    char student_id[11];
    int grade;
    int beforeCross1, beforeCross2;
    StudentLinkedListNode *node;

    while(~scanf("%d%d", &num1, &num2)){
        class1 = class2 = NULL;
        // 存储数据到链表
        for(i = 0;i < num1;i++){
            scanf("%s%d", student_id, &grade);
            node = studentLinkedListCreate(student_id, grade);
            class1 = studentLinkedListAdd(class1, node);
        }
        for(i = 0;i < num2;i++){
            scanf("%s%d", student_id, &grade);
            node = studentLinkedListCreate(student_id, grade);
            class2 = studentLinkedListAdd(class2, node);
        }
        printf("* part1:\nclass1:\n");
        outputStudentLinkedList(class1);
        printf("class2:\n");
        outputStudentLinkedList(class2);

        // 反转链表
        class1 = reverseLinkedList(class1);
        class2 = reverseLinkedList(class2);
        printf("* part2:\nclass1:\n");
        outputStudentLinkedList(class1);
        printf("class2:\n");
        outputStudentLinkedList(class2);

        // 生成相交链表
        scanf("%d%d",&beforeCross1,&beforeCross2);
        createCrossLink(class1, class2, beforeCross1, beforeCross2);

        // 打印相交结点
        node = findCrossBeginNode(class1, class2);
        printf("* part3:\n");
        printf("{ID:%s, Grade:%d}\n", node->StuID, node->Grade);

        //销毁相交链表
        destroyCrossLink(class1, class2, node);

        printf("\n");
    }
    return 0;
}
