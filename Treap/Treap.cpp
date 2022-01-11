/*
Owen Russell
CS201 Phase 2 TREAP Project

This data type maintains a Binary Search Tree and Heap in one data structure.
Larger Priority Floats are near the top with smaller floats towards the leaves

Bugs:
    -Updating Size of Subtrees on delete
    -Rank after delete
    -Select after delete
*/

#include <iostream>
using namespace std;

template <typename keyType>
class Node { //~Node class for the treap to use with built in node constructors for both structure types
public:
        keyType key;    //Holds the key   
        float priority; //Priority of the node
        Node* parent;   //Pointer to the parent of the node
        Node* left;     //Pointer to the left child
        Node* right;    //Pointer to the right child
        int lSize;      //Size of Left Subtree
        int rSize;      //Size of Right Subtree

        Node(keyType k, float p){ //~Node constructor with key and priority
            lSize = 0; //Set sizes to 0
            rSize = 0; //Set sizes to 0
            key = k; //Assign Key
            priority = p; //Assign Priority
            left = nullptr; //Initially set all pointers to null
            right = nullptr; //Initially set all pointers to null
            parent = nullptr; //Initially set all pointers to null
        }       

        Node(keyType k){ //~Node Construction with key and random priority
            lSize = 0; //Set sizes to 0
            rSize = 0; //Set sizes to 0
            key = k; //Assign Key
            priority = (float)(rand() % 100) / 100;  //Generate random priority
            left = nullptr; //Initially set all pointers to null
            right = nullptr; //Initially set all pointers to null
            parent = nullptr; //Initially set all pointers to null
        }
};


template <typename keyType>
class Treap{
    private:
        Node<keyType>* root; //Root node of the treap
        int totalSize;       //Size of the entire TREAP

        Node<keyType>* rightRotate(Node<keyType>* root){//~Right Rotate Function
            Node<keyType>* l = root->left; //L is the left child
            Node<keyType>* lr = l->right; //LR is the right child of the left node
            l->parent = root->parent;  //L parent is root parent
            if(l->parent != nullptr && l->parent->left == root){
                //If the left childs parent is not NULL and the left child is equal to root
                l->parent->left = l;
                //then the roots left child is L
            } 
            else if(l->parent != nullptr && l->parent->right == root){
                //If the left child parent is not null and the right child is the root
                l->parent->right = l;
                //Then the roots right child is L
            }
            root->lSize = l->rSize; //the left subtree size is the right subtree size of L
            l->rSize = root->rSize + root->lSize + 1; //L's right subtree size is now the roots right subtree size plus left subtree size plus the rotated node
            root->left = lr; //Roots left child is now lr
            if(lr != nullptr){ //If lr is not null
                lr->parent = root; //then lr's parent is the root
            }
            l->right = root; //L's right child is root
            root->parent = l; //Root's parent is l
            return root->parent; //Return the root's parent
        }

        Node<keyType>* leftRotate(Node<keyType>* root){//~Left Rotation Function
            Node<keyType>* r = root->right; //R is the right child
            Node<keyType>* rl = r->left; //RL is the left child of the right node
            r->parent = root->parent;  //R's Parent is now the parent of the root node passed in
            if(r->parent != nullptr && r->parent->left == root){
                //If r->parent is not null and r's parent left child is equal to the root node passed in
                r->parent->left = r;
                //Then r is a left child
            } 
            else if(r->parent != nullptr && r->parent->right == root){
                //If r's parent is not null and the curr node passed in is the right child
                r->parent->right = r;
                //then r is a right child
            }
            root->right = rl; //the root passed in's right child is now rl
            root->rSize = r->lSize; //roots right size is now the left size of r
            r->lSize = root->lSize + root->rSize + 1; //r's left size is now roots left subtree size + roots right subtree size + 1 for the rotated node
            if(rl != nullptr){
                //if rl is not null
                rl->parent = root;
                //then rl's new parent is the root passed in
            }
            r->left = root; //r's left child is the root
            root->parent = r; //the root passed in parent is now r
            return root->parent; //return the roots parent
        }

