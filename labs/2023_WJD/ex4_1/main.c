#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define N 10000

int a[N]; // ���ڴ洢��ţ������

// �����������汾1
void FastSort(int* a, int length){
	int Pivotkey = a[0],  LowMove = 0, HighMove = length-1;
	int temp;
	bool right = false, stop = true;
	if(length<=1) return;
	while(stop){
	if(right){
		while(a[LowMove]<=Pivotkey) {
                ++LowMove;
                if(LowMove>=HighMove) {
                    stop=false;
                    break;
                }
		}
		temp = a[LowMove];
		a[LowMove] = a[HighMove];
		a[HighMove] = temp;
		right=false;
		}
	else{
		while(a[HighMove]>=Pivotkey) {
                --HighMove;
                if(HighMove<=LowMove){
                    stop=false;
                    break;
                }
		}
		temp = a[HighMove];
		a[HighMove] = a[LowMove];
		a[LowMove] = temp;
		right=true;
		}
	}
	FastSort(a,LowMove);
	FastSort(a+LowMove+1,length-LowMove-1);
}

// �����������汾2
void QuickSort(int* a, int length){
	int Pivotkey = a[0],  LowMove = 0, HighMove = length-1;
	if(length<=1) return;
	while(1){
		while(a[LowMove]<=Pivotkey) {
            ++LowMove;
            if(LowMove==length-1)    break;
		}
		while(a[HighMove]>=Pivotkey) {
                --HighMove;
                if(HighMove==0) break;
		}
		if(LowMove>=HighMove) break;
        int temp = a[LowMove];
		a[LowMove] = a[HighMove];
		a[HighMove] = temp;
    }
    a[0] = a[HighMove];
    a[HighMove] = Pivotkey;
	QuickSort(a,HighMove);
	QuickSort(a+HighMove+1,length-HighMove-1);
}

//Todo
//��Ҫ������λ����ţ��������
//�����������������������,�����޸Ĵ��ε�����
int solve1(int *a, int length){
	int location = length/2;
	//FastSort(a,length);
	QuickSort(a,length);
//	for(int i=0;i<length;++i) printf("%d ",a[i]);
//	printf("\n");
	return a[location];
}

void test1(){
    int caseNum; //��ʾ��������
    int n;
    int ans[N];
    if (freopen("4_1_input.in", "r", stdin) == NULL) {
		printf("There is an error in reading file 4_1_input.in");
    }
    scanf("%d", &caseNum);
    for (int case1 = 0; case1 < caseNum; case1++) {
        printf("==== Case %d ====\n", case1 + 1);
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
        }
        ans[case1] = solve1(a,n);
        printf("ans is:%d\n", ans[case1]);
        printf("\n");
    }
    fclose(stdin);
}

int main() {
    test1();
    return 0;
}
