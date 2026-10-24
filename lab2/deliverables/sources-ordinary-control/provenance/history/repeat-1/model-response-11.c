#include "ReuseContract.h"
#include "IdEcoLab65.h"

typedef struct {
    IEcoLab65 public_lab65;
    IEcoUnknown private_unknown;
    uint32_t local_refs;
    IEcoUnknown* controller; /* borrowed controller, do not release */
    IEcoSystem1* owned_system;
    IEcoMemoryAllocator1* owned_allocator;
    IEcoCalculatorX* inner_x;
    IEcoCalculatorY* inner_y;
    IEcoCalculatorX public_x;
    IEcoCalculatorY public_y;
} Lab65State;

/* Private local lifetime: The private unknown is the nondelegating unknown for Lab65.
 * It manages the lifetime of the entire object and holds strong references to inner components.
 * The public interfaces (Lab65, X, Y) delegate their AddRef/Release to the borrowed controller.
 * The private unknown's Release method handles cleanup of all owned resources.
 */

static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv != 0) {
        *ppv = 0;
    }
    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    state = (Lab65State*)me;
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoLab65Ptr_t me) {
    Lab65State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab65State*)me;
    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoLab65Ptr_t me) {
    Lab65State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab65State*)me;
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

static int16_t ECOCALLMETHOD Private_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv != 0) {
        *ppv = 0;
    }
    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->private_unknown;
        state->private_unknown.pVTbl->AddRef(&state->private_unknown);
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &state->public_lab65;
        state->public_lab65.pVTbl->AddRef(&state->public_lab65);
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->public_x;
        state->public_x.pVTbl->AddRef(&state->public_x);
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->public_y;
        state->public_y.pVTbl->AddRef(&state->public_y);
        return 0;
    }
    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD Private_AddRef(IEcoUnknownPtr_t me) {
    Lab65State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    state->local_refs++;
    return state->local_refs;
}

static uint32_t ECOCALLMETHOD Private_Release(IEcoUnknownPtr_t me) {
    Lab65State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    state->local_refs--;
    if (state->local_refs == 0) {
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
        return 0;
    }
    return state->local_refs;
}

static IEcoUnknownVTbl private_unknown_vtbl = {
    Private_QueryInterface,
    Private_AddRef,
    Private_Release
};

static int16_t ECOCALLMETHOD X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv != 0) {
        *ppv = 0;
    }
    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD X_AddRef(IEcoCalculatorXPtr_t me) {
    Lab65State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD X_Release(IEcoCalculatorXPtr_t me) {
    Lab65State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    return state->controller->pVTbl->Release(state->controller);
}

static int32_t ECOCALLMETHOD X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (state->inner_x == 0) {
        return 0;
    }
    return state->inner_x->pVTbl->Addition(state->inner_x, a, b);
}

static int16_t ECOCALLMETHOD X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (state->inner_x == 0) {
        return 0;
    }
    return state->inner_x->pVTbl->Subtraction(state->inner_x, a, b);
}

static IEcoCalculatorXVTbl public_x_vtbl = {
    X_QueryInterface,
    X_AddRef,
    X_Release,
    X_Addition,
    X_Subtraction
};

static int16_t ECOCALLMETHOD Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state;
    if (ppv != 0) {
        *ppv = 0;
    }
    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    return state->controller->pVTbl->QueryInterface(state->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Y_AddRef(IEcoCalculatorYPtr_t me) {
    Lab65State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    return state->controller->pVTbl->AddRef(state->controller);
}

static uint32_t ECOCALLMETHOD Y_Release(IEcoCalculatorYPtr_t me) {
    Lab65State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    return state->controller->pVTbl->Release(state->controller);
}

static int32_t ECOCALLMETHOD Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (state->inner_y == 0) {
        return 0;
    }
    return state->inner_y->pVTbl->Multiplication(state->inner_y, a, b);
}

static int16_t ECOCALLMETHOD Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (state->inner_y == 0) {
        return 0;
    }
    return state->inner_y->pVTbl->Division(state->inner_y, a, b);
}

