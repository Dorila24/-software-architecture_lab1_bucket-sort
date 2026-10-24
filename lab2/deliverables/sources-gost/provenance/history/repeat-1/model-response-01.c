#include "ReuseContract.h"
#include "SdkAbiReference.md"
#include "IEcoLab65.h"
#include "IdEcoLab65.h"
#include "IEcoLab2.h"
#include "IdEcoLab2.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoMemoryAllocator1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "Vendor/Eco.CalculatorA/SharedFiles/IdEcoCalculatorA.h"
#include "Vendor/Eco.CalculatorB/SharedFiles/IdEcoCalculatorB.h"
#include "Vendor/Eco.CalculatorC/SharedFiles/IdEcoCalculatorC.h"
#include "Vendor/Eco.CalculatorD/SharedFiles/IdEcoCalculatorD.h"
#include "Vendor/Eco.CalculatorE/SharedFiles/IdEcoCalculatorE.h"

typedef struct {
    IEcoUnknownVTbl vtbl;
    uint32_t refCount;
    IEcoSystem1* system;
    IEcoInterfaceBus1* bus;
    IEcoMemoryAllocator1* allocator;
    IEcoCalculatorX* calculatorX;
    IEcoCalculatorY* calculatorY;
} Lab65PrivateUnknown;

typedef struct {
    IEcoLab65VTbl vtbl;
    Lab65PrivateUnknown* privateUnknown;
} Lab65PublicUnknown;

typedef struct {
    IEcoLab2VTbl vtbl;
    uint32_t refCount;
    Lab65PublicUnknown* lab65;
} Lab2PublicUnknown;

static int16_t ECOCALLMETHOD Lab65PrivateUnknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65PrivateUnknown* self = (Lab65PrivateUnknown*)me;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = self;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = &self->privateUnknown->vtbl;
    } else {
        *ppv = NULL;
        return -1; // E_NOINTERFACE
    }
    ((IEcoUnknownPtr_t)*ppv)->pVTbl->AddRef((IEcoUnknownPtr_t)*ppv);
    return 0; // S_OK
}

static uint32_t ECOCALLMETHOD Lab65PrivateUnknown_AddRef(IEcoUnknownPtr_t me) {
    Lab65PrivateUnknown* self = (Lab65PrivateUnknown*)me;
    return ++self->refCount;
}

static uint32_t ECOCALLMETHOD Lab65PrivateUnknown_Release(IEcoUnknownPtr_t me) {
    Lab65PrivateUnknown* self = (Lab65PrivateUnknown*)me;
    if (--self->refCount == 0) {
        if (self->calculatorX) self->calculatorX->pVTbl->Release(self->calculatorX);
        if (self->calculatorY) self->calculatorY->pVTbl->Release(self->calculatorY);
        if (self->allocator) self->allocator->pVTbl->Release(self->allocator);
        if (self->bus) self->bus->pVTbl->Release(self->bus);
        if (self->system) self->system->pVTbl->Release(self->system);
        self->allocator->pVTbl->Free(self->allocator, self);
        return 0;
    }
    return self->refCount;
}

static int16_t ECOCALLMETHOD Lab65PublicUnknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab65PublicUnknown* self = (Lab65PublicUnknown*)me;
    return self->privateUnknown->vtbl.QueryInterface((IEcoUnknownPtr_t)self->privateUnknown, riid, ppv);
}

static uint32_t ECOCALLMETHOD Lab65PublicUnknown_AddRef(IEcoUnknownPtr_t me) {
    Lab65PublicUnknown* self = (Lab65PublicUnknown*)me;
    return self->privateUnknown->vtbl.AddRef((IEcoUnknownPtr_t)self->privateUnknown);
}

static uint32_t ECOCALLMETHOD Lab65PublicUnknown_Release(IEcoUnknownPtr_t me) {
    Lab65PublicUnknown* self = (Lab65PublicUnknown*)me;
    return self->privateUnknown->vtbl.Release((IEcoUnknownPtr_t)self->privateUnknown);
}

