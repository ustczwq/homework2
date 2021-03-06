// CORE_DLL.cpp: 定义 DLL 应用程序的导出函数。
//
#define ArithmeticDLL
#include "targetver.h"

string* Core::getQue()
{
	Node *head, result;
	que = new string[que_num + 1];
	ans = new string[que_num + 1];
	for (int i = 1; i <= que_num; i++)
	{
		head = creatOptree();
		previsite(head);
		result = calc(head);
		result.simp();
		if (result.bottom == 0 || result.top < 0)
			i--;
		else
		{
			que[0] = "";
			que[i] = expstring(head);
			if (data_type == 1)
			{
				double t = 1.0 * result.top / result.bottom;
				stringstream s1;
				s1 << t;
				ans[i] += s1.str();
			}
			else
			{
				stringstream s2;
				s2 << result.top;
				ans[i] += s2.str();
				if (result.bottom > 1)
				{
					ans[i] += "|";
					stringstream s3;
					s3 << result.bottom;
					ans[i] += s3.str();
				}
			}
		}
	}
	return que;
}
string* Core::getAns()
{
	if (data_type == 1)
	{
		for (int i = 1; i <= que_num; i++)
		{
			for (int j = 0; ans[i][j]; j++)
				if (ans[i][j] == '.')
				{
					for (size_t k = j + accuracy + 1; k < ans[i].length(); k++)
						ans[i][k] = '\0';
					break;
				}
		}
	}
	return ans;
}
string Core::expstring(Node *root)  //生成中缀表达式  分子分母间用 | 
{
	if (root == NULL)
		return que[0];
	if (root->left)
	{
		if ((root->left->operat == '+' || root->left->operat == '-') && (root->operat == '*' || root->operat == '/'))
		{
			que[0] += "( ";
			expstring(root->left);
			que[0] += ") ";
		}
		else if (root->operat == '^' && root->left->operat)
		{
			que[0] += "( ";
			expstring(root->left);
			que[0] += ") ";
		}
		else
			expstring(root->left);
	}

	if (root->operat == 0)
	{
		if (data_type == 1)
		{
			double t = 1.0 * root->top / root->bottom;
			stringstream s1;
			s1 << t;
			que[0] += s1.str();
			que[0] += " ";
		}
		else
		{
			stringstream s2;
			s2 << root->top;
			que[0] += s2.str();
			if (root->bottom > 1)
			{
				que[0] += "|";
				stringstream s3;
				s3 << root->bottom;
				que[0] += s3.str();
			}
			que[0] += " ";
		}
	}
	else
	{
		que[0] += root->operat;
		que[0] += " ";
	}

	if (root->right)
	{
		if ((root->operat == '-' || root->operat == '*') && (root->right->operat == '-' || root->right->operat == '+'))
		{
			que[0] += "( ";
			expstring(root->right);
			que[0] += ") ";
		}
		else if (root->operat == '/' && (root->right->operat && root->right->operat != '^'))
		{
			que[0] += "( ";
			expstring(root->right);
			que[0] += ") ";
		}
		else if (root->operat == '^'&& root->right->operat)
		{
			que[0] += "( ";
			expstring(root->right);
			que[0] += ") ";
		}
		else
			expstring(root->right);
	}
	return que[0];
}
Node* Core::creatOptree()
{
	Node *head = NULL, *p = NULL, *q = NULL;
	int i = 0, j = 0;
	head = new Node;
	head->operat = '+';

	for (i = 1, j = 0; i < data_num - 1; i++)
	{
		p = head;
		while (p)
		{
			if (rand() % 2) {
				if (p->left)  p = p->left;
				else {
					q = new Node;
					if (data_type == 0) q->operat = int_operat();
					else   q->operat = operat();
					p->left = q;
					break;
				}
			}
			else {
				if (p->right)  p = p->right;
				else {
					q = new Node;
					if (data_type == 0) q->operat = int_operat();
					else   q->operat = operat();
					p->right = q;
					break;
				}
			}
		}//end while
	}//end if
	return head;

}
Node Core::calc(Node *root)   //计算树的值
{
	Node answer;
	if (root == NULL)
		return answer;
	char opr = root->operat;

	if (opr == 0)
	{
		return *root;
	}
	else if (opr == '+')
	{
		return calc(root->left) + calc(root->right);
	}
	else if (opr == '-')
	{
		answer = (calc(root->left) - calc(root->right));
		if (answer.top < 0)
		{
			Node *temp = root->left;
			root->left = root->right;
			root->right = temp;
			answer.top *= -1;
		}
		return answer;
	}
	else if (opr == '*')
	{
		return calc(root->left) * calc(root->right);
	}

	else if (opr == '/')
	{
		if (data_type == 0)
		{

		}
		return calc(root->left) / calc(root->right);
	}

	else if (opr == '^')
	{
		return calc(root->left) ^ calc(root->right);
	}
	else
		return answer;
}
void Core::previsite(Node *head)
{
	if (!head) return;
	class Node *q = NULL, *p = NULL;
	if (head->operat && opr_type == 3 && !head->left && !head->right)
	{
		int temp = 0;
		temp = rand() % 4;
		if (temp == 0) head->operat = '^';
	}

	if (head->operat  && data_type == 0 && (opr_type == 2 || opr_type == 3) && !head->left && !head->right)
	{
		int temp = 0;
		temp = rand() % 4;
		if (temp == 0) head->operat = '/';
	}

	if (head->operat && !head->left)
	{
		p = new Node;
		//reset(q);
		p->bottom = rand() % range + 1;
		p->top = rand() % range + 1;
		if (data_type == 0)  p->bottom = 1; //整数
		if (data_type == 0 && head->operat == '/')  p->top = rand() % range + 1;
		if (data_type == 1)     //小数
		{
			p->bottom = 20;
			p->top = rand() % (20 * range);
		}
		if (head->operat == '^')
		{
			int min = (range < 6) ? range : 6;
			p->bottom = 1;
			p->top = rand() % min + 1;
		}
		head->left = p;
	}
	if (head->operat && !head->right)
	{
		q = new Node;
		//reset(q);
		q->bottom = rand() % range + 1;
		q->top = rand() % range + 1;
		if (data_type == 0) q->bottom = 1;  //整数
		if (data_type == 0 && head->operat == '/')  q->top = get_factor(head->left->top);
		if (data_type == 1)  //小数
		{
			q->bottom = 20;
			q->top = rand() % (20 * range);
		}
		if (head->operat == '^')   //乘方
		{
			int min = (range <= 5) ? range : 5;
			q->bottom = 1;
			q->top = rand() % min + 1;
		}
		head->right = q;
	}
	previsite(head->left);
	previsite(head->right);

}
char Core::operat()
{
	int mod = 0;
	switch (opr_type)
	{
	case 0: mod = 2; break; //+-
	case 1: mod = 3; break;//+-*
	case 2: mod = 4; break;//+-*/
	case 3: mod = 4; break;//+-*/^
	default: mod = 2; break;
	}
	int n = rand() % mod + 1;
	switch (n)
	{
	case 1: return '+';
	case 2: return '-';
	case 3: return '*';
	case 4: return '/';
	default:return '+';
	}

}
char Core::int_operat()
{
	int mod = 0;
	switch (opr_type)
	{
	case 0: mod = 2; break; //+-
	case 1: mod = 3; break;//+-*
	case 2: mod = 3; break;//+-*/
	case 3: mod = 3; break;//+-*/^
	default: mod = 2; break;
	}
	int n = rand() % mod + 1;
	switch (n)
	{
	case 1: return '+';
	case 2: return '-';
	case 3: return '*';
	case 4: return '/';
	default:return '+';
	}
}
int Core::get_factor(int a)
{
	int i = 1;
	if (a == 1) return 1;
	for (i = a - 1; i >= 1; i--)
		if (a % i == 0)
			return i;
	return 1;
}

