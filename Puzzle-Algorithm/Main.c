/*
Adi Berkowitz
12/7/2016
Finding solution to puzzle
At a high level my program works like this: I add the beginning position to the Queue and hashTable, I then find every
possible position resulting from that position, add those positions to the hasatable and Queue (unless they were already visited).
I then continue pulling posititions from the Queue and finding all possible positions from that positions (and performing same actions)
until the Queue is empty.  I then find the goal position hashcode and grab the final position from the hastable and reverse
the pointers to previous positions so that my code prints all positions leading to the goal position starting from the beginning
positions.  I then use the methods given to print out the statistics:
*/


#include <stdio.h>
#include <stdlib.h>
#include <Puzzle.h>
// #include <ABPuzzle.h>
#include <Output.h>

typedef struct node {
 struct node *next; /*next pointer for the hash bucket*/
 struct node *back; /*position from which this one came*/
 int cost; /*number of moves to get to this position*/
 char piece; /*piece that moved to this position*/
 int dir; /*direction moved to enter this position*/
 char board[BoardSize]; /*this position's board*/
} PositionBody;
typedef PositionBody *TypePosition;

/***********Function to Allocate Memory for Positions*************/
TypePosition NEWPOSITION() {
	TypePosition p = (TypePosition) malloc(sizeof(PositionBody));
	if (p==NULL) {
	 	printf("Malloc for a new position failed.");
	 	exit(1);
	 }
	return p;
}

/***********Print Board Function(not used now)**************/
void printBoard(char board[]){
	for(int i = 0; i < BoardSize; i+=BoardWidth){
		printf("%c ", board[i]);
		for(int j = i+1; j<i+BoardWidth; j++){
			printf("%c ", board[j]);
		}
		printf("\n");
	}
}

/***************Queue Implamentation*****************/
int front = 0;
int rear = -1;
int numItemsInQ = 0;
int maxQ = 0;
/*************Enque**************/
void ENQUEUE(TypePosition *q, TypePosition addPos){
	if(numItemsInQ == QueueArraySize-1){
		printf("QUEUE is Overflow\n");
	}
	else{
		rear = (rear+1)%QueueArraySize;
		q[rear] = addPos;
		numItemsInQ++;
	}
}
/***********Deque***********/
TypePosition DEQUEUE(TypePosition *q){
	if(numItemsInQ == 0){
		printf("EMPTY");
		return NULL;
	}
	TypePosition toReturn = NEWPOSITION();
	toReturn = q[front];
	front = (front+1)%QueueArraySize;
	numItemsInQ--;
	return toReturn;
}


/*************Make board for start and goal board************/
void makeBoard(char board[], char board1[]){
	for(int i = 0; i < BoardSize; i++){
		board[i] = board1[i];
	}
}

/**********Check if two boards are equal**************/
int checkEqual(char board1[], char board2[]){
	int x = 0;
	for(int i = 0; i < BoardSize; i++){
		if(board1[i] != board2[i]){
			x=1;
			break;
		}
		else{
			x = 0;
		}
	}
	return x;
}

/******************Hash Function*****************/
int hashCode(TypePosition pos){
	int sum = 0;
	for(int i = 0; i < BoardSize; i++){
		char c = pos->board[i];
		// sum = sum + c;
		// sum = sum * 129;
		// sum = sum % HashArraySize;
		sum=(((int)c)+129*sum);
	}
	return abs(sum)%HashArraySize;
}

/****************Checks if position is in Table****************/
int checkHash(TypePosition *hTable, TypePosition pos){
	int check = 0;
	int x = hashCode(pos);

	if(hTable[x] != NULL){
		check = checkEqual(pos->board, hTable[x]->board);
		TypePosition hold = NEWPOSITION();
		hold = hTable[x]->next;
		while(hold != NULL && check!=0){
			check = checkEqual(pos->board, hold->board);
			hold = hold->next;

		}
		return check;
	}
	return 1;
}

