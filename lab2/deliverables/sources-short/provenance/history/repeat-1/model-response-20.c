#include "ReuseContract.h"

typedef struct {
    IEcoLab65 public_lab65;
    IEcoUnknown private_unknown;
    uint32_t local_refs;
    IEcoUnknown* controller;
    IEcoSystem1* owned_system;
    IEcoMemoryAllocator1* owned_allocator;
    IEcoCalculatorX* inner_x;
    IEcoCalculatorY* inner_y;
    IEcoCalculatorX public_calculator_x;
    IEcoCalculatorY public_calculator_y;
} Lab65State;

static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    *ppv = NULL;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_lab65));
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoLab65Ptr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_lab65));
    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoLab65Ptr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_lab65));
    return state->controller->pVTbl->Release(state->controller);
}

static int16_t ECOCALLMETHOD Lab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_int(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    return lab_sort_float(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_double(me, data, count, buckets);
}

static int16_t ECOCALLMETHOD Lab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long_double(me, data, count, buckets);
}

static IEcoLab65VTbl Lab65_Vtbl = {
    Lab65_QueryInterface,
    Lab65_AddRef,
    Lab65_Release,
    Lab65_SortInt,
    Lab65_SortLong,
    Lab65_SortFloat,
    Lab65_SortDouble,
    Lab65_SortLongDouble
};

static int16_t ECOCALLMETHOD CalculatorX_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    *ppv = NULL;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_calculator_x));
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD CalculatorX_AddRef(IEcoCalculatorXPtr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_calculator_x));
    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD CalculatorX_Release(IEcoCalculatorXPtr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_calculator_x));
    return state->controller->pVTbl->Release(state->controller);
}

static int32_t ECOCALLMETHOD CalculatorX_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_calculator_x));
    if (state->inner_x == NULL) return 0;
    return state->inner_x->pVTbl->Addition(state->inner_x, a, b);
}

static int16_t ECOCALLMETHOD CalculatorX_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_calculator_x));
    if (state->inner_x == NULL) return 0;
    return state->inner_x->pVTbl->Subtraction(state->inner_x, a, b);
}

static IEcoCalculatorXVTbl CalculatorX_Vtbl = {
    CalculatorX_QueryInterface,
    CalculatorX_AddRef,
    CalculatorX_Release,
    CalculatorX_Addition,
    CalculatorX_Subtraction
};

static int16_t ECOCALLMETHOD CalculatorY_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    *ppv = NULL;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_calculator_y));
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD CalculatorY_AddRef(IEcoCalculatorYPtr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_calculator_y));
    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD CalculatorY_Release(IEcoCalculatorYPtr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_calculator_y));
    return state->controller->pVTbl->Release(state->controller);
}

static int32_t ECOCALLMETHOD CalculatorY_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_calculator_y));
    if (state->inner_y == NULL) return 0;
    return state->inner_y->pVTbl->Multiplication(state->inner_y, a, b);
}

static int16_t ECOCALLMETHOD CalculatorY_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_calculator_y));
    if (state->inner_y == NULL) return 0;
    return state->inner_y->pVTbl->Division(state->inner_y, a, b);
}

static IEcoCalculatorYVTbl CalculatorY_Vtbl = {
    CalculatorY_QueryInterface,
    CalculatorY_AddRef,
    CalculatorY_Release,
    CalculatorY_Multiplication,
    CalculatorY_Division
};

static int16_t ECOCALLMETHOD Unknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    *ppv = NULL;
    {
        Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, private_unknown));
        if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
            *ppv = &state->private_unknown;
            state->local_refs++;
            return ERR_ECO_SUCCESS;
        } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
            *ppv = &state->public_lab65;
            state->public_lab65.pVTbl->AddRef(&state->public_lab65);
            return ERR_ECO_SUCCESS;
        } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
            *ppv = &state->public_calculator_x;
            state->public_calculator_x.pVTbl->AddRef(&state->public_calculator_x);
            return ERR_ECO_SUCCESS;
        } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
            *ppv = &state->public_calculator_y;
            state->public_calculator_y.pVTbl->AddRef(&state->public_calculator_y);
            return ERR_ECO_SUCCESS;
        } else {
            return ERR_ECO_NOINTERFACE;
        }
    }
}

