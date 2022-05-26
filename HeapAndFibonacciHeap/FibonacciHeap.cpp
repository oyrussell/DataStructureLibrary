/*
FibHeap
Owen Yeats Russell
*/

#include <cmath> //For the log function in consolidate
using namespace std;

template <class keytype>
struct FibHeapNode{
	keytype key;
	FibHeapNode* left; //previous
	FibHeapNode* right; //next
	FibHeapNode* parent; //up
	FibHeapNode* child; //down
	int degree; //for consolidate
	bool mark;  //for deletion and decreasing
};

template <class keytype>		
class FibHeap{
public:
	FibHeapNode<keytype>* m_minNode; //Minimum node pointer
	int m_numOfNodes;       //Total Size of the Fibonacci Heap

	FibHeap(){  // initialize a new and empty Fib Heap
		m_minNode = nullptr;
		m_numOfNodes = 0;
	}

    FibHeap(keytype k[], int s) {//Build Fib Heap with Keytype Array of size s
        m_minNode = nullptr;
        m_numOfNodes = 0;
        for (int i = 0; i < s; i++) {
            this->insert(k[i]);
        }
        this->_consolidate();
    }

	~FibHeap() {//Destructor
		destructorHelper(m_minNode);
	}

	void insert(keytype newKey){//Insert a Node of Value newKey
		FibHeapNode<keytype>* newNode = _create_node(newKey); //Create the Node
		_insert_node(newNode); //Insert the Node
	}

	void merge(FibHeap<keytype> &another){//Merge the current fib heap with the one passed in
		m_minNode = _merge(m_minNode, another.m_minNode); //Main Merge Call
		m_numOfNodes += another.m_numOfNodes; //Combine both the sizes to get the new size
		another.m_minNode = nullptr; //Get rid of the other min node pointer
		another.m_numOfNodes = 0; //Zero out the other heaps size
	}
		
	keytype extractMin(){ //Return the Min Key in the Fib Heap and then Removes that Node
		FibHeapNode<keytype>* minNode = extractMinHelper();
		keytype ret = minNode->key;
		delete minNode;
		return ret;
	}

    keytype peekKey(){//Return the Key of the Min Node in the Fib Heap
        return m_minNode->key;
    }

	//Required Pre-Order Print Function
    void printKey() {	// function code adapted from GO code just below C++
            FibHeapNode<keytype>* p = m_minNode;
            if (p == NULL) {
                cout << "The Heap is Empty" << endl;
                return;
            }
			preorderHelper(m_minNode);
            cout<< endl;
        }

private:

	static keytype m_minimumKey; //The pointer to the minimum node in the Fib Heap, The base ptr for operations

	FibHeapNode<keytype>* _create_node(keytype newKey){
		//Just Creates a FibHeap Node with the given keyValue
		FibHeapNode<keytype>* newNode = new FibHeapNode<keytype>;
		newNode->key = newKey;
		newNode->left = newNode;
		newNode->right = newNode;
		newNode->parent = nullptr;
		newNode->child = nullptr;
		newNode->degree = 0;
		newNode->mark = false;
		return newNode;
	}

	void _insert_node(FibHeapNode<keytype>* newNode){
		//Inserts a Node into the FibHeap Structure by using the Merge Function
		m_minNode = _merge(m_minNode, newNode);
		m_numOfNodes++;
	}

	void _remove_from_circular_list(FibHeapNode<keytype>* x){
		//Removes a node from the list by updating pointers
		if (x->right == x){
			return; // the root list only has one node before the operation
		}
		FibHeapNode<keytype>* leftSib = x->left; 
		FibHeapNode<keytype>* rightSib = x->right;
		leftSib->right = rightSib;
		rightSib->left = leftSib;
	}

	FibHeapNode<keytype>* _merge(FibHeapNode<keytype>* a, FibHeapNode<keytype>* b){
		//Merges Two FibHeaps together
		if(a == nullptr)
			return b;
		if(b == nullptr)	
			return a;
		if( a->key > b->key ) // swap node 
		{
			FibHeapNode<keytype>* temp = a;
			a = b;
			b = temp;
		}
		FibHeapNode<keytype>* aRight = a->right;
		FibHeapNode<keytype>* bLeft	= b->left;
		a->right = b;
		b->left = a;
		aRight->left = bLeft;
		bLeft->right = aRight;
		return a;	
		}