        void destructorHelper(Node<keyType>* node){//~Recursive Destructor for Treap
            if(node->left != nullptr){//Iff there is a left child recurse until you reach the leftmost child
                destructorHelper(node->left);
            }
            if(node->right != nullptr){//Iff there is a right child recurse until you reach the rightmost child
                destructorHelper(node->right);
            }
            delete node; //Delete the current node
        }

        void bubbleUpandDown(Node<keyType>* root){//~A helper function to check for heap order violations
            if(root->left != nullptr && (root->left->priority < root->priority || (root->left->priority == root->priority && root->left->key < root->key))){
                root = rightRotate(root);
                if(root->parent != nullptr){
                    bubbleUpandDown(root->parent);
                } 
                else{
                    this->root = root;
                }
            }
            else if(root->right != nullptr && (root->right->priority < root->priority || (root->right->priority == root->priority && root->right->key < root->key))){
                root = leftRotate(root);
                if(root->parent != nullptr){
                    bubbleUpandDown(root->parent);
                } 
                else{
                    this->root = root;
                }
            }
            return;
        }

        float regularSearch(Node<keyType>* ptr, keyType key){//~Standard Search
            while(true){
                if(ptr->key == key){ //If the key we are looking for is the curr key
                    return ptr->priority;//return the curr's priority
                }
                else if(ptr->key > key){ //If the curr key is larger than the key we want
                    if(ptr->left != nullptr){ //And there is a left child
                        ptr = ptr->left; //Traverse left
                    } 
                    else {
                        return -1; //If there is no left child the key asked for is not in the tree return -1
                    }
                }
                else if(ptr->key < key){//If the curr key is less than the key we want to find
                    if(ptr->right != nullptr){//And there is a right child
                        ptr = ptr->right;//Traverse right
                    } 
                    else {
                        return -1;//If there is no right child then the key we are looking for is not in the tree return -1
                    }
                }
            }
        }

        void preOrderHelper(Node<keyType>* root){//~Print the Pre Order Traversal of the Treap Recurs
            cout<<root->key<<" "; //Print then traverse left then traverse right
            if(root->left != nullptr){
                preOrderHelper(root->left);
            }
            if(root->right != nullptr){
                preOrderHelper(root->right);
            }
        }

        void inOrderHelper(Node<keyType>* root){//~Print the In Order Traversal of the Treap Recurs
            //Traverse left, print, then traverse right
            if(root->left != nullptr){
                inOrderHelper(root->left);
            }
            cout<<root->key<<" ";
            if(root->right != nullptr){
                inOrderHelper(root->right);
            }
        }

        void postOrderHelper(Node<keyType>* root){//~Print the Post Order Traversal of the Treap Recurs
            //traverse left, traverse right, then print
            if(root->left != nullptr){
                postOrderHelper(root->left);
            }
            if(root->right != nullptr){
                postOrderHelper(root->right);
            }
            cout<<root->key<<" ";
        }

        Node<keyType>* copyHelper(Node<keyType>* node){ //Copies each node recursivly
            Node<keyType>* new_node = new Node<keyType>(node->key, node->priority); //Copy node
            new_node->lSize = node->lSize; //Copy size
            if(node->left != nullptr){ //Recurse left aslong as there is a left child
                new_node->left = copyHelper(node->left);
                new_node->left->parent = new_node;
            }
            if(node->right != nullptr){ //Recurse right aslong as there is a right child
                new_node->right = copyHelper(node->right);
                new_node->right->parent = new_node;
            }
            return new_node;
        }

