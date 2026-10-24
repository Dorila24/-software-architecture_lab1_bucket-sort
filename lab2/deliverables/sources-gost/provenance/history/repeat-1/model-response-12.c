#include "ReuseContract.h"
#include "IdEcoLab65.h"

typedef struct Lab65State {
    IEcoLab65 public_lab65;
    IEcoUnknown private_unknown;
    uint32_t local_refs;
    IEcoUnknown* controller;  
    IEcoSystem1* owned_system;
    IEcoMemoryAllocator1* owned_allocator;
    IEcoCalculatorX* inner_x;
    IEcoCalculatorY* inner_y;
    IEcoCalculatorX public_x;
    IEcoCalculatorY public_y;
} Lab65State;

static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv) {
        *ppv = NULL;
    }
    if (!me || !riid || !ppv) {
        return ERR_ECO_POINTER;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
    }
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoLab65Ptr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        return state->controller->pVTbl->AddRef(state->controller);
    }
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoLab65Ptr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        return state->controller->pVTbl->Release(state->controller);
    }
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
    if (ppv) {
        *ppv = NULL;
    }
    if (!me || !riid || !ppv) {
        return ERR_ECO_POINTER;
    }
    {
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
        if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
            *ppv = (voidptr_t)&state->public_x;
            state->public_x.pVTbl->AddRef(&state->public_x);
            return 0;
        }
        if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
            *ppv = (voidptr_t)&state->public_y;
            state->public_y.pVTbl->AddRef(&state->public_y);
            return 0;
        }
        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t ECOCALLMETHOD Lab65_Private_AddRef(IEcoUnknownPtr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
        state->local_refs++;
        return state->local_refs;
    }
}

static uint32_t ECOCALLMETHOD Lab65_Private_Release(IEcoUnknownPtr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
        state->local_refs--;
        if (state->local_refs == 0) {
            IEcoMemoryAllocator1* allocator = state->owned_allocator;
            if (state->inner_x) {
                state->inner_x->pVTbl->Release(state->inner_x);
            }
            if (state->inner_y) {
                state->inner_y->pVTbl->Release(state->inner_y);
            }
            if (state->owned_system) {
                state->owned_system->pVTbl->Release(state->owned_system);
            }
            allocator->pVTbl->Free(allocator, state);
            allocator->pVTbl->Release(allocator);
            return 0;
        }
        return state->local_refs;
    }
}

static IEcoUnknownVTbl private_unknown_vtbl = {
    Lab65_Private_QueryInterface,
    Lab65_Private_AddRef,
    Lab65_Private_Release
};

static int16_t ECOCALLMETHOD Lab65_PublicX_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv) {
        *ppv = NULL;
    }
    if (!me || !riid || !ppv) {
        return ERR_ECO_POINTER;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
    }
}

static uint32_t ECOCALLMETHOD Lab65_PublicX_AddRef(IEcoCalculatorXPtr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        return state->controller->pVTbl->AddRef(state->controller);
    }
}

static uint32_t ECOCALLMETHOD Lab65_PublicX_Release(IEcoCalculatorXPtr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        return state->controller->pVTbl->Release(state->controller);
    }
}

static int32_t ECOCALLMETHOD Lab65_PublicX_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (!me) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        return state->inner_x->pVTbl->Addition(state->inner_x, a, b);
    }
}

static int16_t ECOCALLMETHOD Lab65_PublicX_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (!me) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
        return state->inner_x->pVTbl->Subtraction(state->inner_x, a, b);
    }
}

static IEcoCalculatorXVTbl public_x_vtbl = {
    Lab65_PublicX_QueryInterface,
    Lab65_PublicX_AddRef,
    Lab65_PublicX_Release,
    Lab65_PublicX_Addition,
    Lab65_PublicX_Subtraction
};

static int16_t ECOCALLMETHOD Lab65_PublicY_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv) {
        *ppv = NULL;
    }
    if (!me || !riid || !ppv) {
        return ERR_ECO_POINTER;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
    }
}

