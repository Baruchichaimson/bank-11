/*#ifndef __EXT2_H__
#define __EXT2_H__

#include <stdint.h>
#include <sys/types.h>

typedef struct ext2_fs ext2_fs_t;
typedef struct ext2_super_block ext2_super_block_t;
typedef struct ext2_group_desc ext2_group_desc_t;
typedef struct ext2_inode ext2_inode_t;


ext2_fs_t *ext2Open(const char *device_path);
void ext2Close(ext2_fs_t *fs);
int ext2ReadSuper(ext2_fs_t *fs, ext2_super_block_t *out);
int ext2ReadGroupDesc(ext2_fs_t *fs, unsigned int group, ext2_group_desc_t *out);
int ext2ReadInode(ext2_fs_t *fs, uint32_t inode_no, ext2_inode_t *out);
int ext2PathResolve(ext2_fs_t *fs, const char *path, uint32_t *inode_no_out);
ssize_t ext2ReadFileDirect(ext2_fs_t *fs, uint32_t inode_no, void *buf, size_t max_bytes);
void ext2PrintSummary(ext2_fs_t *fs);

#endif */

#ifndef __EXT2_H__
#define __EXT2_H__

#include <stddef.h>

int Open(char* device, char* filename); 
int Read(int file_inode, char* buffer, size_t count);

#endif /* __EXT2_H__ */

