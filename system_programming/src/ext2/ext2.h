/**************************************
Exercise: 	system_programming - ext2
Date:		11/11/2025
Developer:	Baruch Haimson
Reviewer: 	Ben 
Status:		
**************************************/

#ifndef __EXT2_H__
#define __EXT2_H__

int Open(char* device, char* filename); 
int Read(int file_inode, char* buffer, size_t count);

#endif /* __EXT2_H__ */