static uint32_t ECOCALLMETHOD Lab65_PublicY_AddRef(IEcoCalculatorYPtr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        return state->controller->pVTbl->AddRef(state->controller);
    }
}

static uint32_t ECOCALLMETHOD Lab65_PublicY_Release(IEcoCalculatorYPtr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        return state->controller->pVTbl->Release(state->controller);
    }
}

static int32_t ECOCALLMETHOD Lab65_PublicY_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (!me) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        return state->inner_y->pVTbl->Multiplication(state->inner_y, a, b);
    }
}

static int16_t ECOCALLMETHOD Lab65_PublicY_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (!me) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
        return state->inner_y->pVTbl->Division(state->inner_y, a, b);
    }
}

static IEcoCalculatorYVTbl public_y_vtbl = {
    Lab65_PublicY_QueryInterface,
    Lab65_PublicY_AddRef,
    Lab65_PublicY_Release,
    Lab65_PublicY_Multiplication,
    Lab65_PublicY_Division
};

IEcoMemoryAllocator1* ECOCALLMETHOD lab_allocator(IEcoLab65* me) {
    if (!me) {
        return NULL;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        return state->owned_allocator;
    }
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    int16_t status = 0;
    IEcoSystem1* owned_system = NULL;
    IEcoInterfaceBus1* bus = NULL;
    IEcoMemoryAllocator1* owned_allocator = NULL;
    Lab65State* state = NULL;
    IEcoInterfaceBus1MemExt* mem_ext = NULL;
    UGUID manager_id = CID_EcoMemoryManager1;

    if (out) {
        *out = NULL;
    }
    if (!out || !iid) {
        return ERR_ECO_POINTER;
    }

    if (outer && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        return ERR_ECO_NOAGGREGATION;
    }

    if (!system) {
        return ERR_ECO_NOSYSTEM;
    }

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
    if (status != 0) {
        return status;
    }

    status = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != 0) {
        goto cleanup;
    }

    status = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&mem_ext);
    if (status == 0) {
        const UGUID* manager = mem_ext->pVTbl->get_Manager(mem_ext);
        if (manager) {
            manager_id = *manager;
        }
        mem_ext->pVTbl->Release(mem_ext);
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
    state->controller = outer ? outer : &state->private_unknown;
    state->owned_system = owned_system;
    state->owned_allocator = owned_allocator;
    state->inner_x = NULL;
    state->inner_y = NULL;

    state->public_x.pVTbl = &public_x_vtbl;
    state->public_y.pVTbl = &public_y_vtbl;

    owned_system = NULL;
    owned_allocator = NULL;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
    if (status != 0) {
        goto cleanup;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
    if (status != 0) {
        goto cleanup;
    }

    if (outer) {
        *out = &state->private_unknown;
        bus->pVTbl->Release(bus);
        return 0;
    }

    status = Lab65_Private_QueryInterface(&state->private_unknown, iid, out);
    if (status != 0) {
        goto cleanup;
    }

    Lab65_Private_Release(&state->private_unknown);
    bus->pVTbl->Release(bus);
    return 0;

cleanup:
    if (state) {
        Lab65_Private_Release(&state->private_unknown);
    }
    if (owned_allocator) {
        owned_allocator->pVTbl->Release(owned_allocator);
    }
    if (owned_system) {
        owned_system->pVTbl->Release(owned_system);
    }
    if (bus) {
        bus->pVTbl->Release(bus);
    }
    return status;
}

typedef struct Lab2State {
    IEcoLab2 public_lab2;
    uint32_t local_refs;
    IEcoSystem1* owned_system;
    IEcoMemoryAllocator1* owned_allocator;
    IEcoCalculatorX* owned_inner_x;
    IEcoCalculatorY* owned_inner_y;
    IEcoUnknown* owned_inner_unknown;
} Lab2State;

static int16_t ECOCALLMETHOD Lab2_QueryInterface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv) {
        *ppv = NULL;
    }
    if (!me || !riid || !ppv) {
        return ERR_ECO_POINTER;
    }
    {
        Lab2State* state = (Lab2State*)me;
        if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
            *ppv = (voidptr_t)&state->public_lab2;
            state->local_refs++;
            return 0;
        }
        if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
            *ppv = (voidptr_t)&state->public_lab2;
            state->local_refs++;
            return 0;
        }
        if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
            *ppv = (voidptr_t)&state->public_lab2;
            state->local_refs++;
            return 0;
        }
        if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
            *ppv = (voidptr_t)&state->public_lab2;
            state->local_refs++;
            return 0;
        }
        if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
            return state->owned_inner_unknown->pVTbl->QueryInterface(state->owned_inner_unknown, riid, ppv);
        }
        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t ECOCALLMETHOD Lab2_AddRef(IEcoLab2Ptr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab2State* state = (Lab2State*)me;
        state->local_refs++;
        return state->local_refs;
    }
}

