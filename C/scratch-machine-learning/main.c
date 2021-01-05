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
Stack�� �ʱ�ȭ �մϴ�.
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
Node�� �ʱ�ȭ �մϴ�. ����� ������ �����ϴ�.
- featureNum��ŭ float�� �迭�� ��� data�� �����Ҵ� �մϴ�.
- Node�� �����͸� �����ϴ� ��� next�� NULL�� �ʱ�ȭ �մϴ�.
- enum���� ����� ��� lable�� -1�� �ʱ�ȭ �մϴ�.
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
PointerŸ�� ���� ptr�� �ʱ�ȭ �ϰ� �Է������� ������ ������ �����մϴ�.
�����ʹ� [featureNum]���� �Ǽ��� 'feature'�� ���ڿ� 'label'�� �Ǿ� �ֽ��ϴ�.
[������ ����] 5.1,3.5,1.4,0.2,Iris-setosa
- N���� feauture�� �̸� �����Ҵ��� float �迭�� ������� �����մϴ�.
- enum �����Ϳ� �´� index�� ã�Ƽ� 'label'�� �����մϴ�.
*/
Pointer initTrain(char* inputData)
{
	Pointer ptr = initPointer();
	char* temp = strtok(inputData, ",");

	/*�ݺ����� ���� data �迭�� ���� �Ҵ��մϴ�.*/
	for (int i = 0; i < featureNum; i++)
	{
		float tempFloat = atof(temp);
		ptr->data[i] = tempFloat;
		temp = strtok(NULL, ",");
	}

	/* ���̻� "iris-"�� �����մϴ�.*/
	temp = strtok(temp, "-");

	/* ���̻簡 ���ŵ� �и����� �˸��� enum index�� ��� label�� �����մϴ� */
	temp = strtok(NULL, "\n");
	NameOfIris name = returnLabel(temp);
	ptr->label = name;

	return ptr;
}


/*
�Էµ����� ������ �б� ��� ����������(FILE*)�� ��ȯ�մϴ�.
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
�Էµ������� ��(������ �� ��)�� ���� �Լ��Դϴ�.
���������� ����� ���������� dataCnt�� ���� �ʱ�ȭ �մϴ�.
�� �� ���� dataCnt�� ���ԵǹǷ� ������ ���� ����Ͽ� ������ �ƴ��� Ȯ���մϴ�.
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
���ڿ� �����͸� enum ������ ���� _NameOfIris�� �����Ϳ� �˸��� ���� ã�� 
������ index�� ��ȯ�մϴ�.
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
�Է����Ͽ��� �����͸� �о Pointer�� �����ϰ� Stack�� �����ϰ� ������ �����մϴ�.
�ݺ����� �������� dataCnt��ŭ �����մϴ�. �� ���� �ݺ��� ������ �����ϴ�.
1. ���������� fp_in���κ��� ������ ������ �а� ���ڿ� 'inputData'�� �����մϴ�.
2. ���ڿ��� �Ľ��Ͽ� feature�� label�� �з��ϰ� Pointer�� �����մϴ�.
3. Pointer�� Stack�� �����մϴ�.
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