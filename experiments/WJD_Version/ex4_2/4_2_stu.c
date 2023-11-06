#include<stdio.h>
#include<stdlib.h> 

#define MAX 1000

void Merge(int* InitList, int* MergedList, int Left, int Right){
	int mid = (Right-Left)/2;
	int i=Left, j=mid+1, k = Left, 
	while(i<=mid&&j<=Right){
		if(InitList[i]<InitList[j]){
			MergedList[k] = InitList[i];
			++i;
			++k;
		}
		else{
			MergedList[k] = InitList[j];
			++j;
			++k;
		}
	}
	while(i<=mid) {
		MergedList[k] = InitList[i];
		++i;
		++k;
	}
	memcpy(MergedList+k,InitList+j,sizeof(int)*(Right-j));
}

//Todo
//需要返回一个数组，数组元素是原始数组里最大的k个数
//注意按照从小到大的顺序排序
//可以自行添加其他辅助函数,可以根据实际需要修改函数参数
int *solve2(int* InputData, int Length, int OutputLength){
	int *res, *SortList, MergeLength = 2;
	res = (int*) malloc(sizeof(int)*OutputLength);
	MergedList = (int*) malloc(sizeof(int)*Length);
	while(MergeLength<Length){
		int start = 0;
		while(start<Length){
			Merge(InitList, MergedList, start, start+MergeLength-1);
			start+=MergeLength;
		}
		Merge(InitList, MergedList, start-MergeLength, Length-1);
		MergeLength*=2;
	}
	Merge(InitList, MergedList, 0, Length-1);
	memcpy(res,MergedList-OutputLength,sizeof(int)*OutputLength);
	return res;
}

void test2(){	
	int caseNum; //表示测试轮数
	if (freopen("4_2_input.in", "r", stdin) == NULL) {
		printf("There is an error in reading file 4_2_input.in");
    }	
	scanf("%d", &caseNum);
    int case1 = 0;
    for (; case1 < caseNum; case1++) {
		int k, N;
        scanf("%d%d", &k, &N);
        int arr[MAX];
        int i = 0;
        for (; i < N; i++) {
            scanf("%d", &arr[i]);
        }        
        int *res;
		res = solve2(arr,N,k);
		printf("==== Case %d ====\nres : ", case1 + 1);
        for (i = 0; i < k; i++) {
            printf("%d ",res[i]);
        }
    }
	fclose(stdin);
}


int main(void) {
    test2();
    return 0;
}
