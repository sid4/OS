#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


#if FS
#include <fs.h>

static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;

#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);
void reset_block_cache();
void reset_oft(){
int i=0;
for(;i< NUM_FD;i++){
oft[i].de=NULL;
}
//    memset(oft, 0, NUM_FD * sizeof(struct filetable));
}
int fs_open(char *filename, int flags){
	short file_found=0;
	int i,j,fd;

	for (i=0;i<fsd.root_dir.numentries;i++){
		if(strncmp(fsd.root_dir.entry[i].name,filename,FILENAMELEN)==0){
			file_found=1;
			break;
		}	
	}

	if(file_found){		
		//check if file is already open		
		for (j=0;j<NUM_FD;j++){
			//kprintf("hjk:%s",oft[j].de->name);
			if(oft[j].de!=NULL&&strncmp(oft[j].de->name,filename,FILENAMELEN)==0){
				kprintf("Returning existing file descriptor as file is already 			open");				

				return j;					
			}		
		}
		if(next_open_fd==-1){
			kprintf("The system has exceeded the max open file constraint");
			return SYSERR;
		}
		//assign next_open_fd
		fd=next_open_fd;			
		next_open_fd=-1;
		for (j=0;j<next_open_fd ;j++){
			if(oft[j].state==FSTATE_CLOSED){
				next_open_fd=j;					
			}		
		}
		oft[fd].flag=flags;
		oft[fd].state=FSTATE_OPEN;
		oft[fd].fileptr=0;
		oft[fd].de=&fsd.root_dir.entry[i];
		fs_get_inode_by_num(0,oft[fd].de->inode_num,&oft[fd].in);
		return fd;
				 	
	}
	else{
		kprintf("File not found");
		return SYSERR;
	}
}
int fs_close(int fd){
//write back the file inode to disk
fs_put_inode_by_num(0,oft[fd].in.id,&oft[fd].in);
oft[fd].state=FSTATE_CLOSED;
return OK;
}
int fs_create(char *filename, int mode){
	if(mode!=O_CREAT){
		kprintf("Invalid mode");
		return SYSERR;	
	}
	/*struct dirent *file=(struct dirent *)getmem(sizeof(struct dirent));
	memcpy(file->name,filename,strlen(filename));
	*/struct inode file_node={0};
	file_node.id=fsd.inodes_used++;
	file_node.type=INODE_TYPE_FILE;
	file_node.device=0;
	file_node.size=0;
	int free_mask_bit=fs_get_free_bit_index();
	if(free_mask_bit!=SYSERR){
		int i;		
		for(i=0;i<INODEBLOCKS;i++){
			file_node.blocks[i]=-1;
		}		
		file_node.blocks[0]=fs_get_free_bit_index();
		fsd.root_dir.entry[fsd.root_dir.numentries].inode_num=file_node.id;

		memcpy(&fsd.root_dir.entry[fsd.root_dir.numentries].name,filename,strlen(filename));

fsd.root_dir.numentries++;
		fs_setmaskbit(free_mask_bit);
		fs_put_inode_by_num(0, file_node.id, &file_node);
		bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
		return fs_open(filename,O_RDWR);
	}
	else{
		kprintf("Disk full");
		return SYSERR;
	}
}

int fs_seek(int fd, int offset){
	int fp=oft[fd].fileptr+offset;
	if(fp<0||fp>oft[fd].in.size){
		kprintf("Seek failed. Underflow or overflow condition");
		return SYSERR;
	}
	oft[fd].fileptr = fp;
	return OK;
}

