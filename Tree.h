#include<iostream>
#include<string>
#include<vector>
using namespace std;

template<class K,size_t M>
struct BTreeNode
{
	K _key[M];
	BTreeNode<K, M>*_child[M+1];
	BTreeNode<K, M>* _parent;
	size_t _n;

	BTreeNode()
	{
		_n = 0;
		for (size_t i = 0; i < M; i++)
		{
			_key[i] = K();
			_child[i] = nullptr;
		}
		_child[M] = nullptr;
		_parent = nullptr;
	}
};

template<class K,size_t M=3>
class B_Tree
{
	typedef BTreeNode<K, M> Node;
public:
	pair<Node*, int>find(const K& key)
	{
		Node* cur = _root;
		Node* parent = nullptr;
		while (cur)
		{
			int i = 0;
			while (i < cur->_n)
			{
				if (key < cur->_key[i])
				{
					break;
				}
				else if (key > cur->_key[i])
				{
					i++;
				}
				else
				{
					return make_pair(cur, i);
				}
			}
			parent = cur;
			cur = cur->_child[i];
		}
		
		return make_pair(parent, -1);
	}

	void insertkey(Node* node,const K&key,Node*son)
	{
		int end = (int)node->_n - 1;
		while (end >= 0)
		{
			if (node->_key[end] > key)
			{
				node->_key[end + 1] = node->_key[end];
				node->_child[end + 2] = node->_child[end + 1];
				end--;
			}
			else
			{
				break;
			}
		}
		node->_key[end + 1] = key;
		node->_child[end + 2] = son;

		if (son != nullptr)
		{
			son->_parent = node;
		}
		node->_n++;
	}
	bool insert(const K& key)
	{
		if (_root == nullptr)
		{
			_root = new Node;
			_root->_key[0] = key;
			_root->_n++;
			return true;
		}
		pair<Node*, int>p = find(key);
		if (p.second>=0)
		{
			return false;
		}

		Node* cur = p.first;
		K newkey = key;
		Node* son = nullptr;

		while (1)
		{
			insertkey(cur,newkey,son);
			if (cur->_n < M)
			{
				break;
			}
			else
			{
				Node* bro = new Node;
				size_t mid = M / 2;
				size_t index = 0;
				size_t i;
				for (i = mid + 1; i <= M - 1; i++)
				{
					bro->_key[index] = cur->_key[i];
					bro->_child[index] = cur->_child[i];
					index++;
					if (cur->_child[i] != nullptr)
					{
						cur->_child[i]->_parent = bro;
					}
					cur->_key[i] = K();
					cur->_child[i] = nullptr;
				}
				
				bro->_child[index] = cur->_child[i];
				cur->_child[i] = nullptr;

				bro->_n = index;
				cur->_n -= (index + 1);

				K midkey = cur->_key[mid];
				cur->_key[mid] = K();

				if (cur->_parent == nullptr)
				{
					_root = new Node;
					_root->_key[0] = midkey;
					_root->_child[0] = cur;
					_root->_child[1] = bro;
					_root->_n = 1;

					cur->_parent = _root;
					bro->_parent = _root;
					break;
				}
				else
				{
					newkey = midkey;
					son = bro;
					cur = cur->_parent;
				}
				
			}
		}
		return true;
	}

	/*bool erase(const K& key) //´ýÊµÏÖ
	{
		pair<Node*, int>p = find(key);
		if (p.second == -1)
		{
			return false;
		}

		Node* cur = p.first;
		if (cur->_child[0] == nullptr)
		{
			if (cur->_n >= M / 2 - 1)
			{
				int i = p.second;
				for (; i < end-1; i++)
				{
					cur->_key[i] = cur->_key[i + 1];
				}
				cur->_key[i] = K();
				return true;
			}
			else
			{

			}
		}
		else
		{

		}
	}*/

	void _inOver(Node*n)
	{
		if (n == nullptr)
			return;

		for (int i = 0; i < n->_n + 1; i++)
		{
			_inOver(n->_child[i]);
			if(i<n->_n)
			cout << n->_key[i] << " ";
		}
		//cout << endl;

	}
	void inOver()
	{
		_inOver(_root);
	}
private:
	Node* _root=nullptr;
};

void Test()
{
	B_Tree<int,1000>b;
	for (int i = 1; i <= 10000; i++)
	{
		b.insert(rand()%1000);
	}
	b.inOver();

}
