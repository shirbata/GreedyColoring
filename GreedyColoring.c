//VS-2015
/*
Assigned by:
shir bata 208573501 #ID
*/
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
void GreedyColoring();
typedef struct IntervalG
{
	int a; // left field of each element 
	int b;// right field of each element
	int degree;//degree of each element 
	int color;//color of each element
}IntervalG;

void main()
{
	GreedyColoring();
}
void IntervalSwap(IntervalG *v1, IntervalG *v2)//swaping 2 elements of the array
{
	IntervalG temp = *v1;
	*v1 = *v2;
	*v2 = temp;
}
void SortIntervalGraph(IntervalG* array, int k)//sorting an Interval graph
{
	//each element of the array is [a,b]
	//we want to sort the Graph by the first field a - left field a
	//if the left fields are equal in 2 elements we will sort by the right field b
	int i, j;
	for (j = 0; j < k - 1; j++)
	{
		for (i = 0; i < k - 1; i++)
		{
			if (array[i].a > array[i + 1].a)//if the left field is smaller than we will swap them
				IntervalSwap(&array[i], &array[i + 1]);
			else if (array[i].a == array[i + 1].a)//if 2 left fields are equal ,we need to compare the right sides
			{
				if (array[i].b > array[i + 1].b) // if the right side is bigger we need to swap them both
					IntervalSwap(&array[i], &array[i + 1]);
			}
		}
	}
}

void GreedyColoring()
{
	int k, i, j, m, Max_D, Min_D, ChromaticNum;
	int G_edges = 0;
	printf("please enter your input\n");
	scanf_s("%d", &k);
	IntervalG* Arr = (IntervalG*)calloc(k, sizeof(IntervalG)); //an intervalgraph array
	int* CountColor = (int*)calloc(k + 1, sizeof(CountColor));//Count array that will help us search for the chromatic color
	assert(Arr);
	printf("you will now be asked to insert a family of %d intervals\n", k);
	for (i = 0; i < k; i++)//each element of the array has 2 fields-input of each field of the struct
	{
		printf("%d Interval:\n", i + 1);
		scanf_s("%d", &Arr[i].a);
		scanf_s("%d", &Arr[i].b);
	}
	printf("the Intervals family is:\n");
	for (i = 0; i < k; i++)
	{
		if (i != k - 1)
			printf("[%d,%d],", Arr[i].a, Arr[i].b);
		else//for the last value in the array
			printf("[%d,%d]\n", Arr[i].a, Arr[i].b);
	}
	SortIntervalGraph(Arr, k);//sort the graph 
	for (i = 0; i < k - 1; i++)//searching for edges 
	{
		for (j = i + 1; j < k; j++)//we don't want to compare element to itself so we will start each time from j=i+1
		{
			if (Arr[i].b >= Arr[j].a || Arr[i].a == Arr[j].a || (Arr[i].b >= Arr[j].a) && (Arr[i].a <= Arr[j].b))
			{
				G_edges++;//so we need to increase the counter of the edges
				Arr[i].degree++;// it will help us find the max\min degree of each element
				Arr[j].degree++;//increase the degree for each element
			}
		}
	}
	printf("G Edges = %d\n", G_edges);
	Max_D = Arr[0].degree;
	Min_D = Arr[0].degree;
	for (i = 1; i < k; i++)//searching for the Max and Min degree
	{
		if (Arr[i].degree > Max_D)
			Max_D = Arr[i].degree;
		if (Arr[i].degree < Min_D)
			Min_D = Arr[i].degree;
	}
	printf("Maximun Degree of G = %d\n", Max_D);
	printf("Minumum Degree of G = %d\n", Min_D);
	for (i = 0; i < k; i++)//initalize all the colors to be 1 for search of chromatic color
		Arr[i].color = 1;

	for (i = 1; i < k; i++)//each index of the CountArr symbol a color
	{
		for (j = i; j > 0; j--)//each time we find an edge between the elements,we will increase the CountArr at the right color
		{
			if (Arr[j - 1].b >= Arr[i].a || Arr[j - 1].a == Arr[i].a || (Arr[j - 1].b >= Arr[i].a) && (Arr[j - 1].a <= Arr[i].b))
				CountColor[Arr[j - 1].color]++; // it won't be 0 anymore,it will be 1 that symbol that this color is taken

		}
		for (m = 1; m < k + 1; m++)//we will search the first color(index of CountArr) that is avaliable
		{
			if (CountColor[m] == 0)//The first color that is avaliable, 0= avaliable
			{
				Arr[i].color = m;//initalize the color of Arr[i] to be the color that is avaliable
				break;
			}
		}
		for (m = 1; m < k + 1; m++)
			CountColor[m] = 0;//initalize it to be 0 for next loop
	}

	ChromaticNum = Arr[0].color;//we will compare to the first color and search for the Chromatic from all the colors
	for (i = 1; i < k; i++)
	{
		if (Arr[i].color > ChromaticNum)//if you find a bigger want,change the value of the ChromaticNum
			ChromaticNum = Arr[i].color;
	}
	printf("Chromatic Number of G = %d\n", ChromaticNum);
	printf("G's Complement Edges = %d\n", ((k*(k - 1)) / 2 - G_edges));
	printf("Maximum Degree of G's Complement = %d\n", (k - Min_D - 1));
	printf("Minumum Degree of G's Complement = %d\n", (k - Max_D - 1));
	for (i = 0; i < k; i++)//we will use the CountColor again to find an optional Coloring
	{
		for (j = 1; j < k + 1; j++)
		{
			if (Arr[i].color == j)
				CountColor[Arr[i].color]++;//each index of the CountColor array symbol a different color
		}
	}

	printf("Optional Coloring: ");
	int isFirst;
	for (i = 1; i < k + 1; i++)//printing optional group of coloring by using the Countcolor array that wev'e made 
	{
		isFirst = 1;//every new color 
		if (CountColor[i] == 0) break;
		printf("{");
		for (j = 0; j < k; j++)//searching for the elements that used this color
		{
			if (Arr[j].color == i)//if we do find,print them
			{
				if (!isFirst) //we want to print ',' only if its not before the first element
				{
					printf(",");
				}
				else//we will swich it to 0 when we reach the first element
				{
					isFirst = 0;
				}
				printf("[%d,%d]", Arr[j].a, Arr[j].b);
			}
		}
		if ((i + 1) < k)
			if (CountColor[i + 1] != 0)//if it's not the last group of color
				printf("} = %d , ", i);

			else
				printf("} = %d ", i);
	}
	free(Arr);
	free(CountColor);
}