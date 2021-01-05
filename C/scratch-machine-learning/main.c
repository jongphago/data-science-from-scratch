#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dataCnt = 0;
int featureNum = 4;

typedef enum _NameOfIris {
	setosa = 0,
	versicolor,
	virginica
} NameOfIris;

typedef struct Node* Pointer;
typedef struct Node {
	float* data;
	NameOfIris label;
	Pointer next;
}Node;

typedef struct Head* Stack;
typedef struct Head {
	Pointer top;
}Head;

/*
Stack을 초기화 합니다.
*/
Stack initStack()
{
	Stack stack = malloc(sizeof(Head));
	stack->top = NULL;
	return stack;
}

void push(Stack stack, Pointer first)
{
	first->next = stack->top;
	stack->top = first;
}

/*
Node를 초기화 합니다. 방법은 다음과 같습니다.
- featureNum만큼 float형 배열인 멤버 data를 동적할당 합니다.
- Node의 포인터를 저장하는 멤버 next는 NULL로 초기화 합니다.
- enum으로 선언된 멤버 lable을 -1로 초기화 합니다.
*/
Pointer initPointer()
{
	Pointer ptr = malloc(sizeof(Node));
	ptr->data = malloc(featureNum * sizeof(float));
	ptr->next = NULL;
	ptr->label = -1;
	return ptr;
}

/*
Pointer타입 변수 ptr을 초기화 하고 입력파일의 데이터 한줄을 저장합니다.
데이터는 [featureNum]개의 실수형 'feature'와 문자열 'label'로 되어 있습니다.
[데이터 예시] 5.1,3.5,1.4,0.2,Iris-setosa
- N개의 feauture를 미리 동적할당한 float 배열에 순서대로 저장합니다.
- enum 데이터에 맞는 index를 찾아서 'label'에 저장합니다.
*/
Pointer initTrain(char* inputData)
{
	Pointer ptr = initPointer();
	char* temp = strtok(inputData, ",");

	/*반복문을 돌며 data 배열에 값을 할당합니다.*/
	for (int i = 0; i < featureNum; i++)
	{
		float tempFloat = atof(temp);
		ptr->data[i] = tempFloat;
		temp = strtok(NULL, ",");
	}

	/* 접미사 "iris-"를 제거합니다.*/
	temp = strtok(temp, "-");

	/* 접미사가 제거된 학명으로 알맞은 enum index를 멤버 label에 저장합니다 */
	temp = strtok(NULL, "\n");
	NameOfIris name = returnLabel(temp);
	ptr->label = name;

	return ptr;
}


/*
입력데이터 파일을 읽기 모드 파일포인터(FILE*)로 반환합니다.
*/
FILE* fopenName(char* fileName)
{
	FILE* fp_in = fopen(fileName, "r");
	if (fp_in == NULL) {
		printf("FILE OPEN ERROR");
		exit(1);
	}
	return fp_in;
}

/*
입력데이터의 수(데이터 줄 수)를 세는 함수입니다.
전역변수로 선언된 정수형변수 dataCnt에 값을 초기화 합니다.
빈 줄 또한 dataCnt에 포함되므로 마지막 줄을 출력하여 빈줄이 아닌지 확인합니다.
*/
void fileDataCounter(FILE* fp_in)
{
	char inputData[100];
	while (!feof(fp_in)) {
		fgets(inputData, 100, fp_in);
		dataCnt++;
	}
	printf("[Last Input Row] '%s'\n", inputData);
	rewind(fp_in);
}

void getFeatureNum()
{
	printf("NUMBER OF FEATURE: ");
	int _return = scanf("%d", &featureNum);
	printf("\n");
}

/*
문자열 데이터를 enum 형식의 변수 _NameOfIris의 데이터에 알맞은 값을 찾아 
정수형 index를 반환합니다.
*/
int returnLabel(char* temp)
{
	if (strcmp(temp, "setosa")==0)
		return 0;
	else if (strcmp(temp, "versicolor")==0)
		return 1;
	else if (strcmp(temp, "virginica")==0)
		return 2;
	else return -99999999;
}

/*
입력파일에서 데이터를 읽어서 Pointer에 저장하고 Stack에 저장하고 파일을 해제합니다.
반복문을 전역변수 dataCnt만큼 수행합니다. 한 번의 반복은 다음과 같습니다.
1. 파일포인터 fp_in으로부터 데이터 한줄을 읽고 문자열 'inputData'에 저장합니다.
2. 문자열을 파싱하여 feature와 label을 분류하고 Pointer에 저장합니다.
3. Pointer를 Stack에 저장합니다.
*/
void pushData(FILE* fp_in, Stack stack) 
{
	for (int i = 0; i < dataCnt; i++)
	{
		char inputData[100];
		fgets(inputData, 100, fp_in);
		Pointer ptr = initTrain(inputData);
		push(stack, ptr);
		//printf("%d", ptr->label);
	}
}


void main(void) {
	char* fileName = { "_iris.dat" };
	FILE* fp_in = fopenName(fileName);

	fileDataCounter(fp_in);
	//getFeatureNum();
		
	Stack stack = initStack();
	pushData(fp_in, stack);
	free(fp_in);
}