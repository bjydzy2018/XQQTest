/** 
 * @file    nl_mem.h
 * @brief   iFLY Natural Language Mem Pool
 * 
 * 1. С��MEMPOOL_SIZE �Ĵ��ڴ�ط���
 * 2. ����MEMPOOL_SIZE �Ĵ�ϵͳ����
 * 3. �ڴ���ڴ治��ʱ���ᰴMEMPOOL_SIZE�Ĵ�С��չһ��
 * 4. Ĭ��������ڴ��Ǻ�long�Ĵ�С�������
 * 
 * @author  jjzhuang.
 * @version 1.0
 * @date    2016/06/17
 * 
 * @see
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version    <th>Date        <th>Author    <th>Notes</tr>
 *  <tr> <td>1.0        <td>2016/06/17  <td>jjzhuang <td>Create this file</tr>
 *  <tr> <td>1.0        <td>2016/06/17  <td>jjzhuang <td>��������ڴ�������</tr>
 * </table>
 * 
 */


#ifndef  _NL_MEM_20160617_
#define  _NL_MEM_20160617_


/* �ڴ�ؿ�ʼ���ڴ��С�ͺ���ÿ���������ڴ��С */
#define  MEMPOOL_SIZE_4K    (4095)
#define  MEMPOOL_SIZE_16K   (16*(1<<10)-1)
#define  MEMPOOL_SIZE_20K   (20*(1<<10)-1)
#define  MEMPOOL_SIZE_32K   (32*(1<<10)-1)
#define  MEMPOOL_SIZE_64K   (64*(1<<10)-1)
#define  MEMPOOL_SIZE_80K   (80*(1<<10)-1)
#define  MEMPOOL_SIZE_100K  (100*(1<<10)-1)
#define  MEMPOOL_SIZE_128K  (128*(1<<10)-1)
#define  MEMPOOL_SIZE_256K  (256*(1<<10)-1)
#define  MEMPOOL_SIZE_512K  (512*(1<<10)-1)


#ifdef __cplusplus
extern "C" {
#endif /* C++ */

    typedef void*   MPoolHandle;

    typedef struct _SMemStatus
    {
        int nUsedSize;  /* �ڴ����ʹ�ô�С */
        int nAllSize;   /* �ڴ��ռ�ô�С */
    }SMemStatus,*PSMemStatus;


    /* �����ڴ�� */
    MPoolHandle  MPoolCreate(int size);
    
    /* �����ڴ�أ������ͷ���������ڴ� */
    void MPoolReset(MPoolHandle pool);

    /* �����ڴ�أ�ͬʱ�ͷ�������ϵͳ������ڴ� */
    void  MPoolDestroy(MPoolHandle pool);

    /* �����ڴ� */
    void* MPoolPalloc(MPoolHandle pool,int size);

    /* �����ڴ��С */
    void* MPoolPrealloc(MPoolHandle pool,void*p,int size);

    /* �ͷ��ڴ� */
    int MPoolPfree(MPoolHandle pool,void* p);

    /* ��ȡ�ڴ�״̬ */
    void MPoolStatus(MPoolHandle pool,PSMemStatus pStatus);


#ifdef __cplusplus
}
#endif /* C++ */



#endif 
