/**************************************
Exercise: 	system_programming - ext2
Date:		11/11/2025
Developer:	Baruch Haimson
Reviewer: 	Ben 
Status:		
**************************************/

#include <stdio.h> /* printf */
#include "ext2.h" /* Open */

int main() 
{
    int inode = Open("/dev/ram0", "/myfile.txt"); 
    if (inode > 0) 
    {
        char buf[4096]; 
        int n = Read(inode, buf, sizeof(buf));
        if (n > 0) 
        {
            printf("\n=== File content ===\n\n");
            fwrite(buf, 1, n, stdout);
            printf("\n=== End of file ===\n");
        }
        else
        {
            printf("File is empty or error reading.\n");
        }
    }
    else
    {
        printf("Could not open file.\n");
    }
    return 0;
}
