#include<iostream>
#include<stdlib.h>
using namespace std;
#define MY_OK 1
#define MY_FAIL -1
#define MAX_STACK_SIZE 100

//�ַ��͵�ջ
typedef struct 
{
	char str[MAX_STACK_SIZE] = {0};
	int top = 0;
}STACK;

//���������Ľ������HTNode��ָ������HuffmanTree
typedef struct
{
	int weight = 0;
	int parent = 0, lchild = 0, rchild = 0;
	char character = 0;//�������ַ�
}HTNode, *HuffmanTree;

//�ҵ������������������Ȩֵ��С���������
int Select_2min(HuffmanTree HT, int n, int &label_1, int &label_2)
{
	int temp_label_1 = 1, temp_label_2 = 1;
	for (int i = 1; i <= n; i++)
	{
		int j = i;
		//�Ҹ��ڵ�
		while (HT[j].parent != 0)
		{
			j = HT[j].parent;
		}
		if (HT[j].weight < HT[temp_label_1].weight&&temp_label_1 != j)
		{
			temp_label_2 = temp_label_1;
			temp_label_1 = j;
		}
	}
	label_1 = temp_label_1;
	label_2 = temp_label_2;
}

//�����������,����ֵ�����������ĸ��ڵ���±��MY_FAIL
int Build_HuffmanTree(HuffmanTree &HT, int *weight, int n)
{
	//��������
	if (HT == NULL || weight == NULL || n < 0)
	{
		return MY_FAIL;
	}
	int NodeNum = 2 * n - 1;//���������Ľ������
	HT = new HTNode[NodeNum + 1];//Ϊ������������ռ䣬0��λ�ÿ��ò���
	//��ʼ��
	for (int i = 1; i <= NodeNum; i++)
	{
		HT[i].weight = i <= n ? weight[i] : 0;
		HT[i].lchild = HT[i].rchild = HT[i].parent = 0;
	}
	int label_1 = 0, label_2 = 0;//����������ǩ����¼����������㷨��ÿ�ֺϲ�������Ȩֵ��С�ĸ����
	//����n-1�κϲ�������Huffman��
	for (int i = n + 1; i <= NodeNum; i++)
	{
		//�Һ���
		Select_2min(HT, n, label_1, label_2);
		//������˫�׽�����ϵ
		HT[i].lchild = label_1;
		HT[i].rchild = label_2;
		HT[i].weight = HT[label_1].weight + HT[label_2].weight;
		HT[label_1].parent = HT[label_2].parent = i;
	}
	int root = HT[1].parent;
	while (HT[root].parent != 0)
	{
		root = HT[root].parent;
	}
	return root;
}

//�ַ�ͳ��
void c_statistics(int *weight)
{
	char c = 0;
	//��Ȩֵ��������
	memset(weight, 0, 256 * sizeof(char));
	while ((c = getchar() != EOF))
	{
		weight[c]++;
	}
}

//�����ַ�ͳ�ƵĽ�����õ���������ַ������Ȩֵ,����ֵΪ��������ַ���Ŀ��MY_FAIL
int Analyze_Statics(char *characters,int *weight)
{
	//��������
	if ( NULL == weight || NULL ==characters)
	{
		return MY_FAIL;
	}
	int temp_weight[256] = { 0 };
	int n = 0;//�����ַ�������
	for (int i = 0; i < 256; i++)
	{
		if (weight[i] > 0)
		{
			characters[n + 1] = i + 1;//�����0λ�ÿճ�������
			temp_weight[n + 1] = weight[i];
			n++;
		}
	}
	for (int i = 1; i <= n; i++)
	{
		weight[i] = temp_weight[i];
	}
	return n;
}

//��ͳ�ƽ�����ַ���������������Ҷ�ӽ��
void character2LeafNode(HuffmanTree HT,char *characters,int n)
{
	for (int i = 1; i <= n; i++)
	{
		HT[i].character = characters[i];
	}
}

//ѹջ
int push(STACK &S,char c)
{
	if (S.top == MAX_STACK_SIZE)
	{
		return MY_FAIL;
	}
	S.str[S.top] = c;
	S.top++;
	return MY_OK;
}
//��ջ
int pop(STACK &S)
{
	if (S.top == 0)
	{
		return MY_FAIL;
	}
	S.top--;
	return MY_OK;
}

//�ӹ�������������ù���������
int HuffmanCoding(
	HuffmanTree HT,
	int root,
	char **HC,
	int n
)
{
	STACK S;
	if (root != 0)//���ڵ㲻Ϊ0
	{
		if (0 == HT[root].lchild)
		{
			
		}
	}
}

void Huffman_condense()
{
	HuffmanTree HT;
	int n = 0;
	char code_characters[256];//ASCII�����ַ����ᳬ��256
	int weight[256] = { 0 };

	c_statistics(weight);

	n = Analyze_Statics(code_characters, weight);
	Build_HuffmanTree(HT, weight, n);
	character2LeafNode(HT, code_characters, n);
}
int main()
{
	Huffman_condense();
	return 0;
}