static IEcoCalculatorYVTbl public_y_vtbl = {
    Y_QueryInterface,
    Y_AddRef,
    Y_Release,
    Y_Multiplication,
    Y_Division
};

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    if (me == 0) {
        return 0;
    }
    {
        Lab65State* state = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
        return state->owned_allocator;
    }
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    int16_t status = 0;
    IEcoSystem1* owned_system = 0;
    IEcoInterfaceBus1* bus = 0;
    IEcoMemoryAllocator1* owned_allocator = 0;
    IEcoInterfaceBus1MemExt* mem_ext = 0;
    Lab65State* state = 0;
    UGUID manager_id = CID_EcoMemoryManager1;
    const UGUID* manager_ptr = 0;

    if (out != 0) {
        *out = 0;
    }
    if (system == 0 || iid == 0 || out == 0) {
        return ERR_ECO_POINTER;
    }
    if (outer != 0 && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        return ERR_ECO_NOAGGREGATION;
    }

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
    if (status != 0) {
        return status;
    }

    status = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != 0) {
        owned_system->pVTbl->Release(owned_system);
        return status;
    }

    status = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&mem_ext);
    if (status == 0) {
        manager_ptr = mem_ext->pVTbl->get_Manager(mem_ext);
        if (manager_ptr != 0) {
            manager_id = *manager_ptr;
        }
        mem_ext->pVTbl->Release(mem_ext);
    }

    status = bus->pVTbl->QueryComponent(bus, &manager_id, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
    if (status != 0) {
        bus->pVTbl->Release(bus);
        owned_system->pVTbl->Release(owned_system);
        return status;
    }

    state = (Lab65State*)owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab65State));
    if (state == 0) {
        owned_allocator->pVTbl->Release(owned_allocator);
        bus->pVTbl->Release(bus);
        owned_system->pVTbl->Release(owned_system);
        return ERR_ECO_OUTOFMEMORY;
    }

    state->public_lab65.pVTbl = &public_lab65_vtbl;
    state->private_unknown.pVTbl = &private_unknown_vtbl;
    state->public_x.pVTbl = &public_x_vtbl;
    state->public_y.pVTbl = &public_y_vtbl;
    state->local_refs = 1;
    state->controller = (outer != 0) ? outer : &state->private_unknown;
    state->owned_system = owned_system;
    state->owned_allocator = owned_allocator;
    state->inner_x = 0;
    state->inner_y = 0;

    owned_system = 0;
    owned_allocator = 0;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
    if (status != 0) {
        goto cleanup;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
    if (status != 0) {
        goto cleanup;
    }

    bus->pVTbl->Release(bus);
    bus = 0;

    if (outer != 0) {
        *out = &state->private_unknown;
        return 0;
    }

    status = Private_QueryInterface(&state->private_unknown, iid, out);
    if (status != 0) {
        goto cleanup;
    }

    Private_Release(&state->private_unknown);
    return status;

cleanup:
    if (state != 0) {
        Private_Release(&state->private_unknown);
    }
    if (owned_allocator != 0) {
        owned_allocator->pVTbl->Release(owned_allocator);
    }
    if (owned_system != 0) {
        owned_system->pVTbl->Release(owned_system);
    }
    if (bus != 0) {
        bus->pVTbl->Release(bus);
    }
    return status;
}

typedef struct {
    IEcoLab2 public_lab2;
    IEcoCalculatorX public_x;
    IEcoCalculatorY public_y;
    uint32_t local_refs;
    IEcoSystem1* owned_system;
    IEcoMemoryAllocator1* owned_allocator;
    IEcoCalculatorX* owned_inner_x;
    IEcoCalculatorY* owned_inner_y;
    IEcoUnknown* owned_inner_unknown;
} Lab2State;

/* Lab2 owns two interfaces of the SAME contained E and retains only the aggregate PRIVATE Unknown to avoid a controller cycle.
 * Lab2 has NO embedded private unknown; its primary is canonical public Unknown.
 */

static int16_t ECOCALLMETHOD Lab2_QueryInterface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv != 0) {
        *ppv = 0;
    }
    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    state = (Lab2State*)me;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->public_lab2;
        state->local_refs++;
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = &state->public_lab2;
        state->local_refs++;
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->public_x;
        state->local_refs++;
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->public_y;
        state->local_refs++;
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        return state->owned_inner_unknown->pVTbl->QueryInterface(state->owned_inner_unknown, riid, ppv);
    }
    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD Lab2_AddRef(IEcoLab2Ptr_t me) {
    Lab2State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab2State*)me;
    state->local_refs++;
    return state->local_refs;
}

static uint32_t ECOCALLMETHOD Lab2_Release(IEcoLab2Ptr_t me) {
    Lab2State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab2State*)me;
    state->local_refs--;
    if (state->local_refs == 0) {
        IEcoMemoryAllocator1* allocator = state->owned_allocator;
        if (state->owned_inner_x != 0) {
            state->owned_inner_x->pVTbl->Release(state->owned_inner_x);
        }
        if (state->owned_inner_y != 0) {
            state->owned_inner_y->pVTbl->Release(state->owned_inner_y);
        }
        if (state->owned_inner_unknown != 0) {
            state->owned_inner_unknown->pVTbl->Release(state->owned_inner_unknown);
        }
        if (state->owned_system != 0) {
            state->owned_system->pVTbl->Release(state->owned_system);
        }
        allocator->pVTbl->Free(allocator, state);
        allocator->pVTbl->Release(allocator);
        return 0;
    }
    return state->local_refs;
}

static uint32_t ECOCALLMETHOD Lab2_GetVersion(IEcoLab2Ptr_t me) {
    if (me == 0) {
        return 0;
    }
    return 2;
}

static IEcoLab2VTbl lab2_primary_vtbl = {
    Lab2_QueryInterface,
    Lab2_AddRef,
    Lab2_Release,
    Lab2_GetVersion
};

