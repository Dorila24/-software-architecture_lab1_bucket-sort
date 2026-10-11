/* Common trusted demo console. The model supplies only numerical methods. */
#include <stdio.h>
#include "IEcoSystem1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoLab65.h"
int16_t EcoMain(IEcoUnknown* system) {
    IEcoSystem1* sys = 0;
    IEcoInterfaceBus1* bus = 0;
    IEcoLab65* lab = 0;
    int32_t a[] = {7,-4,0,7,2,-100};
    double d[] = {0.5,-1.25,0.0,1000000.0,-1000000.0,0.5};
    uint32_t i;
    int failed = 0;
    if (!system || system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (void**)&sys) != 0 || !sys) return 1;
    if (sys->pVTbl->QueryInterface(sys, &IID_IEcoInterfaceBus1, (void**)&bus) != 0 || !bus) { sys->pVTbl->Release(sys); return 2; }
    if (bus->pVTbl->RegisterComponent(bus, &CID_EcoLab65, (IEcoUnknown*)GetIEcoComponentFactoryPtr_AE784DFA9C5E49ECA39F168BF4120672) != 0 ||
        bus->pVTbl->QueryComponent(bus, &CID_EcoLab65, 0, &IID_IEcoLab65, (void**)&lab) != 0 || !lab) {
        bus->pVTbl->Release(bus); sys->pVTbl->Release(sys); return 3;
    }
    if (lab->pVTbl->SortInt(lab,a,6,4) != 0 || lab->pVTbl->SortDouble(lab,d,6,4) != 0) failed=1;
    printf("Sorted int:"); for(i=0;i<6;++i) printf(" %d",a[i]); printf("\nSorted double:");
    for(i=0;i<6;++i) printf(" %.6f",d[i]); printf("\n");
    lab->pVTbl->Release(lab); bus->pVTbl->Release(bus); sys->pVTbl->Release(sys);
    return failed ? 4 : 0;
}
