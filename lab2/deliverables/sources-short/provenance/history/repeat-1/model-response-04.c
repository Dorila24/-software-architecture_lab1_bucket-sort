#include "ReuseContract.h"
#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "IdEcoMemoryManager1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"
#include "IEcoLab65.h"
#include "ArpoMethods.h"

typedef struct CEcoLab65 {
    IEcoLab65VTbl* m_pVTblILab65;
    IEcoCalculatorXVTbl* m_pVTblIX;
    IEcoCalculatorYVTbl* m_pVTblIY;
    IEcoUnknownVTbl* m_pVTblINondelegatingUnk;
    uint32_t m_cRef;
    IEcoMemoryAllocator1* m_pIMem;
    IEcoSystem1* m_pISys;
    IEcoUnknown* m_pIUnkOuter;
    IEcoCalculatorX* m_pIX;
    IEcoCalculatorY* m_pIY;
} CEcoLab65, *CEcoLab65Ptr;

typedef struct CEcoLab2 {
    IEcoLab2VTbl* m_pVTblILab2;
    IEcoUnknownVTbl* m_pVTblINondelegatingUnk;
    uint32_t m_cRef;
    IEcoLab65* m_pILab65;
} CEcoLab2, *CEcoLab2Ptr;

// Forward declarations
static int16_t ECOCALLMETHOD CEcoLab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD CEcoLab65_AddRef(IEcoLab65Ptr_t me);
static uint32_t ECOCALLMETHOD CEcoLab65_Release(IEcoLab65Ptr_t me);
static int16_t ECOCALLMETHOD CEcoLab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD CEcoLab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD CEcoLab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD CEcoLab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD CEcoLab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets);
static int16_t ECOCALLMETHOD CEcoLab65_NondelegatingQueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD CEcoLab65_NondelegatingAddRef(IEcoUnknownPtr_t me);
static uint32_t ECOCALLMETHOD CEcoLab65_NondelegatingRelease(IEcoUnknownPtr_t me);

static int16_t ECOCALLMETHOD CEcoLab2_QueryInterface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD CEcoLab2_AddRef(IEcoLab2Ptr_t me);
static uint32_t ECOCALLMETHOD CEcoLab2_Release(IEcoLab2Ptr_t me);
static uint32_t ECOCALLMETHOD CEcoLab2_GetVersion(IEcoLab2Ptr_t me);
static int16_t ECOCALLMETHOD CEcoLab2_NondelegatingQueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t ECOCALLMETHOD CEcoLab2_NondelegatingAddRef(IEcoUnknownPtr_t me);
static uint32_t ECOCALLMETHOD CEcoLab2_NondelegatingRelease(IEcoUnknownPtr_t me);

static void ECOCALLMETHOD deleteCEcoLab65(IEcoLab65Ptr_t pILab65);
static void ECOCALLMETHOD deleteCEcoLab2(IEcoLab2Ptr_t pILab2);

IEcoLab65VTbl g_xD565835CB1F840BC802347E6EBAB0281VTbl = {
    CEcoLab65_QueryInterface,
    CEcoLab65_AddRef,
    CEcoLab65_Release,
    CEcoLab65_SortInt,
    CEcoLab65_SortLong,
    CEcoLab65_SortFloat,
    CEcoLab65_SortDouble,
    CEcoLab65_SortLongDouble
};

IEcoCalculatorXVTbl g_x9322111622484742AE0682819447843DVTbl = {
    CEcoLab65_QueryInterface,
    CEcoLab65_AddRef,
    CEcoLab65_Release,
    NULL, // Addition
    NULL  // Subtraction
};

IEcoCalculatorYVTbl g_xBD6414C29096423EA90C04D77AFD1CADVTbl = {
    CEcoLab65_QueryInterface,
    CEcoLab65_AddRef,
    CEcoLab65_Release,
    NULL, // Multiplication
    NULL  // Division
};

IEcoUnknownVTbl g_x000000000000000000000000000000AAVTbl = {
    CEcoLab65_NondelegatingQueryInterface,
    CEcoLab65_NondelegatingAddRef,
    CEcoLab65_NondelegatingRelease
};

IEcoLab2VTbl g_x108A07E7EFDA4362AED372013EB89C4EVTbl = {
    CEcoLab2_QueryInterface,
    CEcoLab2_AddRef,
    CEcoLab2_Release,
    CEcoLab2_GetVersion
};

