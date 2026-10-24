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
    IEcoUnknown* owner = NULL;

    if (ppv == NULL) return ERR_ECO_POINTER;
    *ppv = NULL;
    if (me == NULL || riid == NULL) return ERR_ECO_POINTER;

    owner = (IEcoUnknown*) ((char_t*)me - offsetof(Lab65State, public_lab65));
    if (owner == NULL) return ERR_ECO_POINTER;

    return owner->pVTbl->QueryInterface(owner, riid, ppv);
}

static int16_t ECOCALLMETHOD Unknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, private_unknown));

    if (ppv == NULL) return ERR_ECO_POINTER;
    *ppv = NULL;
    if (me == NULL || riid == NULL) return ERR_ECO_POINTER;

    if (IID_IEcoUnknown == *riid) {
        state->local_refs++;
        *ppv = me;
        return ERR_ECO_SUCCESS;
    }

    if (IID_IEcoLab65 == *riid) {
        int16_t hr = state->public_lab65.pVTbl->AddRef(&state->public_lab65);
        if (hr != ERR_ECO_SUCCESS) return hr;
        *ppv = &state->public_lab65;
        return ERR_ECO_SUCCESS;
    }

    return ERR_ECO_NOINTERFACE;
}

IEcoMemoryAllocator1* ECOCALLMETHOD lab_allocator(IEcoLab65* me) {
    Lab65State* state = (Lab65State*) ((char_t*)me - offsetof(Lab65State, public_lab65));

    if (me == NULL) return NULL;

    return state->owned_allocator;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    IEcoSystem1* owned_system = NULL;
    IEcoInterfaceBus1* bus = NULL;
    IEcoInterfaceBus1MemExt* ext = NULL;
    IEcoMemoryAllocator1* owned_allocator = NULL;
    Lab65State* state = NULL;
    UGUID manager = {0};
    const UGUID* temp_manager = NULL;
    int16_t hr = ERR_ECO_SUCCESS;

    if (out != NULL) *out = NULL;
    if (system == NULL || iid == NULL || out == NULL) return ERR_ECO_POINTER;
    if (outer != NULL && !IsEqualGUID(iid, &IID_IEcoUnknown)) return ERR_ECO_NOAGGREGATION;

    hr = system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (void**)&owned_system);
    if (hr != ERR_ECO_SUCCESS) goto cleanup;

    hr = owned_system->pVTbl->QueryInterface(owned_system, &IID_IEcoInterfaceBus1, (void**)&bus);
    if (hr != ERR_ECO_SUCCESS) goto cleanup;

    hr = bus->pVTbl->QueryComponent(bus, &CID_EcoMemoryManager1, NULL, &IID_IEcoMemoryAllocator1, (void**)&owned_allocator);
    if (hr != ERR_ECO_SUCCESS) goto cleanup;

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

    hr = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (void**)&state->inner_x);
    if (hr != ERR_ECO_SUCCESS) goto cleanup;

    hr = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (void**)&state->inner_y);
    if (hr != ERR_ECO_SUCCESS) goto cleanup;

    owned_system = NULL;
    bus = NULL;
    owned_allocator = NULL;

    if (outer != NULL) {
        *out = &state->private_unknown;
    } else {
        hr = state->private_unknown.pVTbl->QueryInterface(&state->private_unknown, iid, out);
        if (hr != ERR_ECO_SUCCESS) goto cleanup;
    }

    return ERR_ECO_SUCCESS;

cleanup:
    if (state != NULL) {
        state->private_unknown.pVTbl->Release(&state->private_unknown);
    }
    if (owned_system != NULL) owned_system->pVTbl->Release(owned_system);
    if (bus != NULL) bus->pVTbl->Release(bus);
    if (owned_allocator != NULL) owned_allocator->pVTbl->Release(owned_allocator);
    return hr;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (out != NULL) *out = NULL;
    if (system == NULL || iid == NULL || out == NULL) return ERR_ECO_POINTER;
    if (outer != NULL) return ERR_ECO_NOAGGREGATION;
    return ERR_ECO_NOINTERFACE;
}