        int getRank(keyType k){//~Returns the index at which k would fall if the tree was a sorted list
            Node<keyType>* ptr = root; //Set temp to root
            if(ptr == nullptr){ //If the treap is empty return -1
                return -1;
            }
            int cur_rank = 1+root->lSize; //The current rank is 1 + the current nodes left subtree size
            while(true){ //While loop to traverse w/o recur
                if(ptr->key < k){ //If the current key is less than the key we want to find
                    if(ptr->right == nullptr){ //And there is no right child
                        return -1; //The key is not in the tree return -1
                    } 
                    else {//If there is a right child
                        ptr = ptr->right; //Traverse right
                        cur_rank += (1 + ptr->lSize); //Current rank = current rank + rank of the node just past
                    }
                } 
                else if(ptr->key > k){ //If the current key is larger than the key we are looking for
                    if(ptr->left == nullptr){ //And there is no left child
                        return -1; //The key is not in the tree return -1
                    } 
                    else{//If there is a left child
                        ptr = ptr->left; //Traverse left
                        cur_rank -= (1 + ptr->rSize); //The current rank is the current rank - the rank of the node we just past
                    }
                } 
                else{ //If the key is equal to the key we are trying to find
                    return cur_rank; //Return the current rank
                }
            }
        }

        int removeHelper(keyType k){//~Delete a node from the treap
            Node<keyType>* ptr = root; //Temp node is assigned to the root
            if(ptr == nullptr){ //If the treap is empty
                return 0; //return 0
            }

            while(true){ //While loop to travers without recur
                if(ptr->key == k){ //If the current key is equal to the key we want to remove
                    break; //break out of the while loop
                }
                else if(ptr->key < k){ //If the curr key is less than the key we are looking for
                    if(ptr->right != nullptr){ //And there is a right child
                        ptr = ptr->right; //Traverse Right
                    } 
                    else{//If there is not a right child the key is not in the tree return 0
                        return 0;
                    }
                }
                else if(ptr->key > k){ //If the current key is larger than the key we are searching for
                    if(ptr->left != nullptr){ //And there is a left child
                        ptr = ptr->left; //Traverse left
                    } 
                    else { //If there is not a left child then the key is not in the tree return 0;
                        return 0;
                    }
                }
            }
            totalSize--;    //decrement the total size of the treap by 1 for every valid remove
            if(ptr->left == nullptr && ptr->right == nullptr){
                //If the node we want to delete doesnt have both a left and right child
                if(ptr->parent->right == ptr){
                    //if the node we want to delete is a right child
                    ptr->parent->right = nullptr;
                    //set the parents right child to null
                    //ptr->parent->rSize--; //CHANGED
                    //Decrememnt the right subtree size of the parent node
                }
                else if(ptr->parent->left == ptr){
                    //If the node we want to delete is a left child
                    ptr->parent->left = nullptr;
                    //set the parents left child to null
                    //ptr->parent->lSize--; //CHANGED
                    //Decrement the left subtree size of the parent node by 1
                }
                delete ptr; //Delete the node
                return 1; //Return 1 for a successful delete
            }

            if(ptr->left == nullptr && ptr->right != nullptr){
                //If the node we want to delete doesn't have a laft child and has a right child
                if(ptr->parent->right == ptr){
                    //And the node we want to delete is a right child
                    ptr->parent->right = ptr->right;
                    //Set the parents right child to the right child of the node we want to delete
                    ptr->right->parent = ptr->parent;
                    //Set the new right child's parent to the paren't of the node we are deleting
                } 
                else if(ptr->parent->left == ptr){
                    //If the node we want to delete is a left child
                    ptr->parent->left = ptr->right;
                    //Set the new left child to the right child of the node we are deleting
                    ptr->right->parent = ptr->parent;
                    //Set the new left nodes parent to the parent of the node we are deleting
                }
                Node<keyType>* rev_ptr = ptr; //Set the new temp pointer to the pointer of the node we are trying to delete
                while(true){ //While loop to traverse
                    if(rev_ptr->parent == nullptr){
                        //If the parent of the node we are trying to delete is null do nothing
                        break;
                    }
                    if(rev_ptr->parent->key <= rev_ptr->key){
                        //If the key of the node we are trying to delete is less than or equal to the parent's key
                        rev_ptr->parent->rSize--; //Take one off of the parents right size;
                    } 
                    else{//If the key of the parent is larger than the key we are deleting
                        //Take one off the parent's left subtree size
                        rev_ptr->parent->lSize--;
                    }
                    rev_ptr = rev_ptr->parent; //traverse up the tree
                }
                delete ptr; //Delete the node
                return 1; //Return 1 for a successful delete and size update
            }

            else if(ptr->right == nullptr && ptr->left != nullptr){
                //If the node we want to delete doesn't have a right child and has a left child
                if(ptr->parent->right == ptr){
                    //And the node we want to delete is a right child
                    ptr->parent->right = ptr->left;
                    //Set the parent's right child to the left child of the node we are deleting
                    ptr->left->parent = ptr->parent;
                    //Set the new right childs parent to the parent of the node we are deleting
                }
                else if(ptr->parent->left == ptr){
                    //if the node we want to delete is a left child
                    ptr->parent->left = ptr->left;
                    //Set the new paren'ts left child to the left child of node we are deleting
                    ptr->left->parent = ptr->parent;
                    //set the new left child's parent to the parent of the node we are deleting
                }
                Node<keyType>* rev_ptr = ptr; //Set a temp pointer equal to the node we are deleting to update sizes
                while(true){ //While loop to traverse up the tree
                    if(rev_ptr->parent == nullptr){ //If the parent is null no updates required?
                        break;
                    }
                    if(rev_ptr->parent->key <= rev_ptr->key){ //If the parents key is less than or equal to the key we are deleting
                        rev_ptr->parent->rSize--; //Decrement the parents right subtree size by 1
                    }
                    else{ //If the parents key is larger than the key we are deleting
                        rev_ptr->parent->lSize--; //Decrement the parents left subtree size by 1
                    }
                    rev_ptr = rev_ptr->parent; //Traverse up the tree
                }
                delete  ptr; //Delete the node
                return 1; //Return 1 for a successful delete and size update
            }

            else{//If the node we want to delete has two children
                Node<keyType>* find_pred = ptr->left; //Set the Pred node to the deletion node's left child
                bool is_right = false; //Create an idicator to see if the predeccessor is a right child
                while(find_pred->right != nullptr){ //Wile there is a right child
                    find_pred = find_pred->right; //Then the predecessor is the rightmost child in the left subtree
                    is_right = true; //Set the indicator to true
                }
                ptr->key = find_pred->key; //Set the new key to the predeccessor's  key
                ptr->priority = find_pred->priority; //Set the new priority to the predeccessor's priority
                if(is_right){ //If the predeccessor is a right node
                    find_pred->parent->right = find_pred->left; //Set the parent's right node to the predecessors left node
                    if(find_pred->left != nullptr){ //If the predecessor has a left child
                        find_pred->left->parent = find_pred->parent; //Then the predecessors left child's parent is the parent of the predecessor's node
                    }
                }
                else{//If the predeccessor is not a right child
                    find_pred->parent->left = find_pred->left; //Set the predeccessors's parent's left child to the left child of the predeccessor
                    if(find_pred->left != nullptr){ //If the predeccessor has a left child
                        find_pred->left->parent = find_pred->parent; //Then the predecessors left child's parent is the predecessors node's parent
                    }
                }
                Node<keyType>* rev_ptr = find_pred; //Set a new temp pointer to the predeccessor node
                while(true){ //While loop to tracerse up the treap
                    if(rev_ptr->parent == nullptr){ //If the nodes parent is null then break out of the loop
                        break;
                    }
                    if(rev_ptr->parent->key <= rev_ptr->key){//If the nodes parent key is less than or equal to the current key
                        rev_ptr->parent->rSize--; //Decrement 1 off the current parent's right node size
                    }
                    else{//If the nodes parent key is larger than the current key
                        rev_ptr->parent->lSize--; //Decrement 1 of the current parent's left node size
                    }
                    rev_ptr = rev_ptr->parent; //Update the current node to traverse up the treap
                }
                bubbleUpandDown(ptr); //Check for any heap order violations in the treap
                delete find_pred; //Delete the predecessor node
                return 1; //Return 1 for a successful delete and size update
            }
            return 0; //If it cant successfully delete a node asked for return 0
        }

