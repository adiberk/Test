/*Adi Berkowitz
*/
#include <stdio.h>
#include <stdlib.h>

/*create a vertex with two fields.  One of them a field of that vertex.*/
typedef struct node {
   struct node *next;
   int data;
   } LinkList;


/*Function to allocate memory for a new vertex.*/
LinkList *allocateVertex() {
	struct node *p = malloc(sizeof(struct node));
	if (p==NULL){
		printf("Memory allocation error!"); 
		exit(1);
	}
	return p;
}
/*Inserts data into the linked list*/
void INSERT(LinkList *top, int val) {
    LinkList *list = top;//coppies top to list(the pointer)
    while (list->next != NULL) { //have to go through entire list.
    	//place val into list in between numebrs it is greater than, and the number it is less than.
    	if(val < list->data){
    		int holdNum = list->data;//hold the data information
			list->data = val;//set the data to val
			LinkList *nextL = allocateVertex();//create new linkList object
			nextL->data = holdNum;//make this data be holdNum
			nextL->next = list->next;//then connect this item into the rest of the lsit;
			list->next = nextL;//connect the list from behind as well.
			return;
		}
        list = list->next;
    }
    //this is used to check the last one to see if val is less than the last number
    if(val < list->data){
    	//its a little different form the last call in that we put in null at the end.
		int holdNum = list->data;
		list->data = val;
		list->next = allocateVertex();
		LinkList *nextL = list->next; //nextL = the space allocated with list->next;
		nextL->data = holdNum;
		nextL->next = NULL;
		return;
	}
	//otherwise, all we have to do is add the data to the end of the linked list.
    list->next = allocateVertex();
   	LinkList *nextL = list->next;
	nextL->data = val;
	nextL->next = NULL;
}

/*Function to print the items of a list, one per line.*/
void printList(LinkList *node) {
	LinkList *list = node;
    while (list != NULL) {
        printf("%d\n", list->data);
        fflush(stdout);
        list = list->next;
    }
}

//this method removes the list from memory
void delete(LinkList *list){
	LinkList *hold = list;
    while(hold->next) {
        hold = list->next;
        free(list);
        list = hold;
    }
}
/***************************************************************/
/*main                                                         */
/***************************************************************/
int main() {
	char answer;
	do{
		LinkList *v = allocateVertex();//allocate meory for this link
		printf("Hello.\n");
		fflush(stdout);//Adi suggested this so that my statements would appear when they should.
		printf("INPUT LIST (One integer per line followed by an empty line)(input q to finish)\n");
		fflush(stdout);
		int num;
		int count =1;//will be used to let me know if list is empty);
		char buffer[256];//use this to grab numbers
		do{
			scanf("%s", buffer);
			fflush(stdin);//adi mentioned that I should use this becuase I may have leaking
			num = atoi(buffer);//makes the char number into an int
			if(buffer[0] == 'q')break;//this is my exit flag!
			//we want to do this for the first one because the computer give list->data a value(that is large)
			//and we don't want it to have a value, nor do we want to compare anything yet;
			else if(count == 1){
				v->next = NULL;//If this is the first item, I create it before doing the insert method
				v->data = num;//set the value
				count++;//add one to count
			}
			else{
				INSERT(v, num);//Inserts and orders the lsit
				count++;
			}
		}while(buffer[0] != 'q');

		printf("\nSORTED LIST:\n");
		fflush(stdout);
		if(count == 1){
			printf("List is empty");
		}
		else{
			// orderList(v);
			printList(v);//print the LinkList
			delete(v);//want to remove list so as to free up memory
		}
		printf("\n");
		printf("Do it again (y/n)? ");
		fflush(stdout);
		scanf("%c", &answer);
	}while(answer == 'y');
	printf("Goodbye");
	return 0;
}
