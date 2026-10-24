#include "ReuseContract.h"

typedef struct Lab65State {
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
    if (!me || !riid || !ppv) {
        return ERR_ECO_POINTER;
    }
    *ppv = NULL;
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoLab65Ptr_t me) {
    if (!me) {
        return 0;
    }
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoLab65Ptr_t me) {
    if (!me) {
        return 0;
    }
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
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

static IEcoLab65VTbl public_lab65_vtbl = {
    Lab65_QueryInterface,
    Lab65_AddRef,
    Lab65_Release,
    Lab65_SortInt,
    Lab65_SortLong,
    Lab65_SortFloat,
    Lab65_SortDouble,
    Lab65_SortLongDouble
};

static int16_t ECOCALLMETHOD Lab65_Private_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (!me || !riid || !ppv) {
        return ERR_ECO_POINTER;
    }
    *ppv = NULL;
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = (voidptr_t)&state->private_unknown;
        state->local_refs++;
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = (voidptr_t)&state->public_lab65;
        state->public_lab65.pVTbl->AddRef(&state->public_lab65);
        return 0;
    }
    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD Lab65_Private_AddRef(IEcoUnknownPtr_t me) {
    if (!me) {
        return 0;
    }
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    state->local_refs++;
    return state->local_refs;
}

static uint32_t ECOCALLMETHOD Lab65_Private_Release(IEcoUnknownPtr_t me) {
    if (!me) {
        return 0;
    }
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    state->local_refs--;
    if (state->local_refs == 0) {
        IEcoMemoryAllocator1* allocator = state->owned_allocator;
        if (state->inner_x) {
            state->inner_x->pVTbl->Release((IEcoUnknown*)state->inner_x);
        }
        if (state->inner_y) {
            state->inner_y->pVTbl->Release((IEcoUnknown*)state->inner_y);
        }
        if (state->owned_system) {
            state->owned_system->pVTbl->Release((IEcoUnknown*)state->owned_system);
        }
        state->owned_allocator->Free(state->owned_allocator, state);
        allocator->pVTbl->Release((IEcoUnknown*)allocator);
        return 0;
    }
    return state->local_refs;
}

static IEcoUnknownVTbl private_unknown_vtbl = {
    Lab65_Private_QueryInterface,
    Lab65_Private_AddRef,
    Lab65_Private_Release
};

IEcoMemoryAllocator1* ECOCALLMETHOD lab_allocator(IEcoLab65* me) {
    if (!me) {
        return NULL;
    }
    Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    return state->owned_allocator;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    int16_t status = 0;
    IEcoSystem1* owned_system = NULL;
    IEcoInterfaceBus1* bus = NULL;
    IEcoMemoryAllocator1* owned_allocator = NULL;
    Lab65State* state = NULL;
    IEcoInterfaceBus1MemExt* mem_ext = NULL;
    UGUID manager_id = CID_EcoMemoryManager1;

    if (!out || !iid) {
        return ERR_ECO_POINTER;
    }
    *out = NULL;

    if (outer && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        return ERR_ECO_NOAGGREGATION;
    }

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem1, (voidptr_t*)&owned_system);
    if (status != 0) {
        return status;
    }

    status = owned_system->pVTbl->QueryInterface((IEcoUnknown*)owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != 0) {
        goto cleanup;
    }

    status = bus->pVTbl->QueryInterface((IEcoUnknown*)bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&mem_ext);
    if (status == 0) {
        const UGUID* manager = mem_ext->pVTbl->get_Manager(mem_ext);
        if (manager) {
            manager_id = *manager;
        }
        mem_ext->pVTbl->Release((IEcoUnknown*)mem_ext);
    }

    status = bus->pVTbl->QueryComponent(bus, &manager_id, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
    if (status != 0) {
        goto cleanup;
    }

    state = (Lab65State*)owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab65State));
    if (!state) {
        status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

    state->public_lab65.pVTbl = &public_lab65_vtbl;
    state->private_unknown.pVTbl = &private_unknown_vtbl;
    state->local_refs = 1;
    state->controller = outer ? outer : (IEcoUnknown*)&state->private_unknown;
    state->owned_system = owned_system;
    state->owned_allocator = owned_allocator;
    state->inner_x = NULL;
    state->inner_y = NULL;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
    if (status != 0) {
        goto cleanup;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
    if (status != 0) {
        goto cleanup;
    }

    owned_system = NULL;
    owned_allocator = NULL;

    if (outer) {
        *out = &state->private_unknown;
        return 0;
    }

    status = Lab65_Private_QueryInterface((IEcoUnknown*)&state->private_unknown, iid, out);
    if (status != 0) {
        goto cleanup;
    }

    Lab65_Private_Release((IEcoUnknown*)&state->private_unknown);
    return 0;

cleanup:
    if (state) {
        Lab65_Private_Release((IEcoUnknown*)&state->private_unknown);
    }
    if (owned_allocator) {
        owned_allocator->pVTbl->Free(owned_allocator, state);
    }
    if (owned_system) {
        owned_system->pVTbl->Release((IEcoUnknown*)owned_system);
    }
    if (bus) {
        bus->pVTbl->Release((IEcoUnknown*)bus);
    }
    return status;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!out || !iid) {
        return ERR_ECO_POINTER;
    }
    *out = NULL;

    if (outer) {
        return ERR_ECO_NOAGGREGATION;
    }

    return ERR_ECO_NOINTERFACE;
}
