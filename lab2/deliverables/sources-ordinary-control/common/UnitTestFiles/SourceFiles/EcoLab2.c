/* Fixed demonstration client; independent acceptance checks are outside the seed. */
#include <stdio.h>
#include "ReuseContract.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoLab65.h"
#include "IdEcoLab2.h"
#include "IdEcoCalculatorC.h"
extern IEcoComponentFactory* teacher_calculator_c_factory;
int16_t EcoMain(IEcoUnknown* system) {
    IEcoSystem1* sys=0;IEcoInterfaceBus1* bus=0;
    IEcoLab2* outer=0;IEcoLab65* sorter=0;IEcoCalculatorX* x=0;IEcoCalculatorY* y=0;
    int32_t data[6]={7,-4,2,7,0,-100};int i;int16_t status;
    if(system->pVTbl->QueryInterface(system,&GID_IEcoSystem,(void**)&sys)!=0)return 1;
    if(sys->pVTbl->QueryInterface(sys,&IID_IEcoInterfaceBus1,(void**)&bus)!=0)return 1;
#ifdef TEACHER_PROFILE_C
    bus->pVTbl->RegisterComponent(bus,&CID_EcoCalculatorC,(IEcoUnknown*)teacher_calculator_c_factory);
#else
    bus->pVTbl->RegisterComponent(bus,&CID_EcoCalculatorA,(IEcoUnknown*)GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E);
#endif
    bus->pVTbl->RegisterComponent(bus,&CID_EcoCalculatorB,(IEcoUnknown*)GetIEcoComponentFactoryPtr_AE202E543CE54550899603BD70C62565);
    bus->pVTbl->RegisterComponent(bus,&CID_EcoCalculatorD,(IEcoUnknown*)GetIEcoComponentFactoryPtr_3A8E44677E82475CB4A3719ED8397E61);
    bus->pVTbl->RegisterComponent(bus,&CID_EcoCalculatorE,(IEcoUnknown*)GetIEcoComponentFactoryPtr_872FEF1DE3314B87AD44D1E7C232C2F0);
    bus->pVTbl->RegisterComponent(bus,&CID_EcoLab65,(IEcoUnknown*)GetIEcoComponentFactoryPtr_AE784DFA9C5E49ECA39F168BF4120672);
    bus->pVTbl->RegisterComponent(bus,&CID_EcoLab2,(IEcoUnknown*)GetIEcoComponentFactoryPtr_C776000EC38746C089AD38BFB1C8BB9C);
    status=bus->pVTbl->QueryComponent(bus,&CID_EcoLab2,0,&IID_IEcoLab2,(void**)&outer);
    if(status!=0 || !outer)return 2;
    if(outer->pVTbl->QueryInterface(outer,&IID_IEcoCalculatorX,(void**)&x)!=0)return 3;
    if(x->pVTbl->QueryInterface(x,&IID_IEcoCalculatorY,(void**)&y)!=0)return 4;
    if(y->pVTbl->QueryInterface(y,&IID_IEcoLab65,(void**)&sorter)!=0)return 5;
    printf("Addition 3+5=%d; subtraction 7-2=%d; multiplication 3*5=%d; division 7/2=%d\n",(int)x->pVTbl->Addition(x,3,5),(int)x->pVTbl->Subtraction(x,7,2),(int)y->pVTbl->Multiplication(y,3,5),(int)y->pVTbl->Division(y,7,2));
    if(sorter->pVTbl->SortInt(sorter,data,6,7)!=0)return 6;
    printf("Bucket sort via aggregated Lab65:");for(i=0;i<6;i++)printf(" %d",(int)data[i]);printf("\n");
    sorter->pVTbl->Release(sorter);y->pVTbl->Release(y);x->pVTbl->Release(x);outer->pVTbl->Release(outer);bus->pVTbl->Release(bus);sys->pVTbl->Release(sys);
    return 0;
}
