#include "ReuseContract.h"
#include "SdkAbiReference.h"
#include "SharedFiles/IEcoLab65.h"
#include "SharedFiles/IdEcoLab65.h"
#include "SharedFiles/IEcoLab2.h"
#include "SharedFiles/IdEcoLab2.h"
#include "Vendor/Eco.CalculatorA/HeaderFiles/CEcoCalculatorA.h"
#include "Vendor/Eco.CalculatorB/HeaderFiles/CEcoCalculatorB.h"
#include "Vendor/Eco.CalculatorC/HeaderFiles/CEcoCalculatorC.h"
#include "Vendor/Eco.CalculatorD/HeaderFiles/CEcoCalculatorD.h"
#include "Vendor/Eco.CalculatorE/HeaderFiles/CEcoCalculatorE.h"

typedef struct {
    IEcoUnknownVTbl* pVtbl;
    uint32_t refCount;
    IEcoMemoryAllocator1* allocator;
    IEcoCalculatorX* calcX;
    IEcoCalculatorY* calcY;
} EcoLab65;

typedef struct {
    IEcoUnknownVTbl* pVtbl;
    uint32_t refCount;
    IEcoMemoryAllocator1* allocator;
    IEcoLab65* lab65;
    IEcoCalculatorY* calcY;
} EcoLab2;

IEcoMemoryAllocator1* lab_allocator(IEcoLab65* me) {
    return me->allocator;
}

int16_t ECOCALLMETHOD QueryInterface_EcoLab65(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    EcoLab65* this = (EcoLab65*)me;
    if (!ppv) return -1;

    *ppv = NULL;

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = (voidptr_t)this;
        AddRef_EcoLab65(me);
        return 0;
    }

    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = (voidptr_t)this;
        AddRef_EcoLab65(me);
        return 0;
    }

    if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = (voidptr_t)this->calcX;
        AddRef_EcoLab65(me);
        return 0;
    }

    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = (voidptr_t)this->calcY;
        AddRef_EcoLab65(me);
        return 0;
    }

    return -1;
}

uint32_t ECOCALLMETHOD AddRef_EcoLab65(IEcoUnknownPtr_t me) {
    EcoLab65* this = (EcoLab65*)me;
    return ++this->refCount;
}

uint32_t ECOCALLMETHOD Release_EcoLab65(IEcoUnknownPtr_t me) {
    EcoLab65* this = (EcoLab65*)me;
    if (--this->refCount == 0) {
        this->allocator->pVTbl->Free(this->allocator, this);
        return 0;
    }
    return this->refCount;
}

int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!out || !iid) return -1;
    *out = NULL;

    if (outer && !IsEqualUGUID(iid, &IID_IEcoUnknown)) return -1;

    IEcoSystem1* ecoSystem = NULL;
    IEcoInterfaceBus1* ecoInterfaceBus = NULL;
    IEcoMemoryAllocator1* allocator = NULL;

    int16_t status = system->pVtbl->QueryInterface(system, &IID_IEcoSystem1, (voidptr_t*)&ecoSystem);
    if (status != 0) return status;

    status = ecoSystem->pVtbl->QueryInterface(ecoSystem, &IID_IEcoInterfaceBus1, (voidptr_t*)&ecoInterfaceBus);
    if (status != 0) {
        ecoSystem->pVtbl->Release(ecoSystem);
        return status;
    }

    status = ecoInterfaceBus->pVtbl->QueryComponent(ecoInterfaceBus, &CID_EcoMemoryManager1, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&allocator);
    if (status != 0) {
        ecoInterfaceBus->pVtbl->Release(ecoInterfaceBus);
        ecoSystem->pVtbl->Release(ecoSystem);
        return status;
    }

    EcoLab65* this = (EcoLab65*)allocator->pVTbl->Alloc(allocator, sizeof(EcoLab65));
    if (!this) {
        allocator->pVtbl->Release(allocator);
        ecoInterfaceBus->pVtbl->Release(ecoInterfaceBus);
        ecoSystem->pVtbl->Release(ecoSystem);
        return -2;
    }

    this->pVtbl = (IEcoUnknownVTbl*)allocator->pVTbl->Alloc(allocator, sizeof(IEcoUnknownVTbl));
    if (!this->pVtbl) {
        allocator->pVTbl->Free(allocator, this);
        allocator->pVtbl->Release(allocator);
        ecoInterfaceBus->pVtbl->Release(ecoInterfaceBus);
        ecoSystem->pVtbl->Release(ecoSystem);
        return -2;
    }

    this->pVtbl->QueryInterface = QueryInterface_EcoLab65;
    this->pVtbl->AddRef = AddRef_EcoLab65;
    this->pVtbl->Release = Release_EcoLab65;

    this->refCount = 1;
    this->allocator = allocator;

    status = ecoInterfaceBus->pVtbl->QueryComponent(ecoInterfaceBus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (voidptr_t*)&this->calcX);
    if (status != 0) {
        allocator->pVtbl->Free(allocator, this->pVtbl);
        allocator->pVtbl->Free(allocator, this);
        allocator->pVtbl->Release(allocator);
        ecoInterfaceBus->pVtbl->Release(ecoInterfaceBus);
        ecoSystem->pVtbl->Release(ecoSystem);
        return status;
    }

    status = ecoInterfaceBus->pVtbl->QueryComponent(ecoInterfaceBus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&this->calcY);
    if (status != 0) {
        this->calcX->pVtbl->Release((IEcoUnknownPtr_t)this->calcX);
        allocator->pVtbl->Free(allocator, this->pVtbl);
        allocator->pVtbl->Free(allocator, this);
        allocator->pVtbl->Release(allocator);
        ecoInterfaceBus->pVtbl->Release(ecoInterfaceBus);
        ecoSystem->pVtbl->Release(ecoSystem);
        return status;
    }

    if (outer) {
        *out = (voidptr_t)this;
        return 0;
    }

    *out = (voidptr_t)this;
    return 0;
}

