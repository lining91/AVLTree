#include <iostream>
using namespace std;


struct AVLTreeNode{
public:
	int key;						// 关键字(键值)
	int nHeight;					// 高度
	AVLTreeNode *pLeftChild;		// 左孩子节点
	AVLTreeNode *pRightChild;		// 右孩子节点

	AVLTreeNode( int value, AVLTreeNode *l, AVLTreeNode *r ) : key(value), nHeight(0),pLeftChild(l),pRightChild(r) 
	{
	}
};

class AVLTree
{
public:
	AVLTree();			
	~AVLTree();

	void Preorder();		//	前序遍历
	void Inorder();			//	中序遍历
	void Postorder();		//	后序遍历

	void Insert( int key );	//	插入指定值的节点
	void Delete( int key );			//	删除指定值的节点

	void Destroy();			//	销毁AVL树

	const int GetMiniNum();			//	返回AVL树中的最小值
	const int GetMaxNum();			//	返回AVL树中的最大值
	const int GetHeight();			//	获取AVL树的高度

	AVLTreeNode* SearchRecurse( int key );		//	查找指定值（使用递归进行）
	AVLTreeNode* Search( int key );			//	查找指定值（使用迭代器进行）
private:
	void Preorder( AVLTreeNode* pNode ) const;		//	前序遍历
	void Inorder( AVLTreeNode* pNode ) const;		//	中序遍历
	void Postorder( AVLTreeNode* pNode ) const;	//	后序遍历

	
	AVLTreeNode* Delete( AVLTreeNode* &pNode, int key );	//	删除AVL树中节点pDel,并返回被删除的节点

	void Destroy( AVLTreeNode* &pNode);

	AVLTreeNode* GetMiniNum( AVLTreeNode* pNode ) const;
	AVLTreeNode* GetMaxNum( AVLTreeNode* pNode ) const;
	int GetHeight( AVLTreeNode* pNode );

	AVLTreeNode* SearchRecurse( AVLTreeNode* pNode, int key ) const;
	AVLTreeNode* Search( AVLTreeNode* pNode, int key ) const;


	AVLTreeNode* LeftRotaion( AVLTreeNode* pNode );				//	左旋操作（单旋），返回旋转后的根节点
	AVLTreeNode* RightRotation( AVLTreeNode* pNode );				//	右旋操作（单旋），返回旋转后的根节点
	AVLTreeNode* LeftRightRotation( AVLTreeNode* pNode );			//	先左旋操作，后右旋操作（双旋），返回旋转后的根节点
	AVLTreeNode* RightLeftRotation( AVLTreeNode* pNode );			//	先右旋操作，后左旋操作（双旋），返回旋转后的根节点

	AVLTreeNode* Insert( AVLTreeNode* &pNode, int key );

private:
	AVLTreeNode* pRoot;
};


AVLTree::AVLTree( ) : pRoot( NULL )
{}

AVLTree::~AVLTree( )
{
	Destroy( pRoot );
}

void AVLTree::Preorder()
{
	Preorder( pRoot );
}

void AVLTree::Preorder( AVLTreeNode* pNode ) const
{
	if ( pNode != NULL )
	{
		cout << " " << pNode->key << " " ;
		Preorder( pNode->pLeftChild );
		Preorder( pNode->pRightChild );
	}
}

void AVLTree::Inorder()
{
	Inorder( pRoot );
}

void AVLTree::Inorder( AVLTreeNode* pNode ) const
{
	if ( pNode != NULL )
	{
		Inorder( pNode->pLeftChild );
		cout << " " << pNode->key << " " ;
		Inorder( pNode->pRightChild );
	}
}

void AVLTree::Postorder()
{
	Postorder( pRoot );
}

void AVLTree::Postorder( AVLTreeNode* pNode ) const
{
	if ( pNode != NULL )
	{
		Postorder( pNode->pLeftChild );
		Postorder( pNode->pRightChild );
		cout << " " << pNode->key << " " ;
	}
}

void AVLTree::Destroy()
{
	Destroy( pRoot );
}

void AVLTree::Destroy( AVLTreeNode* &pNode)
{
	if ( pNode == NULL )
		return;

	Destroy( pNode->pLeftChild );
	Destroy( pNode->pRightChild );
	delete pNode;
	pNode = NULL;
}

void AVLTree::Insert( int key )
{

	Insert( pRoot, key );
}

