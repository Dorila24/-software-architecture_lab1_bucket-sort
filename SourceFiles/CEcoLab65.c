#include "ArpoMethods.h"
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
 *   This source code describes the implementation of the interfaces for CEcoLab65_F4120672
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */


#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab65.h"
/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoLab65 interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab65_F4120672_QueryInterface(/* in */ IEcoLab65Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab65_F4120672* pCMe = (CEcoLab65_F4120672*)me;

    /* Pointer Validation */
    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Validate and retrieve requested interface */
    if ( IsEqualUGUID(riid, &IID_IEcoLab65) ) {
        *ppv = &pCMe->m_pVTblIEcoLab65;
        pCMe->m_pVTblIEcoLab65->AddRef((IEcoLab65*)pCMe);
    }
	
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab65;
        pCMe->m_pVTblIEcoLab65->AddRef((IEcoLab65*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   AddRef Function
 * </summary>
 *
 * <description>
 *   AddRef function for the IEcoLab65 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab65_F4120672_AddRef(/* in */ IEcoLab65Ptr_t me) {
    CEcoLab65_F4120672* pCMe = (CEcoLab65_F4120672*)me;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Release Function
 * </summary>
 *
 * <description>
 *   Release function for the IEcoLab65 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab65_F4120672_Release(/* in */ IEcoLab65Ptr_t me) {
    CEcoLab65_F4120672* pCMe = (CEcoLab65_F4120672*)me;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Decrementing the component's reference count */
    --pCMe->m_cRef;
    /* If the count is zero, free the instance data */
    if ( pCMe->m_cRef == 0 ) {
        pCMe->Delete(pCMe);
		
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   MyFunction Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */





/*
 *
 * <summary>
 *   Init Function
 * </summary>
 *
 * <description>
 *   Instance initialization function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD initCEcoLab65_F4120672(/*in*/ CEcoLab65_F4120672Ptr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoLab65_F4120672* pCMe = (CEcoLab65_F4120672*)me;
    IEcoInterfaceBus1* pIBus = 0;

    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    int16_t result = ERR_ECO_POINTER;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;	

    /* Pointer Validation */
    if (me == 0 ) {
        return result;
    }

    /* Storing the pointer to the system interface */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Getting the interface for working with the interface bus */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Getting the component ID for working with memory */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Getting the memory allocator interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);
    /* Check */
    if (result != 0 || pCMe->m_pIMem == 0) {
        result = ERR_ECO_GET_MEMORY_ALLOCATOR;
    }



    /* Freeing */
    pIBus->pVTbl->Release(pIBus);

    return result;
}

/*
 *
 * <summary>
 *   Create Function
 * </summary>
 *
 * <description>
 *   Instance creation function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD createCEcoLab65_F4120672(/* in */ CEcoLab65_F4120672Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
    int16_t result = ERR_ECO_POINTER;

    /* Pointer Validation */
    if (pCMe == 0) {
        return result; /* ERR_ECO_POINTER */
    }



    return ERR_ECO_SUCCESS;
}

/*
 *
 * <summary>
 *   Delete Function
 * </summary>
 *
 * <description>
 *   Instance freeing function
 * </description>
 *
 */
static void ECOCALLMETHOD deleteCEcoLab65_F4120672(/* in */ CEcoLab65_F4120672Ptr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

/* IEcoLab65 Virtual Table */
IEcoLab65VTbl g_xD565835CB1F840BC802347E6EBAB0281VTbl_F4120672 = {
    CEcoLab65_F4120672_QueryInterface,
    CEcoLab65_F4120672_AddRef,
    CEcoLab65_F4120672_Release,
    lab_sort_int,
    lab_sort_long,
    lab_sort_float,
    lab_sort_double,
    lab_sort_long_double
};



/* Object Instance */
CEcoLab65_F4120672 g_xCEcoLab65_F4120672 = {
    &g_xD565835CB1F840BC802347E6EBAB0281VTbl_F4120672,
   
    initCEcoLab65_F4120672,
    createCEcoLab65_F4120672,
    deleteCEcoLab65_F4120672,
    1, /* m_cRef */
    0, /* m_pISys */
    0, /* m_pISys */
    0  /* m_Name */
};

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    if (me == 0) return 0;
    return ((CEcoLab65_F4120672*)me)->m_pIMem;
}
