/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoLab65_F4120672
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoLab65_F4120672 component
 * </description>
 *
 * <author>
 *   Copyright (c) 2018 . All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOLAB65_H__
#define __C_ECOLAB65_H__

#include "IEcoLab65.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoLab65_F4120672* CEcoLab65_F4120672Ptr_t;

typedef struct CEcoLab65_F4120672 {

    /* IEcoLab65 interface function table */
    IEcoLab65VTbl* m_pVTblIEcoLab65;


    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoLab65_F4120672Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoLab65_F4120672Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoLab65_F4120672Ptr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    char_t* m_Name;

} CEcoLab65_F4120672;

#endif /* __C_ECOLAB65_H__ */