AVLTreeNode* AVLTree::Insert( AVLTreeNode* &pNode, int key )
{
	if ( pNode == NULL )
		pNode = new AVLTreeNode( key, NULL, NULL );
	else if ( pNode->key > key )								//	插入值比当前节点的值小，插入到当前节点的左子树
	{
		pNode->pLeftChild = Insert( pNode->pLeftChild, key );
		if ( GetHeight( pNode->pLeftChild) - GetHeight( pNode->pRightChild ) == 2 )
		{
			if ( key < pNode->pLeftChild->key )					//	插入到左子树的左孩子节点上，进行右旋
				pNode = RightRotation( pNode );
			else if ( key > pNode->pLeftChild->key )			//	插入到左子树的左孩子节点上，进行先左旋后右旋
				pNode = LeftRightRotation( pNode );
		}
	}
	else if ( pNode->key < key )								//	插入值比当前节点的值大，插入到当前节点的右子树
	{
		pNode->pRightChild = Insert( pNode->pRightChild, key );
		if ( GetHeight( pNode->pRightChild ) - GetHeight( pNode->pLeftChild ) == 2 )
		{
			if ( key > pNode->pRightChild->key )				//	插入到右子树的右节点上，进行左旋
				pNode = LeftRotaion( pNode );
			else if ( key < pNode->pRightChild->key )
				pNode = LeftRightRotation( pNode );
		}
	}

	pNode->nHeight = max( GetHeight( pNode->pLeftChild ), GetHeight( pNode->pRightChild ) ) + 1;
	return pNode;
}

void AVLTree::Delete( int key )
{
	pRoot = Delete( pRoot, key );
}

AVLTreeNode* AVLTree::Delete( AVLTreeNode* &pNode, int key )
{
	if ( pNode == NULL )
		return NULL;

	if ( key == pNode->key )									//	找到要删除的节点
	{
		if ( pNode->pLeftChild != NULL && pNode->pRightChild != NULL )
		{
			//	左子树比右子树高，在左子树上选择节点进行替换
			if ( GetHeight( pNode->pLeftChild ) > GetHeight( pNode->pRightChild ) )
			{
				//	使用左子树的最大节点来代替被删节点，而删除该最大节点
				AVLTreeNode* pNodeTemp = GetMaxNum( pNode->pLeftChild );				//	左子树最大节点
				pNode->key = pNodeTemp->key;
				pNode->pLeftChild = Delete( pNode->pLeftChild, pNodeTemp->key );		//	递归地删除最大节点
			}
			else
			{
				//	使用最小节点来代替被删节点，而删除该最小节点
				AVLTreeNode* pNodeTemp = GetMiniNum( pNode->pRightChild );				//	右子树的最小节点
				pNode->key = pNodeTemp->key;
				pNode->pRightChild = Delete( pNode->pRightChild, pNodeTemp->key );		//	递归地删除最小节点
			}

		}
		else
		{
			AVLTreeNode* pNodeTemp = pNode;
			if ( pNode->pLeftChild != NULL )
				pNode = pNode->pLeftChild;

			if ( pNode->pRightChild != NULL )
				pNode = pNode->pRightChild;

			delete pNodeTemp;
			pNodeTemp = NULL;
			return pNode;

		}
	}
	else if ( key > pNode->key )							//	要删除的节点比当前结点大，则在右子树进行查找
	{
		pNode->pRightChild = Delete( pNode->pRightChild, key );
		if ( GetHeight( pNode->pLeftChild ) - GetHeight( pNode->pRightChild ) == 2 )
		{
			if ( GetHeight( pNode->pLeftChild->pRightChild ) > GetHeight( pNode->pLeftChild->pLeftChild ) )
				pNode = LeftRightRotation( pNode );
			else
				pNode = RightRotation( pNode );
		}

	}
	else													//	要删除的节点比当前结点小，则在左子树进行查找
	{
		pNode->pLeftChild = Delete( pNode->pLeftChild, key );
		if ( GetHeight( pNode->pLeftChild->pRightChild ) > GetHeight( pNode->pLeftChild->pLeftChild ) )
			pNode = RightLeftRotation( pNode );
		else
			pNode = LeftRotaion( pNode );
	}

	return pNode;
}

const int AVLTree::GetMiniNum()
{
	AVLTreeNode* pFindNode = GetMiniNum( pRoot );
	if ( pFindNode == NULL )
		return 0;

	return pFindNode->key;
}

