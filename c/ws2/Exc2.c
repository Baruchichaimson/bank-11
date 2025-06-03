#include <stdio.h>
#include <stdlib.h>

void CopyArray(int* arr, size_t size, int* newArr)
{
    size_t i;
    for(i = 0; i < size; i++)
    {
        newArr[i] = arr[i];
    }
}

int arr1[] = {1, 2, 3, 4, 5};
int arr2[] = {0, 0, 0, 0, 0, 0};
int arr3[] = {-1, -2, -3, -4};
int arr4[] = {5, 5, 5, 5, 5, 5, 5};

int main()
{
    size_t i;
    size_t size1 = sizeof(arr1) / sizeof(arr1[0]);
    size_t size2 = sizeof(arr2) / sizeof(arr2[0]);
    size_t size3 = sizeof(arr3) / sizeof(arr3[0]);
    size_t size4 = sizeof(arr4) / sizeof(arr4[0]);

    int* newArr1;
    int* newArr2;
    int* newArr3;
    int* newArr4;

    newArr1 = (int*)malloc(size1 * sizeof(int));
    newArr2 = (int*)malloc(size2 * sizeof(int));
    newArr3 = (int*)malloc(size3 * sizeof(int));
    newArr4 = (int*)malloc(size4 * sizeof(int));

    if (!newArr1 || !newArr2 || !newArr3 || !newArr4)
    {
        printf("Memory allocation failed\n");
        free(newArr1);
        free(newArr2);
        free(newArr3);
        free(newArr4);
        return 1;
    }

    CopyArray(arr1, size1, newArr1);
    CopyArray(arr2, size2, newArr2);
    CopyArray(arr3, size3, newArr3);
    CopyArray(arr4, size4, newArr4);

    printf("newArr1 is = ");
    for (i = 0; i < size1; i++)
    {
        printf("%d ", newArr1[i]);
    }
    printf("\n");

    printf("newArr2 is = ");
    for (i = 0; i < size2; i++)
    {
        printf("%d ", newArr2[i]);
    }
    printf("\n");

    printf("newArr3 is = ");
    for (i = 0; i < size3; i++)
    {
        printf("%d ", newArr3[i]);
    }
    printf("\n");

    printf("newArr4 is = ");
    for (i = 0; i < size4; i++)
    {
        printf("%d ", newArr4[i]);
    }
    printf("\n");

    free(newArr1);
    free(newArr2);
    free(newArr3);
    free(newArr4);

    return 0;
}

