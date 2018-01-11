#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdlib.h>
#include<string.h>
using namespace std;
#define MY_OK 1
#define MY_FAIL -1
#define MAX_STACK_SIZE 100
#define MAX_STR_SIZE 2000
#define PATH1 "C://Users//lf//Desktop//1.txt"
#define PATH2 "C://Users//lf//Desktop//condense2.dat"
#define PATH3 "C://Users//lf//Desktop//decode.txt"

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

//找到构造哈夫曼树过程中权值最小的结点
int Select_min(HuffmanTree HT,bool *used ,int n, int &label)
{
	int temp_label = 1;
	while (used[temp_label] == true && temp_label <= n)
		temp_label++;
	for (int i = 1; i <= n; i++)
	{
		if (used[i] == true)
			continue;
		if (HT[i].weight < HT[temp_label].weight)
		{
			temp_label = i;
		}
	}
	label = temp_label;
	used[label] = true;//标记此节点已经找到双亲
	return MY_OK;
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
	bool used[MAX_STACK_SIZE] = {0};//定义结点是否已经找到双亲，若是则为true
	for (int i = n + 1; i <= NodeNum; i++)
	{
		//找孩子
		Select_min(HT,used,i-1,label_1);
		Select_min(HT, used, i-1, label_2);
		//孩子与双亲建立联系
		HT[i].lchild = label_1;
		HT[i].rchild = label_2;
		HT[i].weight = HT[label_1].weight + HT[label_2].weight;
		HT[label_1].parent = HT[label_2].parent = i;
	}

	return NodeNum;
}

//字符统计
void c_statistics(int *weight)
{
	FILE*fp = NULL;
	if ((fp = fopen(PATH1, "rb")) == NULL)
	{
		cout << "open essay1 failed" << endl;
		cin.ignore();
		exit(0);
	}
	char ch = 0;
	//将权值数组清零
	memset(weight, 0, 256 * sizeof(char));

	while (fread(&ch, sizeof(char), 1, fp))
	{
		int pos = ch + 128;//二进制读取可能有负数，故进行映射将-127~127映射到1~255；
		weight[pos]++;
	}
	fclose(fp);
}

