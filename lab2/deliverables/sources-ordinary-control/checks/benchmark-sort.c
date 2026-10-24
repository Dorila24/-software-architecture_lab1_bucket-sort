/* Independent trusted C89 reference. libc is confined to this client. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "IEcoSystem1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoLab65.h"
#include "ReuseContract.h"
#include "IdEcoCalculatorC.h"
#include "IdEcoLab2.h"
extern IEcoComponentFactory* teacher_calculator_c_factory;
static unsigned long rng=1234567UL;
static unsigned long random32(void) { rng=(1664525UL*rng+1013904223UL)&0xffffffffUL; return rng; }
static int failures=0;
static int cases=0;
static void check(int ok,const char* name) { ++cases; if(!ok) { ++failures; printf("FAIL %s\n",name); } }
static int cmpInt(const void* x,const void* y) {
    int32_t a=*(int32_t const*)x;
    int32_t b=*(int32_t const*)y;
    return (a>b)-(a<b);
}
static int cmpLong(const void* x,const void* y) {
    int64_t a=*(int64_t const*)x;
    int64_t b=*(int64_t const*)y;
    return (a>b)-(a<b);
}
static int cmpFloat(const void* x,const void* y) {
    float a=*(float const*)x;
    float b=*(float const*)y;
    return (a>b)-(a<b);
}
static int cmpDouble(const void* x,const void* y) {
    double a=*(double const*)x;
    double b=*(double const*)y;
    return (a>b)-(a<b);
}
static int cmpLongDouble(const void* x,const void* y) {
    long double a=*(long double const*)x;
    long double b=*(long double const*)y;
    return (a>b)-(a<b);
}
int16_t EcoMain(IEcoUnknown* system) {
    IEcoSystem1* sys=0;
    IEcoInterfaceBus1* bus=0;
    IEcoLab65* lab=0;
    double* a=0;
    double* b=0;
    uint32_t ns[]={1000,10000,50000,1000,5000,10000};
    uint32_t n,i,c,rep;
    clock_t start;
    double tb,tq;
    int16_t status=0;
    int bad=0;
    if (!system || system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (void**)&sys) != 0 || !sys) return 1;
    if (sys->pVTbl->QueryInterface(sys, &IID_IEcoInterfaceBus1, (void**)&bus) != 0 || !bus) { sys->pVTbl->Release(sys); return 2; }
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
    if (bus->pVTbl->QueryComponent(bus, &CID_EcoLab2, 0, &IID_IEcoLab65, (void**)&lab) != 0 || !lab) {
        bus->pVTbl->Release(bus);sys->pVTbl->Release(sys);return 3;
    }
    a=(double*)malloc(50000*sizeof(double)); b=(double*)malloc(50000*sizeof(double));
    if(!a || !b) { free(a); free(b);
    lab->pVTbl->Release(lab); bus->pVTbl->Release(bus); sys->pVTbl->Release(sys);
        return 4;
    }
    for(c=0;c<6;++c) for(rep=0;rep<5;++rep) {
        n=ns[c]; rng=1234567UL;
        for(i=0;i<n;++i) {
            a[i]=c<3 ? ((double)random32()/4294967295.0)*2000000.0-1000000.0 : 100.0+(double)(n-i)/(double)n;
            b[i]=a[i];
        }
        if(rep%2==0) {
            start=clock(); status=lab->pVTbl->SortDouble(lab,a,n,128); tb=(double)(clock()-start)/CLOCKS_PER_SEC;
            start=clock(); qsort(b,n,sizeof(double),cmpDouble); tq=(double)(clock()-start)/CLOCKS_PER_SEC;
        } else {
            start=clock(); qsort(b,n,sizeof(double),cmpDouble); tq=(double)(clock()-start)/CLOCKS_PER_SEC;
            start=clock(); status=lab->pVTbl->SortDouble(lab,a,n,128); tb=(double)(clock()-start)/CLOCKS_PER_SEC;
        }
        if(status!=0) bad++;
        for(i=0;i<n;++i) if(a[i]!=b[i]) { bad++; break; }
        printf("BENCH,%s,%u,%u,%.9f,%.9f,%d\n",c<3 ? "uniform" : "cluster_reverse",n,rep,tb,tq,bad);
    }
    free(a); free(b);
    lab->pVTbl->Release(lab); bus->pVTbl->Release(bus); sys->pVTbl->Release(sys);
    return bad ? 4 : 0;
}