int16_t ECOCALLMETHOD QueryInterface_EcoLab2(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    EcoLab2* this = (EcoLab2*)me;
    if (!ppv) return -1;

    *ppv = NULL;

    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = (voidptr_t)this;
        AddRef_EcoLab2(me);
        return 0;
    }

    if (IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = (voidptr_t)this;
        AddRef_EcoLab2(me);
        return 0;
    }

    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = (voidptr_t)this->lab65;
        AddRef_EcoLab2(me);
        return 0;
    }

    if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = (voidptr_t)this->calcY;
        AddRef_EcoLab2(me);
        return 0;
    }

    return -1;
}

uint32_t ECOCALLMETHOD AddRef_EcoLab2(IEcoUnknownPtr_t me) {
    EcoLab2* this = (EcoLab2*)me;
    return ++this->refCount;
}

uint32_t ECOCALLMETHOD Release_EcoLab2(IEcoUnknownPtr_t me) {
    EcoLab2* this = (EcoLab2*)me;
    if (--this->refCount == 0) {
        this->lab65->pVtbl->Release((IEcoUnknownPtr_t)this->lab65);
        this->allocator->pVTbl->Free(this->allocator, this);
        return 0;
    }
    return this->refCount;
}

int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!out || !iid) return -1;
    *out = NULL;

    if (outer) return -1;

    IEcoSystem1* ecoSystem = NULL;
    IEcoInterfaceBus1* ecoInterfaceBus = NULL;
    IEcoMemoryAllocator1* allocator = NULL;

    int16_t status = system->pVtbl->QueryInterface(system, &IID_IEcoSystem1, (voidptr_t*)&ecoSystem);
    if (status != 0) return status;

    status = ecoSystem->pVtbl->QueryInterface(ecoSystem, &IID_IEcoInterfaceBus1, (voidptr_t*)&ecoInterfaceBus);
    if (status != 0) {
        ecoSystem->pVtbl->Release(ecoSystem);
        return status;
    }

    status = ecoInterfaceBus->pVtbl->QueryComponent(ecoInterfaceBus, &CID_EcoMemoryManager1, NULL, &IID_IEcoMemoryAllocator1, (voidptr_t*)&allocator);
    if (status != 0) {
        ecoInterfaceBus->pVtbl->Release(ecoInterfaceBus);
        ecoSystem->pVtbl->Release(ecoSystem);
        return status;
    }

    EcoLab2* this = (EcoLab2*)allocator->pVTbl->Alloc(allocator, sizeof(EcoLab2));
    if (!this) {
        allocator->pVtbl->Release(allocator);
        ecoInterfaceBus->pVtbl->Release(ecoInterfaceBus);
        ecoSystem->pVtbl->Release(ecoSystem);
        return -2;
    }

    this->pVtbl = (IEcoUnknownVTbl*)allocator->pVTbl->Alloc(allocator, sizeof(IEcoUnknownVTbl));
    if (!this->pVtbl) {
        allocator->pVtbl->Free(allocator, this);
        allocator->pVtbl->Release(allocator);
        ecoInterfaceBus->pVtbl->Release(ecoInterfaceBus);
        ecoSystem->pVtbl->Release(ecoSystem);
        return -2;
    }

    this->pVtbl->QueryInterface = QueryInterface_EcoLab2;
    this->pVtbl->AddRef = AddRef_EcoLab2;
    this->pVtbl->Release = Release_EcoLab2;

    this->refCount = 1;
    this->allocator = allocator;

    status = createEcoLab65(system, NULL, &IID_IEcoLab65, (void**)&this->lab65);
    if (status != 0) {
        allocator->pVtbl->Free(allocator, this->pVtbl);
        allocator->pVtbl->Free(allocator, this);
        allocator->pVtbl->Release(allocator);
        ecoInterfaceBus->pVtbl->Release(ecoInterfaceBus);
        ecoSystem->pVtbl->Release(ecoSystem);
        return status;
    }

    status = ecoInterfaceBus->pVtbl->QueryComponent(ecoInterfaceBus, &CID_EcoCalculatorE, NULL, &IID_IEcoCalculatorY, (voidptr_t*)&this->calcY);
    if (status != 0) {
        this->lab65->pVtbl->Release((IEcoUnknownPtr_t)this->lab65);
        allocator->pVtbl->Free(allocator, this->pVtbl);
        allocator->pVtbl->Free(allocator, this);
        allocator->pVtbl->Release(allocator);
        ecoInterfaceBus->pVtbl->Release(ecoInterfaceBus);
        ecoSystem->pVtbl->Release(ecoSystem);
        return status;
    }

    *out = (voidptr_t)this;
    return 0;
}