int fs_read(int fd, void *buf, int nbytes){
	int index=oft[fd].fileptr/dev0_blocksize;
	int block= oft[fd].in.blocks[index];
	int offset=oft[fd].fileptr%dev0_blocksize;
	int read_c_actual=0;
	int read_c_from_block=0;
	int to_read_c=nbytes;
	if((oft[fd].flag==O_RDONLY||oft[fd].flag==O_RDWR) && oft[fd].state!=FSTATE_CLOSED ){
		while(read_c_actual<to_read_c){
			if(oft[fd].fileptr>=oft[fd].in.size){
				return read_c_actual;		
			}
			if(block == -1)
			{ 
				kprintf("\nThere is no more data to read"); 
				return SYSERR;
			}
			read_c_from_block = (dev0_blocksize - offset >  to_read_c)?to_read_c:dev0_blocksize - offset;
			reset_block_cache();	
			if(bs_bread(0,block, 0, &block_cache, 512) == SYSERR){
				kprintf("\nFailed to cache the block");
				oft[fd].fileptr += read_c_actual;
				return read_c_actual;
			}
			else{
				memcpy(buf+read_c_actual, (block_cache+offset), read_c_from_block);
				to_read_c -= read_c_actual;
				oft[fd].fileptr += read_c_actual;
				read_c_actual += read_c_from_block;
				index++;
				block = oft[fd].in.blocks[index];
				offset = 0;
			}
		}
		reset_block_cache();	
		return read_c_actual;
	}
	else{
		kprintf("File not open with r or rw flag or file already closed");
		return SYSERR;
	}
}
int fs_write(int fd, void *buf, int nbytes){
	int block,index, offset,to_write_c,write_c_to_block=0, write_c_actual=0;
	if((oft[fd].flag==O_WRONLY||oft[fd].flag==O_RDWR) && oft[fd].state!=FSTATE_CLOSED ){
		struct inode in = oft[fd].in;
/*
//check for errors here
	in.size = in.size - (in.size - oft[fd].fileptr);//if the fileptr was readjusted using fseek() the size is reduced to calculate the correct file size at the end
*/
		index = oft[fd].fileptr / dev0_blocksize;
        	block= oft[fd].in.blocks[index];
		if(block== -1){
			block = fs_get_free_bit_index();
			oft[fd].in.blocks[index] = block;
		}
		offset = oft[fd].fileptr % dev0_blocksize;
		to_write_c = nbytes;
		while (to_write_c > 0){
			fs_setmaskbit(block);
			if(dev0_blocksize-offset > to_write_c){
				write_c_to_block = to_write_c;
			}
			else{
				write_c_to_block = dev0_blocksize - offset;
			}
//TODO write to cache block
			if(bs_bwrite(dev0,block,offset,&buf[write_c_actual],write_c_to_block ) == SYSERR){
				kprintf("\nERROR!!In file writing");
				oft[fd].in.size += write_c_actual;
				return write_c_actual;
			}
			to_write_c-= write_c_to_block;
			write_c_actual+= write_c_to_block;
			index++;
			oft[fd].in.blocks[index] = fs_get_free_bit_index();
			block =  oft[fd].in.blocks[index]; 
			offset = 0;
		}
		oft[fd].in.size += write_c_actual; 
		oft[fd].fileptr = oft[fd].in.size;
		return write_c_actual;		
	}
	else{
		kprintf("File not open with r or rw flag or file already closed");
		return SYSERR;
	}


}

int fs_init(int dev){
	if(dev==0){
	fsd.root_dir.numentries=0;
	/*fsd.root_dir.entry="sid_fs";
	struct inode root={0};
	root.id=fsd.inodes_used++;
	root.type=INODE_TYPE_DIR;
	root.device=dev;
	*/
	return OK;
	}

	return SYSERR;
}
/* YOUR CODE GOES HERE */

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    printf("No indirect block support\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}

/* read in an inode and fill in the pointer */
int fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  printf("inn*sizeof(struct inode): %d\n", inode_off);
  */

  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */

  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}
     
int fs_mkfs(int dev, int num_inodes) {
  int i;
  
  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }

  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8; 
  
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    printf("fs_mkfs memget failed.\n");
    return SYSERR;
  }
  
  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }
  
  fsd.inodes_used = 0;
  
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  return 1;
}

void fs_print_fsd(void) {

  printf("fsd.ninodes: %d\n", fsd.ninodes);
  printf("sizeof(struct inode): %d\n", sizeof(struct inode));
  printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}

/**
Finds the index of the available free block
**/
int fs_get_free_bit_index(){
	int i;
	int retval=-1;
	for(i=0;i<fsd.freemaskbytes;i++){
			if(fs_getmaskbit(i)!=1){
				retval=i;
				break;			
			}
	}
	if(retval==-1){
		return SYSERR;	
	}
	else return retval;
}

void reset_block_cache(){
int i;
for(i=0;i<512;i++)
{
    block_cache[i]='0';
}
}

#endif /* FS */

