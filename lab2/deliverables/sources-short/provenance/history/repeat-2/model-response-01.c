#include "ReuseContract.h"
#include "SdkAbiReference.h"
#include "IEcoLab65.h"
#include "IdEcoLab65.h"
#include "IEcoLab2.h"
#include "IdEcoLab2.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "Vendor/Eco.CalculatorA/SharedFiles/CEcoCalculatorA.h"
#include "Vendor/Eco.CalculatorB/SharedFiles/CEcoCalculatorB.h"
#include "Vendor/Eco.CalculatorC/SharedFiles/CEcoCalculatorC.h"
#include "Vendor/Eco.CalculatorD/SharedFiles/CEcoCalculatorD.h"
#include "Vendor/Eco.CalculatorE/SharedFiles/CEcoCalculatorE.h"
#include <stddef.h>

typedef struct {
    IEcoUnknownVTbl unknownVtbl;
    IEcoLab65VTbl lab65Vtbl;
    IEcoUnknown* privateUnknown;
    IEcoCalculatorX* calculatorA;
    IEcoCalculatorY* calculatorD;
} EcoLab65Impl;

typedef struct {
    IEcoUnknownVTbl unknownVtbl;
    IEcoLab2VTbl lab2Vtbl;
    IEcoUnknown* privateUnknown;
    IEcoLab65* lab65;
    IEcoCalculatorY* calculatorE;
} EcoLab2Impl;

static int16_t ECOCALLMETHOD EcoLab65_QueryInterface(IEcoLab65Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown) || IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = impl->privateUnknown;
        AddRef(impl->privateUnknown);
        return 0;
    }
    *ppv = NULL;
    return -1;
}

static uint32_t ECOCALLMETHOD EcoLab65_AddRef(IEcoLab65Ptr_t me) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    return AddRef(impl->privateUnknown);
}

static uint32_t ECOCALLMETHOD EcoLab65_Release(IEcoLab65Ptr_t me) {
    EcoLab65Impl* impl = (EcoLab65Impl*)me;
    return Release(impl->privateUnknown);
}

