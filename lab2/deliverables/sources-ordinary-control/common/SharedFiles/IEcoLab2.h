/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoLab2
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoLab2
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

#ifndef __I_ECOLAB2_H__
#define __I_ECOLAB2_H__

#include "IEcoBase1.h"

/* IEcoLab2 IID = {108A07E7-EFDA-4362-AED3-72013EB89C4E} */
#ifndef __IID_IEcoLab2
static const UGUID IID_IEcoLab2 = {0x01, 0x10, {0x10, 0x8A, 0x07, 0xE7, 0xEF, 0xDA, 0x43, 0x62, 0xAE, 0xD3, 0x72, 0x01, 0x3E, 0xB8, 0x9C, 0x4E}};
#endif /* __IID_IEcoLab2 */

typedef struct IEcoLab2* IEcoLab2Ptr_t;

typedef struct IEcoLab2VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoLab2Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoLab2Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoLab2Ptr_t me);

    /* IEcoLab2 */
    uint32_t (ECOCALLMETHOD *GetVersion)(IEcoLab2Ptr_t me);

} IEcoLab2VTbl, *IEcoLab2VTblPtr_t;

interface IEcoLab2 {
    struct IEcoLab2VTbl *pVTbl;
} IEcoLab2;


#endif /* __I_ECOLAB2_H__ */

