/* Trusted ACOM contract client. Host libc is used only to track allocations. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoMemoryManager1.h"
#include "ReuseContract.h"
#include "IdEcoLab65.h"
#include "IdEcoLab2.h"
#include "IdEcoCalculatorC.h"
extern IEcoComponentFactory* teacher_calculator_c_factory;

typedef struct MockEnvironment {
    IEcoUnknown unknown;
    IEcoSystem1 system;
    IEcoInterfaceBus1 bus;
    IEcoInterfaceBus1MemExt extension;
    IEcoMemoryAllocator1 memory;
    uint32_t refs;
    uint32_t memory_refs;
    uint32_t live;
    uint32_t system_queries;
    uint32_t bus_queries;
    uint32_t memory_queries;
    uint32_t allocations;
    uint32_t fail_system;
    uint32_t fail_bus;
    uint32_t fail_memory;
    uint32_t fail_alloc;
    uint32_t no_extension;
    uint32_t invalidate_manager;
    UGUID manager;
    const UGUID* fail_component;
    uint32_t component_calls[5];
    IEcoUnknown* lab65_inner;
} MockEnvironment;

static MockEnvironment mock;
static unsigned int cases;
static unsigned int errors;

static int check(int ok, const char* name) {
    cases++;
    if (!ok) { errors++; printf("FAIL,%s\n", name); }
    return ok;
}

static int16_t query_environment(const UGUID* iid, void** out) {
    if (!out) return ERR_ECO_POINTER;
    *out = 0;
    if (!iid) return ERR_ECO_POINTER;
    if (IsEqualUGUID(iid, &IID_IEcoUnknown)) *out = &mock.unknown;
    else if (IsEqualUGUID(iid, &GID_IEcoSystem)) {
        mock.system_queries++;
        if (mock.system_queries == mock.fail_system) return ERR_ECO_NOSYSTEM;
        *out = &mock.system;
    }
    else if (IsEqualUGUID(iid, &IID_IEcoInterfaceBus1)) {
        mock.bus_queries++;
        if (mock.bus_queries == mock.fail_bus) return ERR_ECO_NOBUS;
        *out = &mock.bus;
    }
    else if (IsEqualUGUID(iid, &IID_IEcoInterfaceBus1MemExt) && !mock.no_extension)
        *out = &mock.extension;
    else return ERR_ECO_NOINTERFACE;
    mock.refs++;
    return 0;
}
static int16_t ECOCALLMETHOD unknown_query(IEcoUnknown* me, const UGUID* iid, void** out) {
    if (me != &mock.unknown) return ERR_ECO_POINTER;
    return query_environment(iid, out);
}
static uint32_t ECOCALLMETHOD unknown_addref(IEcoUnknown* me) {
    return me == &mock.unknown ? ++mock.refs : 0;
}
static uint32_t ECOCALLMETHOD unknown_release(IEcoUnknown* me) {
    return me == &mock.unknown && mock.refs ? --mock.refs : 0;
}
static int16_t ECOCALLMETHOD system_query(IEcoSystem1* me, const UGUID* iid, void** out) {
    if (me != &mock.system) return ERR_ECO_POINTER;
    return query_environment(iid, out);
}
static uint32_t ECOCALLMETHOD system_addref(IEcoSystem1* me) {
    return me == &mock.system ? ++mock.refs : 0;
}
static uint32_t ECOCALLMETHOD system_release(IEcoSystem1* me) {
    return me == &mock.system && mock.refs ? --mock.refs : 0;
}
static void ECOCALLMETHOD system_set(IEcoSystem1* me, void* data) { (void)me; (void)data; }
static void* ECOCALLMETHOD system_get(IEcoSystem1* me) { (void)me; return 0; }
static int16_t ECOCALLMETHOD bus_query(IEcoInterfaceBus1* me, const UGUID* iid, void** out) {
    if (me != &mock.bus) return ERR_ECO_POINTER;
    return query_environment(iid, out);
}
static uint32_t ECOCALLMETHOD bus_addref(IEcoInterfaceBus1* me) {
    return me == &mock.bus ? ++mock.refs : 0;
}
static uint32_t ECOCALLMETHOD bus_release(IEcoInterfaceBus1* me) {
    return me == &mock.bus && mock.refs ? --mock.refs : 0;
}
static int16_t ECOCALLMETHOD bus_init(IEcoInterfaceBus1* me) { (void)me; return 0; }
static int16_t ECOCALLMETHOD bus_init_with(IEcoInterfaceBus1* me, void* p, uint32_t n) {
    (void)me; (void)p; (void)n; return 0;
}
static int16_t ECOCALLMETHOD bus_register(IEcoInterfaceBus1* me, const UGUID* cid, IEcoUnknown* f) {
    (void)me; (void)cid; (void)f; return 0;
}
static int16_t ECOCALLMETHOD bus_unregister(IEcoInterfaceBus1* me, const UGUID* cid) {
    (void)me; (void)cid; return 0;
}
static int16_t ECOCALLMETHOD extension_query(IEcoInterfaceBus1MemExt* me, const UGUID* iid, void** out) {
    if (me != &mock.extension) return ERR_ECO_POINTER;
    return query_environment(iid, out);
}
static uint32_t ECOCALLMETHOD extension_addref(IEcoInterfaceBus1MemExt* me) {
    return me == &mock.extension ? ++mock.refs : 0;
}
static uint32_t ECOCALLMETHOD extension_release(IEcoInterfaceBus1MemExt* me) {
    if (me == &mock.extension && mock.invalidate_manager)
        ((unsigned char*)&mock.manager)[sizeof(mock.manager)-1] ^= 1;
    return me == &mock.extension && mock.refs ? --mock.refs : 0;
}
static int16_t ECOCALLMETHOD extension_set(IEcoInterfaceBus1MemExt* me, const UGUID* cid) {
    (void)me; (void)cid; return 0;
}
static const UGUID* ECOCALLMETHOD extension_get(IEcoInterfaceBus1MemExt* me) {
    (void)me; return &mock.manager;
}
static int16_t ECOCALLMETHOD extension_expand(IEcoInterfaceBus1MemExt* me, bool_t value) {
    (void)me; (void)value; return 0;
}
static int16_t ECOCALLMETHOD memory_query(IEcoMemoryAllocator1* me, const UGUID* iid, void** out) {
    if (!out) return ERR_ECO_POINTER;
    *out = 0;
    if (me != &mock.memory || !iid) return ERR_ECO_POINTER;
    if (!IsEqualUGUID(iid, &IID_IEcoMemoryAllocator1) && !IsEqualUGUID(iid, &IID_IEcoUnknown))
        return ERR_ECO_NOINTERFACE;
    *out = me; mock.memory_refs++; return 0;
}
static uint32_t ECOCALLMETHOD memory_addref(IEcoMemoryAllocator1* me) {
    return me == &mock.memory ? ++mock.memory_refs : 0;
}
static uint32_t ECOCALLMETHOD memory_release(IEcoMemoryAllocator1* me) {
    return me == &mock.memory && mock.memory_refs ? --mock.memory_refs : 0;
}
static void* ECOCALLMETHOD memory_alloc(IEcoMemoryAllocator1* me, uint32_t n) {
    void* p;
    if (me != &mock.memory) return 0;
    mock.allocations++;
    if (mock.allocations == mock.fail_alloc) return 0;
    p = malloc(n); if (p) mock.live++; return p;
}
static void ECOCALLMETHOD memory_free(IEcoMemoryAllocator1* me, void* p) {
    if (me == &mock.memory && p) { mock.live--; free(p); }
}
static void* ECOCALLMETHOD memory_realloc(IEcoMemoryAllocator1* me, void* p, uint32_t n) {
    (void)me; return realloc(p, n);
}
static void* ECOCALLMETHOD memory_copy(IEcoMemoryAllocator1* me, void* dst, void* src, uint32_t n) {
    (void)me; return memcpy(dst, src, n);
}
static void* ECOCALLMETHOD memory_fill(IEcoMemoryAllocator1* me, void* dst, char_t v, uint32_t n) {
    (void)me; return memset(dst, v, n);
}
static int16_t ECOCALLMETHOD memory_compare(IEcoMemoryAllocator1* me, void* a, void* b, uint32_t n) {
    int result;
    (void)me; result = memcmp(a, b, n); return (int16_t)((result > 0) - (result < 0));
}
static uint32_t ECOCALLMETHOD memory_size(IEcoMemoryAllocator1* me, void* p) {
    (void)me; (void)p; return 0;
}
static int16_t ECOCALLMETHOD bus_component(IEcoInterfaceBus1* me, const UGUID* cid,
        IEcoUnknown* outer, const UGUID* iid, void** out) {
    IEcoComponentFactory* factory=0;
    unsigned int slot=0;
    int16_t status;
    if (!out) return ERR_ECO_POINTER;
    *out=0;
    if (me!=&mock.bus || !cid || !iid) return ERR_ECO_POINTER;
    if (IsEqualUGUID(cid,&CID_EcoMemoryManager1)) {
        mock.memory_queries++;
        if (mock.memory_queries==mock.fail_memory) return ERR_ECO_GET_MEMORY_ALLOCATOR;
        return memory_query(&mock.memory,iid,out);
    }
    if (mock.fail_component && IsEqualUGUID(cid,mock.fail_component)) return ERR_ECO_NOINTERFACE;
    if(IsEqualUGUID(cid,&CID_EcoCalculatorA)) {
#ifdef TEACHER_PROFILE_C
        factory=teacher_calculator_c_factory;
#else
        factory=GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E;
#endif
        slot=0;
    }
    else if(IsEqualUGUID(cid,&CID_EcoCalculatorB)) {factory=GetIEcoComponentFactoryPtr_AE202E543CE54550899603BD70C62565;slot=1;}
    else if(IsEqualUGUID(cid,&CID_EcoCalculatorD)) {factory=GetIEcoComponentFactoryPtr_3A8E44677E82475CB4A3719ED8397E61;slot=2;}
    else if(IsEqualUGUID(cid,&CID_EcoCalculatorE)) {factory=GetIEcoComponentFactoryPtr_872FEF1DE3314B87AD44D1E7C232C2F0;slot=3;}
    else if(IsEqualUGUID(cid,&CID_EcoLab65)) {factory=GetIEcoComponentFactoryPtr_AE784DFA9C5E49ECA39F168BF4120672;slot=4;}
    if(!factory)return ERR_ECO_NOINTERFACE;
    mock.component_calls[slot]++;
    status=factory->pVTbl->Alloc(factory,&mock.unknown,outer,iid,out);
    /* Alloc in the original factories performs Init before returning the requested interface. */
    if(slot==4 && outer && status==0)mock.lab65_inner=(IEcoUnknown*)*out;
    return status;
}
static IEcoUnknownVTbl unknown_vtable = {unknown_query, unknown_addref, unknown_release};
static IEcoSystem1VTbl system_vtable = {
    system_query, system_addref, system_release, system_set, system_get
};
static IEcoInterfaceBus1VTbl bus_vtable = {
    bus_query, bus_addref, bus_release, bus_init, bus_init_with,
    bus_register, bus_unregister, bus_component
};
static IEcoMemoryAllocator1VTbl memory_vtable = {
    memory_query, memory_addref, memory_release, memory_alloc, memory_free,
    memory_realloc, memory_copy, memory_fill, memory_compare, memory_size
};
static IEcoInterfaceBus1MemExtVTbl extension_vtable = {
    extension_query, extension_addref, extension_release, extension_set, extension_get, extension_expand
};

