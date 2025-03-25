#include <stdio.h> 
#include <string.h> 

#define MAX_SIZE										 \ 
	100 // Maximum number of elements in the map 

struct map{
    int size = 0; // Current number of elements in the map 
    char keys[MAX_SIZE][100]; // Array to store the keys 
    int values[MAX_SIZE]; // Array to store the values 
}
// Function to get the index of a key in the keys array 
int getIndex(struct map &m, char key[])
{ 
	for (int i = 0; i < size; i++) { 
		if (strcmp(m.keys[i], key) == 0) { 
			return i; 
		} 
	} 
	return -1; // Key not found 
}

// Function to get the first index that matches an integer value
int getValueIndex(struct map &m, int value){
	for (int i = 0; i < size; i++) { 
		if (m.values[i] == value) { 
			return i;
		} 
	} 
	return -1; // Value not found
}

// Function to insert a key-value pair into the map 
void insert(struct map &m, char key[], int value) 
{ 
	int index = getIndex(m, key); 
	if (index == -1) { // Key not found 
		strcpy(m.keys[size], key); 
		m.values[size] = value; 
		size++; 
	} 
	else { // Key found 
		m.values[index] = value; 
	} 
} 

// Function to get the value of a key in the map 
int getValue(struct map &m, char key[]) 
{ 
	int index = getIndex(m, key); 
	if (index == -1) { // Key not found 
		return -1; //this works for port numbers as they cannot be negative
	} 
	else { // Key found 
		return m.values[index]; 
	} 
}

//attempt a function overload? no
//Function to get key from value
char * getKey(struct map &m, int value){
	int index = getValueIndex(m, value);
	if (index == -1) { // Key not found 
		return 0; //return a nullptr and detect it in call function
	} 
	else { // Key found 
		return m.keys[index];
	}
}

// Function to print the map 
void printMap(struct map &m) 
{ 
	for (int i = 0; i < size; i++) { 
		printf("%s: %d\n", m.keys[i], m.values[i]); 
	} 
}