/***************Adds Position to Hash Table************/
int numPositions = 0;//keeping track of positions in Hash Table
int maxBucketSize = 0;//Max Bucket Size;
int hashStats[HashStatsMAX];//Holds hashStats

void addPos(TypePosition *hTable, TypePosition pos){
	int x = hashCode(pos);
	numPositions+=1;
	if(hTable[x] != NULL){
		TypePosition hold = NEWPOSITION();
		hold = hTable[x];
		while(hold->next != NULL){
			hold = hold->next;
		}
		hold->next = pos;
		return;
	}
	hTable[x] = pos;
}

/********Searches HashTable to figure out bucket information***********/
void searchHash(TypePosition *hTable){
	for(int i = 0; i < HashArraySize; i++){
		int count= 0;
		if(hTable[i] != NULL){
			TypePosition hold = NEWPOSITION();
			hold = hTable[i];
			count+=1;
			while(hold->next != NULL){
				count+=1;

				hold = hold->next;
			}
			if(count >= maxBucketSize){
				maxBucketSize = count;
			}
		}
		hashStats[count]+=1;
	}
}

/***************Main function to find Goal Postion****************
****************By adding and taking away from the****************
****************Queue, and adds to the Hash Table****************/
void findGoal(TypePosition *queue, TypePosition *hTable, TypePosition fromQ){
	TypePosition newPos = NEWPOSITION();
	for(int i = 0; i < BoardSize; i++){
		//check if there is a space above a letter and make sure the item being moved is not a $ sign or a space (".")	
		if ( (i-BoardWidth >=0) && (fromQ->board[i-BoardWidth] == '.' && fromQ->board[i] != '$' && fromQ->board[i] != '.')){
			for(int g = 0; g < BoardSize; g++){
				//perform switch
				if(g == i){
					char hold = fromQ->board[g];
					newPos->piece = hold;
					newPos->board[g] = fromQ->board[g-BoardWidth];
					newPos->board[g-BoardWidth] = hold;
				}
				else if(g == i-BoardWidth){}//we alredy edited that spot
				else{newPos->board[g] = fromQ->board[g];}
			}
			newPos->next = NULL;
			if (checkHash(hTable, newPos) == 1){
				newPos->back = fromQ;
				newPos->dir = 0;
				addPos(hTable, newPos);
				ENQUEUE(queue, newPos);
			}
			newPos = NEWPOSITION();
		}
		//checks if there is a space bellow a letter and make sure the item being moved is not a $ sign or a space (".")	
		if((i+BoardWidth <= BoardSize-1)&&(fromQ->board[i+BoardWidth] == '.' && fromQ->board[i] != '$' && fromQ->board[i] != '.')){
			for(int g = 0; g < BoardSize; g++){
				//perform switch
				if(g == i){

					char hold = fromQ->board[g];
					newPos->piece = hold;
					newPos->board[g] = fromQ->board[g+BoardWidth];
					newPos->board[g+BoardWidth] = hold;
				}
				else if(g == i+BoardWidth){}//already edited this spot
				else{newPos->board[g] = fromQ->board[g];}
			}
			newPos->next = NULL;
			if (checkHash(hTable, newPos) == 1){
				newPos->back = fromQ;
				newPos->dir = 2;
				addPos(hTable, newPos);
				ENQUEUE(queue, newPos);
			}
			newPos = NEWPOSITION();
		}
		//checks if there is a space ahead of a letter and make sure the item being moved is not a $ sign or a space (".")	
		if(( (i+1)%BoardWidth != 0)&& (fromQ->board[i+1] == '.' && fromQ->board[i] != '$' && fromQ->board[i] != '.')) {
			for(int g = 0; g < BoardSize; g++){
				//perform switch
				if(g == i){
					char hold = fromQ->board[g];
					newPos->piece = hold;
					newPos->board[g] = fromQ->board[g+1];
					newPos->board[g+1] = hold;
				}
				else if(g == i+1){}//already edited this spot
				else{newPos->board[g] = fromQ->board[g];}
			}
			newPos->next = NULL;
			if (checkHash(hTable, newPos) == 1){
				newPos->back = fromQ;
				newPos->dir = 1;
				addPos(hTable, newPos);
				ENQUEUE(queue, newPos);
			}
			newPos = NEWPOSITION();
		}
		//checks if there is a space behind a letter and make sure the item being moved is not a $ sign or a space (".")	
		if( ((i)%BoardWidth !=0)&& (fromQ->board[i-1] == '.' && fromQ->board[i] != '$' && fromQ->board[i] != '.')){
			for(int g = 0; g < BoardSize; g++){
				//perform switch
				if(g == i){
					char hold = fromQ->board[g];
					newPos->piece = hold;
					newPos->board[g] = fromQ->board[g-1];
					newPos->board[g-1] = hold;
				}
				else if(g == i-1){}//already edited this spot
				else{newPos->board[g] = fromQ->board[g];}
			}
			newPos->next = NULL;
			if (checkHash(hTable, newPos) == 1){
				newPos->back = fromQ;
				newPos->dir = 3;
				addPos(hTable, newPos);
				ENQUEUE(queue, newPos);
			}
			newPos = NEWPOSITION();
		}
	}
}