        void randomInsert(keyType k){//~Insert with Random Priority
            totalSize++; //Increment total size by one on every insert
            if(root == nullptr){ //If this is the first insert
                root = new Node<keyType>(k); //Use the random Node Constructor to create the root
                return; //Break out of insert after assigning root
            }
            Node<keyType>* ptr = root; //Temp set the ptr to the root of the treap
            while(true){ //While loop to traverse the treap non recursivly
                if(k < ptr->key){
                    if(ptr->left != nullptr){ //If the key is less than the current nodes key, and the current node has a left child
                        ptr->lSize++; //Add one to the current nodes left subtree size [CHANGED?]
                        ptr = ptr->left; //Then traverse left
                    }
                    else{ //If the key is less than the current nodes key, and the current node doesn't have a left child
                        ptr->left = new Node<keyType>(k); //Use the random priority Node Constructor to create the node
                        ptr->left->parent = ptr; //Assign the parent
                        ptr->lSize++; //Add 1 to the left subtree size of the current node
                        bubbleUpandDown(ptr); //Fix any heap order violations [Sizes must also update]
                        return; //Break out of the insert function
                    }
                } 
                else {
                    if(ptr->right != nullptr){ //If the key is larger than the current nodes key, and the current node has a right child
                        ptr->rSize++; //Add 1 to the size of right subtree for the current node
                        ptr = ptr->right; //Traverse right
                    } 
                    else{ //If the key is larger than the current nodes key, and the current node doesn't have a right child
                        ptr->right = new Node<keyType>(k); //Use the random priority node constructor to create the right child
                        ptr->right->parent = ptr; //Assign the right nodes parent
                        ptr->rSize++; //Add 1 to the parents right subtree size
                        bubbleUpandDown(ptr); //Fix any heap order violations [Sizes must also update]
                        return; //Break out of the insert function
                    }
                }
            }
        }