static uint32_t ECOCALLMETHOD Unknown_AddRef(IEcoUnknownPtr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, private_unknown));
    state->local_refs++;
    return state->local_refs;
}

static uint32_t ECOCALLMETHOD Unknown_Release(IEcoUnknownPtr_t me) {
    Lab65State* state;
    if (me == NULL) return 0;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, private_unknown));
    if (--state->local_refs == 0) {
        IEcoMemoryAllocator1* allocator = state->owned_allocator;
        IEcoCalculatorX* inner_x = state->inner_x;
        IEcoCalculatorY* inner_y = state->inner_y;
        IEcoSystem1* owned_system = state->owned_system;

        if (inner_x != NULL) inner_x->pVTbl->Release(inner_x);
        if (inner_y != NULL) inner_y->pVTbl->Release(inner_y);
        if (owned_system != NULL) owned_system->pVTbl->Release(owned_system);

        allocator->pVTbl->Free(allocator, state);
        allocator->pVTbl->Release(allocator);
        return 0;
    }
    return state->local_refs;
}

static IEcoUnknownVTbl Unknown_Vtbl = {
    Unknown_QueryInterface,
    Unknown_AddRef,
    Unknown_Release
};

IEcoMemoryAllocator1* ECOCALLMETHOD lab_allocator(IEcoLab65* me) {
    if (me == NULL) return NULL;
    {
        Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_lab65));
        return state->owned_allocator;
    }
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    int16_t creation_status = 0;

    if (out != NULL) *out = NULL;
    if (system == NULL || iid == NULL || out == NULL) return ERR_ECO_POINTER;
    if (outer != NULL && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;
    {
        IEcoSystem1* owned_system = NULL;
        IEcoInterfaceBus1* bus = NULL;
        IEcoMemoryAllocator1* owned_allocator = NULL;
        Lab65State* state = NULL;
        IEcoInterfaceBus1MemExt* extension = NULL;
        UGUID manager = CID_EcoMemoryManager1;

        creation_status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
        if (creation_status != 0) goto cleanup;

        creation_status = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
        if (creation_status != 0) goto cleanup;

        creation_status = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&extension);
        if (creation_status == 0) {
            const UGUID* manager_ptr = extension->pVTbl->get_Manager(extension);
            if (manager_ptr != NULL) {
                manager = *manager_ptr;
            }
            extension->pVTbl->Release(extension);
        }

        creation_status = bus->pVTbl->QueryComponent(bus, &manager, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
        if (creation_status != 0) goto cleanup;

        state = (Lab65State*) owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab65State));
        if (state == NULL) {
            creation_status = ERR_ECO_OUTOFMEMORY;
            goto cleanup;
        }

        state->public_lab65.pVTbl = &Lab65_Vtbl;
        state->private_unknown.pVTbl = &Unknown_Vtbl;
        state->local_refs = 1;
        state->controller = (outer != NULL) ? outer : &state->private_unknown;
        state->owned_system = owned_system;
        state->owned_allocator = owned_allocator;
        state->inner_x = NULL;
        state->inner_y = NULL;

        // Initialize public calculator interfaces
        state->public_calculator_x.pVTbl = &CalculatorX_Vtbl;
        state->public_calculator_y.pVTbl = &CalculatorY_Vtbl;

        // Inserted local assignments
        owned_system = NULL;
        owned_allocator = NULL;

        creation_status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
        if (creation_status != 0) goto cleanup;

        creation_status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
        if (creation_status != 0) goto cleanup;

        if (outer == NULL) {
            creation_status = Unknown_QueryInterface(&state->private_unknown, iid, out);
            if (creation_status == 0) {
                Unknown_Release(&state->private_unknown);
            }
        } else {
            *out = &state->private_unknown;
        }

cleanup:
        if (creation_status != 0) {
            if (state != NULL) {
                Unknown_Release(&state->private_unknown);
            }
            if (owned_allocator != NULL) owned_allocator->pVTbl->Release(owned_allocator);
        }
        if (bus != NULL) bus->pVTbl->Release(bus);
        if (owned_system != NULL) owned_system->pVTbl->Release(owned_system);
    }
    return creation_status;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out != NULL) *out = NULL;
    if (system == NULL || iid == NULL || out == NULL) return ERR_ECO_POINTER;
    if (outer != NULL) return ERR_ECO_NOAGGREGATION;
    return ERR_ECO_NOINTERFACE;
}