	void _make_child(FibHeapNode<keytype> *child, FibHeapNode<keytype> *parent){
		//Helper Function that makes a node the Child of Another node
		_remove_from_circular_list(child);
		child->left = child->right = child;
		child->parent = parent;
		parent->child = _merge(parent->child, child); // add child into parent's children list
		parent->degree++;
		child->mark = false;
	}

	void _consolidate(){//Fib Heap Consolidate Function As Described in Textbook
		//Ensures the Binomial Trees are not in violation
		int Dn = (int)( log2(m_numOfNodes) / log2(1.618) ) ;
		FibHeapNode<keytype>** A = new FibHeapNode<keytype>*[Dn+1];
		for (int i = 0; i < Dn+1; i++){
			A[i] = nullptr;
		}
		//Begin Textbook Consolidate after creating the list
		FibHeapNode<keytype>* x = m_minNode;
		bool breakFlag = false;
		while(true)
		{
			int d = x->degree;
			while( A[d] != nullptr)
			{
				FibHeapNode<keytype>* y = A[d]; 
				if (y == x){
					breakFlag = true;  // when y == x, all root nodes have different degree
					break;
				}					
				if ( x->key > y->key ){ // swap x and y, so x will point to the node with smaller key
					FibHeapNode<keytype>* temp = x;
					x = y;
					y = temp;				
				}
				_make_child(y, x); // make y the child of x
				A[d++] = nullptr; // now the new node has (d + 1) child, so A[d] = nullptr,d = d + 1  
			}
			if (breakFlag){
				break;
			}
			A[x->degree] = x;
			x = x->right; // to next node in the root list
		}
		m_minNode = x;  // update the m_minNode
		FibHeapNode<keytype>* iter = x;
		do {
			if ( iter->key < m_minNode->key ) 
				m_minNode = iter;
			iter = iter->right;
		} while( iter != x );
		//_sortRoots(m_minNode);
		delete []A;
	}

	void _unparent_all(FibHeapNode<keytype>* x){
		//Traverses and unlinks the parent from the nodes
		if(x == nullptr)
			return;
		FibHeapNode<keytype>* y = x;
		do {
			y->parent = nullptr;
			y = y->right;
		}while(y != x);
	}

	FibHeapNode<keytype>* extractMinHelper(){
		//Extract the Minimum node and make sure the Fib Heap is still intact
		FibHeapNode<keytype>* min = m_minNode;
		if ( min != nullptr) // the heap is not empty
		{
			_unparent_all(min->child);
			_merge(min, min->child); // merge the child circular list into root
			_remove_from_circular_list(min);
			if ( min == min->right) // the heap will be empty after the operation
				m_minNode = nullptr;
			else
			{
				m_minNode = min->right; // minNode should not be the minimum
				_consolidate();
			}
			m_numOfNodes--;
		}
		return min;
	}

	void destructorHelper(FibHeapNode<keytype>* x){
		//Traverses the Entire Fib heap deleting everything
		if ( x != nullptr )
		{
			FibHeapNode<keytype>* t1 = x;  
			do{
				FibHeapNode<keytype>* t2 = t1;
				t1 = t1->right; //Traverse to the Next node in the LL
				destructorHelper(t2->child); //Traverses the Children Recursivly
				delete t2; //Delete the Node
			} while(t1 != x); //Traverses the roots
		}
	}

	void preorderHelper(FibHeapNode<keytype>* n){
		//Prints the Fib Heap in Pre-Order I think
		FibHeapNode<keytype>* x = n;
		do{
			if(x->child == nullptr){
				if(x->parent == nullptr){
					cout << endl <<"B" << x->degree << endl;
				}
				cout << x->key << " ";
			}
			else{
				if(x->parent == nullptr){
					cout << endl <<"B" << x->degree << endl;
				}
				cout << x->key << " ";
				preorderHelper(x->child->left); //Traverse To the leftmost in the child LL
			}
			x = x->left; //Traverse Left
		}while(x != n);
	}
};