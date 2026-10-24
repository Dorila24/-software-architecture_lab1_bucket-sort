/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoLab65
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoLab65
 * </description>
 *
 * <reference>
 *
 * </reference>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __I_ECOLAB65_H__
#define __I_ECOLAB65_H__

#include "IEcoBase1.h"

/* IEcoLab65 IID = {D565835C-B1F8-40BC-8023-47E6EBAB0281} */
#ifndef __IID_IEcoLab65
static const UGUID IID_IEcoLab65 = {0x01, 0x10, {0xD5, 0x65, 0x83, 0x5C, 0xB1, 0xF8, 0x40, 0xBC, 0x80, 0x23, 0x47, 0xE6, 0xEB, 0xAB, 0x02, 0x81}};
#endif /* __IID_IEcoLab65 */

typedef struct IEcoLab65* IEcoLab65Ptr_t;

typedef struct IEcoLab65VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoLab65Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoLab65Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoLab65Ptr_t me);

    /* IEcoLab65 */
    int16_t (ECOCALLMETHOD *SortInt)(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets);
    int16_t (ECOCALLMETHOD *SortLong)(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets);
    int16_t (ECOCALLMETHOD *SortFloat)(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets);
    int16_t (ECOCALLMETHOD *SortDouble)(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets);
    int16_t (ECOCALLMETHOD *SortLongDouble)(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets);

} IEcoLab65VTbl, *IEcoLab65VTblPtr_t;

interface IEcoLab65 {
    struct IEcoLab65VTbl *pVTbl;
} IEcoLab65;


#endif /* __I_ECOLAB65_H__ */