IEcoUnknownVTbl g_x000000000000000000000000000000BBVTbl = {
    CEcoLab2_NondelegatingQueryInterface,
    CEcoLab2_NondelegatingAddRef,
    CEcoLab2_NondelegatingRelease
};

IEcoMemoryAllocator1* ECOCALLMETHOD lab_allocator(IEcoLab65* me) {
    if (me == 0) {
        return 0;
    }
    CEcoLab65* pCMe = (CEcoLab65*)me;
    return pCMe->m_pIMem;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* pIUnkSystem, IEcoUnknown* pIUnkOuter, const UGUID* iid, void** out) {
    CEcoLab65* pCMe = 0;
    int16_t result = ERR_ECO_POINTER;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    UGUID rcid = CID_EcoMemoryManager1;
    if (out == 0 || pIUnkSystem == 0 || iid == 0) {
        return result;
    }
    *out = 0;
    if (pIUnkOuter != 0 && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        return ERR_ECO_NOAGGREGATION;
    }
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (voidptr_t*)&pISys);
    if (result != 0 || pISys == 0) {
        return ERR_ECO_NOSYSTEM;
    }
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (voidptr_t*)&pIBus);
    if (result != 0 || pIBus == 0) {
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_NOBUS;
    }
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        const UGUID* extManager = pIMemExt->pVTbl->get_Manager(pIMemExt);
        if (extManager != 0) {
            rcid = *extManager;
        }
        pIMemExt->pVTbl->Release(pIMemExt);
    }
    result = pIBus->pVTbl->QueryComponent(pIBus, &rcid, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&pIMem);
    if (result != 0 || pIMem == 0) {
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_GET_MEMORY_ALLOCATOR;
    }
    pCMe = (CEcoLab65*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab65));
    if (pCMe == 0) {
        pIMem->pVTbl->Release(pIMem);
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_OUTOFMEMORY;
    }
    pCMe->m_pISys = pISys;
    pCMe->m_pIMem = pIMem;
    pCMe->m_cRef = 1;
    pCMe->m_pVTblILab65 = &g_xD565835CB1F840BC802347E6EBAB0281VTbl;
    pCMe->m_pVTblIX = &g_x9322111622484742AE0682819447843DVTbl;
    pCMe->m_pVTblIY = &g_xBD6414C29096423EA90C04D77AFD1CADVTbl;
    pCMe->m_pVTblINondelegatingUnk = &g_x000000000000000000000000000000AAVTbl;
    pCMe->m_pIUnkOuter = 0;
    if (pIUnkOuter != 0) {
        pCMe->m_pIUnkOuter = pIUnkOuter;
    } else {
        pCMe->m_pIUnkOuter = (IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk;
    }
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (voidptr_t*)&pCMe->m_pIX);
    if (result != 0 || pCMe->m_pIX == 0) {
        deleteCEcoLab65((IEcoLab65*)pCMe);
        pIBus->pVTbl->Release(pIBus);
        return ERR_ECO_GET_COMPONENT;
    }
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (voidptr_t*)&pCMe->m_pIY);
    if (result != 0 || pCMe->m_pIY == 0) {
        deleteCEcoLab65((IEcoLab65*)pCMe);
        pIBus->pVTbl->Release(pIBus);
        return ERR_ECO_GET_COMPONENT;
    }
    if (IsEqualUGUID(iid, &IID_IEcoLab65)) {
        *out = &pCMe->m_pVTblILab65;
    } else if (IsEqualUGUID(iid, &IID_IEcoCalculatorX)) {
        *out = &pCMe->m_pVTblIX;
    } else if (IsEqualUGUID(iid, &IID_IEcoCalculatorY)) {
        *out = &pCMe->m_pVTblIY;
    } else if (IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        *out = &pCMe->m_pVTblINondelegatingUnk;
    } else {
        deleteCEcoLab65((IEcoLab65*)pCMe);
        return ERR_ECO_NOINTERFACE;
    }
    pIBus->pVTbl->Release(pIBus);
    return ERR_ECO_SUCCESES;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* pIUnkSystem, IEcoUnknown* pIUnkOuter, const UGUID* iid, void** out) {
    CEcoLab2* pCMe = 0;
    int16_t result = ERR_ECO_POINTER;
    IEcoLab65* pILab65 = 0;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    UGUID rcid = CID_EcoMemoryManager1;
    if (out == 0 || pIUnkSystem == 0 || iid == 0) {
        return result;
    }
    *out = 0;
    if (pIUnkOuter != 0) {
        return ERR_ECO_NOAGGREGATION;
    }
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (voidptr_t*)&pISys);
    if (result != 0 || pISys == 0) {
        return ERR_ECO_NOSYSTEM;
    }
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (voidptr_t*)&pIBus);
    if (result != 0 || pIBus == 0) {
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_NOBUS;
    }
    result = pIBus->pVTbl->QueryComponent(pIBus, &rcid, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&pIMem);
    if (result != 0 || pIMem == 0) {
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_GET_MEMORY_ALLOCATOR;
    }
    pCMe = (CEcoLab2*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab2));
    if (pCMe == 0) {
        pIMem->pVTbl->Release(pIMem);
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_OUTOFMEMORY;
    }
    pCMe->m_pISys = pISys;
    pCMe->m_pIMem = pIMem;
    pCMe->m_cRef = 1;
    pCMe->m_pVTblILab2 = &g_x108A07E7EFDA4362AED372013EB89C4EVTbl;
    pCMe->m_pVTblINondelegatingUnk = &g_x000000000000000000000000000000BBVTbl;
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab65, 0, &IID_IEcoLab65, (voidptr_t*)&pILab65);
    if (result != 0 || pILab65 == 0) {
        deleteCEcoLab2((IEcoLab2*)pCMe);
        pIBus->pVTbl->Release(pIBus);
        return ERR_ECO_GET_COMPONENT;
    }
    pCMe->m_pILab65 = pILab65;
    if (IsEqualUGUID(iid, &IID_IEcoLab2)) {
        *out = &pCMe->m_pVTblILab2;
    } else if (IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        *out = &pCMe->m_pVTblINondelegatingUnk;
    } else {
        deleteCEcoLab2((IEcoLab2*)pCMe);
        return ERR_ECO_NOINTERFACE;
    }
    pIBus->pVTbl->Release(pIBus);
    return ERR_ECO_SUCCESES;
}

