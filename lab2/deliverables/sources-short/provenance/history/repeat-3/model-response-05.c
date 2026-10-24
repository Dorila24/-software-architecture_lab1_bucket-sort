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

static int16_t lab65_query_interface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t lab65_add_ref(IEcoLab65Ptr_t me);
static uint32_t lab65_release(IEcoLab65Ptr_t me);
static int16_t lab65_sort_int(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets);
static int16_t lab65_sort_long(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets);
static int16_t lab65_sort_float(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets);
static int16_t lab65_sort_double(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets);
static int16_t lab65_sort_long_double(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets);

static int16_t unknown_query_interface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t unknown_add_ref(IEcoUnknownPtr_t me);
static uint32_t unknown_release(IEcoUnknownPtr_t me);

static int16_t x_query_interface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t x_add_ref(IEcoCalculatorXPtr_t me);
static uint32_t x_release(IEcoCalculatorXPtr_t me);
static int32_t x_addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);
static int16_t x_subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b);

static int16_t y_query_interface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv);
static uint32_t y_add_ref(IEcoCalculatorYPtr_t me);
static uint32_t y_release(IEcoCalculatorYPtr_t me);
static int32_t y_multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);
static int16_t y_division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b);

static IEcoLab65VTbl lab65_vtbl = {
    lab65_query_interface,
    lab65_add_ref,
    lab65_release,
    lab65_sort_int,
    lab65_sort_long,
    lab65_sort_float,
    lab65_sort_double,
    lab65_sort_long_double
};

static IEcoUnknownVTbl unknown_vtbl = {
    unknown_query_interface,
    unknown_add_ref,
    unknown_release
};

static IEcoCalculatorXVTbl x_vtbl = {
    x_query_interface,
    x_add_ref,
    x_release,
    x_addition,
    x_subtraction
};

static IEcoCalculatorYVTbl y_vtbl = {
    y_query_interface,
    y_add_ref,
    y_release,
    y_multiplication,
    y_division
};

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    if (me == 0) return 0;
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return state->owned_allocator;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;
    if (system == 0 || iid == 0) return ERR_ECO_NOSYSTEM;
    if (outer != 0 && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;
     
    return ERR_ECO_NOINTERFACE;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == 0) return ERR_ECO_POINTER;
    *out = 0;
    if (system == 0 || iid == 0) return ERR_ECO_NOSYSTEM;
    if (outer != 0) return ERR_ECO_NOAGGREGATION;
     
    return ERR_ECO_NOINTERFACE;
}

static int16_t lab65_query_interface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));

        if (state->controller != 0) {
            return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
        }

        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t lab65_add_ref(IEcoLab65Ptr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        if (state->controller != 0) {
            return state->controller->pVTbl->AddRef(state->controller);
        }
        return 0;
    }
}

static uint32_t lab65_release(IEcoLab65Ptr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        if (state->controller != 0) {
            return state->controller->pVTbl->Release(state->controller);
        }
        return 0;
    }
}

static int16_t lab65_sort_int(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    if (me == 0 || data == 0) return ERR_ECO_POINTER;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        return lab_sort_int(me, data, count, buckets);
    }
}

static int16_t lab65_sort_long(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    if (me == 0 || data == 0) return ERR_ECO_POINTER;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        return lab_sort_long(me, data, count, buckets);
    }
}

static int16_t lab65_sort_float(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    if (me == 0 || data == 0) return ERR_ECO_POINTER;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        return lab_sort_float(me, data, count, buckets);
    }
}

static int16_t lab65_sort_double(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    if (me == 0 || data == 0) return ERR_ECO_POINTER;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        return lab_sort_double(me, data, count, buckets);
    }
}

static int16_t lab65_sort_long_double(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    if (me == 0 || data == 0) return ERR_ECO_POINTER;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        return lab_sort_long_double(me, data, count, buckets);
    }
}

static int16_t unknown_query_interface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));

        if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
            *ppv = &state->public_lab65;
            state->public_lab65.pVTbl->AddRef(&state->public_lab65);
            return ERR_ECO_SUCCESS;
        }

        if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
            *ppv = &state->private_unknown;
            state->local_refs++;
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
}

static uint32_t unknown_add_ref(IEcoUnknownPtr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
        return ++state->local_refs;
    }
}

static uint32_t unknown_release(IEcoUnknownPtr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
        if (--state->local_refs == 0) {
            IEcoMemoryAllocator1* allocator = state->owned_allocator;
            if (state->inner_x != 0) {
                state->inner_x->pVTbl->Release(state->inner_x);
            }
            if (state->inner_y != 0) {
                state->inner_y->pVTbl->Release(state->inner_y);
            }
            if (state->owned_system != 0) {
                state->owned_system->pVTbl->Release(state->owned_system);
            }
            allocator->pVTbl->Free(allocator, state);
            allocator->pVTbl->Release(allocator);
        }
        return 0;
    }
}

static int16_t x_query_interface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));

        if (state->controller != 0) {
            return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
        }

        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t x_add_ref(IEcoCalculatorXPtr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state->controller != 0) {
            return state->controller->pVTbl->AddRef(state->controller);
        }
        return 0;
    }
}

static uint32_t x_release(IEcoCalculatorXPtr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state->controller != 0) {
            return state->controller->pVTbl->Release(state->controller);
        }
        return 0;
    }
}

static int32_t x_addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state->inner_x != 0) {
            return state->inner_x->pVTbl->Addition(state->inner_x, a, b);
        }
        return 0;
    }
}

static int16_t x_subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        if (state->inner_x != 0) {
            return state->inner_x->pVTbl->Subtraction(state->inner_x, a, b);
        }
        return 0;
    }
}

static int16_t y_query_interface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv == 0) return ERR_ECO_POINTER;
    *ppv = 0;
    if (me == 0 || riid == 0) return ERR_ECO_POINTER;

    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));

        if (state->controller != 0) {
            return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
        }

        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t y_add_ref(IEcoCalculatorYPtr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state->controller != 0) {
            return state->controller->pVTbl->AddRef(state->controller);
        }
        return 0;
    }
}

static uint32_t y_release(IEcoCalculatorYPtr_t me) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state->controller != 0) {
            return state->controller->pVTbl->Release(state->controller);
        }
        return 0;
    }
}

static int32_t y_multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state->inner_y != 0) {
            return state->inner_y->pVTbl->Multiplication(state->inner_y, a, b);
        }
        return 0;
    }
}

static int16_t y_division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (me == 0) return 0;
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        if (state->inner_y != 0) {
            return state->inner_y->pVTbl->Division(state->inner_y, a, b);
        }
        return 0;
    }
}
