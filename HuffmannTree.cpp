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

//�ҵ������������������Ȩֵ��С�Ľ��
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
	used[label] = true;//��Ǵ˽ڵ��Ѿ��ҵ�˫��
	return MY_OK;
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
	bool used[MAX_STACK_SIZE] = {0};//�������Ƿ��Ѿ��ҵ�˫�ף�������Ϊtrue
	for (int i = n + 1; i <= NodeNum; i++)
	{
		//�Һ���
		Select_min(HT,used,i-1,label_1);
		Select_min(HT, used, i-1, label_2);
		//������˫�׽�����ϵ
		HT[i].lchild = label_1;
		HT[i].rchild = label_2;
		HT[i].weight = HT[label_1].weight + HT[label_2].weight;
		HT[label_1].parent = HT[label_2].parent = i;
	}

	return NodeNum;
}

//�ַ�ͳ��
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
	//��Ȩֵ��������
	memset(weight, 0, 256 * sizeof(char));

	while (fread(&ch, sizeof(char), 1, fp))
	{
		int pos = ch + 128;//�����ƶ�ȡ�����и������ʽ���ӳ�佫-127~127ӳ�䵽1~255��
		weight[pos]++;
	}
	fclose(fp);
}

//�����ַ�ͳ�ƵĽ�����õ���������ַ������Ȩֵ,����ֵΪ��������ַ���Ŀ��MY_FAIL
int Analyze_Statics(char *characters,int *weight,int *table)
{
	//��������
	if ( NULL == weight || NULL ==characters)
	{
		return MY_FAIL;
	}
	int temp_weight[500] = { 0 };
	int n = 0;//�����ַ�������
	for (int i = 0; i < 256; i++)
	{
		if (weight[i] > 0)
		{
			characters[n + 1] = i-128;//�����0λ�ÿճ�������
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

//��ӡ����������
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
	if (root != 0)//���ڵ㲻Ϊ0
	{
		if (0 == HT[root].lchild)//����Ҷ���
		{
			push(S, '\0');
			HC[root] = new char[S.top + 1];
			strcpy(HC[root], S.str);
			pop(S);
		}
		else
		{
			push(S, '0');//�������ӣ�д��0
			Coding(HT, HC, HT[root].lchild, S);
			pop(S);
			push(S, '1');//�����Һ��ӣ�д��1
			Coding(HT, HC, HT[root].rchild, S);
			pop(S);

		}
	}
}
//�ӹ�������������ù���������
int HuffmanCoding(
	HuffmanTree HT,
	char **&HC,
	int root,
	int n
)
{
	STACK S;
	//HCΪ���������������ַ�������
	HC = new char*[n + 1];
	Coding(HT, HC, root, S);
	return MY_OK;
}

//�����½��й���������,�������code�ַ�����
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
	cout << "ѹ����Ϊ" << (double(count_code) / 8) / count_essay<<endl;
}

//�����½��н��룬��������essay�ַ�����
void code2essay(char*code,char *essay,HuffmanTree HT, int root)
{
	char ch1 = 0, ch2 = 0;
	int pt = root;
	int j = 0;
	for (int i = 0; code[i] != '\0'; i++)
	{
		if (HT[pt].lchild == 0)//�Ը��ڵ����Ҷ�ӽ������������
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
	cout << "����Ϊ��" <<endl<< essay<<endl;
}

//�õ�ѹ���ļ�
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

//���ַ����д��ѹ���ļ�
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
				i = table[ch+128];//������һ���ַ�
				p = HC[i];
			}
			temp += ((*p - '0') << (7 - count));
			if (count == 7)//����һ���ַ�,����д����
			{
				fwrite(&temp, sizeof(char), 1, fp2);
				temp = 0;
				count = -1;
			}
			if (*p == '\0'&&feof(fp1))
				break;
		}
		if (count != 0)//����ʣ�಻��8λ�ı���û����,��8λд��
		{
			fwrite(&temp, sizeof(char), 1, fp2);
		}
	}

	fclose(fp1);
	fclose(fp2);
	return  MY_OK;
}

//����
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
			if (HT[pt].lchild == 0)//�Ը��ڵ����Ҷ�ӽ������������
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
	HuffmanTree HT;//��������
	char **HC;//ͳ���ַ��Ĺ���������
	int n = 0;//�����ַ�������
	char code_characters[256]=" ";//��Լʱ�䣬���ַ���ASCII����Ϊ�ַ����±��Ƚ���һ��ͳ�ƣ��±겻�ᳬ��256
	int weight[256] = { 0 };//�ַ���ͳ��Ȩ��
	int table[256] = { 0 };//����֪��������ַ���������code_character�ĵڼ���λ��

	c_statistics(weight);//ͳ���ַ�����Ƶ��

	n = Analyze_Statics(code_characters, weight,table);//ͳ���ַ�����������������������
	Build_HuffmanTree(HT, weight, n);//����
	//��������ʽ��ӡ��������
	print_HT(HT, n);
	//�Թ���������Ҷ��㸳���ַ�����
	character2LeafNode(HT, code_characters, n);
	//�ӹ���������������ַ��Ĺ���������
	HuffmanCoding(HT, HC, 2 * n - 1, n);
	//��ӡ�ַ��������������
	print_code(code_characters, HC, n);
	condense(table, HC, n);//ѹ��
	decode(HT, 2 * n - 1);//��ѹ��
	

	//���ı���ʽ����ԭ�ģ��������������01���У�����ѹ����
	char essay[MAX_STR_SIZE],code[MAX_STR_SIZE],new_essay[MAX_STR_SIZE];
	cout << "������ԭ�ģ�" << endl;
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