        void mainInsertHelper(keyType k, float p){//~Insert with Known Priority and Key
            totalSize++; //Add one to total size on every insert
            if(root == nullptr){//If this is the first insert
                root = new Node<keyType>(k,p); //Assign root and break out of insert [NODE CONSTRUCTOR]
                return; //Break out of insert
            }
            //If this isn't the first insert
            Node<keyType>* ptr = root; 
            while(true){ //While loop to traverse the treap w/o recursion
                if(k < ptr->key){
                    if(ptr->left != nullptr){ //If k is less than the current nodes key, and the current node has a left child
                        ptr->lSize++; //Increase the left subtree size
                        ptr = ptr->left; //Then Traverse Left
                    } 
                    else{ //If k is less than the current node, and the current node has no left child
                        ptr->left = new Node<keyType>(k,p); //Create the new Node to the Left [NODE CONSTRUCTOR]
                        ptr->left->parent = ptr; //Assign the parent
                        ptr->lSize++; //Add 1 to the left size of the parent node
                        bubbleUpandDown(ptr); //Fix any heap order violations [UPDATE SIZE ON ROTATE broken]
                        return; //Break out of insert
                    }
                } 
                else {
                    if(ptr->right != nullptr){ //If k is larger than the current nodes key, and the current node has a right child
                        ptr->rSize++; //Add 1 to the size of right subtree of current node
                        ptr = ptr->right; //Then trasverse right
                    } 
                    else{ //If k is larger than the current nodes key, and the current node has no right child
                        ptr->right = new Node<keyType>(k,p); //Create the new node with the key and priority [NODE CONSTRUCTOR]
                        ptr->rSize++; //Add 1 to the current nodes right size
                        ptr->right->parent = ptr; //Assign the parent pointer
                        bubbleUpandDown(ptr); //Fix any heap order violations [Must update Size on rotates also]
                        return; //Break out of insert
                    }
                }
            }
        }

