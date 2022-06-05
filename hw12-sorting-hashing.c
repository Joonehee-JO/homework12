/*
 * hw12-sorting-hashing.c
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_SIZE			13	/* prime number */
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a);		//
int freeArray(int *a);
void printArray(int *a);

int selectionSort(int *a);
int insertionSort(int *a);
int bubbleSort(int *a);
int shellSort(int *a);
/* recursive function으로 구현 */
int quickSort(int *a, int n);


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key);

/* array a에대 한 hash table을 만든다. */
int hashing(int *a, int **ht);

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key);


int main()
{
	char command;
	int *array = NULL;
	int *hashtable = NULL;
	int key = -1;
	int index = -1;

	srand(time(NULL));		//난수 생성 시 중복방지를 위함

	do{
		printf("----------------------------------------------------------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&array);
			break;
		case 'q': case 'Q':
			freeArray(array);
			break;
		case 's': case 'S':
			selectionSort(array);
			break;
		case 'i': case 'I':
			insertionSort(array);
			break;
		case 'b': case 'B':
			bubbleSort(array);
			break;
		case 'l': case 'L':
			shellSort(array);
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			quickSort(array, MAX_ARRAY_SIZE);
			printf("----------------------------------------------------------------\n");
			printArray(array);

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			hashing(array, &hashtable);
			printArray(hashtable);
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key);
			printArray(hashtable);
			index = search(hashtable, key);
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);
			break;

		case 'p': case 'P':
			printArray(array);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initialize(int** a)		//정렬 전 초기화 하는 함수
{
	int *temp = NULL;	//정렬할 원소를 저장할 배열의 메모리를 받을 포인터

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) {		//a가 가리키는 것이 NULL이라면
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);	//정렬할 원소를 저장할 배열의 메모리 공간을 할당받음
		*a = temp;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else
		temp = *a;

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE;		//0~12 사이의 수가 채워짐

	return 0;
}

int freeArray(int *a)		//정렬을 위해 할당받은 메모리를 반납하는 함수
{
	if(a != NULL)			//메모리를 할당 받았다면 해당 메모리 반납
		free(a);			
	return 0;
}

void printArray(int *a)		//현재 배열의 상태를 출력하는 함수
{
	if (a == NULL) {		//메모리를 할당받지 못하였다면
		printf("nothing to print.\n");		//에러문 출력 후 함수종료
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)	//해당 원소의 index를 출력
		printf("a[%02d] ", i);
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)	//해당 index의 원소값 출력
		printf("%5d ", a[i]);
	printf("\n");
}


int selectionSort(int *a)		//선택정렬을 하는 함수
{
	int min;		//최소값을 저장하는 변수
	int minindex;	//최소값의 index를 가리키는 변수
	int i, j;		

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);	//정렬 전 현재 배열의 상태 출력

	for (i = 0; i < MAX_ARRAY_SIZE; i++)	//정렬시작
	{
		minindex = i;	//반복문 시작 시 현재 i값을 최소값의 index로 저장
		min = a[i];		//i번째 index의 값을 최소값으로 저장
		for(j = i+1; j < MAX_ARRAY_SIZE; j++)	//가장 작은 값의 index찾기 시작, j는 i+1부터 시작
		{
			if (min > a[j])		//만일 j번째 index에 있는 값이 더 작다면
			{
				min = a[j];		//min은 j번째 index의 값 저장
				minindex = j;	//minindex는 해당 index j 저장
			}
		}
		a[minindex] = a[i];	//찾은 최소값과 i번째 원소값 스왑해줌
		a[i] = min;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);	//정렬 후 배열의 현재 상태 출력
	return 0;
}