int Node::gcd(int p, int q)
{
	if (p == 0 && q == 0)
		return 1;
	if (p < 0)
		p = -p;
	if (q < 0)
		q = -q;
	if (q == 0)
		return p;
	else if (p == 0)
		return q;
	else if (p >= q)
		return gcd(q, p%q);
	else
		return gcd(p, q%p);
}
void Node::simp()
{
	int d = gcd(top, bottom);
	top /= d;
	bottom /= d;
}
Node Node::operator + (Node n2)
{
	Node n;
	n.top = top*n2.bottom + bottom*n2.top;
	n.bottom = bottom*n2.bottom;
	n.operat = 0;
	n.left = NULL;
	n.right = NULL;
	n.simp();
	return n;
}
Node Node::operator - (Node n2)
{
	Node n;
	n.top = top*n2.bottom - bottom*n2.top;
	n.bottom = bottom*n2.bottom;
	n.operat = 0;
	n.left = NULL;
	n.right = NULL;
	n.simp();
	return n;
}
Node Node::operator * (Node n2)
{
	Node n;
	n.top = top*n2.top;
	n.bottom = bottom*n2.bottom;
	n.operat = 0;
	n.left = NULL;
	n.right = NULL;
	n.simp();
	return n;
}
Node Node::operator / (Node n2)
{
	Node n;
	n.top = top*n2.bottom;
	n.bottom = bottom*n2.top;
	n.operat = 0;
	n.left = NULL;
	n.right = NULL;
	n.simp();
	return n;
}
Node Node::operator ^ (Node n2)
{
	Node n;
	n.top = (int)pow(top, n2.top);
	n.bottom = (int)pow(bottom, n2.top);
	n.operat = 0;
	n.left = NULL;
	n.right = NULL;
	n.simp();

	return n;
}
Node Node::operator = (Node n2)
{
	top = n2.top;
	bottom = n2.bottom;
	operat = n2.operat;
	return Node(n2.top, n2.bottom, 0);
}





