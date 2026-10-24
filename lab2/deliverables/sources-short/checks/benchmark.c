/* Independent timing client. Host clock is used only by this client. */
#include <stdio.h>
#include <time.h>
#include "ReuseContract.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoLab65.h"
#include "IdEcoLab2.h"
#include "IdEcoCalculatorC.h"
extern IEcoComponentFactory* teacher_calculator_c_factory;
static volatile int64_t sink;
static int measure(IEcoUnknown* object,const char* route) {
    IEcoCalculatorX* x=0;
    IEcoCalculatorY* y=0;
    IEcoCalculatorX* queried=0;
    unsigned long i;
    int repeat;
    clock_t begin,end;
    int64_t checksum;
    double seconds;
    if(object->pVTbl->QueryInterface(object,&IID_IEcoCalculatorX,(void**)&x)!=0 || !x)return 1;
    if(object->pVTbl->QueryInterface(object,&IID_IEcoCalculatorY,(void**)&y)!=0 || !y)return 1;
    for(i=0;i<10000UL;i++)sink=x->pVTbl->Addition(x,7,2);
    for(repeat=0;repeat<5;repeat++) {
        checksum=0;
        begin=clock();
        for(i=0;i<500000UL;i++) {
            checksum+=x->pVTbl->Addition(x,7,2);
            checksum+=x->pVTbl->Subtraction(x,7,2);
            checksum+=y->pVTbl->Multiplication(y,7,2);
            checksum+=y->pVTbl->Division(y,7,2);
        }
        end=clock();sink=checksum;
        if(checksum!=15500000)return 2;
        seconds=(double)(end-begin)/(double)CLOCKS_PER_SEC;
        printf("BENCH,%s,arithmetic,%d,2000000,%.9f,%.3f,%ld\n",route,repeat,seconds,seconds*1e9/2000000.0,(long)checksum);
        begin=clock();
        for(i=0;i<500000UL;i++) {
            if(object->pVTbl->QueryInterface(object,&IID_IEcoCalculatorX,(void**)&queried)!=0 || !queried)return 3;
            sink=queried->pVTbl->Release(queried);
        }
        end=clock();
        seconds=(double)(end-begin)/(double)CLOCKS_PER_SEC;
        printf("BENCH,%s,qi_release,%d,500000,%.9f,%.3f,0\n",route,repeat,seconds,seconds*1e9/500000.0);
    }
    y->pVTbl->Release(y);x->pVTbl->Release(x);return 0;
}
int16_t EcoMain(IEcoUnknown* system) {
    IEcoSystem1* sys=0;
    IEcoInterfaceBus1* bus=0;
    IEcoUnknown* object=0;
    int kind,status;
    const UGUID* cid[3];
    const UGUID* iid[3];
    const char* names[3];
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
    cid[0]=&CID_EcoCalculatorE;iid[0]=&IID_IEcoCalculatorY;names[0]="direct_E";
    cid[1]=&CID_EcoLab65;iid[1]=&IID_IEcoLab65;names[1]="Lab65_containment";
    cid[2]=&CID_EcoLab2;iid[2]=&IID_IEcoLab2;names[2]="Lab2_with_aggregate";
    for(kind=0;kind<3;kind++) {
        status=bus->pVTbl->QueryComponent(bus,cid[kind],0,iid[kind],(void**)&object);
        if(status!=0 || !object)return 2;
        status=measure(object,names[kind]);object->pVTbl->Release(object);
        if(status)return (int16_t)(10+status);
    }
    bus->pVTbl->Release(bus);sys->pVTbl->Release(sys);return 0;
}