static uint32_t ECOCALLMETHOD Lab2_Release(IEcoLab2Ptr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab2State* state = (Lab2State*)me;
        state->local_refs--;
        if (state->local_refs == 0) {
            IEcoMemoryAllocator1* allocator = state->owned_allocator;
            if (state->owned_inner_x) {
                state->owned_inner_x->pVTbl->Release(state->owned_inner_x);
            }
            if (state->owned_inner_y) {
                state->owned_inner_y->pVTbl->Release(state->owned_inner_y);
            }
            if (state->owned_inner_unknown) {
                state->owned_inner_unknown->pVTbl->Release(state->owned_inner_unknown);
            }
            if (state->owned_system) {
                state->owned_system->pVTbl->Release(state->owned_system);
            }
            allocator->pVTbl->Free(allocator, state);
            allocator->pVTbl->Release(allocator);
            return 0;
        }
        return state->local_refs;
    }
}

static uint32_t ECOCALLMETHOD Lab2_GetVersion(IEcoLab2Ptr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab2State* state = (Lab2State*)me;
        return 2;
    }
}

static int16_t ECOCALLMETHOD Lab2_PublicX_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv) {
        *ppv = NULL;
    }
    if (!me || !riid || !ppv) {
        return ERR_ECO_POINTER;
    }
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        return Lab2_QueryInterface(&state->public_lab2, riid, ppv);
    }
}

static uint32_t ECOCALLMETHOD Lab2_PublicX_AddRef(IEcoCalculatorXPtr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        return Lab2_AddRef(&state->public_lab2);
    }
}

static uint32_t ECOCALLMETHOD Lab2_PublicX_Release(IEcoCalculatorXPtr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        return Lab2_Release(&state->public_lab2);
    }
}

static int32_t ECOCALLMETHOD Lab2_PublicX_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (!me) {
        return 0;
    }
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        return state->owned_inner_x->pVTbl->Addition(state->owned_inner_x, a, b);
    }
}

static int16_t ECOCALLMETHOD Lab2_PublicX_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    if (!me) {
        return 0;
    }
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        return state->owned_inner_x->pVTbl->Subtraction(state->owned_inner_x, a, b);
    }
}

static int16_t ECOCALLMETHOD Lab2_PublicY_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (ppv) {
        *ppv = NULL;
    }
    if (!me || !riid || !ppv) {
        return ERR_ECO_POINTER;
    }
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        return Lab2_QueryInterface(&state->public_lab2, riid, ppv);
    }
}

static uint32_t ECOCALLMETHOD Lab2_PublicY_AddRef(IEcoCalculatorYPtr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        return Lab2_AddRef(&state->public_lab2);
    }
}

static uint32_t ECOCALLMETHOD Lab2_PublicY_Release(IEcoCalculatorYPtr_t me) {
    if (!me) {
        return 0;
    }
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        return Lab2_Release(&state->public_lab2);
    }
}

