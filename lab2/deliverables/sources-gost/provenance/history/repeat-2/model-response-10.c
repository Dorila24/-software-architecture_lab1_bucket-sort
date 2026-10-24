#include "ReuseContract.h"

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me);
int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out);
int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out);

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

static int16_t ECOCALLMETHOD Lab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pCMe;
    if (ppv != NULL) *ppv = NULL;
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (pCMe->controller == NULL) return ERR_ECO_NOINTERFACE;
    return pCMe->controller->pVTbl->QueryInterface(pCMe->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65_AddRef(IEcoLab65Ptr_t me) {
    Lab65State* pCMe;
    if (me == NULL) return 0;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (pCMe->controller == NULL) return 0;
    return pCMe->controller->pVTbl->AddRef(pCMe->controller);
}

static uint32_t ECOCALLMETHOD Lab65_Release(IEcoLab65Ptr_t me) {
    Lab65State* pCMe;
    if (me == NULL) return 0;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (pCMe->controller == NULL) return 0;
    return pCMe->controller->pVTbl->Release(pCMe->controller);
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

static int16_t ECOCALLMETHOD Lab65X_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pCMe;
    if (ppv != NULL) *ppv = NULL;
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (pCMe->controller == NULL) return ERR_ECO_NOINTERFACE;
    return pCMe->controller->pVTbl->QueryInterface(pCMe->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65X_AddRef(IEcoCalculatorXPtr_t me) {
    Lab65State* pCMe;
    if (me == NULL) return 0;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (pCMe->controller == NULL) return 0;
    return pCMe->controller->pVTbl->AddRef(pCMe->controller);
}

static uint32_t ECOCALLMETHOD Lab65X_Release(IEcoCalculatorXPtr_t me) {
    Lab65State* pCMe;
    if (me == NULL) return 0;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (pCMe->controller == NULL) return 0;
    return pCMe->controller->pVTbl->Release(pCMe->controller);
}

static int32_t ECOCALLMETHOD Lab65X_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* pCMe;
    if (me == NULL) return 0;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (pCMe->inner_x == NULL) return 0;
    return pCMe->inner_x->pVTbl->Addition(pCMe->inner_x, a, b);
}

static int16_t ECOCALLMETHOD Lab65X_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    Lab65State* pCMe;
    if (me == NULL) return 0;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_x));
    if (pCMe->inner_x == NULL) return 0;
    return pCMe->inner_x->pVTbl->Subtraction(pCMe->inner_x, a, b);
}

static int16_t ECOCALLMETHOD Lab65Y_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pCMe;
    if (ppv != NULL) *ppv = NULL;
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (pCMe->controller == NULL) return ERR_ECO_NOINTERFACE;
    return pCMe->controller->pVTbl->QueryInterface(pCMe->controller, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65Y_AddRef(IEcoCalculatorYPtr_t me) {
    Lab65State* pCMe;
    if (me == NULL) return 0;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (pCMe->controller == NULL) return 0;
    return pCMe->controller->pVTbl->AddRef(pCMe->controller);
}

static uint32_t ECOCALLMETHOD Lab65Y_Release(IEcoCalculatorYPtr_t me) {
    Lab65State* pCMe;
    if (me == NULL) return 0;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (pCMe->controller == NULL) return 0;
    return pCMe->controller->pVTbl->Release(pCMe->controller);
}

static int32_t ECOCALLMETHOD Lab65Y_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* pCMe;
    if (me == NULL) return 0;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (pCMe->inner_y == NULL) return 0;
    return pCMe->inner_y->pVTbl->Multiplication(pCMe->inner_y, a, b);
}

static int16_t ECOCALLMETHOD Lab65Y_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    Lab65State* pCMe;
    if (me == NULL) return 0;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_y));
    if (pCMe->inner_y == NULL) return 0;
    return pCMe->inner_y->pVTbl->Division(pCMe->inner_y, a, b);
}

static int16_t ECOCALLMETHOD Lab65Private_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* pCMe;
    if (ppv != NULL) *ppv = NULL;
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->private_unknown;
        pCMe->local_refs++;
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &pCMe->public_lab65;
        pCMe->public_lab65.pVTbl->AddRef(&pCMe->public_lab65);
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->public_x;
        pCMe->public_x.pVTbl->AddRef(&pCMe->public_x);
        return ERR_ECO_SUCCESS;
    }
    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->public_y;
        pCMe->public_y.pVTbl->AddRef(&pCMe->public_y);
        return ERR_ECO_SUCCESS;
    }
    return ERR_ECO_NOINTERFACE;
}

static uint32_t ECOCALLMETHOD Lab65Private_AddRef(IEcoUnknownPtr_t me) {
    Lab65State* pCMe;
    if (me == NULL) return 0;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    pCMe->local_refs++;
    return pCMe->local_refs;
}

static uint32_t ECOCALLMETHOD Lab65Private_Release(IEcoUnknownPtr_t me) {
    Lab65State* pCMe;
    if (me == NULL) return 0;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, private_unknown));
    if (pCMe->local_refs > 0) pCMe->local_refs--;
    if (pCMe->local_refs == 0) {
        IEcoMemoryAllocator1* allocator = pCMe->owned_allocator;
        if (pCMe->inner_x) pCMe->inner_x->pVTbl->Release(pCMe->inner_x);
        if (pCMe->inner_y) pCMe->inner_y->pVTbl->Release(pCMe->inner_y);
        if (pCMe->owned_system) pCMe->owned_system->pVTbl->Release(pCMe->owned_system);
        allocator->pVTbl->Free(allocator, pCMe);
        allocator->pVTbl->Release(allocator);
        return 0;
    }
    return pCMe->local_refs;
}

