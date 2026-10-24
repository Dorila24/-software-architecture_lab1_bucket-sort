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
} Lab65State;

static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == NULL) return ERR_ECO_POINTER;
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoLab65Ptr_t me) {
    Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == NULL) return 0;
    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoLab65Ptr_t me) {
    Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_lab65));
    if (state->controller == NULL) return 0;
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

static int16_t ECOCALLMETHOD Unknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    Lab65State* state = NULL;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, private_unknown));
    *ppv = NULL;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->private_unknown;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &state->public_lab65;
        return state->public_lab65.pVTbl->AddRef(&state->public_lab65);
    } else {
        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t ECOCALLMETHOD Unknown_AddRef(IEcoUnknownPtr_t me) {
    Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, private_unknown));
    state->local_refs++;
    return state->local_refs;
}

static uint32_t ECOCALLMETHOD Unknown_Release(IEcoUnknownPtr_t me) {
    Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, private_unknown));
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
    Lab65State* state = NULL;
    state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_lab65));
    return state->owned_allocator;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out != NULL) *out = NULL;
    if (system == NULL || iid == NULL || out == NULL) return ERR_ECO_POINTER;
    if (outer != NULL && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;

    IEcoSystem1* owned_system = NULL;
    IEcoInterfaceBus1* bus = NULL;
    IEcoMemoryAllocator1* owned_allocator = NULL;
    Lab65State* state = NULL;
    IEcoInterfaceBus1MemExt* extension = NULL;
    UGUID manager = CID_EcoMemoryManager1;
    int16_t hr = 0;

    hr = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
    if (hr != 0) goto cleanup;

    hr = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (hr != 0) goto cleanup;

    hr = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&extension);
    if (hr == 0) {
        const UGUID* manager_ptr = extension->pVTbl->get_Manager(extension);
        if (manager_ptr != NULL) {
            manager = *manager_ptr;
        }
        extension->pVTbl->Release(extension);
    }

    hr = bus->pVTbl->QueryComponent(bus, &manager, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
    if (hr != 0) goto cleanup;

    state = (Lab65State*) owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab65State));
    if (state == NULL) {
        hr = ERR_ECO_OUTOFMEMORY;
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

    hr = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
    if (hr != 0) goto cleanup;

    hr = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
    if (hr != 0) goto cleanup;

    if (outer == NULL) {
        hr = Unknown_QueryInterface(&state->private_unknown, iid, out);
        if (hr == 0) {
            Unknown_Release(&state->private_unknown);
        }
    } else {
        *out = &state->private_unknown;
    }

cleanup:
    if (hr != 0) {
        if (state != NULL) {
            IEcoMemoryAllocator1* allocator = state->owned_allocator;
            if (state->inner_x != NULL) state->inner_x->pVTbl->Release(state->inner_x);
            if (state->inner_y != NULL) state->inner_y->pVTbl->Release(state->inner_y);
            if (state->owned_system != NULL) state->owned_system->pVTbl->Release(state->owned_system);
            allocator->pVTbl->Free(allocator, state);
        }
        if (owned_allocator != NULL) owned_allocator->pVTbl->Release(owned_allocator);
    }
    if (bus != NULL) bus->pVTbl->Release(bus);
    if (owned_system != NULL) owned_system->pVTbl->Release(owned_system);
    return hr;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out != NULL) *out = NULL;
    if (system == NULL || iid == NULL || out == NULL) return ERR_ECO_POINTER;
    if (outer != NULL) return ERR_ECO_NOAGGREGATION;
    return ERR_ECO_NOINTERFACE;
}
