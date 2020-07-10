#include <iostream>
#include <random>
#include <time.h>
#include <string>
#include <stack>

using namespace std;

const int DISK_NUM = 3;
const int POP_SIZE = 10;
const int MAX_GEN = 10;
const int MIN_MOVES = 7;	//2^DISK_NUM -1
const int PEGS = 3;
const int NUM_PARENT = 2;
const int cProb = 90;
const int mProb = 30;
const int EPOCH = 10;

struct gene {
	int from;
	int to;
}pool[POP_SIZE][MIN_MOVES], newPool[POP_SIZE][MIN_MOVES], children[NUM_PARENT][MIN_MOVES], parent[NUM_PARENT][MIN_MOVES];

double goalNum[POP_SIZE];
bool validity[POP_SIZE][MIN_MOVES];
double fitness[POP_SIZE];

void initialize() {
	for (int c = 0; c < POP_SIZE; c++) {
		for (int g = 0; g < MIN_MOVES; g++) {
			pool[c][g].from = rand() % 3;
			do {
				pool[c][g].to = rand() % 3;
			} while (pool[c][g].to == pool[c][g].from);
		}
	}
}

void print() {
	for (int c = 0; c < POP_SIZE; c++) {
		for (int g = 0; g < MIN_MOVES; g++) {
			cout << pool[c][g].from;
			cout << pool[c][g].to << " ";
		}
		cout << endl;
	}
}

void play() {
	stack <int> pegs[PEGS];
	int originDisk, destinationDisk;
	
	for (int p = 0; p < POP_SIZE; p++) {
		for (int i = 0; i < PEGS; i++) {
			while (!pegs[i].empty())
				pegs[i].pop();
		}
		for (int i = DISK_NUM - 1; i >= 0; i--) {
			pegs[0].push(i);
			//cout << "Disk at position " << i << " is " << i << endl;
		}
		for (int m = 0; m < MIN_MOVES; m++) {			
			//cout << "Step " << m + 1 << endl;
			if (pegs[pool[p][m].from].empty()) {
				validity[p][m] = false;
				//cout << "Invalid Origin\n";
			}
			else {
				originDisk = pegs[pool[p][m].from].top();
				//cout << "Origin Disk is: " << originDisk << endl;
				if (!pegs[pool[p][m].to].empty()) {
					//cout << "Destination not empty\n";
					destinationDisk = pegs[pool[p][m].to].top();
					//cout << "Destination Disk is: " << destinationDisk << endl;
					if (originDisk < destinationDisk) {
						validity[p][m] = true;
						//cout << "Moving\n";
						pegs[pool[p][m].from].pop();
						pegs[pool[p][m].to].push(originDisk);
					}
					else {
						validity[p][m] = false;
						//cout << "Origin disk is bigger than destination disk\n";
					}
				}
				else {
					/*cout << "Destination is empty\n";
					cout << "Moving\n";*/
					validity[p][m] = true;
					pegs[pool[p][m].from].pop();
					pegs[pool[p][m].to].push(originDisk);
				}
			}
			//if (!pegs[0].empty())
			//	cout << "Top of peg 1 is " << pegs[0].top() << endl;
			//else
			//	cout << "Peg 1 is empty\n";
			//if (!pegs[1].empty())
			//	cout << "Top of peg 2 is " << pegs[1].top() << endl;
			//else
			//	cout << "Peg 2 is empty\n";
			//if (!pegs[2].empty())
			//	cout << "Top of peg 3 is " << pegs[2].top() << endl;
			//else
			//	cout << "Peg 3 is empty\n";
			////getchar();
		}
		goalNum[p] = pegs[2].size();
	}
	// for (int i = 0; i < POP_SIZE; i++) {
	// 	for (int j = 0; j < MIN_MOVES; j++) {
	// 		cout << validity[i][j] << "  ";
	// 	}
	// 	cout << "\tOn Goal: " << goalNum[i] << endl;
	// }

}

void evaluate() {
	double fm, fg, fmSum, fgSum;
	for (int c = 0; c < POP_SIZE; c++) {
		fmSum = 0.0;
		fgSum = 0.0;
		fm = 0.0;
		fg = 0.0;
		for (int g = 0; g < MIN_MOVES; g++) {
			if (validity[c][g])
				//fmSum = fmSum + (1/((double)g+1));
				fmSum++;
			fm = (fmSum / MIN_MOVES);
		}
		fg = (double)goalNum[c] / DISK_NUM;
		fitness[c] = fm * fg;
		cout << "Fitness of " << c << ": " << fitness[c] << endl;
	}
}

//random selection
void parentSelection() {
	int p[NUM_PARENT];
	bool same = false;
	p[0] = rand() % POP_SIZE;
	int currP;
	
	for(int i = 1; i < NUM_PARENT; i++){
		do{
			same = false;
			p[i] = rand() % POP_SIZE;
			for (int j = 0; j < i; j++){
				if (p[i] == p[j]){
					same = true;
				}
			}
		}while(same);
	}
	
	cout << "The selected parents are: ";
	for (int s = 0; s < NUM_PARENT; s++){
		cout << p[s] << " ";
	}
	cout <<endl;
	
	for(int k = 0; k < NUM_PARENT; k++){
		cout << "Parent " << k << ": ";
		for(int g = 0; g < MIN_MOVES; g++){
			currP = p[k];
			parent[k][g] = pool[currP][g];
			cout << parent[k][g].from << parent[k][g].to << " ";
		}
		cout << endl;
	}
}

//one point
void crossover(){
	int cPoint;
	int cRoll;

	cRoll = rand() % 100;

	if (cRoll <= cProb){
		cPoint = (rand() % (MIN_MOVES - 1)) + 1;
		cout << "Crossover occurs at " << cPoint << endl;
		for (int g = 0; g < cPoint; g++){
			children[0][g] = parent[0][g];
			children[1][g] = parent[1][g];
		}

		for (int g = cPoint; g < MIN_MOVES; g++){
			children[0][g] = parent[1][g];
			children[1][g] = parent[0][g];
		}
	}else{
		for (int g = 0; g < MIN_MOVES; g++){
			children[0][g] = parent[0][g];
			children[1][g] = parent[1][g];
		}

	}
}

//random
void mutate(){
	int mRoll, mPoint, mValFrom, mValTo;
	for (int c = 0; c < NUM_PARENT; c++){
		mRoll = rand() % 100;
		if (mRoll <= mProb){
			cout << "Mutation occurs\n";
			mPoint = rand() % MIN_MOVES;
			do{
				mValFrom = rand() % 3;
			}while(children[c][mPoint].from == mValFrom);
			do{
				mValTo = rand() % 3;
			}while(children[c][mPoint].to == mValTo);
			children[c][mPoint].from = mValFrom;
			children[c][mPoint].to = mValTo;
		}
	}
}

void copy(int start){
	int points = start * 2;
	for (int i = points; i < points + 2; i++){
		for (int g = 0; g < MIN_MOVES; g++){
			newPool[i][g] = children[i%2][g];
		}
	}
}

void overwrite(){
	for(int c = 0; c < POP_SIZE; c++){
		for(int g = 0; g < MIN_MOVES; g++){
			pool[c][g] = newPool[c][g];
		}
	}
}

int main() {
	srand(time(NULL));
	initialize();
	
	for(int e = 0; e < EPOCH; e++){
		cout << "Epoch " << e << endl;
		print();
		play();
		evaluate();
		getchar();

		for (int p = 0; p < (POP_SIZE/NUM_PARENT); p++){
			parentSelection();
			crossover();
			mutate();
			copy(p);
		}
		overwrite();
	}

	return 0;
}