static int16_t ECOCALLMETHOD EcoLab65_SortInt(IEcoLab65Ptr_t me, int32_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_int((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD EcoLab65_SortLong(IEcoLab65Ptr_t me, int64_t* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD EcoLab65_SortFloat(IEcoLab65Ptr_t me, float* data, uint32_t count, uint32_t buckets) {
    return lab_sort_float((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD EcoLab65_SortDouble(IEcoLab65Ptr_t me, double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_double((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD EcoLab65_SortLongDouble(IEcoLab65Ptr_t me, long double* data, uint32_t count, uint32_t buckets) {
    return lab_sort_long_double((IEcoLab65*)me, data, count, buckets);
}

static int16_t ECOCALLMETHOD EcoLab2_QueryInterface(IEcoLab2Ptr_t me, const UGUID* riid, voidptr_t* ppv) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown) || IsEqualUGUID(riid, &IID_IEcoLab2)) {
        *ppv = impl->privateUnknown;
        AddRef(impl->privateUnknown);
        return 0;
    }
    if (IsEqualUGUID(riid, &IID_IEcoLab65)) {
        *ppv = impl->lab65;
        AddRef(impl->lab65);
        return 0;
    }
    *ppv = NULL;
    return -1;
}

static uint32_t ECOCALLMETHOD EcoLab2_AddRef(IEcoLab2Ptr_t me) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    return AddRef(impl->privateUnknown);
}

static uint32_t ECOCALLMETHOD EcoLab2_Release(IEcoLab2Ptr_t me) {
    EcoLab2Impl* impl = (EcoLab2Impl*)me;
    return Release(impl->privateUnknown);
}

static uint32_t ECOCALLMETHOD EcoLab2_GetVersion(IEcoLab2Ptr_t me) {
    return 2;
}

static int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!outer && IsEqualUGUID(iid, &IID_IEcoUnknown)) {
        EcoLab65Impl* impl = (EcoLab65Impl*)system->pVTbl->Alloc(system, sizeof(EcoLab65Impl));
        if (!impl) {
            return -1;
        }
        impl->privateUnknown = (IEcoUnknown*)impl;
        impl->unknownVtbl.QueryInterface = EcoLab65_QueryInterface;
        impl->unknownVtbl.AddRef = EcoLab65_AddRef;
        impl->unknownVtbl.Release = EcoLab65_Release;
        impl->lab65Vtbl.QueryInterface = EcoLab65_QueryInterface;
        impl->lab65Vtbl.AddRef = EcoLab65_AddRef;
        impl->lab65Vtbl.Release = EcoLab65_Release;
        impl->lab65Vtbl.SortInt = EcoLab65_SortInt;
        impl->lab65Vtbl.SortLong = EcoLab65_SortLong;
        impl->lab65Vtbl.SortFloat = EcoLab65_SortFloat;
        impl->lab65Vtbl.SortDouble = EcoLab65_SortDouble;
        impl->lab65Vtbl.SortLongDouble = EcoLab65_SortLongDouble;

        IEcoInterfaceBus1* bus;
        int16_t status = system->pVTbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (void**)&bus);
        if (status != 0) {
            system->pVTbl->Free(system, impl);
            return status;
        }

        status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorA, NULL, &IID_IEcoCalculatorX, (void**)&impl->calculatorA);
        if (status != 0) {
            system->pVTbl->Free(system, impl);
            bus->pVTbl->Release(bus);
            return status;
        }

        status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorD, NULL, &IID_IEcoCalculatorY, (void**)&impl->calculatorD);
        if (status != 0) {
            system->pVTbl->Free(system, impl);
            Release(impl->calculatorA);
            bus->pVTbl->Release(bus);
            return status;
        }

        bus->pVTbl->Release(bus);

        *out = impl;
        return 0;
    }
    *out = NULL;
    return -1;
}

static int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!outer && (IsEqualUGUID(iid, &IID_IEcoUnknown) || IsEqualUGUID(iid, &IID_IEcoLab2))) {
        EcoLab2Impl* impl = (EcoLab2Impl*)system->pVTbl->Alloc(system, sizeof(EcoLab2Impl));
        if (!impl) {
            return -1;
        }
        impl->privateUnknown = (IEcoUnknown*)impl;
        impl->unknownVtbl.QueryInterface = EcoLab2_QueryInterface;
        impl->unknownVtbl.AddRef = EcoLab2_AddRef;
        impl->unknownVtbl.Release = EcoLab2_Release;
        impl->lab2Vtbl.QueryInterface = EcoLab2_QueryInterface;
        impl->lab2Vtbl.AddRef = EcoLab2_AddRef;
        impl->lab2Vtbl.Release = EcoLab2_Release;
        impl->lab2Vtbl.GetVersion = EcoLab2_GetVersion;

        IEcoInterfaceBus1* bus;
        int16_t status = system->pVTbl->QueryInterface(system, &IID_IEcoInterfaceBus1, (void**)&bus);
        if (status != 0) {
            system->pVTbl->Free(system, impl);
            return status;
        }

        status = createEcoLab65(system, NULL, &IID_IEcoLab65, (void**)&impl->lab65);
        if (status != 0) {
            system->pVTbl->Free(system, impl);
            bus->pVTbl->Release(bus);
            return status;
        }

        status = bus->pVTbl->QueryComponent(bus, &CID_EcoCalculatorE, NULL, &IID_IEcoCalculatorY, (void**)&impl->calculatorE);
        if (status != 0) {
            system->pVTbl->Free(system, impl);
            Release(impl->lab65);
            bus->pVTbl->Release(bus);
            return status;
        }

        bus->pVTbl->Release(bus);

        *out = impl;
        return 0;
    }
    *out = NULL;
    return -1;
}