int insertionSort(int *a)	//삽입정렬을 하는 함수
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for(i = 1; i < MAX_ARRAY_SIZE; i++)
	{
		t = a[i];	//i번째 index의 값을 t에 저장(삽입할 원소)
		j = i;		//j는 현재 i값을 저장
		while (a[j-1] > t && j > 0)	//j-1번째 값보다 비교하는 원소의 값이 작다면(j는 음수면 안됨)
		{
			a[j] = a[j-1];		//j-1번째 값을 한칸 오른쪽으로 옮긴 후
			j--;				//j값 1만큼 줄임
		}
		a[j] = t;	//while루프를 빠져나온 후 삽입할 원소가 들어갈 자리에 저장
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);	//정렬 후 배열의 현재 상태 출력

	return 0;
}

int bubbleSort(int *a)	//버블정렬을 하는 함수
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);		//버블정렬 전 배열의 현재 상태 출력

	for(i = 0; i < MAX_ARRAY_SIZE; i++)	//버블정렬 시작
	{
		for (j = 1; j < MAX_ARRAY_SIZE; j++)	//원소를 하나하나 비교해나감, 제일 큰 원소가 마지막에 배치됨
		{
			if (a[j-1] > a[j])		//j-1번째 원소가 j번째 원소보다 크다면
			{
				t = a[j-1];			//두 원소를 스왑해줌
				a[j-1] = a[j];
				a[j] = t;
			}
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);		//정렬 후 현재 배열의 상태 출력

	return 0;
}

int shellSort(int *a)		//셸정렬을 하는 함수
{
	int i, j, k, h, v;		//삽입정렬을 보완하기 위해 나옴(크기가 역순으로 배치돼있을 시 n번의 비교와 이동이 필요하기에)
							//h만큼의 간격을 벗어난 레코드를 삽입정렬한다는 개념
	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);			//정렬 전 배열의 현재 상태 출력

	//h만큼의 간격을 벗어난 레코드를 삽입정렬을 하는 셸정렬
	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2)	//h는 배열사이즈 / 2로 시작, 반복문을 돌아가면서 1/2씩 줄어들음
	{
		for (i = 0; i < h; i++)
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h)
			{
				v = a[j];	//i에서부터 h만큼 벗어난 index의 값을 v에 저장
				k = j;		//j가 갖고있는 index번호 k에 저장
				while (k > h-1 && a[k-h] > v)	//k가 h-1보다 크고 h만큼 벗어난 간격에 있는 값이 더 작다면
				{
					a[k] = a[k-h];	//i에서 h만큼 벗어난 위치에 비교한 값(더 큰 값) 저장
					k -= h;
				}
				a[k] = v;			//k번째 인덱스에 v값 저장
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a);	//셸 정렬 후 현재 배열의 상태 출력

	return 0;
}

int quickSort(int *a, int n)	//퀵정렬을 하는 함수
{
	int v, t;
	int i, j;

	if (n > 1)
	{
		v = a[n-1];
		i = -1;
		j = n - 1;

		while(1)
		{
			while(a[++i] < v);
			while(a[--j] > v);

			if (i >= j) break;
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
		t = a[i];
		a[i] = a[n-1];
		a[n-1] = t;

		quickSort(a, i);
		quickSort(a+i+1, n-i-1);
	}


	return 0;
}

int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE;
}

int hashing(int *a, int **ht)
{
	int *hashtable = NULL;

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1;

	/*
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		printf("hashtable[%d] = %d\n", i, hashtable[i]);
	*/

	int key = -1;
	int hashcode = -1;
	int index = -1;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		key = a[i];
		hashcode = hashCode(key);
		/*
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		*/
		if (hashtable[hashcode] == -1)
		{
			hashtable[hashcode] = key;
		} else 	{

			index = hashcode;

			while(hashtable[index] != -1)
			{
				index = (++index) % MAX_HASH_TABLE_SIZE;
				/*
				printf("index = %d\n", index);
				*/
			}
			hashtable[index] = key;
		}
	}

	return 0;
}

int search(int *ht, int key)
{
	int index = hashCode(key);

	if(ht[index] == key)
		return index;

	while(ht[++index] != key)
	{
		index = index % MAX_HASH_TABLE_SIZE;
	}
	return index;
}



