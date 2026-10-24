#include "ReuseContract.h"
#include "IdEcoLab65.h"

typedef struct {
    IEcoLab2 public_lab2;
    IEcoUnknown private_unknown;
    uint32_t local_refs;
    IEcoSystem1* owned_system;
    IEcoMemoryAllocator1* owned_allocator;
    IEcoCalculatorX* inner_x;
    IEcoCalculatorY* inner_y;
    IEcoLab65* aggregated_lab65;
} Lab2State;

static int16_t ECOCALLMETHOD Lab2_QueryInterface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2State* state;
    if (ppv != NULL) *ppv = NULL;
    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    state = (Lab2State*) ((char_t*)me - offsetof(Lab2State, public_lab2));
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &state->private_unknown;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = &state->public_lab2;
        state->local_refs++;
        return ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &state->inner_x;
        state->inner_x->pVTbl->AddRef(state->inner_x);
        return ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &state->inner_y;
        state->inner_y->pVTbl->AddRef(state->inner_y);
        return ERR_ECO_SUCCESS;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        return state->aggregated_lab65->pVTbl->QueryInterface(state->aggregated_lab65, riid, ppv);
    } else {
        return ERR_ECO_NOINTERFACE;
    }
}

static uint32_t ECOCALLMETHOD Lab2_AddRef(IEcoLab2Ptr_t me) {
    Lab2State* state;
    if (me == NULL) return 0;
    state = (Lab2State*) ((char_t*)me - offsetof(Lab2State, public_lab2));
    return ++state->local_refs;
}

static uint32_t ECOCALLMETHOD Lab2_Release(IEcoLab2Ptr_t me) {
    Lab2State* state;
    if (me == NULL) return 0;
    state = (Lab2State*) ((char_t*)me - offsetof(Lab2State, public_lab2));
    if (--state->local_refs == 0) {
        IEcoMemoryAllocator1* allocator = state->owned_allocator;
        IEcoCalculatorX* inner_x = state->inner_x;
        IEcoCalculatorY* inner_y = state->inner_y;
        IEcoLab65* aggregated_lab65 = state->aggregated_lab65;

        if (inner_x != NULL) inner_x->pVTbl->Release(inner_x);
        if (inner_y != NULL) inner_y->pVTbl->Release(inner_y);
        if (aggregated_lab65 != NULL) aggregated_lab65->pVTbl->Release(aggregated_lab65);

        allocator->pVTbl->Free(allocator, state);
        allocator->pVTbl->Release(allocator);
        return 0;
    }
    return state->local_refs;
}

static uint32_t ECOCALLMETHOD Lab2_GetVersion(IEcoLab2Ptr_t me) {
    return 2;
}

static IEcoLab2VTbl Lab2_Vtbl = {
    Lab2_QueryInterface,
    Lab2_AddRef,
    Lab2_Release,
    Lab2_GetVersion
};

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    int16_t creation_status = 0;

    if (out != NULL) *out = NULL;
    if (system == NULL || iid == NULL || out == NULL) return ERR_ECO_POINTER;
    if (outer != NULL) return ERR_ECO_NOAGGREGATION;

    IEcoSystem1* owned_system = NULL;
    IEcoInterfaceBus1* bus = NULL;
    IEcoMemoryAllocator1* owned_allocator = NULL;
    Lab2State* state = NULL;
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

    state = (Lab2State*) owned_allocator->pVTbl->Alloc(owned_allocator, sizeof(Lab2State));
    if (state == NULL) {
        creation_status = ERR_ECO_OUTOFMEMORY;
        goto cleanup;
    }

    state->public_lab2.pVTbl = &Lab2_Vtbl;
    state->private_unknown.pVTbl = &Unknown_Vtbl;
    state->local_refs = 1;
    state->owned_system = owned_system;
    state->owned_allocator = owned_allocator;
    state->inner_x = NULL;
    state->inner_y = NULL;
    state->aggregated_lab65 = NULL;

    creation_status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorE, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&state->inner_y);
    if (creation_status != 0) goto cleanup;

    creation_status = state->inner_y->pVTbl->QueryInterface(state->inner_y, &IID_IEcoCalculatorX, (voidptr_t*)&state->inner_x);
    if (creation_status != 0) goto cleanup;

    creation_status = bus->pVTbl->QueryComponent(bus, &CID_EcoLab65, &state->private_unknown, &IID_IEcoUnknown, (voidptr_t*)&state->aggregated_lab65);
    if (creation_status != 0) goto cleanup;

    if (IsEqualUGUID(iid, &IID_IEcoLab2)) {
        *out = &state->public_lab2;
    } else if (IsEqualUGUID(iid, &IID_IEcoCalculatorX)) {
        *out = state->inner_x;
    } else if (IsEqualUGUID(iid, &IID_IEcoCalculatorY)) {
        *out = state->inner_y;
    } else if (IsEqualUGUID(iid, &IID_IEcoLab65)) {
        creation_status = state->aggregated_lab65->pVTbl->QueryInterface(state->aggregated_lab65, iid, out);
        if (creation_status == 0) {
            state->aggregated_lab65->pVTbl->Release(state->aggregated_lab65);
        }
        goto cleanup;
    } else {
        creation_status = ERR_ECO_NOINTERFACE;
    }

cleanup:
    if (creation_status != 0) {
        if (state != NULL) {
            Lab2_Release(&state->public_lab2);
        }
        if (owned_allocator != NULL) owned_allocator->pVTbl->Release(owned_allocator);
    }
    if (bus != NULL) bus->pVTbl->Release(bus);
    if (owned_system != NULL) owned_system->pVTbl->Release(owned_system);

    return creation_status;
}