static IEcoLab65VTbl lab65_vtbl = {
    Lab65_QueryInterface,
    Lab65_AddRef,
    Lab65_Release,
    Lab65_SortInt,
    Lab65_SortLong,
    Lab65_SortFloat,
    Lab65_SortDouble,
    Lab65_SortLongDouble
};

static IEcoCalculatorXVTbl lab65x_vtbl = {
    Lab65X_QueryInterface,
    Lab65X_AddRef,
    Lab65X_Release,
    Lab65X_Addition,
    Lab65X_Subtraction
};

static IEcoCalculatorYVTbl lab65y_vtbl = {
    Lab65Y_QueryInterface,
    Lab65Y_AddRef,
    Lab65Y_Release,
    Lab65Y_Multiplication,
    Lab65Y_Division
};

static IEcoUnknownVTbl lab65_private_vtbl = {
    Lab65Private_QueryInterface,
    Lab65Private_AddRef,
    Lab65Private_Release
};

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    Lab65State* pCMe;
    if (me == NULL) return NULL;
    pCMe = (Lab65State*)((char*)me - offsetof(Lab65State, public_lab65));
    if (pCMe->owned_allocator == NULL) return NULL;
    return pCMe->owned_allocator;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    IEcoSystem1* owned_system = NULL;
    IEcoInterfaceBus1* bus = NULL;
    IEcoInterfaceBus1MemExt* extension = NULL;
    IEcoMemoryAllocator1* owned_allocator = NULL;
    Lab65State* state = NULL;
    UGUID manager_guid = CID_EcoMemoryManager1;
    const UGUID* temp_manager = NULL;
    int16_t status = ERR_ECO_SUCCESS;

    if (out != NULL) *out = NULL;

    if (system == NULL || iid == NULL || out == NULL) {
        status = ERR_ECO_POINTER;
        goto cleanup;
    }

    if (outer != NULL && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        status = ERR_ECO_NOAGGREGATION;
        goto cleanup;
    }

    status = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (voidptr_t*)&owned_system);
    if (status != ERR_ECO_SUCCESS) {
        status = ERR_ECO_NOSYSTEM;
        goto cleanup;
    }

    status = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (voidptr_t*)&bus);
    if (status != ERR_ECO_SUCCESS) {
        status = ERR_ECO_NOBUS;
        goto cleanup;
    }

    status = bus->pVTbl->QueryInterface(bus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&extension);
    if (status == ERR_ECO_SUCCESS) {
        temp_manager = extension->pVTbl->get_Manager(extension);
        if (temp_manager) {
            manager_guid = *temp_manager;
        }
        extension->pVTbl->Release(extension);
    }

    status = bus->pVTbl->QueryComponent(bus, &manager_guid, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&owned_allocator);
    if (status != ERR_ECO_SUCCESS) {
        status = ERR_ECO_GET_MEMORY_ALLOCATOR;
        goto cleanup;
    }

    state = (Lab65State*)owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab65State));
    if (state == NULL) {
        status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

    state->public_lab65.pVTbl = &lab65_vtbl;
    state->private_unknown.pVTbl = &lab65_private_vtbl;
    state->public_x.pVTbl = &lab65x_vtbl;
    state->public_y.pVTbl = &lab65y_vtbl;
    state->local_refs = 1;
    state->controller = outer ? outer : (IEcoUnknown*)&state->private_unknown;
    state->owned_system = owned_system;
    state->owned_allocator = owned_allocator;
    state->inner_x = NULL;
    state->inner_y = NULL;

    owned_system = NULL;
    owned_allocator = NULL;

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
    if (status != ERR_ECO_SUCCESS || state->inner_y == NULL) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
    if (status != ERR_ECO_SUCCESS || state->inner_x == NULL) {
        status = ERR_ECO_NOINTERFACE;
        goto cleanup;
    }

    if (outer) {
        *out = &state->private_unknown;
        goto cleanup;
    }

    status = Lab65Private_QueryInterface((IEcoUnknown*)&state->private_unknown, iid, out);
    if (status != ERR_ECO_SUCCESS) {
        Lab65Private_Release((IEcoUnknown*)&state->private_unknown);
    }

cleanup:
    if (status != ERR_ECO_SUCCESS && state) {
        Lab65Private_Release((IEcoUnknown*)&state->private_unknown);
    }

    if (owned_system) owned_system->pVTbl->Release(owned_system);
    if (owned_allocator) owned_allocator->pVTbl->Release(owned_allocator); /* This is an ANSI C89 block comment. */
    if (bus) bus->pVTbl->Release(bus);

    return status;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out == NULL) return ERR_ECO_POINTER;
    *out = NULL;
    if (system == NULL || iid == NULL) return ERR_ECO_POINTER;
    if (outer != NULL) return ERR_ECO_NOAGGREGATION;
    return ERR_ECO_NOINTERFACE;
}