/***************************************************************
							MAIN
***************************************************************/
int main() {
/**************Initialize HashTable and Queue*************/
	TypePosition point = NEWPOSITION();
	TypePosition *queue = malloc(sizeof(*point)*QueueArraySize);
	TypePosition *hTable = malloc(sizeof(*point)*HashArraySize);

/*****Assigning NULL to all indexes of Hash Table*******/	
	for(int i = 0; i < HashArraySize; i++){
		hTable[i] = NULL;
	}

/*********Creating start Position and************
**********Making back and next = NULL***********/
	TypePosition firstPosition = NEWPOSITION();
	makeBoard(firstPosition->board, StartBoard);
	firstPosition->next = NULL;
	firstPosition->back = NULL;

/***adding first position to hashTable and then adding to Queue***/
	checkHash(hTable, firstPosition);
	addPos(hTable, firstPosition);
	ENQUEUE(queue, firstPosition);

/**********method to run the program to search spots*********/
	while(numItemsInQ > 0){
		if(numItemsInQ > maxQ){//makes sure we know the max amount of items ever in Q
			maxQ = numItemsInQ;
		}
		TypePosition fromQ = NEWPOSITION();
		fromQ = DEQUEUE(queue);
		if(checkEqual(fromQ->board, GoalBoard) == 0){
			break;;
		}
		findGoal(queue, hTable, fromQ);
	}
/******Find Bucket information in HTable*******/
	searchHash(hTable);


/*******This part is grabs the actual Goal postion from**********
********The Hashtable, and going through bucket if needed*******/
	TypePosition goalPos = NEWPOSITION();
	makeBoard(goalPos->board, GoalBoard);//need goal position to get hash code
	TypePosition stats = NEWPOSITION();
	int x = hashCode(goalPos);
	stats = hTable[x];
/********While loop to find goal position from hashtable***********/
	while(stats->next != NULL){
		int f = checkEqual(stats->board, GoalBoard);
		if(f == 0){
			break;
		}
		else{
			stats = stats->next;
		}
	}

/******Reverses list - so that list begins with start position*******/
	TypePosition hold = NEWPOSITION();
	stats->next = NULL;
	while(stats->back != NULL){
		hold = stats;
		stats = stats->back;
		stats->next = hold;
	}
/*********Prints the steps, board, and direction***********/
	int step = 0;
	while(stats!=NULL){
		OutputPosition(stats->board, step, stats->piece, stats->dir);
		stats = stats->next;
		step++;
	}
/*********Prints the rest of the stats**********/
	QueueStats(QueueArraySize, maxQ, front, rear);
	HashStats(HashArraySize, numPositions, maxBucketSize);
	for(int i = 0; i <=maxBucketSize; i++){
		BucketStat(i, hashStats[i]);
	}

	return 1;

}

