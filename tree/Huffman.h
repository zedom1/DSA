#include <Heap.h>

struct TreeNode
{
	int weight;
	TreeNode * left, *right;
};

TreeNode* Huffman( Heap<TreeNode*>* heap )
{
	int i;
	TreeNode * huff;
	BuildMinHeap(heap);
	for( i =1; i< heap->size; i++)
	{
		huff = new TreeNode();
		huff->left = DeleteMin(heap);
		huff->right = DeleteMin(heap);
		huff->weight = huff->left->weight + huff->right->weight;
		Insert( heap ,huff );
	}
	return huff=DeleteMin(heap);
}

