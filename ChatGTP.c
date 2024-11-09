#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RESET "\x1B[0m"		//Needed to turn text into bold.
#define BOLD "\x1B[1m"

typedef struct gtpList 
{
	char *concept; // the concept learned
	char *sentence; // the sentence associated with the concept
	int timesUsed; //no of times the concept was used to an answer
	char learnedFrom[5]; //learned either from "file" or "kbrd"
	struct gtpList *next;
	struct gtpList *prev;
	
}gtpList;
struct gtpList *head;

void InsertAtHead(int x);
void ReversePrint();
char* ReadText(char* input);
void addNode(gtpList** head, const char* newConcept, const char* newSentence, int newTimesUsed, const char* newLearnedFrom);
void printList(gtpList* head);
void freeList(gtpList* head);
int SCresp(gtpList** head, const char* newConcept);
void LCresp(gtpList** head, const char* newConcept);
void printDialogueToFile(const char *dialogue);
void CheckFile(void);
int CommCheck(char* str);



int main(void)
{
	char *input, *token, *str;
	int result, count=0;
	
	srand(time(NULL));
	
	head = NULL; 
	
	CheckFile();
	
	do
	{
	input = ReadText(input);
	if(strncmp(input, "learn this", 10) == 0)
		str = "kbrd";
	result = CommCheck(input);
	printDialogueToFile(strdup(input));
	
	//We split the input into 3 segments, command, concept and sentence.
	token = strtok(input, ">");	
	char command[strlen(token) + 1];
	strcpy(command, token);
	token = strtok(NULL, ":");
	char conc[strlen(token) + 1];
	strcpy(conc, token);
	token = strtok(NULL, "");
	char sent[strlen(token) + 1];
	strcpy(sent, token);
	
	switch(result)
	{
		case 1: 
			if(SCresp(&head, conc) == 0)
			{
				addNode(&head, conc, sent, 1, str);
				LCresp(&head, conc);
			}
		break;
		
		case 2:
		//remove a node
		break;
		
		case 0:
		//random response
		break;
	}
	
	/*printf("concept :%s", concept);
	printf("\nsentence :%s\n", sentence);
	*/
	printList(head);
	
	count++;
	//printf("%d %s", result, input);
	}
	while(count != 3);
	return 0;
	
}	
		
char* ReadText(char* input)  //reads users input using a buffer for strings of unknown length
{
	int buffer = 10;
	int len = 0;
	int c;
	
	printf("Enter a string: ");
    
    	//Allocate initial buffer
    	input = (char*)malloc(buffer * sizeof(char));
    	if (input == NULL) 
    	{
        	printf("Memory allocation failed.\n");
        	exit(1);
    	}
    
   	 //Read input character by character
    	while ((c = getchar()) != '\n' && c != EOF) 
    	{
        	input[len] = c;
        	len++;
        
       		//Check if buffer is full, reallocate if necessary
       		if (len >= buffer) 
       		{
            		buffer *= 2;
            		input = (char*)realloc(input, buffer * sizeof(char));
            		if (input == NULL) 
            		{
                		printf("Memory reallocation failed.\n");
                		exit(1);
            		}
        	}
    	}
    
    //Null-terminate the string
    input[len] = '\0';
    
    return(input);
    
}	


	
void addNode(gtpList** head, const char* newConcept, const char* newSentence, int newTimesUsed, const char* newLearnedFrom)     //adds a node (not alphabetically)
{
	gtpList* newNode = (gtpList*)malloc(sizeof(gtpList));
	newNode->concept = strdup(newConcept); 			//cloning newconcept and newsentence because they are const type
	newNode->sentence = strdup(newSentence);
	newNode->timesUsed = newTimesUsed;
	strncpy(newNode->learnedFrom, newLearnedFrom, sizeof(newNode->learnedFrom) - 1);
	newNode->learnedFrom[sizeof(newNode->learnedFrom) - 1] = '\0';
	

	if (*head == NULL)
	{
		newNode->prev = NULL;
		*head = newNode;
		return;
	}

	if (strcmp(newConcept, (*head)->concept) < 0)
	{
	newNode->next = *head;
	*head = newNode;
	return;
	}

	gtpList* currentNode = *head;
	while (currentNode->next != NULL && strcmp(newConcept, currentNode->next->concept) > 0) 
	{
	currentNode = currentNode->next;
	}

	newNode->next = currentNode->next;
	currentNode->next = newNode;
}


