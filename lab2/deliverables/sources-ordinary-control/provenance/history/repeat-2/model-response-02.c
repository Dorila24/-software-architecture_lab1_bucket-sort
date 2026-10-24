#include "ReuseContract.h"

typedef struct Lab65State {
    IEcoLab65 public_lab65;
    IEcoUnknown private_unknown;
    IEcoCalculatorX public_x;
    IEcoCalculatorY public_y;
    uint32_t local_refs;
    IEcoUnknown* controller;
    IEcoSystem1* owned_system;
    IEcoMemoryAllocator1* owned_allocator;
    IEcoCalculatorX* inner_x;
    IEcoCalculatorY* inner_y;
} Lab65State;

/* Private unknown methods */
static int16_t ECOCALLMETHOD Lab65Private_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    int16_t result = ERR_ECO_NOINTERFACE;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->private_unknown;
        pCMe->local_refs++;
        result = ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &pCMe->public_lab65;
        pCMe->public_lab65.pVTbl->AddRef(&pCMe->public_lab65);
        result = ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->public_x;
        pCMe->public_x.pVTbl->AddRef(&pCMe->public_x);
        result = ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->public_y;
        pCMe->public_y.pVTbl->AddRef(&pCMe->public_y);
        result = ERR_ECO_SUCCESS;
    }
    return result;
}

static uint32_t ECOCALLMETHOD Lab65Private_AddRef(IEcoUnknownPtr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (me == 0) return 0;
    pCMe->local_refs++;
    return pCMe->local_refs;
}

static uint32_t ECOCALLMETHOD Lab65Private_Release(IEcoUnknownPtr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (me == 0) return 0;
    pCMe->local_refs--;
    if (pCMe->local_refs == 0) {
        pCMe->owned_allocator->pVTbl->Free(pCMe->owned_allocator, pCMe);
        pCMe->owned_allocator->pVTbl->Release(pCMe->owned_allocator);
        return 0;
    }
    return pCMe->local_refs;
}

/* Public Lab65 methods */
static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    return pCMe->controller->pVTbl->QueryInterface(pCMe->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoLab65Ptr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (me == 0) return 0;
    return pCMe->controller->pVTbl->AddRef(pCMe->controller);
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoLab65Ptr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (me == 0) return 0;
    return pCMe->controller->pVTbl->Release(pCMe->controller);
}

static int16_t ECOCALLMETHOD Lab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (me == 0) return ERR_ECO_POINTER;
    return lab_sort_int(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (me == 0) return ERR_ECO_POINTER;
    return lab_sort_long(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (me == 0) return ERR_ECO_POINTER;
    return lab_sort_float(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (me == 0) return ERR_ECO_POINTER;
    return lab_sort_double(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (me == 0) return ERR_ECO_POINTER;
    return lab_sort_long_double(me, data, count, buckets);
}

/* Public X methods */
static int16_t ECOCALLMETHOD Lab65X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    return pCMe->controller->pVTbl->QueryInterface(pCMe->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65X_AddRef(IEcoCalculatorXPtr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (me == 0) return 0;
    return pCMe->controller->pVTbl->AddRef(pCMe->controller);
}

static uint32_t ECOCALLMETHOD Lab65X_Release(IEcoCalculatorXPtr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (me == 0) return 0;
    return pCMe->controller->pVTbl->Release(pCMe->controller);
}

static int32_t ECOCALLMETHOD Lab65X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (me == 0) return 0;
    return pCMe->inner_x->pVTbl->Addition(pCMe->inner_x, a, b);
}

static int16_t ECOCALLMETHOD Lab65X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (me == 0) return 0;
    return pCMe->inner_x->pVTbl->Subtraction(pCMe->inner_x, a, b);
}

/* Public Y methods */
static int16_t ECOCALLMETHOD Lab65Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_NOINTERFACE;

    return pCMe->controller->pVTbl->QueryInterface(pCMe->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65Y_AddRef(IEcoCalculatorYPtr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (me == 0) return 0;
    return pCMe->controller->pVTbl->AddRef(pCMe->controller);
}

static uint32_t ECOCALLMETHOD Lab65Y_Release(IEcoCalculatorYPtr_t me) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (me == 0) return 0;
    return pCMe->controller->pVTbl->Release(pCMe->controller);
}

static int32_t ECOCALLMETHOD Lab65Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (me == 0) return 0;
    return pCMe->inner_y->pVTbl->Multiplication(pCMe->inner_y, a, b);
}

static int16_t ECOCALLMETHOD Lab65Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (me == 0) return 0;
    return pCMe->inner_y->pVTbl->Division(pCMe->inner_y, a, b);
}

/* Static tables */
static IEcoLab65VTbl Lab65Vtbl = {
    Lab65_QueryInterface,
    Lab65_AddRef,
    Lab65_Release,
    Lab65_SortInt,
    Lab65_SortLong,
    Lab65_SortFloat,
    Lab65_SortDouble,
    Lab65_SortLongDouble
};

static IEcoCalculatorXVTbl Lab65XVtbl = {
    Lab65X_QueryInterface,
    Lab65X_AddRef,
    Lab65X_Release,
    Lab65X_Addition,
    Lab65X_Subtraction
};

static IEcoCalculatorYVTbl Lab65YVtbl = {
    Lab65Y_QueryInterface,
    Lab65Y_AddRef,
    Lab65Y_Release,
    Lab65Y_Multiplication,
    Lab65Y_Division
};

static IEcoUnknownVTbl Lab65PrivateVtbl = {
    Lab65Private_QueryInterface,
    Lab65Private_AddRef,
    Lab65Private_Release
};

/* Safe temporary error stubs */
int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;
    if (system == 0 || iid == 0) return ERR_ECO_NOINTERFACE;
    if (outer != 0 && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;
    return ERR_ECO_NOINTERFACE;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;
    if (system == 0 || iid == 0) return ERR_ECO_NOINTERFACE;
    if (outer != 0) return ERR_ECO_NOAGGREGATION;
    return ERR_ECO_NOINTERFACE;
}

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    if (me == 0) return 0;
    Lab65State* pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return pCMe->owned_allocator;
}