static int16_t ECOCALLMETHOD Lab2X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv != 0) {
        *ppv = 0;
    }
    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_x));
    return Lab2_QueryInterface(&state->public_lab2, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab2X_AddRef(IEcoCalculatorXPtr_t me) {
    Lab2State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab2State*)((char*)me - offsetof(Lab65State, public_x));
    return Lab2_AddRef(&state->public_lab2);
}

static uint32_t ECOCALLMETHOD Lab2X_Release(IEcoCalculatorXPtr_t me) {
    Lab2State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab2State*)((char*)me - offsetof(Lab65State, public_x));
    return Lab2_Release(&state->public_lab2);
}

static IEcoCalculatorXVTbl lab2_x_vtbl = {
    Lab2X_QueryInterface,
    Lab2X_AddRef,
    Lab2X_Release,
    Lab2X_Addition,
    Lab2X_Subtraction
};

static int16_t ECOCALLMETHOD Lab2Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv != 0) {
        *ppv = 0;
    }
    if (me == 0 || riid == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }
    state = (Lab2State*)((char*)me - offsetof(Lab2State, public_y));
    return Lab2_QueryInterface(&state->public_lab2, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab2Y_AddRef(IEcoCalculatorYPtr_t me) {
    Lab2State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab2State*)((char*)me - offsetof(Lab65State, public_y));
    return Lab2_AddRef(&state->public_lab2);
}

static uint32_t ECOCALLMETHOD Lab2Y_Release(IEcoCalculatorYPtr_t me) {
    Lab2State* state;
    if (me == 0) {
        return 0;
    }
    state = (Lab2State*)((char*)me - offsetof(Lab65State, public_y));
    return Lab2_Release(&state->public_lab2);
}

static IEcoCalculatorYVTbl lab2_y_vtbl = {
    Lab2Y_QueryInterface,
    Lab2Y_AddRef,
    Lab2Y_Release,
    Lab2Y_Multiplication,
    Lab2Y_Division
};

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    int16_t status = 0;
    IEcoSystem1* owned_system = 0;
    IEcoInterfaceBus1* bus = 0;
    IEcoMemoryAllocator1* owned_allocator = 0;
    IEcoInterfaceBus1MemExt* mem_ext = 0;
    Lab2State* state = 0;
    UGUID manager_id = CID_EcoMemoryManager1;
    const UGUID* manager_ptr = 0;

    if (out != 0) {
        *out = 0;
    }
    if (system == 0 || iid == 0 || out == 0) {
        return ERR_ECO_POINTER;
    }
    if (outer != 0) {
        return ERR_ECO_NOAGGREGATION;
    }

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
    if (status != 0) {
        return status;
    }

    status = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != 0) {
        owned_system->pVTbl->Release(owned_system);
        return status;
    }

    status = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&mem_ext);
    if (status == 0) {
        manager_ptr = mem_ext->pVTbl->get_Manager(mem_ext);
        if (manager_ptr != 0) {
            manager_id = *manager_ptr;
        }
        mem_ext->pVTbl->Release(mem_ext);
    }

    status = bus->pVTbl->QueryComponent(bus, &manager_id, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
    if (status != 0) {
        bus->pVTbl->Release(bus);
        owned_system->pVTbl->Release(owned_system);
        return status;
    }

    state = (Lab2State*)owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab2State));
    if (state == 0) {
        owned_allocator->pVTbl->Release(owned_allocator);
        bus->pVTbl->Release(bus);
        owned_system->pVTbl->Release(owned_system);
        return ERR_ECO_OUTOFMEMORY;
    }

    state->public_lab2.pVTbl = &lab2_primary_vtbl;
    state->public_x.pVTbl = &lab2_x_vtbl;
    state->public_y.pVTbl = &lab2_y_vtbl;
    state->local_refs = 1;
    state->owned_system = owned_system;
    state->owned_allocator = owned_allocator;
    state->owned_inner_x = 0;
    state->owned_inner_y = 0;
    state->owned_inner_unknown = 0;

    owned_system = 0;
    owned_allocator = 0;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (voidptr_t*)&state->owned_inner_y);
    if (status != 0) {
        goto cleanup;
    }

    status = state->owned_inner_y->pVTbl->QueryInterface(state->owned_inner_y, &IID_IEcoCalculatorX, (voidptr_t*)&state->owned_inner_x);
    if (status != 0) {
        goto cleanup;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoLab65, (IEcoUnknown*)&state->public_lab2, &IID_IEcoUnknown, (voidptr_t*)&state->owned_inner_unknown);
    if (status != 0) {
        goto cleanup;
    }

    bus->pVTbl->Release(bus);
    bus = 0;

    status = Lab2_QueryInterface(&state->public_lab2, iid, out);
    if (status != 0) {
        goto cleanup;
    }

    Lab2_Release(&state->public_lab2);
    return status;

cleanup:
    if (state != 0) {
        Lab2_Release(&state->public_lab2);
    }
    if (owned_allocator != 0) {
        owned_allocator->pVTbl->Release(owned_allocator);
    }
    if (owned_system != 0) {
        owned_system->pVTbl->Release(owned_system);
    }
    if (bus != 0) {
        bus->pVTbl->Release(bus);
    }
    return status;
}