static int16_t ECOCALLMETHOD CEcoLab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    CEcoLab65* pCMe = (CEcoLab65*)me;
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    *ppv = 0;
    if (pCMe->m_pIUnkOuter != 0) {
        return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
    }
    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD CEcoLab65_AddRef(IEcoLab65Ptr_t me) {
    CEcoLab65* pCMe = (CEcoLab65*)me;
    if (me == 0) {
        return -1;
    }
    if (pCMe->m_pIUnkOuter != 0) {
        return pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
    }
    return -1;
}

static uint32_t ECOCALLMETHOD CEcoLab65_Release(IEcoLab65Ptr_t me) {
    CEcoLab65* pCMe = (CEcoLab65*)me;
    if (me == 0) {
        return -1;
    }
    if (pCMe->m_pIUnkOuter != 0) {
        return pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter);
    }
    return -1;
}

static int16_t ECOCALLMETHOD CEcoLab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_int(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD CEcoLab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD CEcoLab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    return lab_sort_float(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD CEcoLab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_double(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD CEcoLab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long_double(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD CEcoLab65_NondelegatingQueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    CEcoLab65* pCMe = (CEcoLab65*)((char*)me - offsetof(CEcoLab65, m_pVTblINondelegatingUnk));
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    *ppv = 0;
    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &pCMe->m_pVTblILab65;
        ((IEcoUnknown*)(*ppv))->pVTbl->AddRef((IEcoUnknown*)(*ppv));
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->m_pVTblIX;
        ((IEcoUnknown*)(*ppv))->pVTbl->AddRef((IEcoUnknown*)(*ppv));
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->m_pVTblIY;
        ((IEcoUnknown*)(*ppv))->pVTbl->AddRef((IEcoUnknown*)(*ppv));
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblINondelegatingUnk;
        ++pCMe->m_cRef;
    } else {
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

static uint32_t ECOCALLMETHOD CEcoLab65_NondelegatingAddRef(IEcoUnknownPtr_t me) {
    CEcoLab65* pCMe = (CEcoLab65*)((char*)me - offsetof(CEcoLab65, m_pVTblINondelegatingUnk));
    if (me == 0) {
        return -1;
    }
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoLab65_NondelegatingRelease(IEcoUnknownPtr_t me) {
    CEcoLab65* pCMe = (CEcoLab65*)((char*)me - offsetof(CEcoLab65, m_pVTblINondelegatingUnk));
    if (me == 0) {
        return -1;
    }
    --pCMe->m_cRef;
    if (pCMe->m_cRef == 0) {
        deleteCEcoLab65((IEcoLab65*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

static void ECOCALLMETHOD deleteCEcoLab65(IEcoLab65Ptr_t pILab65) {
    CEcoLab65* pCMe = (CEcoLab65*)pILab65;
    IEcoMemoryAllocator1* pIMem = 0;
    if (pILab65 != 0) {
        pIMem = pCMe->m_pIMem;
        if (pCMe->m_pIX != 0) {
            pCMe->m_pIX->pVTbl->Release(pCMe->m_pIX);
        }
        if (pCMe->m_pIY != 0) {
            pCMe->m_pIY->pVTbl->Release(pCMe->m_pIY);
        }
        if (pCMe->m_pISys != 0) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

static int16_t ECOCALLMETHOD CEcoLab2_QueryInterface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    CEcoLab2* pCMe = (CEcoLab2*)me;
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    *ppv = 0;
    if (pCMe->m_pILab65 != 0) {
        return pCMe->m_pILab65->pVTbl->QueryInterface(pCMe->m_pILab65, riid, ppv);
    }
    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD CEcoLab2_AddRef(IEcoLab2Ptr_t me) {
    CEcoLab2* pCMe = (CEcoLab2*)me;
    if (me == 0) {
        return -1;
    }
    if (pCMe->m_pILab65 != 0) {
        return pCMe->m_pILab65->pVTbl->AddRef(pCMe->m_pILab65);
    }
    return -1;
}

static uint32_t ECOCALLMETHOD CEcoLab2_Release(IEcoLab2Ptr_t me) {
    CEcoLab2* pCMe = (CEcoLab2*)me;
    if (me == 0) {
        return -1;
    }
    if (pCMe->m_pILab65 != 0) {
        return pCMe->m_pILab65->pVTbl->Release(pCMe->m_pILab65);
    }
    return -1;
}

static uint32_t ECOCALLMETHOD CEcoLab2_GetVersion(IEcoLab2Ptr_t me) {
    return 2;
}

static int16_t ECOCALLMETHOD CEcoLab2_NondelegatingQueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    CEcoLab2* pCMe = (CEcoLab2*)((char*)me - offsetof(CEcoLab2, m_pVTblINondelegatingUnk));
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    *ppv = 0;
    if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = &pCMe->m_pVTblILab2;
        ((IEcoUnknown*)(*ppv))->pVTbl->AddRef((IEcoUnknown*)(*ppv));
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblINondelegatingUnk;
        ++pCMe->m_cRef;
    } else {
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

static uint32_t ECOCALLMETHOD CEcoLab2_NondelegatingAddRef(IEcoUnknownPtr_t me) {
    CEcoLab2* pCMe = (CEcoLab2*)((char*)me - offsetof(CEcoLab2, m_pVTblINondelegatingUnk));
    if (me == 0) {
        return -1;
    }
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoLab2_NondelegatingRelease(IEcoUnknownPtr_t me) {
    CEcoLab2* pCMe = (CEcoLab2*)((char*)me - offsetof(CEcoLab2, m_pVTblINondelegatingUnk));
    if (me == 0) {
        return -1;
    }
    --pCMe->m_cRef;
    if (pCMe->m_cRef == 0) {
        deleteCEcoLab2((IEcoLab2*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

static void ECOCALLMETHOD deleteCEcoLab2(IEcoLab2Ptr_t pILab2) {
    CEcoLab2* pCMe = (CEcoLab2*)pILab2;
    IEcoMemoryAllocator1* pIMem = 0;
    if (pILab2 != 0) {
        pIMem = pCMe->m_pIMem;
        if (pCMe->m_pILab65 != 0) {
            pCMe->m_pILab65->pVTbl->Release(pCMe->m_pILab65);
        }
        if (pCMe->m_pISys != 0) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
