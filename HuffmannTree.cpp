#include<iostream>
#include<stdlib.h>
using namespace std;
#define MY_OK 1
#define MY_FAIL -1
#define MAX_STACK_SIZE 100

//字符型的栈
typedef struct 
{
	char str[MAX_STACK_SIZE] = {0};
	int top = 0;
}STACK;

//哈夫曼树的结点类型HTNode和指针类型HuffmanTree
typedef struct
{
	int weight = 0;
	int parent = 0, lchild = 0, rchild = 0;
	char character = 0;//被编码字符
}HTNode, *HuffmanTree;

//找到构造哈夫曼树过程中权值最小的两个结点
int Select_2min(HuffmanTree HT, int n, int &label_1, int &label_2)
{
	int temp_label_1 = 1, temp_label_2 = 1;
	for (int i = 1; i <= n; i++)
	{
		int j = i;
		//找根节点
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

//构造哈夫曼树,返回值：哈夫曼树的根节点的下标或MY_FAIL
int Build_HuffmanTree(HuffmanTree &HT, int *weight, int n)
{
	//参数防御
	if (HT == NULL || weight == NULL || n < 0)
	{
		return MY_FAIL;
	}
	int NodeNum = 2 * n - 1;//哈夫曼树的结点总数
	HT = new HTNode[NodeNum + 1];//为哈夫曼树申请空间，0号位置空置不用
	//初始化
	for (int i = 1; i <= NodeNum; i++)
	{
		HT[i].weight = i <= n ? weight[i] : 0;
		HT[i].lchild = HT[i].rchild = HT[i].parent = 0;
	}
	int label_1 = 0, label_2 = 0;//建立两个标签，记录构造哈夫曼算法中每轮合并中两个权值最小的根结点
	//进行n-1次合并，构建Huffman树
	for (int i = n + 1; i <= NodeNum; i++)
	{
		//找孩子
		Select_2min(HT, n, label_1, label_2);
		//孩子与双亲建立联系
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

//字符统计
void c_statistics(int *weight)
{
	char c = 0;
	//将权值数组清零
	memset(weight, 0, 256 * sizeof(char));
	while ((c = getchar() != EOF))
	{
		weight[c]++;
	}
}

//处理字符统计的结果，得到被编码的字符数组和权值,返回值为被编码的字符数目或MY_FAIL
int Analyze_Statics(char *characters,int *weight)
{
	//参数防御
	if ( NULL == weight || NULL ==characters)
	{
		return MY_FAIL;
	}
	int temp_weight[256] = { 0 };
	int n = 0;//编码字符种类数
	for (int i = 0; i < 256; i++)
	{
		if (weight[i] > 0)
		{
			characters[n + 1] = i + 1;//数组的0位置空出来不用
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

//把统计结果的字符赋给哈夫曼树的叶子结点
void character2LeafNode(HuffmanTree HT,char *characters,int n)
{
	for (int i = 1; i <= n; i++)
	{
		HT[i].character = characters[i];
	}
}

//压栈
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
//弹栈
int pop(STACK &S)
{
	if (S.top == 0)
	{
		return MY_FAIL;
	}
	S.top--;
	return MY_OK;
}

//从哈夫曼树出发获得哈夫曼编码
int HuffmanCoding(
	HuffmanTree HT,
	int root,
	char **HC,
	int n
)
{
	STACK S;
	if (root != 0)//根节点不为0
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
	char code_characters[256];//ASCII编码字符不会超过256
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