#ifndef _VOICE_ENERGY_H_
#define _VOICE_ENERGY_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//int FixFrontSimple_table_ln(unsigned int w32Param, char cParamQ);

/* ��ȡ��Ƶ���� */
// ����nMaxLevel:Ϊ����ֵ
int iFlyFixFrontCalcVolumLevel(void* pData, int nBytes, int* pVolumLevel, int nMaxLevel);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

