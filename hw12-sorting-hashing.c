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
int initialize(int **a);		//정렬 전 초기화하는 함수
int freeArray(int *a);			//정렬을 위해 할당받은 메모리 반납하는 함수
void printArray(int *a);		//현재 배열의 상태를 출력하는 함수

int selectionSort(int *a);		//선택정렬을 하는 함수
int insertionSort(int *a);		//삽입정렬을 하는 함수
int bubbleSort(int *a);			//버블정렬을 하는 함수
int shellSort(int *a);			//셸정렬을 하는 함수
/* recursive function으로 구현 */
int quickSort(int *a, int n);	//퀵정렬을 하는 함수


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key);	//들어온 key의 홈 버킷을 리턴하는 함수

/* array a에대 한 hash table을 만든다. */
int hashing(int *a, int **ht);	//해시테이블을 생성 후 해싱하는 함수

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key);	//해시테이블에서 입력한 key값에 맞는 버킷인덱스를 리턴하는 함수


int main()
{
	char command;
	int *array = NULL;		//배열의 메모리 공간을 가리킬 포인터
	int *hashtable = NULL;	//해시테이블을 가리킬 포인터
	int key = -1;
	int index = -1;

	srand(time(NULL));		//난수 생성 시 중복방지를 위함

	printf("[----- [Cho Joon Hee]  [2017038076] -----]\n");

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
			initialize(&array);		//정렬 전 초기화 실행
			break;
		case 'q': case 'Q':
			freeArray(array);		//프로그램 종료 전 할당받은 메모리 반납
			break;
		case 's': case 'S':
			selectionSort(array);	//선택정렬 실행
			break;
		case 'i': case 'I':
			insertionSort(array);	//삽입정렬 실행
			break;
		case 'b': case 'B':
			bubbleSort(array);		//버블정렬 실행
			break;
		case 'l': case 'L':
			shellSort(array);		//셸정렬 실행
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);		//정렬 전 현재 배열상태 출력
			quickSort(array, MAX_ARRAY_SIZE);	//퀵정렬 실행
			printf("----------------------------------------------------------------\n");
			printArray(array);		//정렬 후 배열 상태 출력

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);			//해싱 전 현재 배열 상태 출력
			hashing(array, &hashtable);	//해싱함수 실행
			printArray(hashtable);		//해싱 후 현재 해시테이블 출력
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key);			//찾을 key값 입력
			printArray(hashtable);		//함수 실행 전 현재 해시테이블 출력
			index = search(hashtable, key);	//해시테이블에서 입력한 key값에 맞는 버킷 주소를 저장
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);	//해당 버킷 주소 출력
			break;

		case 'p': case 'P':
			printArray(array);		//현재 배열 상태를 출력
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

	if (n > 1)		//n이 1보다 클 경우
	{
		v = a[n-1];	//피벗 값 설정
		i = -1;		//i는 -1부터 시작
		j = n - 1;	//j는 오른쪽부터 시작

		while(1)
		{
			while(a[++i] < v);	//피벗보다 작은 값을 찾음,i는 피벗보다 작은 index값을 가짐
			while(a[--j] > v);	//피벗보다 큰 값을 찾음,j는 피벗보다 큰 index값을 가짐

			if (i >= j) break;	//i가 j보다 크거나 같다면, 크로스되었다면 반복문탈출
			t = a[i];		//i,j가 크로스 되지 않았다면 오른쪽과 왼쪽 값을 스왑해줌
			a[i] = a[j];
			a[j] = t;
		}
		t = a[i];	//피벗값을 i번째 위치로 바꿔줌
		a[i] = a[n-1];
		a[n-1] = t;

		quickSort(a, i);		//바뀐 위치의 피벗보다 작은 값들이 위치한 부분집합 재귀적으로 퀵정렬 진행
		quickSort(a+i+1, n-i-1);	//피벗보다 큰 값들이 위치한 부분집합 퀵정렬 진행
	}


	return 0;
}

int hashCode(int key) {	//들어온 key의 홈 버킷을 리턴하는 함수
   return key % MAX_HASH_TABLE_SIZE;	//0~12 사이의 수 리턴
}

int hashing(int *a, int **ht)	//해시테이블을 생성 후 해싱하는 함수
{
	int *hashtable = NULL;	//해시테이블을 가리킬 포인터

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) {	//해시테이블 공간을 할당받지 않았었다면
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);		//메모리공간 할당받음
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)	//해싱 전 해시테이블 -1로 초기화
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
		key = a[i];		//a의 i번째 값 key에 저장
		hashcode = hashCode(key);	//해당 key값의 버킷 저장
		/*
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		*/
		if (hashtable[hashcode] == -1)	//해시테이블에서 해당 버킷이 비어있을 경우
		{
			hashtable[hashcode] = key;	//해당 버킷에 키값을 사상시킴
		} else 	{		//키값을 사상시킬 때 충돌과 오버플로과 발생한다면

			index = hashcode;	//index에 버킷값 저장

			while(hashtable[index] != -1)	//비어있는 버킷을 찾음
			{
				index = (++index) % MAX_HASH_TABLE_SIZE;	//모드 연산을 통해 index값 변경
				/*
				printf("index = %d\n", index);
				*/
			}
			hashtable[index] = key;	//비어있는 버킷에 들어온 key값 매핑시킴
		}
	}

	return 0;
}

int search(int *ht, int key)	//해시테이블에서 입력한 key값에 맞는 주소를 리턴하는 함수
{
	int index = hashCode(key);	//입력한 key값의 버킷값을 index에 저장

	if(ht[index] == key)	//해시테이블의 해당 버킷 index에 입력한 값이 매핑되어있다면
		return index;		//해당 index 리턴

	while(ht[++index] != key)	//입력한 값이 매핑되어있지 않다면 입력한 값을 찾을 때까지 반복문을 돌음
	{
		index = index % MAX_HASH_TABLE_SIZE;	//해시테이블 크기만큼 index사이즈 조정
	}
	return index;	//입력한 값이 매핑되어있는 버킷 index 리턴
}



