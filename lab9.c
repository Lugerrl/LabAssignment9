#include <stdio.h>
#include <stdlib.h>


// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order;
	struct RecordType* next; 
};

// Fill out this structure
struct HashType
{
	struct RecordType **table;
	int size;
};

// Compute the hash function
int hash(int x)
{
	return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;
    struct RecordType *current;

    for (i = 0; i < hashSz; ++i)
    {
        current = pHashArray->table[i];
        if (current != NULL)
        {
            printf("Index %d -> ", i);
            while (current != NULL)
            {
                printf("%d, %c, %d\n", current->id, current->name, current->order);
                current = current->next;
            }
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input_lab_9.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Hash implementation5
    struct HashType hashTable;
    hashTable.size = recordSz;
    hashTable.table = (struct RecordType**) calloc(hashTable.size, sizeof(struct RecordType*));

    if (hashTable.table == NULL)
    {
        printf("Cannot allocate memory for hash table\n");
        exit(-1);
    }

    // Insert records into hash table
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);
        struct RecordType* newRecord = (struct RecordType*) malloc(sizeof(struct RecordType));
        if (newRecord == NULL)
        {
            printf("Cannot allocate memory for new record\n");
            exit(-1);
        }
        *newRecord = pRecords[i]; // Copy record
        newRecord->next = hashTable.table[index]; // Insert at beginning of list
        hashTable.table[index] = newRecord; // Update hash table
    }

    // Display records in hash table
    displayRecordsInHash(&hashTable, hashTable.size);

    // Free memory
    for (int i = 0; i < hashTable.size; ++i)
    {
        struct RecordType* current = hashTable.table[i];
        while (current != NULL)
        {
            struct RecordType* next = current->next;
            free(current);
            current = next;
        }
    }
    free(hashTable.table);
    free(pRecords);

    return 0;
}