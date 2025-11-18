/**************************************
Exercise: 	system_programming - ext2
Date:		11/11/2025
Developer:	Baruch Haimson
Reviewer: 	Ben 
Status:		
**************************************/

#define _POSIX_C_SOURCE 200809L /* strdup */
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* strtok */
#include <unistd.h> /* pread */
#include <fcntl.h> /* open */
#include <errno.h> /* perror */
#include <ext2fs/ext2_fs.h> /* struct ext2_super_block */
#include <sys/stat.h> /* S_ISDIR */

#include "ext2.h"

#define BASE_OFFSET (1024)
#define SUPER_SIZE (1024)

/* Forward declarations required by C90 */
static ssize_t preadfull(int fd, void *buf, size_t count, off_t offset);
static int readinode(unsigned int inode_no, struct ext2_inode *inode_out);
static int readblock(uint32_t block_no, void *buf);
static int resolvepath(const char *path);
static int findindir(unsigned int dir_inode_no, const char *name);
static int nameequals(const char *name, const char *entry_name, unsigned short name_len);
static void printsuperandgd(void);

static int g_fd = -1;
static struct ext2_super_block g_super;
static struct ext2_group_desc g_gd;
static unsigned int g_block_size = 0;
static unsigned int g_inode_size = 0;
static unsigned long g_inode_table_block = 0;

int Open(char* device, char* filename)
{
    int fd;
    ssize_t r;
    off_t gd_offset;
    ssize_t rg;
    int inode;

    if (!device || !filename)
    {
        fprintf(stderr, "Open: device and filename required\n");
        return -1;
    }
    if (g_fd != -1)
    {
        close(g_fd);
        g_fd = -1;
    }

    fd = open(device, O_RDONLY);
    if (fd < 0)
    {
        perror("open device");
        return -1;
    }
    g_fd = fd;

    r = preadfull(g_fd, &g_super, sizeof(g_super), BASE_OFFSET);
    if (r != sizeof(g_super))
    {
        fprintf(stderr, "failed to read superblock\n");
        close(g_fd);
        g_fd = -1;
        return -1;
    }

    g_block_size = 1024u << g_super.s_log_block_size;
    g_inode_size = (g_super.s_rev_level == EXT2_GOOD_OLD_REV) ? EXT2_GOOD_OLD_INODE_SIZE : g_super.s_inode_size;

    if (g_inode_size == 0)
    {
        g_inode_size = sizeof(struct ext2_inode);
    }

    if (g_block_size == 1024)
    {
        gd_offset = (BASE_OFFSET + SUPER_SIZE);
    }
    else
    {
        gd_offset = g_block_size;
    }

    rg = preadfull(g_fd, &g_gd, sizeof(g_gd), gd_offset);
    if (rg != sizeof(g_gd))
    {
        fprintf(stderr, "failed to read group descriptor\n");
        close(g_fd);
        g_fd = -1;
        return -1;
    }

    g_inode_table_block = g_gd.bg_inode_table;

    printsuperandgd();

    inode = resolvepath(filename);
    if (inode < 0)
    {
        fprintf(stderr, "error resolving path: %s\n", filename);
        return -1;
    }

    return inode;
}

int Read(int file_inode, char* buffer, size_t count)
{
    struct ext2_inode inode;
    size_t max_bytes = 0;
    size_t to_read;
    size_t written = 0;
    int i;

    if (g_fd < 0)
    {
        fprintf(stderr, "device not opened\n");
        return -1;
    }

    if (file_inode <= 0 || !buffer) 
    {
        return -1;
    }

    if (readinode((unsigned int)file_inode, &inode) != 0)
    {
        fprintf(stderr, "failed to read inode %d\n", file_inode);
        return -1;
    }

    for (i = 0; i < EXT2_NDIR_BLOCKS; ++i)
    {
        if (inode.i_block[i] != 0) 
        {
            max_bytes += g_block_size;
        }
    }

    if (max_bytes == 0) 
    {
        return 0;
    }

    to_read = (count < max_bytes) ? count : max_bytes;

    for (i = 0; i < EXT2_NDIR_BLOCKS && written < to_read; ++i)
    {
        uint32_t b = inode.i_block[i];
        off_t offset;
        size_t want;
        ssize_t r;

        if (b == 0) 
        {
            continue;
        }

        offset = (off_t)b * g_block_size;
        want = (to_read - written) < g_block_size ? (to_read - written) : g_block_size;

        r = preadfull(g_fd, buffer + written, want, offset);
        if (r < 0) 
        {
            return -1;
        }

        written += (size_t)r;

        if ((size_t)r != want) 
        {
            break;
        }
    }

    return (int)written;
}