static void reset_mock(void) {
    memset(&mock, 0, sizeof(mock));
    mock.unknown.pVTbl = &unknown_vtable;
    mock.system.pVTbl = &system_vtable;
    mock.bus.pVTbl = &bus_vtable;
    mock.extension.pVTbl = &extension_vtable;
    mock.memory.pVTbl = &memory_vtable;
    mock.refs = 1; mock.memory_refs = 1;
    mock.manager = CID_EcoMemoryManager1;
}
static void release_interface(void* p) {
    IEcoUnknown* u = (IEcoUnknown*)p;
    if (u) u->pVTbl->Release(u);
}
static void check_balanced(void) {
    check(mock.refs == 1, "system_and_bus_references_balanced");
    check(mock.memory_refs == 1, "allocator_references_balanced");
    check(mock.live == 0, "all_component_allocations_freed");
}


static IEcoComponentFactory* lab1_factory;
static IEcoComponentFactory* lab2_factory;
static UGUID missing;

static void matrix(IEcoUnknown* primary,int is_lab2) {
    const UGUID* ids[5];
    IEcoUnknown *canonical=0,*from=0,*to=0,*back=0;
    unsigned int i,j,k,count=is_lab2?5:4;
    void* output;
    uint32_t refs;
    int16_t status;
    ids[0]=&IID_IEcoUnknown;ids[1]=&IID_IEcoLab65;ids[2]=&IID_IEcoCalculatorX;ids[3]=&IID_IEcoCalculatorY;ids[4]=&IID_IEcoLab2;
    if(!check(primary->pVTbl->QueryInterface(primary,&IID_IEcoUnknown,(void**)&canonical)==0 && canonical,"canonical_unknown"))return;
    for(i=0;i<count;i++) {
        from=0;
        if(!check(primary->pVTbl->QueryInterface(primary,ids[i],(void**)&from)==0 && from,"matrix_source"))continue;
        for(j=0;j<count;j++) for(k=0;k<2;k++) {
            to=0;back=0;
            if(check(from->pVTbl->QueryInterface(from,ids[j],(void**)&to)==0 && to,"matrix_transition")) {
                check(to->pVTbl->QueryInterface(to,&IID_IEcoUnknown,(void**)&back)==0 && back==canonical,"canonical_identity");
                release_interface(back);release_interface(to);
            }
        }
        output=from;status=from->pVTbl->QueryInterface(from,&missing,&output);
        check(status!=0 && !output,"unsupported_clears_output");
        output=from;status=from->pVTbl->QueryInterface(from,0,&output);
        check(status!=0 && !output,"null_iid_clears_output");
        check(from->pVTbl->QueryInterface(from,ids[i],0)!=0,"null_output_rejected");
        refs=from->pVTbl->AddRef(from);
        check(refs>1 && from->pVTbl->Release(from)==refs-1,"addref_release_pair");
        release_interface(from);
    }
    release_interface(canonical);
}
static void arithmetic(IEcoUnknown* u) {
    IEcoCalculatorX* x=0;
    IEcoCalculatorY* y=0;
    int a,b;
    if(check(u->pVTbl->QueryInterface(u,&IID_IEcoCalculatorX,(void**)&x)==0 && x,"arithmetic_X")) {
        for(a=-100;a<=100;a+=17)for(b=-100;b<=100;b+=19) {
            check(x->pVTbl->Addition(x,(int16_t)a,(int16_t)b)==a+b,"addition_exact");
            check(x->pVTbl->Subtraction(x,(int16_t)a,(int16_t)b)==a-b,"subtraction_exact");
        }
        x->pVTbl->Release(x);
    }
    if(check(u->pVTbl->QueryInterface(u,&IID_IEcoCalculatorY,(void**)&y)==0 && y,"arithmetic_Y")) {
        for(a=-100;a<=100;a+=17)for(b=-100;b<=100;b+=19) {
            check(y->pVTbl->Multiplication(y,(int16_t)a,(int16_t)b)==a*b,"multiplication_exact");
            if(b)check(y->pVTbl->Division(y,(int16_t)a,(int16_t)b)==a/b,"division_exact");
        }
        y->pVTbl->Release(y);
    }
}
static unsigned long random_state=1234567UL;
static unsigned long random32(void) {random_state=(1664525UL*random_state+1013904223UL)&0xffffffffUL;return random_state;}
static int compareInt(const void* a,const void* b) { int32_t x=*(int32_t const*)a,y=*(int32_t const*)b;return (x>y)-(x<y); }
static void sortInt(IEcoLab65* lab) {
    int32_t values[257],reference[257],saved[257];
    uint32_t i,b;
    uint32_t bs[4]={1,2,7,1024};
    int16_t status;
    for(b=0;b<4;b++) {
        for(i=0;i<257;i++)values[i]=reference[i]=(int32_t)((int)(random32()%2001UL)-1000);
        qsort(reference,257,sizeof(int32_t),compareInt);
        check(lab->pVTbl->SortInt(lab,values,257,bs[b])==0,"sort_Int_status");
        for(i=0;i<257;i++)check(values[i]==reference[i],"sort_Int_exact");
    }
    check(lab->pVTbl->SortInt(lab,0,0,7)==0,"sort_Int_empty");
    check(lab->pVTbl->SortInt(0,0,0,7)==-1,"sort_Int_null_me");
    check(lab->pVTbl->SortInt(lab,0,1,7)==-1,"sort_Int_null_array");
    for(i=0;i<257;i++)values[i]=saved[i]=(int32_t)(257-i);
    check(lab->pVTbl->SortInt(lab,values,257,0)==-1,"sort_Int_invalid_B");
    check(memcmp(values,saved,sizeof(values))==0,"sort_Int_error_unchanged");
    if(mock.live) {
        mock.fail_alloc=mock.allocations+1;
        status=lab->pVTbl->SortInt(lab,values,257,7);
        mock.fail_alloc=0;
        check(status==-2,"sort_Int_allocation_error");
        check(memcmp(values,saved,sizeof(values))==0,"sort_Int_allocation_unchanged");
    }
}
static int compareLong(const void* a,const void* b) { int64_t x=*(int64_t const*)a,y=*(int64_t const*)b;return (x>y)-(x<y); }
static void sortLong(IEcoLab65* lab) {
    int64_t values[257],reference[257],saved[257];
    uint32_t i,b;
    uint32_t bs[4]={1,2,7,1024};
    int16_t status;
    for(b=0;b<4;b++) {
        for(i=0;i<257;i++)values[i]=reference[i]=(int64_t)((int)(random32()%2001UL)-1000);
        qsort(reference,257,sizeof(int64_t),compareLong);
        check(lab->pVTbl->SortLong(lab,values,257,bs[b])==0,"sort_Long_status");
        for(i=0;i<257;i++)check(values[i]==reference[i],"sort_Long_exact");
    }
    check(lab->pVTbl->SortLong(lab,0,0,7)==0,"sort_Long_empty");
    check(lab->pVTbl->SortLong(0,0,0,7)==-1,"sort_Long_null_me");
    check(lab->pVTbl->SortLong(lab,0,1,7)==-1,"sort_Long_null_array");
    for(i=0;i<257;i++)values[i]=saved[i]=(int64_t)(257-i);
    check(lab->pVTbl->SortLong(lab,values,257,0)==-1,"sort_Long_invalid_B");
    check(memcmp(values,saved,sizeof(values))==0,"sort_Long_error_unchanged");
    if(mock.live) {
        mock.fail_alloc=mock.allocations+1;
        status=lab->pVTbl->SortLong(lab,values,257,7);
        mock.fail_alloc=0;
        check(status==-2,"sort_Long_allocation_error");
        check(memcmp(values,saved,sizeof(values))==0,"sort_Long_allocation_unchanged");
    }
}
static int compareFloat(const void* a,const void* b) { float x=*(float const*)a,y=*(float const*)b;return (x>y)-(x<y); }
static void sortFloat(IEcoLab65* lab) {
    float values[257],reference[257],saved[257];
    uint32_t i,b;
    uint32_t bs[4]={1,2,7,1024};
    int16_t status;
    for(b=0;b<4;b++) {
        for(i=0;i<257;i++)values[i]=reference[i]=(float)((int)(random32()%2001UL)-1000);
        qsort(reference,257,sizeof(float),compareFloat);
        check(lab->pVTbl->SortFloat(lab,values,257,bs[b])==0,"sort_Float_status");
        for(i=0;i<257;i++)check(values[i]==reference[i],"sort_Float_exact");
    }
    check(lab->pVTbl->SortFloat(lab,0,0,7)==0,"sort_Float_empty");
    check(lab->pVTbl->SortFloat(0,0,0,7)==-1,"sort_Float_null_me");
    check(lab->pVTbl->SortFloat(lab,0,1,7)==-1,"sort_Float_null_array");
    for(i=0;i<257;i++)values[i]=saved[i]=(float)(257-i);
    check(lab->pVTbl->SortFloat(lab,values,257,0)==-1,"sort_Float_invalid_B");
    check(memcmp(values,saved,sizeof(values))==0,"sort_Float_error_unchanged");
    if(mock.live) {
        mock.fail_alloc=mock.allocations+1;
        status=lab->pVTbl->SortFloat(lab,values,257,7);
        mock.fail_alloc=0;
        check(status==-2,"sort_Float_allocation_error");
        check(memcmp(values,saved,sizeof(values))==0,"sort_Float_allocation_unchanged");
    }
}
static int compareDouble(const void* a,const void* b) { double x=*(double const*)a,y=*(double const*)b;return (x>y)-(x<y); }
static void sortDouble(IEcoLab65* lab) {
    double values[257],reference[257],saved[257];
    uint32_t i,b;
    uint32_t bs[4]={1,2,7,1024};
    int16_t status;
    for(b=0;b<4;b++) {
        for(i=0;i<257;i++)values[i]=reference[i]=(double)((int)(random32()%2001UL)-1000);
        qsort(reference,257,sizeof(double),compareDouble);
        check(lab->pVTbl->SortDouble(lab,values,257,bs[b])==0,"sort_Double_status");
        for(i=0;i<257;i++)check(values[i]==reference[i],"sort_Double_exact");
    }
    check(lab->pVTbl->SortDouble(lab,0,0,7)==0,"sort_Double_empty");
    check(lab->pVTbl->SortDouble(0,0,0,7)==-1,"sort_Double_null_me");
    check(lab->pVTbl->SortDouble(lab,0,1,7)==-1,"sort_Double_null_array");
    for(i=0;i<257;i++)values[i]=saved[i]=(double)(257-i);
    check(lab->pVTbl->SortDouble(lab,values,257,0)==-1,"sort_Double_invalid_B");
    check(memcmp(values,saved,sizeof(values))==0,"sort_Double_error_unchanged");
    if(mock.live) {
        mock.fail_alloc=mock.allocations+1;
        status=lab->pVTbl->SortDouble(lab,values,257,7);
        mock.fail_alloc=0;
        check(status==-2,"sort_Double_allocation_error");
        check(memcmp(values,saved,sizeof(values))==0,"sort_Double_allocation_unchanged");
    }
}
static int compareLongDouble(const void* a,const void* b) { long double x=*(long double const*)a,y=*(long double const*)b;return (x>y)-(x<y); }
static void sortLongDouble(IEcoLab65* lab) {
    long double values[257],reference[257],saved[257];
    uint32_t i,b;
    uint32_t bs[4]={1,2,7,1024};
    int16_t status;
    for(b=0;b<4;b++) {
        for(i=0;i<257;i++)values[i]=reference[i]=(long double)((int)(random32()%2001UL)-1000);
        qsort(reference,257,sizeof(long double),compareLongDouble);
        check(lab->pVTbl->SortLongDouble(lab,values,257,bs[b])==0,"sort_LongDouble_status");
        for(i=0;i<257;i++)check(values[i]==reference[i],"sort_LongDouble_exact");
    }
    check(lab->pVTbl->SortLongDouble(lab,0,0,7)==0,"sort_LongDouble_empty");
    check(lab->pVTbl->SortLongDouble(0,0,0,7)==-1,"sort_LongDouble_null_me");
    check(lab->pVTbl->SortLongDouble(lab,0,1,7)==-1,"sort_LongDouble_null_array");
    for(i=0;i<257;i++)values[i]=saved[i]=(long double)(257-i);
    check(lab->pVTbl->SortLongDouble(lab,values,257,0)==-1,"sort_LongDouble_invalid_B");
    check(memcmp(values,saved,sizeof(values))==0,"sort_LongDouble_error_unchanged");
    if(mock.live) {
        mock.fail_alloc=mock.allocations+1;
        status=lab->pVTbl->SortLongDouble(lab,values,257,7);
        mock.fail_alloc=0;
        check(status==-2,"sort_LongDouble_allocation_error");
        check(memcmp(values,saved,sizeof(values))==0,"sort_LongDouble_allocation_unchanged");
    }
}
static void sorting(IEcoUnknown* unknown) {
    IEcoLab65* lab=0;
    if(!check(unknown->pVTbl->QueryInterface(unknown,&IID_IEcoLab65,(void**)&lab)==0 && lab,"previous_lab_available"))return;
    sortInt(lab);sortLong(lab);sortFloat(lab);sortDouble(lab);sortLongDouble(lab);
    lab->pVTbl->Release(lab);
}
extern IEcoCalculatorXVTbl g_x9322111622484742AE0682819447843DVTblA;
extern IEcoCalculatorXVTbl g_x9322111622484742AE0682819447843DVTblB;
extern IEcoCalculatorXVTbl g_x9322111622484742AE0682819447843DVTblC;
extern IEcoCalculatorYVTbl g_xBD6414C29096423EA90C04D77AFD1CADVTblD;
extern IEcoCalculatorYVTbl g_xBD6414C29096423EA90C04D77AFD1CADVTblE;
static int32_t ECOCALLMETHOD trap_add(IEcoCalculatorX* me,int16_t a,int16_t b) {(void)me;return a+b+1234;}
static int16_t ECOCALLMETHOD trap_sub(IEcoCalculatorX* me,int16_t a,int16_t b) {(void)me;return (int16_t)(a-b-123);}
static int32_t ECOCALLMETHOD trap_mul(IEcoCalculatorY* me,int16_t a,int16_t b) {(void)me;return a*b+2345;}
static int16_t ECOCALLMETHOD trap_div(IEcoCalculatorY* me,int16_t a,int16_t b) {(void)me;return (int16_t)(a/b+234);}
static void forwarding(IEcoUnknown* u,int lab2) {
    IEcoCalculatorXVTbl *xv;
    IEcoCalculatorYVTbl *yv;
    IEcoCalculatorXVTbl oldx;
    IEcoCalculatorYVTbl oldy;
    IEcoCalculatorX* x=0;
    IEcoCalculatorY* y=0;
    if(lab2)xv=&g_x9322111622484742AE0682819447843DVTblB;
    else {
#ifdef TEACHER_PROFILE_C
        xv=&g_x9322111622484742AE0682819447843DVTblC;
#else
        xv=&g_x9322111622484742AE0682819447843DVTblA;
#endif
    }
    yv=lab2?&g_xBD6414C29096423EA90C04D77AFD1CADVTblE:&g_xBD6414C29096423EA90C04D77AFD1CADVTblD;
    oldx=*xv;oldy=*yv;
    xv->Addition=trap_add;xv->Subtraction=trap_sub;yv->Multiplication=trap_mul;yv->Division=trap_div;
    u->pVTbl->QueryInterface(u,&IID_IEcoCalculatorX,(void**)&x);
    u->pVTbl->QueryInterface(u,&IID_IEcoCalculatorY,(void**)&y);
    check(x && x->pVTbl->Addition(x,3,5)==1242,"forwards_addition_to_vendor");
    check(x && x->pVTbl->Subtraction(x,7,2)==-118,"forwards_subtraction_to_vendor");
    check(y && y->pVTbl->Multiplication(y,3,5)==2360,"forwards_multiplication_to_vendor");
    check(y && y->pVTbl->Division(y,7,2)==237,"forwards_division_to_vendor");
    *xv=oldx;*yv=oldy;release_interface(x);release_interface(y);
}
static void normal_case(int lab2) {
    IEcoComponentFactory* f=lab2?lab2_factory:lab1_factory;
    const UGUID* iid=lab2?&IID_IEcoLab2:&IID_IEcoLab65;
    IEcoUnknown* component=0;
    IEcoCalculatorX* held=0;
    IEcoLab65 *a=0,*b=0;
    reset_mock();
    if(!check(f->pVTbl->Alloc(f,&mock.unknown,0,iid,(void**)&component)==0 && component,"create_normal")) {check_balanced();return;}
    matrix(component,lab2);arithmetic(component);sorting(component);forwarding(component,lab2);
    check(mock.component_calls[0]>0 && mock.component_calls[1]>0 && mock.component_calls[2]>0,"Lab65_contains_A_or_C_and_D_containing_B");
    if(lab2) {
        check(mock.component_calls[3]>0 && mock.component_calls[4]>0,"Lab2_contains_E_and_aggregates_Lab65");
        check(((IEcoLab2*)component)->pVTbl->GetVersion((IEcoLab2*)component)==2,"lab2_version");
        if(check(mock.lab65_inner!=0,"private_inner_unknown_observed")) {
            component->pVTbl->QueryInterface(component,&IID_IEcoLab65,(void**)&a);
            mock.lab65_inner->pVTbl->QueryInterface(mock.lab65_inner,&IID_IEcoLab65,(void**)&b);
            check(a==b && a!=0,"direct_aggregated_sort_interface");
            release_interface(a);release_interface(b);
        }
    }
    check(component->pVTbl->QueryInterface(component,&IID_IEcoCalculatorX,(void**)&held)==0 && held,"hold_interface");
    release_interface(component);
    check(mock.live>0,"alive_after_primary_release");
    if(held) {check(held->pVTbl->Addition(held,20,22)==42,"held_interface_operational");matrix((IEcoUnknown*)held,lab2);held->pVTbl->Release(held);}
    check_balanced();
}
typedef struct TestOuter {
    IEcoUnknown unknown;
    IEcoUnknown* inner;
    uint32_t refs,adds,releases,queries;
} TestOuter;
static int16_t ECOCALLMETHOD outer_query(IEcoUnknown* me,const UGUID* iid,void** out) {
    TestOuter* o=(TestOuter*)me;
    if(!out)return ERR_ECO_POINTER;*out=0;
    if(!iid)return ERR_ECO_POINTER;
    o->queries++;
    if(IsEqualUGUID(iid,&IID_IEcoUnknown)){*out=me;o->refs++;return 0;}
    return o->inner?o->inner->pVTbl->QueryInterface(o->inner,iid,out):ERR_ECO_NOINTERFACE;
}
static uint32_t ECOCALLMETHOD outer_add(IEcoUnknown* me) {TestOuter* o=(TestOuter*)me;o->adds++;return ++o->refs;}
static uint32_t ECOCALLMETHOD outer_release(IEcoUnknown* me) {
    TestOuter* o=(TestOuter*)me;uint32_t n;
    o->releases++;n=--o->refs;
    if(!n && o->inner){o->inner->pVTbl->Release(o->inner);o->inner=0;}
    return n;
}
static IEcoUnknownVTbl outer_vtable={outer_query,outer_add,outer_release};
static void aggregation_case(void) {
    TestOuter outer;
    IEcoLab65* lab=0;
    IEcoUnknown* canonical=0;
    IEcoCalculatorX* held=0;
    uint32_t refs,adds,releases;
    void* out;
    reset_mock();memset(&outer,0,sizeof(outer));outer.unknown.pVTbl=&outer_vtable;outer.refs=1;
    out=&outer;check(lab1_factory->pVTbl->Alloc(lab1_factory,&mock.unknown,&outer.unknown,&IID_IEcoLab65,&out)==ERR_ECO_NOAGGREGATION && !out,"aggregate_creation_requires_unknown");
    if(!check(lab1_factory->pVTbl->Alloc(lab1_factory,&mock.unknown,&outer.unknown,&IID_IEcoUnknown,(void**)&outer.inner)==0 && outer.inner,"create_aggregate_private_unknown"))return;
    check(outer.refs==1 && outer.adds==0,"no_outer_ownership_cycle");
    check(outer.inner!=&outer.unknown,"private_unknown_distinct_from_controller");
    check(outer.inner->pVTbl->QueryInterface(outer.inner,&IID_IEcoLab65,(void**)&lab)==0 && lab,"inner_public_interface");
    if(lab){
        check(lab->pVTbl->QueryInterface(lab,&IID_IEcoUnknown,(void**)&canonical)==0 && canonical==&outer.unknown,"aggregate_public_identity");release_interface(canonical);
        refs=outer.refs;adds=outer.adds;releases=outer.releases;
        lab->pVTbl->AddRef(lab);check(outer.refs==refs+1 && outer.adds==adds+1,"delegating_addref");
        lab->pVTbl->Release(lab);check(outer.refs==refs && outer.releases==releases+1,"delegating_release");
        matrix((IEcoUnknown*)lab,0);sorting((IEcoUnknown*)lab);
        lab->pVTbl->QueryInterface(lab,&IID_IEcoCalculatorX,(void**)&held);
        lab->pVTbl->Release(lab);outer_release(&outer.unknown);
        check(outer.refs==1 && mock.live>0,"aggregate_kept_by_public_interface");
        if(held){check(held->pVTbl->Addition(held,19,23)==42,"aggregate_held_interface_works");held->pVTbl->Release(held);}
    }
    check(outer.refs==0 && outer.inner==0,"outer_final_release_destroys_inner");check_balanced();
}
static void failures_case(int lab2) {
    IEcoComponentFactory* f=lab2?lab2_factory:lab1_factory;
    const UGUID* iid=lab2?&IID_IEcoLab2:&IID_IEcoLab65;
    const UGUID* failed_ids[4];
    unsigned int i;
    void* out;
    int16_t status;
    failed_ids[0]=&CID_EcoCalculatorA;failed_ids[1]=&CID_EcoCalculatorD;failed_ids[2]=&CID_EcoCalculatorE;failed_ids[3]=&CID_EcoLab65;
    reset_mock();out=f;check(f->pVTbl->Alloc(f,0,0,iid,&out)!=0 && !out,"null_system");
    out=f;check(f->pVTbl->Alloc(f,&mock.unknown,0,0,&out)!=0 && !out,"null_creation_iid");
    check(f->pVTbl->Alloc(f,&mock.unknown,0,iid,0)!=0,"null_creation_output");check_balanced();
    for(i=0;i<4;i++) {
        reset_mock();if(i==0)mock.fail_system=1;if(i==1)mock.fail_bus=1;if(i==2)mock.fail_memory=1;if(i==3)mock.fail_alloc=1;
        out=f;status=f->pVTbl->Alloc(f,&mock.unknown,0,iid,&out);
        check(status!=0 && !out,"construction_failure_not_published");if(status==0)release_interface(out);check_balanced();
    }
    for(i=0;i<(lab2?4:2);i++) {
        reset_mock();mock.fail_component=failed_ids[i];out=f;status=f->pVTbl->Alloc(f,&mock.unknown,0,iid,&out);
        check(status!=0 && !out,"missing_direct_dependency");if(status==0)release_interface(out);check_balanced();
    }
    reset_mock();out=f;status=f->pVTbl->Alloc(f,&mock.unknown,0,&missing,&out);
    check(status!=0 && !out,"unsupported_creation_iid");if(status==0)release_interface(out);check_balanced();
    if(lab2){out=f;check(f->pVTbl->Alloc(f,&mock.unknown,&mock.unknown,&IID_IEcoUnknown,&out)==ERR_ECO_NOAGGREGATION && !out,"Lab2_rejects_unsupported_aggregation");check_balanced();}
}
static void register_real(IEcoInterfaceBus1* bus) {
#ifdef TEACHER_PROFILE_C
    check(bus->pVTbl->RegisterComponent(bus,&CID_EcoCalculatorC,(IEcoUnknown*)teacher_calculator_c_factory)==0,"real_register_C");
#else
    check(bus->pVTbl->RegisterComponent(bus,&CID_EcoCalculatorA,(IEcoUnknown*)GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E)==0,"real_register_A");
#endif
    check(bus->pVTbl->RegisterComponent(bus,&CID_EcoCalculatorB,(IEcoUnknown*)GetIEcoComponentFactoryPtr_AE202E543CE54550899603BD70C62565)==0,"real_register_B");
    check(bus->pVTbl->RegisterComponent(bus,&CID_EcoCalculatorD,(IEcoUnknown*)GetIEcoComponentFactoryPtr_3A8E44677E82475CB4A3719ED8397E61)==0,"real_register_D");
    check(bus->pVTbl->RegisterComponent(bus,&CID_EcoCalculatorE,(IEcoUnknown*)GetIEcoComponentFactoryPtr_872FEF1DE3314B87AD44D1E7C232C2F0)==0,"real_register_E");
    check(bus->pVTbl->RegisterComponent(bus,&CID_EcoLab65,(IEcoUnknown*)lab1_factory)==0,"real_register_Lab65");
    check(bus->pVTbl->RegisterComponent(bus,&CID_EcoLab2,(IEcoUnknown*)lab2_factory)==0,"real_register_Lab2");
}
int16_t EcoMain(IEcoUnknown* system) {
    IEcoSystem1* sys=0;
    IEcoInterfaceBus1* bus=0;
    IEcoUnknown* component=0;
    int kind;
    lab1_factory=GetIEcoComponentFactoryPtr_AE784DFA9C5E49ECA39F168BF4120672;
    lab2_factory=GetIEcoComponentFactoryPtr_C776000EC38746C089AD38BFB1C8BB9C;
    missing=IID_IEcoLab2;missing.Data[0]^=127;
    if(system->pVTbl->QueryInterface(system,&GID_IEcoSystem,(void**)&sys)!=0)return 1;
    if(sys->pVTbl->QueryInterface(sys,&IID_IEcoInterfaceBus1,(void**)&bus)!=0)return 1;
    register_real(bus);
    for(kind=0;kind<2;kind++) {
        component=0;
        if(check(bus->pVTbl->QueryComponent(bus,kind?&CID_EcoLab2:&CID_EcoLab65,0,kind?&IID_IEcoLab2:&IID_IEcoLab65,(void**)&component)==0 && component,"real_bus_component")){
            matrix(component,kind);arithmetic(component);sorting(component);forwarding(component,kind);release_interface(component);
        }
    }
    bus->pVTbl->Release(bus);sys->pVTbl->Release(sys);
    for(kind=0;kind<2;kind++){normal_case(kind);failures_case(kind);}
    aggregation_case();
    printf("REUSE_CHECK cases=%u failures=%u mock_live=%u environment_refs=%u memory_refs=%u\n",cases,errors,mock.live,mock.refs,mock.memory_refs);
    return errors?1:0;
}