AVLTreeNode* AVLTree::GetMiniNum( AVLTreeNode* pNode ) const
{
	if ( pNode == NULL )
		return NULL;

	while ( pNode->pLeftChild != NULL )
		pNode = pNode->pLeftChild;

	return pNode;
}

const int AVLTree::GetMaxNum()
{
	AVLTreeNode* pFindNode = GetMaxNum( pRoot );
	if ( pFindNode == NULL )
		return 0;

	return pFindNode->key;
}

AVLTreeNode* AVLTree::GetMaxNum( AVLTreeNode* pNode ) const
{
	if ( pNode == NULL )
		return NULL;

	while ( pNode->pRightChild != NULL )
		pNode = pNode->pRightChild;

	return pNode;
}

const int AVLTree::GetHeight()
{
	return GetHeight( pRoot );
}

int AVLTree::GetHeight( AVLTreeNode* pNode )
{
	if ( pNode == NULL )
		return 0;

	return pNode->nHeight;
}

AVLTreeNode* AVLTree::SearchRecurse( int key )
{
	return SearchRecurse( pRoot, key );
}

AVLTreeNode* AVLTree::SearchRecurse( AVLTreeNode* pNode, int key ) const
{
	if ( pNode == NULL )
		return NULL;

	if ( key == pNode->key )
		return pNode;
	
	if ( key > pNode->key )
		return SearchRecurse( pNode->pRightChild, key);
	else
		return SearchRecurse( pNode->pLeftChild, key);
}

AVLTreeNode* AVLTree::Search( int key )
{
	return Search( pRoot, key );
}

AVLTreeNode* AVLTree::Search( AVLTreeNode* pNode, int key ) const
{
	while ( pNode != NULL )
	{
		if ( key == pNode->key )
			return pNode;
		if ( key > pNode->key )
			pNode = pNode->pRightChild;
		else
			pNode = pNode->pLeftChild;
	}
	return NULL;
}

AVLTreeNode* AVLTree::LeftRotaion( AVLTreeNode* pNode )
{
	AVLTreeNode* pRChlid = pNode->pRightChild;
	pNode->pRightChild = pRChlid->pLeftChild;
	pRChlid->pLeftChild = pNode;

	pNode->nHeight = max( GetHeight( pNode->pLeftChild ), GetHeight( pNode->pRightChild ) ) + 1 ;
	pRChlid->nHeight = max( GetHeight( pRChlid->pLeftChild ), GetHeight( pRChlid->pRightChild ) ) + 1 ;

	return pRChlid;
}

AVLTreeNode* AVLTree::RightRotation( AVLTreeNode* pNode )
{
	AVLTreeNode* pLChild = pNode->pLeftChild;
	pNode->pLeftChild = pLChild->pRightChild;
	pLChild->pRightChild = pNode;

	pNode->nHeight = max( GetHeight( pNode->pLeftChild ), GetHeight( pNode->pRightChild ) ) + 1 ;
	pLChild->nHeight = max( GetHeight( pLChild->pLeftChild ), GetHeight( pLChild->pRightChild ) ) + 1 ;
	return pLChild;
}

AVLTreeNode* AVLTree::LeftRightRotation( AVLTreeNode* pNode )
{
	pNode->pRightChild = RightRotation( pNode->pRightChild );
	return LeftRotaion( pNode );
}

AVLTreeNode* AVLTree::RightLeftRotation( AVLTreeNode* pNode )
{
	pNode->pLeftChild = LeftRotaion( pNode->pLeftChild );
	return RightRotation( pNode );
}

void main( )
{
	AVLTree* pTree = new AVLTree( );
	for ( int nIdx = 0; nIdx < 10; nIdx++ )
	{
		pTree->Insert( nIdx );
	}

	cout << " 树的高度为：" << pTree->GetHeight( ) << endl;

	cout << " 前序遍历：" ;
	pTree->Preorder( );
	cout << endl;

	cout << " 中序遍历：" ;
	pTree->Inorder( );
	cout << endl;

	cout << " 后序遍历：" ;
	pTree->Postorder( );
	cout << endl;

	int nDeleteKey = 8;
	AVLTreeNode* pFindNode = pTree->SearchRecurse( nDeleteKey );
	if ( pFindNode == NULL )
		cout << " 无元素" << nDeleteKey << endl;
	else
	{
		cout << " 删除元素" << nDeleteKey << endl;
		pTree->Delete( nDeleteKey );
		cout << " 前序遍历：" ;
		pTree->Preorder( );
	}

	cout << endl;

	system( "pause");
	return;
}