﻿#ifndef VMM_H
#define VMM_H

#ifndef DEBUG
#define DEBUG
#endif


/* 模拟辅存的文件路径 */
#define AUXILIARY_MEMORY "vmm_auxMem"

/* 页面大小（字节）*/
#define PAGE_SIZE 4
/* 虚存空间大小（字节） */
#define VIRTUAL_MEMORY_SIZE (64 * 4)
/* 实存空间大小（字节） */ 
#define ACTUAL_MEMORY_SIZE (32 * 4)
/* 总虚页数 */
#define PAGE_SUM (VIRTUAL_MEMORY_SIZE / PAGE_SIZE)
/* 一级页表项数 */
#define OUTER_PAGE_SUM 4
/* 二级页表项数 */
#define INNER_PAGE_SUM 16

/* 总物理块数 */
#define BLOCK_SUM (ACTUAL_MEMORY_SIZE / PAGE_SIZE)

/*一级页表项映射的内存空间大小*/
#define OUTER_PAGE_SIZE (PAGE_SIZE * INNER_PAGE_SUM)



#define PROCESS_SUM 2

/* 可读标识位 */
#define READABLE 0x01u
/* 可写标识位 */
#define WRITABLE 0x02u
/* 可执行标识位 */
#define EXECUTABLE 0x04u

/*页面老化算法的周期*/
#define CYCLE 1

/* 定义字节类型 */
#define BYTE unsigned char

typedef enum {
	TRUE = 1, FALSE = 0
} BOOL;



/* 页表项 */
typedef struct
{
	unsigned int pageIndex;//页目录号
	unsigned int pageNum;//页号
	unsigned int blockNum; //物理块号
	unsigned int proccessNum;//该页所属的进程号
	BOOL filled; //页面装入特征位
	BYTE proType; //页面保护类型
	BOOL edited; //页面修改标识
	unsigned long auxAddr; //外存地址
	unsigned long count; //页面使用计数器
	unsigned char shiftReg;//移位寄存器，用于页面老化算法
	BOOL r;//页面访问位，表示最近一段时间该页面有无被访问
} PageTableItem, *Ptr_PageTableItem;

/*
//二级页表
typedef struct{
	unsigned int pageNum;//一级页表页号
	unsigned int pageIndex;//存储二级页表项的首地址
}OuterPageTableItem,*Ptr_OuterPageTableItem;
*/

/* 访存请求类型 */
typedef enum { 
	REQUEST_READ, 
	REQUEST_WRITE, 
	REQUEST_EXECUTE 
} MemoryAccessRequestType;

/* 访存请求 */
typedef struct
{
	MemoryAccessRequestType reqType; //访存请求类型
	unsigned int proccessNum;//访问的进程号
	unsigned long virAddr; //虚地址
	BYTE value; //写请求的值
} MemoryAccessRequest, *Ptr_MemoryAccessRequest;

#define DATALEN sizeof(MemoryAccessRequest)


/* 访存错误代码 */
typedef enum {
	ERROE_PROCESS_DENY,//该页不属于该进程
	ERROR_READ_DENY, //该页不可读
	ERROR_WRITE_DENY, //该页不可写
	ERROR_EXECUTE_DENY, //该页不可执行
	ERROR_INVALID_REQUEST, //非法请求类型
	ERROR_OVER_BOUNDARY, //地址越界
	ERROR_FILE_OPEN_FAILED, //文件打开失败
	ERROR_FILE_CLOSE_FAILED, //文件关闭失败
	ERROR_FILE_SEEK_FAILED, //文件指针定位失败
	ERROR_FILE_READ_FAILED, //文件读取失败
	ERROR_FILE_WRITE_FAILED //文件写入失败
	
} ERROR_CODE;

/* 产生访存请求 */
void do_request();

/* 响应访存请求 */
void do_response();

/* 处理缺页中断 */
void do_page_fault(Ptr_PageTableItem);

/* LFU页面替换 */
void do_LFU(Ptr_PageTableItem);
/* 页面老化页面替换 */
void do_LRU(Ptr_PageTableItem);
/* 装入页面 */
void do_page_in(Ptr_PageTableItem, unsigned in);

/* 写出页面 */
void do_page_out(Ptr_PageTableItem);

/* 错误处理 */
void do_error(ERROR_CODE);

/* 打印页表相关信息 */
void do_print_info();
/* 打印实存 */
void do_print_real();
/* 打印辅存 */
void do_print_virtual();


/* 获取页面保护类型字符串 */
char *get_proType_str(char *, BYTE);

void init_file();

void init_fifo();

void do_update();

#endif