static ssize_t preadfull(int fd, void *buf, size_t count, off_t offset)
{
    size_t done = 0;
    while (done < count)
    {
        ssize_t r = pread(fd, (char*)buf + done, count - done, offset + done);
        if (r < 0) return -1;
        if (r == 0) break;
        done += r;
    }
    return done;
}

static int readinode(unsigned int inode_no, struct ext2_inode *inode_out)
{
    off_t inode_table_block;
    off_t inode_table_offset;
    unsigned long index;
    off_t inode_offset;
    ssize_t r;

    if (!inode_out)
    {
        return -1;
    }

    inode_table_block = g_inode_table_block;
    inode_table_offset = inode_table_block * g_block_size;
    index = inode_no - 1;
    inode_offset = inode_table_offset + index * g_inode_size;

    r = preadfull(g_fd, inode_out, sizeof(struct ext2_inode), inode_offset);
    if (r != sizeof(struct ext2_inode))
    {
        return -1;
    }
    return 0;
}

static void printsuperandgd(void)
{
    printf("=== ext2 superblock ===\n");
    printf("inodes_count: %u\n", g_super.s_inodes_count);
    printf("blocks_count: %u\n", g_super.s_blocks_count);
    printf("free_blocks_count: %u\n", g_super.s_free_blocks_count);
    printf("free_inodes_count: %u\n", g_super.s_free_inodes_count);
    printf("log_block_size: %u -> block size: %u\n", g_super.s_log_block_size, g_block_size);
    printf("inode_size: %u\n", g_inode_size);
    printf("first_ino: %u\n", g_super.s_first_ino);

    printf("\n=== first group descriptor ===\n");
    printf("bg_block_bitmap: %u\n", g_gd.bg_block_bitmap);
    printf("bg_inode_bitmap: %u\n", g_gd.bg_inode_bitmap);
    printf("bg_inode_table: %u\n", g_gd.bg_inode_table);
    printf("bg_free_blocks_count: %u\n", g_gd.bg_free_blocks_count);
    printf("bg_free_inodes_count: %u\n", g_gd.bg_free_inodes_count);
    printf("bg_used_dirs_count: %u\n", g_gd.bg_used_dirs_count);
}

static int readblock(uint32_t block_no, void *buf)
{
    off_t offset = (off_t)block_no * g_block_size;
    ssize_t r = preadfull(g_fd, buf, g_block_size, offset);
    if (r != (ssize_t)g_block_size) 
    {
        return -1;
    }
    return 0;
}

static int nameequals(const char *name, const char *entry_name, unsigned short name_len)
{
    return (strlen(name) == (size_t)name_len) && (strncmp(name, entry_name, name_len) == 0);
}

static int findindir(unsigned int dir_inode_no, const char *name)
{
    struct ext2_inode dir_inode;
    void *block;
    int i;

    if (readinode(dir_inode_no, &dir_inode) != 0)
    { 
        return -1;
    }

    if (!S_ISDIR(dir_inode.i_mode))
    {
        return -1;
    } 

    block = malloc(g_block_size);
    if (!block)
    {
        return -1;
    } 

    for (i = 0; i < EXT2_NDIR_BLOCKS; ++i)
    {
        uint32_t b = dir_inode.i_block[i];
        unsigned int offset;
        if (b == 0)
        {
            continue;
        }

        if (readblock(b, block) != 0)
        {
            free(block);
            return -1;
        }

        offset = 0;
        while (offset < g_block_size)
        {
            struct ext2_dir_entry_2 *de = (struct ext2_dir_entry_2 *)((char*)block + offset);

            if (de->inode != 0)
            {
                if (nameequals(name, de->name, de->name_len))
                {
                    unsigned int found = de->inode;
                    free(block);
                    return found;
                }
            }

            if (de->rec_len == 0)
            {
                break;
            }
            offset += de->rec_len;
        }
    }
    free(block);
    return 0;
}

static int resolvepath(const char *path)
{
    char *copy;
    char *token;
    unsigned int cur_inode;

    if (!path || path[0] != '/') 
    {
        return -1;
    }

    copy = strdup(path);
    if (!copy) 
    {
        return -1;
    }

    cur_inode = EXT2_ROOT_INO;
    token = strtok(copy, "/");
    while (token)
    {
        int found = findindir(cur_inode, token);
        if (found <= 0)
        {
            free(copy);
            if (found == 0) 
            {
                return 0;
            }
            return -1;
        }
        cur_inode = (unsigned int)found;
        token = strtok(NULL, "/");
    }
    free(copy);
    return (int)cur_inode;
}