        keyType selectHelper(int pos){//Return the key of a node at rank pos
            keyType temp;
            if(pos > totalSize){    //If pos is larger than the entire Treap
                keyType error;
                return error;       //Return an error
            }
            Node<keyType>* ptr = root; //Set the temp ptr to the root
            if(ptr == nullptr){ //IF the treap is empty just return the empty keyType
                return temp;
            }
            int cur_rank = 1+root->lSize; //Current Rank is 1 + the roots left subtree size
            while(true){    //While loop to traverse tree
                if(cur_rank < pos){ //If current rank is < the desired rank
                    if(ptr->right == nullptr){  //If the current node doesn't have a right child
                        return temp;    //Return the temp keytype
                    }
                    else { //If it does have a right child
                        ptr = ptr->right; //Traverse right
                        cur_rank += (1 + ptr->lSize);   //New Current rank = current rank + rank of right node
                    }
                } 
                else if(cur_rank > pos){ //If the current rank is > the desired ranks
                    if(ptr->left == nullptr){ //IF the node doesn't have a left child
                        return temp; //return temp keytype
                    } 
                    else{ //If the node has a left child
                        ptr = ptr->left; //Traverse left
                        cur_rank -= (1 + ptr->rSize); //New_Rank = NewRank - rank of left node
                    }
                } 
                else{
                    return ptr->key;    //Return the key of the current node
                }
            }
            return temp; //Return the temp keyType
        }

        keyType predHelper(keyType k){//Return the key of the predeccessor of a given key
            Node<keyType>* ptr = root; //Set temp Pointer is set to the root
            while(true){ //while loop to traverse the tree
                if(ptr->key == k){ //If the current key is equal to the key we want to get
                    break; //Break out of the while loop
                } 
                else if(ptr->key > k){ //If the current key is larger than the desired key and we can traverse left, traverse left
                    if(ptr->left != nullptr){
                        ptr = ptr->left;
                    } 
                    else { //Otherwise return the key desired
                        return k;
                    }
                } 
                else if(ptr->key < k){ //If the current key is smaller than the desired key and we can traverse right, then traverse right
                    if(ptr->right != nullptr){
                        ptr = ptr->right;
                    }
                    else { //Otherwise return the key desired
                        return k;
                    }
                }
            }
            //Ptr is now the node we want to find the predeccessor of

            if(ptr->left == nullptr){ //If node we are looking for doesn't have a left child
                if(ptr->parent == nullptr){//And doesn't have a parent
                    return k; //Return the key desired
                }
                while(ptr->parent->key > k){ //While the Parent Key is larger than the desired key
                    ptr = ptr->parent; //Travel up the tree
                    if(ptr->parent == nullptr){ //If the parent doesn't exist
                        return k; //return key desired
                    }
                }
                return ptr->parent->key; //Return the parents key
            }
            else{//If the node we are looking for has a left child
                ptr = ptr->left; //Traverse Left 1 
                while(ptr->right != nullptr){ //While there is a right child
                    ptr = ptr->right; //Traverse Right
                }
                return ptr->key; //Return the nodes key
            }
        }