static int32_t ECOCALLMETHOD Lab2_PublicY_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (!me) {
        return 0;
    }
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        return state->owned_inner_y->pVTbl->Multiplication(state->owned_inner_y, a, b);
    }
}

static int16_t ECOCALLMETHOD Lab2_PublicY_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    if (!me) {
        return 0;
    }
    {
        Lab2State* state = (Lab2State*)((char*)me - offsetof(Lab2State, public_lab2));
        return state->owned_inner_y->pVTbl->Division(state->owned_inner_y, a, b);
    }
}

static IEcoLab2VTbl public_lab2_vtbl = {
    Lab2_QueryInterface,
    Lab2_AddRef,
    Lab2_Release,
    Lab2_GetVersion
};

static IEcoCalculatorXVTbl public_x_vtbl = {
    Lab2_PublicX_QueryInterface,
    Lab2_PublicX_AddRef,
    Lab2_PublicX_Release,
    Lab2_PublicX_Addition,
    Lab2_PublicX_Subtraction
};

static IEcoCalculatorYVTbl public_y_vtbl = {
    Lab2_PublicY_QueryInterface,
    Lab2_PublicY_AddRef,
    Lab2_PublicY_Release,
    Lab2_PublicY_Multiplication,
    Lab2_PublicY_Division
};

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    int16_t status = 0;
    IEcoSystem1* owned_system = NULL;
    IEcoInterfaceBus1* bus = NULL;
    IEcoMemoryAllocator1* owned_allocator = NULL;
    Lab2State* state = NULL;
    IEcoInterfaceBus1MemExt* mem_ext = NULL;
    UGUID manager_id = CID_EcoMemoryManager1;

    if (out) {
        *out = NULL;
    }
    if (!out || !iid) {
        return ERR_ECO_POINTER;
    }

    if (outer) {
        return ERR_ECO_NOAGGREGATION;
    }

    if (!system) {
        return ERR_ECO_NOSYSTEM;
    }

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
    if (status != 0) {
        return status;
    }

    status = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != 0) {
        goto cleanup;
    }

    status = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&mem_ext);
    if (status == 0) {
        const UGUID* manager = mem_ext->pVTbl->get_Manager(mem_ext);
        if (manager) {
            manager_id = *manager;
        }
        mem_ext->pVTbl->Release(mem_ext);
    }

    status = bus->pVTbl->QueryComponent(bus, &manager_id, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
    if (status != 0) {
        goto cleanup;
    }

    state = (Lab2State*)owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab2State));
    if (!state) {
        status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

    state->public_lab2.pVTbl = &public_lab2_vtbl;
    state->local_refs = 1;
    state->owned_system = owned_system;
    state->owned_allocator = owned_allocator;
    state->owned_inner_x = NULL;
    state->owned_inner_y = NULL;
    state->owned_inner_unknown = NULL;

    owned_system = NULL;
    owned_allocator = NULL;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorE, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&state->owned_inner_y);
    if (status != 0) {
        goto cleanup;
    }

    status = state->owned_inner_y->pVTbl->QueryInterface(state->owned_inner_y, &IID_IEcoCalculatorX, (voidptr_t*)&state->owned_inner_x);
    if (status != 0) {
        goto cleanup;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoLab65, &state->public_lab2, &IID_IEcoUnknown, (voidptr_t*)&state->owned_inner_unknown);
    if (status != 0) {
        goto cleanup;
    }

    status = Lab2_QueryInterface(&state->public_lab2, iid, out);
    if (status != 0) {
        goto cleanup;
    }

    Lab2_Release(&state->public_lab2);
    bus->pVTbl->Release(bus);
    return 0;

cleanup:
    if (state) {
        Lab2_Release(&state->public_lab2);
    }
    if (owned_allocator) {
        owned_allocator->pVTbl->Release(owned_allocator);
    }
    if (owned_system) {
        owned_system->pVTbl->Release(owned_system);
    }
    if (bus) {
        bus->pVTbl->Release(bus);
    }
    return status;
}