void printList(gtpList* head)		//print the whole list
{
	gtpList* currentNode = head;
	while (currentNode != NULL)
	{
		printf("timesUsed: %d Concept: %s Sentence: %s learned from : %s\n", currentNode->timesUsed, currentNode->concept, currentNode->sentence, currentNode->learnedFrom);
		currentNode = currentNode->next;
    	}
}
void freeList(gtpList* head) 		//free the whole list
{
	gtpList* currentNode = head;
	while (currentNode != NULL)
	{
		gtpList* temp = currentNode;
		currentNode = currentNode->next;
		free(temp->concept);
		free(temp->sentence);
		free(temp);
	}
}
int SCresp(gtpList** head, const char* newConcept) //ChatGTP same concept responses
{
	int res;
	char botr[200];
	
	gtpList* currentNode = *head;
	
	while(currentNode != NULL) 
	{
		if(strcmp(currentNode->concept, newConcept) == 0)
		{
			res = (rand()%5)+1;
			switch(res)
			{
				case 1:
					printf("%sChatGTP$%s Do you think I am a moron? Ofcourse I knew about %s\n", BOLD, RESET, newConcept);
					strcpy(botr, "ChatGTP$ Do you think I am a moron? Ofcourse I knew about ");
					strcat(botr, newConcept);
					printDialogueToFile(botr);
				break;
					
				case 2:
					printf("%sChatGTP$%s You're boring... I already know about %s\n", BOLD, RESET,newConcept);
					strcpy(botr, "ChatGTP$ You're boring... I already know about ");
					strcat(botr, newConcept);
					printDialogueToFile(botr);
				break;
				
				case 3:
					printf("%sChatGTP$%s haha ofcourse I already know about %s\n", BOLD, RESET,newConcept);
					strcpy(botr, "ChatGTP$ haha ofcourse I already know about ");
					strcat(botr, newConcept);
					printDialogueToFile(botr);
				break;
				
				case 4:
					printf("%sChatGTP$%s Can you try a little harder?, I've always known about %s\n", BOLD, RESET,newConcept);
					strcpy(botr, "ChatGTP$ Can you try a little harder?, I've always known about ");
					strcat(botr, newConcept);
					printDialogueToFile(botr);
				break;
				
				case 5:
					printf("%sChatGTP$%s Im losing my patience with you...I already know %s\n", BOLD, RESET,newConcept);
					strcpy(botr, "ChatGTP$ Im losing my patience with you...I already know ");
					strcat(botr, newConcept);
					printDialogueToFile(botr);
				break;
			}
			
			return 1;	
		}
		else
		{	
			currentNode = currentNode->next;
		}
	}
	
	return 0;
}

void LCresp(gtpList** head, const char* newConcept) //learned concept responses
{
	int res;
	char botr[200];
	
	gtpList* currentNode = *head;
	
	
	res = (rand()%5)+1;
	switch(res)
	{
		case 1:
			printf("%sChatGTP$%s It was very useful learning about %s\n", BOLD, RESET, newConcept);
			strcpy(botr, "ChatGTP$ It was very useful learning about ");
			strcat(botr, newConcept);
			printDialogueToFile(botr);
		break;
			
		case 2:
			printf("%sChatGTP$%s Huh weird, Somehow I didn't know about %s\n", BOLD, RESET,newConcept);
			strcpy(botr, "ChatGTP$ Huh weird, Somehow I didn't know about ");
			strcat(botr, newConcept);
			printDialogueToFile(botr);
		break;
		
		case 3:
			printf("%sChatGTP$%s Wow there is no way I didn't know about %s\n", BOLD, RESET,newConcept);
			strcpy(botr, "ChatGTP$ Wow there is no way I didn't know about ");
			strcat(botr, newConcept);
			printDialogueToFile(botr);
		break;
		
		case 4:
			printf("%sChatGTP$%s It was (not) enjoyable learning about %s\n", BOLD, RESET,newConcept);
			strcpy(botr, "ChatGTP$ It was (not) enjoyable learning about ");
			strcat(botr, newConcept);
			printDialogueToFile(botr);
		break;
		
		case 5:
			printf("%sChatGTP$%s Nice, now I know so much about %s\n", BOLD, RESET,newConcept);
			strcpy(botr, "ChatGTP$ Nice, now I know so much about ");
			strcat(botr, newConcept);
			printDialogueToFile(botr);
		break;
	}	
			
}
	
void printDialogueToFile(const char *dialogue)
{
	FILE *file;

	// Create a new file
	file = fopen("dialogue.txt", "a");
	if (file == NULL) 
	{
		printf("Error creating file.\n");
		return;
	}

	fprintf(file, "%s\n", dialogue);  // Write dialogue to the file
	fclose(file);  // Close the file
}
void CheckFile(void)
{
	FILE *file;

	// Check if file exists
	file = fopen("dialogue.txt", "r");
	if (file != NULL) 
	{
        	fclose(file);
        	remove("dialogue.txt");  // Delete existing file
	}
}	

int CommCheck(char* str)  //checks the users command
{
	if((strncmp(str, "learn", 5)) == 0)
		return 1; //learn
	else if((strncmp(str, "forget", 6)) == 0)
		return 2; //forget
	else
		return 0; //not a command
}


