        keyType succHelper(keyType k){//Return the key of the successor of a given key
            Node<keyType>* ptr = root; //Set the temp pointer to the root
            while(true){ //While loop to traverse
                if(ptr->key == k){ //If the current key is equal to the one we want break out
                    break;
                } 
                else if(ptr->key > k){ //If the current key is larger than the desired key and we are able to traverse left, then traverse left
                    if(ptr->left != nullptr){
                        ptr = ptr->left;
                    } 
                    else { //Else return the key we are searching for
                        return k; //Because it doesn't exist in the treap
                    }
                } 
                else if(ptr->key < k){ //If the current key is smaller than the desired key and we are able to traverse right, then traverse right
                    if(ptr->right != nullptr){
                        ptr = ptr->right;
                    } 
                    else { //Otherwise return the key we are searching for
                        return k; //Because it doesn't exist in the treap
                    }
                }
            }
            //PTR is now the node we want to find the successor of

            if(ptr->right == nullptr){ //If the node doesn't have a right child
                if(ptr->parent == nullptr){ //And the node doesn't have a parent
                    return k; //Return the key desired
                }
                while(ptr->parent->key < k){ //While the parent key is less than the desired key
                    ptr = ptr->parent; //traverse up
                    if(ptr->parent == nullptr){//If the parent is null
                        return k; //return the key desired
                    }
                }
                return ptr->parent->key; //return the key of the parent
            }
            else{//If the node has a right child
                ptr = ptr->right; //Traverse right 1 
                while(ptr->left != nullptr){ //While we are able to traverse left
                    ptr = ptr->left; //Continue to traverse left
                } 
                return ptr->key; //Return the leftmost node's key
            }
        }

    public:
        Treap<keyType>(){//~Default Constructor [EMPTY TREAP] Working
            totalSize = 0;
            root = nullptr;
        }

        Treap<keyType>(keyType k[], float p[], int s){//~Construct with Array Working
            root = nullptr;
            totalSize = 0;
            for(int i = 0; i < s; i++){
                insert(k[i], p[i]);
            }
        }

        Treap(const Treap &source){ //~COPY CONSTRUCTOR
            totalSize = source.totalSize; //Set the size
            if(source.root == nullptr){
                root = nullptr; //Set root
            }
            root = copyHelper(source.root); //Recursively copy the treap starting at the root of the treap
        }

        Treap& operator=(Treap source){ //~COPY ASSIGNMENT OPERATOR
            totalSize = source.totalSize; //Set the size
            if(source.root == nullptr){ //If the Treap is empty just set root to null
                root = nullptr; //Set root
            }
            root = copyHelper(source.root); //Recursively copy the treap starting at the of the treap
            return *this; //Return the treap
        }

        ~Treap(){//~TREAP DESTRUCTOR
            if(root == nullptr){
                return; //If the treap is empty do nothing
            }
            else{
                destructorHelper(root); //Else recursievly destruct the treap from the current treap's root
            }
        }

        int rank(keyType k){//~Main Rank Call
            return getRank(k); //Private to Hide Root
        }

        void insert(keyType k, float p){//~MAIN INSERT FUNCTION [KEY AND PRIORITY GIVEN]
            mainInsertHelper(k, p); //Private to Hide Root
        }

        void insert(keyType k){//~Insert with random Priority
            randomInsert(k); //Private to Hide Root
        }

        int remove(keyType k){//~Main Remove Function
            return removeHelper(k); //Remove is private to hide the root node
        }

        keyType select(int pos){//~Return the key at rank given
            return selectHelper(pos);
        }

        keyType predecessor(keyType k){//~Return the predecessor to a given key
            return predHelper(k);
        }

        keyType successor(keyType k){//~Return the successor to a given key
            return succHelper(k);
        }

        int size(){//~Just returns the size of the entire treap
            return totalSize;
        }

        float search(keyType k){//~Using a given key search and return priority
            return regularSearch(root, k);
        }

        void preorder(){//~Print PreORder
            preOrderHelper(root);
            cout<<endl;
        }

        void inorder(){//~Print InORder
            inOrderHelper(root);
            cout<<endl;
        }

        void postorder(){//~Print PostORder
            postOrderHelper(root);
            cout<<endl;
        }

} ;