//处理字符统计的结果，得到被编码的字符数组和权值,返回值为被编码的字符数目或MY_FAIL
int Analyze_Statics(char *characters,int *weight,int *table)
{
	//参数防御
	if ( NULL == weight || NULL ==characters)
	{
		return MY_FAIL;
	}
	int temp_weight[500] = { 0 };
	int n = 0;//编码字符种类数
	for (int i = 0; i < 256; i++)
	{
		if (weight[i] > 0)
		{
			characters[n + 1] = i-128;//数组的0位置空出来不用
			temp_weight[n + 1] = weight[i];
			table[i] = n + 1;
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

//打印哈夫曼编码
void print_code(char *characters,char **HC,int n)
{
	for (int i = 1; i <= n; i++)
	{
		cout << characters[i]<<":";
		puts(HC[i]);
		cout << endl;
	}
}

void Coding
(
	HuffmanTree HT,
	char **HC,
	int root,
	STACK &S
)
{
	if (root != 0)//根节点不为0
	{
		if (0 == HT[root].lchild)//到达叶结点
		{
			push(S, '\0');
			HC[root] = new char[S.top + 1];
			strcpy(HC[root], S.str);
			pop(S);
		}
		else
		{
			push(S, '0');//访问左孩子，写入0
			Coding(HT, HC, HT[root].lchild, S);
			pop(S);
			push(S, '1');//访问右孩子，写入1
			Coding(HT, HC, HT[root].rchild, S);
			pop(S);

		}
	}
}
//从哈夫曼树出发获得哈夫曼编码
int HuffmanCoding(
	HuffmanTree HT,
	char **&HC,
	int root,
	int n
)
{
	STACK S;
	//HC为储存哈夫曼编码的字符串数组
	HC = new char*[n + 1];
	Coding(HT, HC, root, S);
	return MY_OK;
}

//对文章进行哈夫曼编码,编码存入code字符串中
void essay2code(char *essay,char*code,int *table, char**HC, int n)
{
	int count_essay = 0,count_code=0;
	char *p = NULL;
	for (count_essay = 0; essay[count_essay] != '\0'; count_essay++)
	{
		char ch = essay[count_essay];
		int pos = ch + 128;
		int label = table[pos];
		for (p = HC[label]; *p != '\0'; p++)
		{
			code[count_code] = *p;
			count_code++;
		}
	}
	code[count_code] = '\0';
	cout <<code<<endl;
	cout << "压缩率为" << (double(count_code) / 8) / count_essay<<endl;
}

//对文章进行解码，解码后存入essay字符串中
void code2essay(char*code,char *essay,HuffmanTree HT, int root)
{
	char ch1 = 0, ch2 = 0;
	int pt = root;
	int j = 0;
	for (int i = 0; code[i] != '\0'; i++)
	{
		if (HT[pt].lchild == 0)//对根节点就是叶子结点的情况做处理
		{
			ch2 = HT[pt].character;
			essay[j] = ch2;
			j++;
			pt = root;
		}
		if (code[i] == '0')
			pt = HT[pt].lchild;
		if (code[i] == '1')
			pt = HT[pt].rchild;

		if (HT[pt].lchild == 0)
		{
			ch2 = HT[pt].character;
			essay[j] = ch2;
			j++;
			pt = root;
		}
	}
	essay[j] = '\0';
	cout << "解码为：" <<endl<< essay<<endl;
}

//得到压缩文件
int condense(int *table,char**HC,int n)
{
	FILE*fp1 = NULL, *fp2 = NULL;
	if ((fp1 = fopen(PATH1, "rb")) == NULL)
	{
		cout << "open file failed" << endl;
		cin.ignore();
		exit(0);
	}
	if ((fp2 = fopen(PATH2, "wb")) == NULL)
	{
		cout << "open file failed" << endl;
		cin.ignore();
		exit(0);
	}
	char ch = 0;

//按字符逐个写入压缩文件
	char*p=NULL;
	char temp=0;
	int count = 0;
	if(fread(&ch,sizeof(char),1,fp1))
	{
		
		int i = table[ch+128];   
		
		for (p = HC[i]; ; p++,count++)
		{
			if (*p == '\0'&& !feof(fp1))
			{
				fread(&ch, sizeof(char), 1, fp1);
				i = table[ch+128];//编码下一个字符
				p = HC[i];
			}
			temp += ((*p - '0') << (7 - count));
			if (count == 7)//凑足一个字符,可以写入了
			{
				fwrite(&temp, sizeof(char), 1, fp2);
				temp = 0;
				count = -1;
			}
			if (*p == '\0'&&feof(fp1))
				break;
		}
		if (count != 0)//仍有剩余不足8位的编码没读完,按8位写入
		{
			fwrite(&temp, sizeof(char), 1, fp2);
		}
	}

	fclose(fp1);
	fclose(fp2);
	return  MY_OK;
}

//解码
void decode(HuffmanTree HT,int root)
{
	FILE*fp1 = NULL, *fp2 = NULL;
	if ((fp1 = fopen(PATH2, "rb")) == NULL)
	{
		cout << "open file failed" << endl;
		cin.ignore();
		exit(0);
	}
	if ((fp2 = fopen(PATH3, "wb")) == NULL)
	{
		cout << "open file failed" << endl;
		cin.ignore();
		exit(0);
	}
	char ch1 = 0, ch2 = 0;
	int pt = root;
	if (!feof(fp1))
	{
		fread(&ch1, sizeof(char), 1, fp1);
		char temp[8];
		for (int i = 7; i >= 0; i--)
		{
			temp[7 - i] = '0' + ((ch1 >> i)&1);
		}
		for (int i = 0; i < 8; i++)
		{
			if (HT[pt].lchild == 0)//对根节点就是叶子结点的情况做处理
			{
				ch2 = HT[pt].character;
				fputc(ch2, fp2);
				pt = root;
			}
			if (temp[i] == '0')
				pt = HT[pt].lchild;
			if (temp[i] == '1')
				pt = HT[pt].rchild;

			if (HT[pt].lchild == 0)
			{
				ch2 = HT[pt].character;
				fputc(ch2, fp2);
				pt = root;
			}
			if (i == 7 && !feof(fp1))
			{
				i = -1;
				fread(&ch1, sizeof(char), 1, fp1);
				for (int j = 7; j >= 0; j--)
				{
					temp[7 - j] = '0' + ((ch1 >> j) & 1);
				}
			}
		}
	}
	fclose(fp1);
	fclose(fp2);
}

void print_HT(HuffmanTree HT,int n)
{
	for (int i = 0; i <= 2 * n - 1; i++)
	{
		cout << HT[i].character << " " << HT[i].parent << " " << HT[i].lchild << " " << HT[i].rchild << " " << HT[i].weight << endl;
	}
}
void Huffman()
{
	HuffmanTree HT;//哈夫曼树
	char **HC;//统计字符的哈夫曼编码
	int n = 0;//编码字符种类数
	char code_characters[256]=" ";//节约时间，按字符的ASCII码作为字符的下标先进行一轮统计，下标不会超过256
	int weight[256] = { 0 };//字符的统计权重
	int table[256] = { 0 };//用于知道读入的字符被放在了code_character的第几个位置

	c_statistics(weight);//统计字符出现频率

	n = Analyze_Statics(code_characters, weight,table);//统计字符种类数并将其排入数组中
	Build_HuffmanTree(HT, weight, n);//建树
	//按数组形式打印哈夫曼树
	print_HT(HT, n);
	//对哈夫曼树的叶结点赋予字符意义
	character2LeafNode(HT, code_characters, n);
	//从哈夫曼树出发获得字符的哈夫曼编码
	HuffmanCoding(HT, HC, 2 * n - 1, n);
	//打印字符及其哈夫曼编码
	print_code(code_characters, HC, n);
	condense(table, HC, n);//压缩
	decode(HT, 2 * n - 1);//解压缩
	

	//按文本形式输入原文，输出哈夫曼编码01序列，计算压缩比
	char essay[MAX_STR_SIZE],code[MAX_STR_SIZE],new_essay[MAX_STR_SIZE];
	cout << "请输入原文：" << endl;
	gets_s(essay,MAX_STR_SIZE);
	essay2code(essay, code, table, HC, n);
	code2essay(code, new_essay, HT, 2 * n - 1);
	for (int i = 1; i <= n; i++)
	delete[] HC[i];
	delete[] HC;
	delete HT;
}
int main()
{
	Huffman();
	cin.ignore();
	return 0;
}