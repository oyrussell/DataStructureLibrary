#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <math.h>
using namespace std;
#include "Treap.cpp"

void printErrors(string errors, int numOfErrors){
	if(numOfErrors < 5){
		cout << errors << " caused less than 5 errors so +1 line" << endl;
	}
	if(numOfErrors < 100){
		cout << errors << " caused less than 100 errors so +1 line" << endl;
	}
	if (numOfErrors < 500){
		cout << errors << " caused less than 500 errors so +1 line" << endl;
	}
	if (numOfErrors < 5000){
		cout << errors << " caused less than 5000 errors so +1 line" << endl;
	}
	else{
		cout << errors << " caused more than 5000 errors resulting in 0 points" << endl;
	}
}

void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void removeIndex(int removeIndex);

int main(int argc, char **argv){
	int testToRun = atoi(argv[1]);
	switch (testToRun){
		case 1:
			test1();
			break;
		case 2:
			test2();
			break;
		case 3:
			test3();
			break;
		case 4:
			test4();
			break;
		case 5:
			test5();
			break;
		case 6:
			test6();
			break;
	}
	return 0;
}


void test1(){
	string K[10] = {"A","B","CC","D","E","F","G","H","I","K"};
	float P[10] = {0.2,0.95,0.79,0.4,0.5,0.3,0.8,0.1,0.9,0.6};
	
	Treap<string> T1, T2(K,P,10), T3;
	
	for(int i=0; i<10; i++) T1.insert(K[i],P[i]);
	// T1 and T2 should be identical trees
	
	cout << T2.search("CC") << endl;
	// Should output 0.79
	
	cout << T1.rank("CC") << endl;
	//Should output 3
	
	cout << T1.select(3) << endl;
	//Should output CC 
	
	T2.preorder();
	//Should output "H A F D CC B E G K I\n"
	//There should be a single 
	//newline after any of the order 
	//method calls
	
	T2.inorder();
	//Should output	A B CC D E F G H I K\n
	
	cout << T2.remove("F") << endl;
	//Should output 1
	
	T2.preorder();
	//Should output H A D CC B E G K I\n
	
	T2.inorder();
	//Should output	A B CC D E G H I K\n
	
	cout << T2.remove("MISSING") << endl;
	//Should output 0
	
	cout << T2.rank("G") << endl;
	//Should output 6
	
	cout << T1.successor("E") << endl;
	//Should output F
	
	cout << T1.predecessor("B") << endl;
	//Should output A	
    T3 = T1;
    T3.remove("F");	
	
	cout << T1.successor("E") << endl;
	//Should output F

	//Generics test
	Treap<long int> gen1;
	Treap<short int> gen2;
	Treap<double> gen3;

	cout << "Finished without failing" << endl << endl;
}

void test2(){
	
	Treap<int> X;
	
	//for (int i = 100; i>=0; i--) cout << rand()/(1.0 * RAND_MAX) << ",";
	//cout << endl; 

	float priorites[101] = {0.833196,0.985795,0.12659,0.972883,0.640143,0.975811,0.458807,0.925745,0.965442,0.43944,0.916469,0.0851875,0.205926,0.118186,
	0.487496,0.627917,0.466443,0.268288,0.244239,0.779252,0.278695,0.598347,0.385913,0.72324,0.666463,0.247342,0.190859,0.509635,0.879947,0.772612,0.484657,
	0.713143,0.758406,0.611246,0.686026,0.398549,0.587057,0.144833,0.324294,0.5525,0.584273,0.240763,0.637687,0.790199,0.358949,0.125183,0.418116,0.825392,
	0.393471,0.662355,0.604644,0.672166,0.260702,0.990557,0.395406,0.927165,0.237899,0.586265,0.4368,0.117847,0.358877,0.921457,0.830989,0.117283,0.532703,
	0.517016,0.515832,0.11976,0.661849,0.840127,0.67226,0.246122,0.0808894,0.309947,0.0363204,0.439839,0.43513,0.454436,0.265231,0.828601,0.116791,0.869875,
	0.500768,0.377494,0.860432,0.896174,0.304659,0.0983315,0.482439,0.741459,0.216178,0.841315,0.662915,0.0471674,0.958598,0.195618,0.564183,0.474431,0.315378,
	0.226032,0.314557};

	for (int i=95;i>=0;i--) X.insert(i,priorites[i]);
	
	X.preorder(); 					//Correct
    
	X.postorder(); 					//Correct
    
	X.inorder(); 					//Correct

	Treap<int> Y = X;	//Copy Constructor not working?
	for (int i=0; i < 52; i++){
		X.rank(i);
		X.remove(i);
	}

    X.preorder(); 					//Correct

    X.postorder(); 					//Correct

    X.inorder(); 					//correct

	Y.preorder(); 					//Incorrect
	
	Y.postorder(); 					//Incorrect
    
	Y.inorder(); 					//Incorrect

    // Treap<int> Z;
    // for(int i=0; i < 100300; i++) Z.insert(i,rand()/(1.0 * RAND_MAX));
    
	// //This output is not graded
	// Z.preorder();

	// //This output is not graded
	// Z.postorder();

	//This verifies the preorder and postorder finished
	cout << "Finished without failing" << endl << endl;
}