static int16_t ECOCALLMETHOD Lab2PublicUnknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    Lab2PublicUnknown* self = (Lab2PublicUnknown*)me;
    if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = self;
    } else if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = self->lab65;
    } else {
        *ppv = NULL;
        return -1; // E_NOINTERFACE
    }
    ((IEcoUnknownPtr_t)*ppv)->pVTbl->AddRef((IEcoUnknownPtr_t)*ppv);
    return 0; // S_OK
}

static uint32_t ECOCALLMETHOD Lab2PublicUnknown_AddRef(IEcoUnknownPtr_t me) {
    Lab2PublicUnknown* self = (Lab2PublicUnknown*)me;
    return ++self->refCount;
}

static uint32_t ECOCALLMETHOD Lab2PublicUnknown_Release(IEcoUnknownPtr_t me) {
    Lab2PublicUnknown* self = (Lab2PublicUnknown*)me;
    if (--self->refCount == 0) {
        self->lab65->vtbl.Release((IEcoUnknownPtr_t)self->lab65);
        self->lab65 = NULL;
        self->allocator->pVTbl->Free(self->allocator, self);
        return 0;
    }
    return self->refCount;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!system || !iid || !out) {
        *out = NULL;
        return -1; // E_POINTER
    }
    if (outer && !IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        *out = NULL;
        return -1; // E_INVALIDARG
    }
    Lab65PrivateUnknown* privateUnknown = (Lab65PrivateUnknown*)malloc(sizeof(Lab65PrivateUnknown));
    if (!privateUnknown) {
        *out = NULL;
        return -2; // E_OUTOFMEMORY
    }
    privateUnknown->vtbl.QueryInterface = Lab65PrivateUnknown_QueryInterface;
    privateUnknown->vtbl.AddRef = Lab65PrivateUnknown_AddRef;
    privateUnknown->vtbl.Release = Lab65PrivateUnknown_Release;
    privateUnknown->refCount = 1;
    privateUnknown->system = NULL;
    privateUnknown->bus = NULL;
    privateUnknown->allocator = NULL;
    privateUnknown->calculatorX = NULL;
    privateUnknown->calculatorY = NULL;

    int16_t status = system->pVTbl->QueryInterface(system, &IID_IEcoSystem1, (void**)&privateUnknown->system);
    if (status != 0) {
        free(privateUnknown);
        *out = NULL;
        return status;
    }
    status = system->pVTbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (void**)&privateUnknown->bus);
    if (status != 0) {
        privateUnknown->system->pVTbl->Release(privateUnknown->system);
        free(privateUnknown);
        *out = NULL;
        return status;
    }
    const UGUID* managerGuid = &IID_IEcoMemoryAllocator1;
    IEcoInterfaceBus1MemExt* memExt = NULL;
    status = privateUnknown->bus->pVTbl->QueryInterface(privateUnknown->bus, &IID_IEcoInterfaceBus1MemExt, (void**)&memExt);
    if (status == 0) {
        const UGUID* managerGuid = memExt->pVTbl->get_Manager(memExt);
        memExt->pVTbl->Release(memExt);
    }
    status = privateUnknown->bus->pVTbl->QueryComponent(privateUnknown->bus, managerGuid, NULL, &IID_IEcoMemoryAllocator1, (void**)&privateUnknown->allocator);
    if (status != 0) {
        privateUnknown->bus->pVTbl->Release(privateUnknown->bus);
        privateUnknown->system->pVTbl->Release(privateUnknown->system);
        free(privateUnknown);
        *out = NULL;
        return status;
    }
    const UGUID* calculatorGuidA = &CID_EcoCalculatorA;
    const UGUID* calculatorGuidD = &CID_EcoCalculatorD;
    status = privateUnknown->bus->pVTbl->QueryComponent(privateUnknown->bus, calculatorGuidA, NULL, &IID_IEcoCalculatorX, (void**)&privateUnknown->calculatorX);
    if (status != 0) {
        privateUnknown->allocator->pVTbl->Release(privateUnknown->allocator);
        privateUnknown->bus->pVTbl->Release(privateUnknown->bus);
        privateUnknown->system->pVTbl->Release(privateUnknown->system);
        free(privateUnknown);
        *out = NULL;
        return status;
    }
    status = privateUnknown->bus->pVTbl->QueryComponent(privateUnknown->bus, calculatorGuidD, NULL, &IID_IEcoCalculatorY, (void**)&privateUnknown->calculatorY);
    if (status != 0) {
        privateUnknown->calculatorX->pVTbl->Release(privateUnknown->calculatorX);
        privateUnknown->allocator->pVTbl->Release(privateUnknown->allocator);
        privateUnknown->bus->pVTbl->Release(privateUnknown->bus);
        privateUnknown->system->pVTbl->Release(privateUnknown->system);
        free(privateUnknown);
        *out = NULL;
        return status;
    }
    Lab65PublicUnknown* publicUnknown = (Lab65PublicUnknown*)privateUnknown->allocator->pVTbl->Alloc(privateUnknown->allocator, sizeof(Lab65PublicUnknown));
    if (!publicUnknown) {
        privateUnknown->calculatorY->pVTbl->Release(privateUnknown->calculatorY);
        privateUnknown->calculatorX->pVTbl->Release(privateUnknown->calculatorX);
        privateUnknown->allocator->pVTbl->Release(privateUnknown->allocator);
        privateUnknown->bus->pVTbl->Release(privateUnknown->bus);
        privateUnknown->system->pVTbl->Release(privateUnknown->system);
        free(privateUnknown);
        *out = NULL;
        return -2; // E_OUTOFMEMORY
    }
    publicUnknown->vtbl.QueryInterface = Lab65PublicUnknown_QueryInterface;
    publicUnknown->vtbl.AddRef = Lab65PublicUnknown_AddRef;
    publicUnknown->vtbl.Release = Lab65PublicUnknown_Release;
    publicUnknown->privateUnknown = privateUnknown;
    if (outer) {
        *out = outer;
        publicUnknown->vtbl.Release((IEcoUnknownPtr_t)publicUnknown);
        return 0; // S_OK
    } else {
        *out = publicUnknown;
        return 0; // S_OK
    }
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!system || !iid || !out) {
        *out = NULL;
        return -1; // E_POINTER
    }
    if (outer) {
        *out = NULL;
        return -1; // E_INVALIDARG
    }
    Lab2PublicUnknown* publicUnknown = (Lab2PublicUnknown*)malloc(sizeof(Lab2PublicUnknown));
    if (!publicUnknown) {
        *out = NULL;
        return -2; // E_OUTOFMEMORY
    }
    publicUnknown->vtbl.QueryInterface = Lab2PublicUnknown_QueryInterface;
    publicUnknown->vtbl.AddRef = Lab2PublicUnknown_AddRef;
    publicUnknown->vtbl.Release = Lab2PublicUnknown_Release;
    publicUnknown->refCount = 1;
    publicUnknown->lab65 = NULL;

    IEcoInterfaceBus1* bus = NULL;
    int16_t status = system->pVTbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (void**)&bus);
    if (status != 0) {
        free(publicUnknown);
        *out = NULL;
        return status;
    }
    const UGUID* lab65Guid = &CID_EcoLab65;
    status = bus->pVTbl->QueryComponent(bus, lab65Guid, NULL, &IID_IEcoLab65, (void**)&publicUnknown->lab65);
    if (status != 0) {
        bus->pVTbl->Release(bus);
        free(publicUnknown);
        *out = NULL;
        return status;
    }
    bus->pVTbl->Release(bus);
    *out = publicUnknown;
    return 0; // S_OK
}

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    Lab65PublicUnknown* publicUnknown = (Lab65PublicUnknown*)me;
    Lab65PrivateUnknown* privateUnknown = publicUnknown->privateUnknown;
    return privateUnknown->allocator;
}
