#include "ReuseContract.h"
#include <stddef.h>

typedef struct {
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

// Public Lab65 methods
int16_t ECOCALLMETHOD public_Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (ppv == NULL) return ERR_ECO_POINTER;
    *ppv = NULL;
    if (state->controller == NULL) return ERR_ECO_NOINTERFACE;
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

uint32_t ECOCALLMETHOD public_Lab65_AddRef(IEcoLab65Ptr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == NULL) return 0;
    return state->controller->pVTbl->AddRef(state->controller);
}

uint32_t ECOCALLMETHOD public_Lab65_Release(IEcoLab65Ptr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == NULL) return 0;
    return state->controller->pVTbl->Release(state->controller);
}

int16_t ECOCALLMETHOD public_Lab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == NULL) return ERR_ECO_NOINTERFACE;
    return lab_sort_int(me, data, count, buckets);
}

int16_t ECOCALLMETHOD public_Lab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == NULL) return ERR_ECO_NOINTERFACE;
    return lab_sort_long(me, data, count, buckets);
}

int16_t ECOCALLMETHOD public_Lab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == NULL) return ERR_ECO_NOINTERFACE;
    return lab_sort_float(me, data, count, buckets);
}

int16_t ECOCALLMETHOD public_Lab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == NULL) return ERR_ECO_NOINTERFACE;
    return lab_sort_double(me, data, count, buckets);
}

int16_t ECOCALLMETHOD public_Lab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == NULL) return ERR_ECO_NOINTERFACE;
    return lab_sort_long_double(me, data, count, buckets);
}

// Public X methods
int16_t ECOCALLMETHOD public_X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (ppv == NULL) return ERR_ECO_POINTER;
    *ppv = NULL;
    if (state->controller == NULL) return ERR_ECO_NOINTERFACE;
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

uint32_t ECOCALLMETHOD public_X_AddRef(IEcoCalculatorXPtr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (state->controller == NULL) return 0;
    return state->controller->pVTbl->AddRef(state->controller);
}

uint32_t ECOCALLMETHOD public_X_Release(IEcoCalculatorXPtr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (state->controller == NULL) return 0;
    return state->controller->pVTbl->Release(state->controller);
}

int32_t ECOCALLMETHOD public_X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (state->inner_x == NULL) return 0;
    return state->inner_x->pVTbl->Addition(state->inner_x, a, b);
}

int16_t ECOCALLMETHOD public_X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (state->inner_x == NULL) return 0;
    return state->inner_x->pVTbl->Subtraction(state->inner_x, a, b);
}

// Public Y methods
int16_t ECOCALLMETHOD public_Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (ppv == NULL) return ERR_ECO_POINTER;
    *ppv = NULL;
    if (state->controller == NULL) return ERR_ECO_NOINTERFACE;
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

uint32_t ECOCALLMETHOD public_Y_AddRef(IEcoCalculatorYPtr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (state->controller == NULL) return 0;
    return state->controller->pVTbl->AddRef(state->controller);
}

uint32_t ECOCALLMETHOD public_Y_Release(IEcoCalculatorYPtr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (state->controller == NULL) return 0;
    return state->controller->pVTbl->Release(state->controller);
}

int32_t ECOCALLMETHOD public_Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (state->inner_y == NULL) return 0;
    return state->inner_y->pVTbl->Multiplication(state->inner_y, a, b);
}

int16_t ECOCALLMETHOD public_Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (state->inner_y == NULL) return 0;
    return state->inner_y->pVTbl->Division(state->inner_y, a, b);
}

// Private Unknown methods
int16_t ECOCALLMETHOD private_Unknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (ppv == NULL) return ERR_ECO_POINTER;
    *ppv = NULL;
    if (me == NULL || riid == NULL) return ERR_ECO_POINTER;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->private_unknown;
        state->private_unknown.pVTbl->AddRef(&state->private_unknown);
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &state->public_lab65;
        state->public_lab65.pVTbl->AddRef(&state->public_lab65);
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->public_x;
        state->public_x.pVTbl->AddRef(&state->public_x);
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->public_y;
        state->public_y.pVTbl->AddRef(&state->public_y);
        return ERR_ECO_SUCCESS;
    }
    return ERR_ECO_NOINTERFACE;
}

uint32_t ECOCALLMETHOD private_Unknown_AddRef(IEcoUnknownPtr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (me == NULL) return 0;
    return ++state->local_refs;
}

uint32_t ECOCALLMETHOD private_Unknown_Release(IEcoUnknownPtr_t me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (me == NULL) return 0;
    if (--state->local_refs == 0) {
        IEcoMemoryAllocator1* allocator = state->owned_allocator;
        if (state->inner_x != NULL) state->inner_x->pVTbl->Release(state->inner_x);
        if (state->inner_y != NULL) state->inner_y->pVTbl->Release(state->inner_y);
        if (state->owned_system != NULL) state->owned_system->pVTbl->Release(state->owned_system);
        allocator->pVTbl->Free(allocator, state);
        allocator->pVTbl->Release(allocator);
        return 0;
    }
    return state->local_refs;
}

// Lab65State functions
IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (state == NULL) return NULL;
    return state->owned_allocator;
}

// Static tables
static IEcoLab65VTbl public_Lab65_vtbl = {
    public_Lab65_QueryInterface,
    public_Lab65_AddRef,
    public_Lab65_Release,
    public_Lab65_SortInt,
    public_Lab65_SortLong,
    public_Lab65_SortFloat,
    public_Lab65_SortDouble,
    public_Lab65_SortLongDouble
};

static IEcoCalculatorXVTbl public_X_vtbl = {
    public_X_QueryInterface,
    public_X_AddRef,
    public_X_Release,
    public_X_Addition,
    public_X_Subtraction
};

static IEcoCalculatorYVTbl public_Y_vtbl = {
    public_Y_QueryInterface,
    public_Y_AddRef,
    public_Y_Release,
    public_Y_Multiplication,
    public_Y_Division
};

static IEcoUnknownVTbl private_Unknown_vtbl = {
    private_Unknown_QueryInterface,
    private_Unknown_AddRef,
    private_Unknown_Release
};

// Safe error stubs for creators
int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == NULL) return ERR_ECO_POINTER;
    *out = NULL;
    if (system == NULL || iid == NULL) return ERR_ECO_POINTER;
    if (outer != NULL && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;
    return ERR_ECO_NOINTERFACE;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == NULL) return ERR_ECO_POINTER;
    *out = NULL;
    if (system == NULL || iid == NULL) return ERR_ECO_POINTER;
    if (outer != NULL) return ERR_ECO_NOAGGREGATION;
    return ERR_ECO_NOINTERFACE;
}