void test3(){
	int rankError = 0;
	int selectError = 0;
	int searchError = 0;
	int treeSize = 100300;
    Treap<int> X;
	float* priorities = new float[treeSize]();
	for (int i=treeSize;i>=0;i--){
		priorities[i] = rand()/(1.0 * RAND_MAX);
		X.insert(i,priorities[i]);
		//cout << "HIT THIS" << endl;
	}
	//X.inorder();
	for (int i=1;i<treeSize;i++) {
		if(X.rank(i) != i+1) rankError++; 
		//cout << "FINISHED RANK LOOP:" << endl << i << " : "<< X.rank(i) << endl;
		if(X.select(i) != i-1) selectError++;
		//cout << "FINISHED SELECT LOOP:" << endl << i << " : " << X.select(i) << endl;
		float search = X.search(i); 
		if(fabs(search - i) < 0.0001 || search != priorities[i]) searchError++; 
	}
	printErrors("Rank",rankError);
	printErrors("Select",selectError);
	printErrors("Search",searchError);
	cout << "Finished without failing" << endl << endl;
}

void test4(){
	removeIndex(13);
}

void test5(){
	removeIndex(19);
}

void test6(){
	Treap<int> X;
	int predErrors = 0;
	int succErrors = 0;
    int size = 100100;
	for (int i=size;i>=0;i--) X.insert(i,rand()/(1.0 * RAND_MAX));
	for (int i=0; i < X.size(); i++){
		int pred = X.predecessor(i);
		if(i == 0){
            if(pred == i) cout << "Successfully returns k for predecessor" << endl;
		}
        else if(pred != i-1){
            predErrors++;
        }
	}
	printErrors("Pred errors",predErrors);

	for (int i=X.size(); i >= 0; i--){
		int succ = X.successor(i);
		if(i == size){
            if(succ == i) cout << "Successfully returns k for successor" << endl;
		}
        else if(succ != i+1){ 
            succErrors++;
        }
	}	
	printErrors("Succ Errors",succErrors);
	
	cout << "Finished without failing" << endl << endl;
}

void removeIndex(int removeIndex){
	int rankError = 0;
	int selectError = 0;
	int searchError = 0;
    Treap<int> X;
	int size = 100300;
	float* priorities = new float[size]();
	for (int i=size;i>=0;i--){
		priorities[i] = rand()/(1.0 * RAND_MAX);
		X.insert(i,priorities[i]);
	}
	for (int i=0;i<size;i+=removeIndex) X.remove(i);
	for (int i=1;i<size-size/removeIndex;i++){
        
        int part1Answer = i / removeIndex;
		// Needed for round off error
		int answer = i+(i+(i+(i+(i+(i + i / removeIndex)/removeIndex)/removeIndex)/removeIndex)/removeIndex)/removeIndex;

		//printf("i is %d, Select %d, Rank %d, Search %d\n",i, answer, i-i/removeIndex, size-i);
        
        if(X.select(i) != answer){				
			selectError++;
			//cout << "i is " << i << " Select error after delete " << X.select(i)  << " should be " <<  answer << " or " << part1Answer << endl;
		}
		if (i%removeIndex) {
			//cout << i << " is i" << endl;
			if(X.rank(i) != i-i/removeIndex){
                rankError++; 
                //cout << "Rank error after delete " << X.rank(i) << " should be " << i << " - " << i-i/removeIndex << endl;
			}
			float search = X.search(i);
            if(fabs(search - i) < 0.0001 || search != priorities[i]) searchError++;// cout << "Search error after delete" << endl;
            X.remove(i);
			X.insert(i,priorities[i]);
		}
	}
	printErrors("Rank after delete",rankError);
	printErrors("Select after delete",selectError);
	printErrors("Search after delete",searchError);
	cout << "Finished without failing" << endl << endl;
}

