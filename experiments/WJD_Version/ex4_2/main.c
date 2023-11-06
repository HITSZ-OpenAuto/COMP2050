#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 1000

void Merge(int* InitList, int* MergedList, int Left, int Right, int Mid){
	int i=Left, j=Mid+1, k = Left;
	while(i<=Mid&&j<=Right){
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
	while(i<=Mid) {
		MergedList[k] = InitList[i];
		++i;
		++k;
	}
	while(j<=Right) {
		MergedList[k] = InitList[j];
		++j;
		++k;
	}
}

//Todo
//需要返回一个数组，数组元素是原始数组里最大的k个数
//注意按照从小到大的顺序排序
//可以自行添加其他辅助函数,可以根据实际需要修改函数参数
int *solve2(int* InputData, int Length, int OutputLength){
	int *res, *MergedList, MergeLength = 2;
	res = (int*) malloc(sizeof(int)*OutputLength);
	MergedList = (int*) malloc(sizeof(int)*Length);
	if(res==NULL||MergedList==NULL) exit(1);
	memcpy(MergedList,InputData,sizeof(int)*Length);
	while(MergeLength<Length){
		int start = 0;
		int mod = Length%MergeLength;
		while(start+MergeLength<=Length){
			Merge(InputData, MergedList, start, start+MergeLength-1, start+(MergeLength-1)/2);
			start+=MergeLength;
		}
		memcpy(InputData,MergedList,sizeof(int)*Length);
		if(mod!=0) {
                Merge(InputData, MergedList, start-MergeLength, Length-1, Length-mod-1);
                for(int i=start-MergeLength;i<=Length-1;++i) InputData[i] = MergedList[i];
		}
		MergeLength*=2;
	}
	memcpy(res,MergedList+Length-OutputLength,sizeof(int)*OutputLength);
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
		printf("\n==== Case %d ====\nres : ", case1 + 1);
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
