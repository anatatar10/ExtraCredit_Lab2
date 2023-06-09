#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>


void readFile(float arr[26]) {

	FILE* fin;
	int i = 0;
	fopen_s(&fin, "distribution.txt", "r");
	while (feof(fin) == 0)
	{
		fscanf_s(fin, "%f", &arr[i]);
		i++;
	}
	fclose(fin);
}

void computeFrequency(float histogram[26], char text[])
{	
	int count = 0;
	for (int i = 0; i < strlen(text); i++)
	{
		int pos;
		if (text[i] >= 'A' && text[i] <= 'Z')
		{
			count++;
			pos = text[i] - 'A';
			histogram[pos] = histogram[pos] + 1.0;
		}
		else if (text[i] >= 'a' && text[i] <= 'z')
		{
			count++;
			pos = text[i] - 'a';
			histogram[pos] = histogram[pos] + 1.0;
		}
	}
	for (int i = 0; i < 26; i++)
	{
		histogram[i] = (histogram[i] / count)*100;
	}
}

float Chi_Squared_Distance(float occurence[26], float expected[26])
{
	float distance = 0.0;
	for (int i = 0; i < 26; i++)
	{
		float numarator = (occurence[i] - expected[i])*(occurence[i] - expected[i]);
		distance = distance + numarator / expected[i];

	}
	return distance;
}

int getIndex(char alphabet1[], char character) //function to return the position of a given letter in the alphabet (normal/rotated)
{
	int i;
	for (i = 0; i < 26; i++)
	{
		if (alphabet1[i] == character)
			return i;
	}
	return -1; //returns -1 if the letter is not found
}

void encrypt(char s[], int n) {
	// TODO your code here
	// add your encryption code here. the string s will be modified in place, using a displacement of n
	char alphabet[26], rotatedAlphabet[26];
	int i, posN;
	if (n < 0)
		posN = -n;
	else
		posN = n;
	for (i = 0; i < 26; i++) //creating the normal alphabet with the small letters from 'a' to 'z'
	{
		alphabet[i] = 'a' + i;
	}
	for (i = 0; i < 26; i++) //creating the rotated alphabet with the given rotation
	{
		rotatedAlphabet[i] = alphabet[(i + posN) % 26]; //
	}
	for (i = 0; i < strlen(s); i++) //we are going through the normal message or the encrypted one, depending on n, if it is positive/negative
	{
		if (s[i] >= 'a' && s[i] <= 'z') //changing only the letters
		{
			if (n < 0) // the encrypted message is given
			{
				int index = getIndex(rotatedAlphabet, s[i]); //we take the index of the letters in the encrypted message
				s[i] = alphabet[index]; //we decode each letter accordin to the normal alphabet
			}
			else
			{
				int index = getIndex(alphabet, s[i]); //we take the index of the letters in the message
				s[i] = rotatedAlphabet[index]; //we encode each letter accordin to the normal alphabet
			}
		}
		else if (s[i] >= 'A' && s[i] <= 'Z') //in case there are upper letters in the normal/encrypted message, they are first lowered, the index is found, and transformed back into an upper letter
		{
			if (n < 0)
			{
				int index = getIndex(rotatedAlphabet, tolower(s[i]));
				s[i] = toupper(alphabet[index]);
			}
			else
			{
				int index = getIndex(alphabet, tolower(s[i]));
				s[i] = toupper(rotatedAlphabet[index]);
			}
		}
	}
}


int breakCaesar(char text[], float expected[26])
{
	float minDistance = FLT_MAX;
	int result = 0;
	for (int i = 0; i < 26; i++)
	{
		char copy[500];
		strcpy_s(copy, 500, text);
		encrypt(copy, i);
		float histogram[26] = { 0.0 };
		computeFrequency(histogram, copy);
		float distance = Chi_Squared_Distance(histogram, expected);
		if (distance < minDistance)
		{
			minDistance = distance;
			result = i;
		}
	}
	return result;
	
}

void displayMenu()
{
	printf("Menu");
	printf("\n---------------------------------\n");
	printf("0 - print menu\n");
	printf("1 - compute the normalized frequency of each character (a histogram) in a text\n");
	printf("2 - compute the Chi - square distance between two histograms\n");
	printf("3 - break the Caesar's cipher and print the permutation with the least Chi squared distance\n");
	printf("-1 - exit");
	printf("\n---------------------------------\n");
}

int main()
{
	float arr[26];
	float histogram[26] = { 0.0 };
	readFile(arr);
	displayMenu();
	int option;
	char text[500];
	printf("Enter the text: ");
	fgets(text, 500, stdin);
	printf("Enter a command: ");
	scanf_s("%d", &option);
	switch (option)
	{
	case -1:
		exit(-1);
	case 0:
		displayMenu();
		break;
	case 1:
		computeFrequency(histogram, text);
		for (int i = 0; i < 26; i++)
		{
			printf("Frequency of %c: %f\n", text[i], histogram[i]);
		}
		printf("\n");
		break;
	case 2:
		computeFrequency(histogram, text);
		float distance = Chi_Squared_Distance(histogram, arr);
		printf("The distance is %f\n", distance);
		break;
	case 3:
		int n; 
		n = breakCaesar(text, arr);
		encrypt(text, n);
		printf("\n");
		printf("%s\nN: %d\n", text, n);
		break;
	default:
		printf("\nInvalid option. Please try again\n");
		break;
	}
	
	return 0;
}