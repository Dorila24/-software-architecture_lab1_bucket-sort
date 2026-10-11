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
#include "CEcoLab65.h"
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
static IEcoMemoryAllocator1* real_mem=0;
static void* live_ptrs[4096];
static int live=0;
static int calls=0;
static int fail_at=-1;
static int bad_free=0;
static voidptr_t ECOCALLMETHOD proxy_alloc(IEcoMemoryAllocator1* me,uint32_t size) {
    void* ptr;
    (void)me;
    if(calls++==fail_at) return 0;
    ptr=real_mem->pVTbl->Alloc(real_mem,size);
    if(ptr) { if(live>=4096) { real_mem->pVTbl->Free(real_mem,ptr); return 0; } live_ptrs[live++]=ptr; }
    return ptr;
}
static void ECOCALLMETHOD proxy_free(IEcoMemoryAllocator1* me,voidptr_t ptr) {
    int i;
    (void)me;
    if(!ptr) return;
    for(i=0;i<live;++i) if(live_ptrs[i]==ptr) break;
    if(i==live) { bad_free++; return; }
    live_ptrs[i]=live_ptrs[--live];
    real_mem->pVTbl->Free(real_mem,ptr);
}
static IEcoMemoryAllocator1VTbl proxy_vtbl={0,0,0,proxy_alloc,proxy_free,0,0,0,0,0};
static IEcoMemoryAllocator1 proxy_mem={&proxy_vtbl};
static void testInt(IEcoLab65* lab) {
    int32_t a[4096];
    int32_t r[4096];
    int32_t saved[17];
    uint32_t sizes[]={0,1,2,17,257,4096};
    uint32_t bs[]={1,2,7,128,1024};
    uint32_t n,i,s,b,mode;
    int16_t status;
    int k,ok;
    char name[100];
    for(s=0;s<6;++s) for(b=0;b<5;++b) for(mode=0;mode<5;++mode) {
        n=sizes[s];
        for(i=0;i<n;++i) {
            if(mode==0) a[i]=(int32_t)(random32()%2000001UL)-1000000;
            else if(mode==1) a[i]=(int32_t)((int)i-2000);
            else if(mode==2) a[i]=(int32_t)(2000-(int)i);
            else if(mode==3) a[i]=(int32_t)42;
            else a[i]=(int32_t)((int)(i%7)-3);
        }
        if(n>=5 && mode==0) { a[0]=-2147483647-1; a[1]=2147483647; a[2]=-1; a[3]=0; a[4]=1; }
        for(i=0;i<n;++i) r[i]=a[i];
        qsort(r,n,sizeof(int32_t),cmpInt);
        status=lab->pVTbl->SortInt(lab,a,n,bs[b]);
        ok=status==0;
        for(i=0;i<n;++i) if(a[i]!=r[i]) { ok=0; printf("MISMATCH Int n=%u B=%u mode=%u i=%u got=%.17Lg ref=%.17Lg\n",n,bs[b],mode,i,(long double)a[i],(long double)r[i]); break; }
        sprintf(name,"Int n=%u B=%u mode=%u",n,bs[b],mode); check(ok,name);
        check(live==0 && bad_free==0,"Int allocator balance");
    }
    check(lab->pVTbl->SortInt(lab,0,0,7)==0,"Int null empty");
    check(lab->pVTbl->SortInt(0,a,0,7)==-1,"Int null me empty");
    check(lab->pVTbl->SortInt(0,a,1,7)==-1,"Int null me");
    check(lab->pVTbl->SortInt(lab,0,1,7)==-1,"Int null data");
    check(lab->pVTbl->SortInt(lab,a,0,0)==-1,"Int zero buckets empty");
    check(lab->pVTbl->SortInt(lab,a,0,1025)==-1,"Int too many buckets empty");
    for(i=0;i<17;++i) a[i]=saved[i]=(int32_t)(17-i);
    check(lab->pVTbl->SortInt(lab,a,1000001,7)==-1,"Int count limit");
    check(memcmp(a,saved,sizeof(saved))==0,"Int invalid count unchanged");
    check(lab->pVTbl->SortInt(lab,a,17,0)==-1,"Int buckets invalid");
    check(memcmp(a,saved,sizeof(saved))==0,"Int invalid buckets unchanged");
    for(k=0;k<64;++k) {
        for(i=0;i<17;++i) a[i]=saved[i]=(int32_t)(17-i);
        calls=0; fail_at=k;
        status=lab->pVTbl->SortInt(lab,a,17,7);
        fail_at=-1;
        sprintf(name,"Int allocation fail point %d",k);
        if(status==0) { check(live==0 && bad_free==0,name); break; }
        check(status==-2 && memcmp(a,saved,sizeof(saved))==0 && live==0 && bad_free==0,name);
        if(live || bad_free) break;
    }
}
static void testLong(IEcoLab65* lab) {
    int64_t a[4096];
    int64_t r[4096];
    int64_t saved[17];
    uint32_t sizes[]={0,1,2,17,257,4096};
    uint32_t bs[]={1,2,7,128,1024};
    uint32_t n,i,s,b,mode;
    int16_t status;
    int k,ok;
    char name[100];
    for(s=0;s<6;++s) for(b=0;b<5;++b) for(mode=0;mode<5;++mode) {
        n=sizes[s];
        for(i=0;i<n;++i) {
            if(mode==0) a[i]=(int64_t)(random32()%2000001UL)-1000000;
            else if(mode==1) a[i]=(int64_t)((int)i-2000);
            else if(mode==2) a[i]=(int64_t)(2000-(int)i);
            else if(mode==3) a[i]=(int64_t)42;
            else a[i]=(int64_t)((int)(i%7)-3);
        }
        if(n>=5 && mode==0) { a[0]=-((int64_t)1<<62)-((int64_t)1<<62); a[1]=(((int64_t)1<<62)-1)+((int64_t)1<<62); a[2]=((int64_t)1<<53)+1; a[3]=((int64_t)1<<53); a[4]=-1; }
        for(i=0;i<n;++i) r[i]=a[i];
        qsort(r,n,sizeof(int64_t),cmpLong);
        status=lab->pVTbl->SortLong(lab,a,n,bs[b]);
        ok=status==0;
        for(i=0;i<n;++i) if(a[i]!=r[i]) { ok=0; printf("MISMATCH Long n=%u B=%u mode=%u i=%u got=%ld ref=%ld\n",n,bs[b],mode,i,(long)a[i],(long)r[i]); break; }
        sprintf(name,"Long n=%u B=%u mode=%u",n,bs[b],mode); check(ok,name);
        check(live==0 && bad_free==0,"Long allocator balance");
    }
    check(lab->pVTbl->SortLong(lab,0,0,7)==0,"Long null empty");
    check(lab->pVTbl->SortLong(0,a,0,7)==-1,"Long null me empty");
    check(lab->pVTbl->SortLong(0,a,1,7)==-1,"Long null me");
    check(lab->pVTbl->SortLong(lab,0,1,7)==-1,"Long null data");
    check(lab->pVTbl->SortLong(lab,a,0,0)==-1,"Long zero buckets empty");
    check(lab->pVTbl->SortLong(lab,a,0,1025)==-1,"Long too many buckets empty");
    for(i=0;i<17;++i) a[i]=saved[i]=(int64_t)(17-i);
    check(lab->pVTbl->SortLong(lab,a,1000001,7)==-1,"Long count limit");
    check(memcmp(a,saved,sizeof(saved))==0,"Long invalid count unchanged");
    check(lab->pVTbl->SortLong(lab,a,17,0)==-1,"Long buckets invalid");
    check(memcmp(a,saved,sizeof(saved))==0,"Long invalid buckets unchanged");
    for(k=0;k<64;++k) {
        for(i=0;i<17;++i) a[i]=saved[i]=(int64_t)(17-i);
        calls=0; fail_at=k;
        status=lab->pVTbl->SortLong(lab,a,17,7);
        fail_at=-1;
        sprintf(name,"Long allocation fail point %d",k);
        if(status==0) { check(live==0 && bad_free==0,name); break; }
        check(status==-2 && memcmp(a,saved,sizeof(saved))==0 && live==0 && bad_free==0,name);
        if(live || bad_free) break;
    }
}
static void testFloat(IEcoLab65* lab) {
    float a[4096];
    float r[4096];
    float saved[17];
    uint32_t sizes[]={0,1,2,17,257,4096};
    uint32_t bs[]={1,2,7,128,1024};
    uint32_t n,i,s,b,mode;
    int16_t status;
    int k,ok;
    int actual_negative_zeros,reference_negative_zeros;
    char name[100];
    for(s=0;s<6;++s) for(b=0;b<5;++b) for(mode=0;mode<5;++mode) {
        n=sizes[s];
        for(i=0;i<n;++i) {
            if(mode==0) a[i]=(float)((double)(random32()%2000001UL)-1000000.0)/(float)7.0;
            else if(mode==1) a[i]=(float)((int)i-2000);
            else if(mode==2) a[i]=(float)(2000-(int)i);
            else if(mode==3) a[i]=(float)42;
            else a[i]=(float)((int)(i%7)-3);
        }
        if(n>=5 && mode==0) { a[0]=(float)-1000000.0; a[1]=(float)1000000.0; a[2]=(float)-0.0; a[3]=(float)0.0; a[4]=(float)1e-30; }
        for(i=0;i<n;++i) r[i]=a[i];
        qsort(r,n,sizeof(float),cmpFloat);
        status=lab->pVTbl->SortFloat(lab,a,n,bs[b]);
        ok=status==0;
        for(i=0;i<n;++i) if(a[i]!=r[i]) { ok=0; printf("MISMATCH Float n=%u B=%u mode=%u i=%u got=%.17Lg ref=%.17Lg\n",n,bs[b],mode,i,(long double)a[i],(long double)r[i]); break; }
        actual_negative_zeros=0; reference_negative_zeros=0;
        for(i=0;i<n;++i) {
            if(a[i]==0 && 1.0L/(long double)a[i]<0) ++actual_negative_zeros;
            if(r[i]==0 && 1.0L/(long double)r[i]<0) ++reference_negative_zeros;
        }
        if(actual_negative_zeros!=reference_negative_zeros) ok=0;
        sprintf(name,"Float n=%u B=%u mode=%u",n,bs[b],mode); check(ok,name);
        check(live==0 && bad_free==0,"Float allocator balance");
    }
    check(lab->pVTbl->SortFloat(lab,0,0,7)==0,"Float null empty");
    check(lab->pVTbl->SortFloat(0,a,0,7)==-1,"Float null me empty");
    check(lab->pVTbl->SortFloat(0,a,1,7)==-1,"Float null me");
    check(lab->pVTbl->SortFloat(lab,0,1,7)==-1,"Float null data");
    check(lab->pVTbl->SortFloat(lab,a,0,0)==-1,"Float zero buckets empty");
    check(lab->pVTbl->SortFloat(lab,a,0,1025)==-1,"Float too many buckets empty");
    for(i=0;i<17;++i) a[i]=saved[i]=(float)(17-i);
    check(lab->pVTbl->SortFloat(lab,a,1000001,7)==-1,"Float count limit");
    check(memcmp(a,saved,sizeof(saved))==0,"Float invalid count unchanged");
    check(lab->pVTbl->SortFloat(lab,a,17,0)==-1,"Float buckets invalid");
    check(memcmp(a,saved,sizeof(saved))==0,"Float invalid buckets unchanged");
    for(k=0;k<64;++k) {
        for(i=0;i<17;++i) a[i]=saved[i]=(float)(17-i);
        calls=0; fail_at=k;
        status=lab->pVTbl->SortFloat(lab,a,17,7);
        fail_at=-1;
        sprintf(name,"Float allocation fail point %d",k);
        if(status==0) { check(live==0 && bad_free==0,name); break; }
        check(status==-2 && memcmp(a,saved,sizeof(saved))==0 && live==0 && bad_free==0,name);
        if(live || bad_free) break;
    }
}
static void invalidFloat(IEcoLab65* lab) {
    float a[3];
    float saved[3];
    float invalids[5];
    double z=0.0;
    int i,k;
    invalids[0]=(float)1000001.0; invalids[1]=(float)-1000001.0;
    invalids[2]=(float)(1.0/z); invalids[3]=(float)(-1.0/z); invalids[4]=(float)(z/z);
    for(k=0;k<5;++k) {
        a[0]=(float)2.0; a[1]=(float)1.0; a[2]=invalids[k];
        for(i=0;i<3;++i) saved[i]=a[i];
        check(lab->pVTbl->SortFloat(lab,a,3,7)==-1,"Float invalid floating tail");
        check(memcmp(a,saved,sizeof(a))==0,"Float invalid floating unchanged");
        check(lab->pVTbl->SortFloat(lab,a+2,1,7)==-1,"Float invalid singleton");
        check(live==0 && bad_free==0,"Float invalid allocator balance");
    }
}
static void testDouble(IEcoLab65* lab) {
    double a[4096];
    double r[4096];
    double saved[17];
    uint32_t sizes[]={0,1,2,17,257,4096};
    uint32_t bs[]={1,2,7,128,1024};
    uint32_t n,i,s,b,mode;
    int16_t status;
    int k,ok;
    int actual_negative_zeros,reference_negative_zeros;
    char name[100];
    for(s=0;s<6;++s) for(b=0;b<5;++b) for(mode=0;mode<5;++mode) {
        n=sizes[s];
        for(i=0;i<n;++i) {
            if(mode==0) a[i]=(double)((double)(random32()%2000001UL)-1000000.0)/(double)7.0;
            else if(mode==1) a[i]=(double)((int)i-2000);
            else if(mode==2) a[i]=(double)(2000-(int)i);
            else if(mode==3) a[i]=(double)42;
            else a[i]=(double)((int)(i%7)-3);
        }
        if(n>=5 && mode==0) { a[0]=(double)-1000000.0; a[1]=(double)1000000.0; a[2]=(double)-0.0; a[3]=(double)0.0; a[4]=(double)1e-30; }
        for(i=0;i<n;++i) r[i]=a[i];
        qsort(r,n,sizeof(double),cmpDouble);
        status=lab->pVTbl->SortDouble(lab,a,n,bs[b]);
        ok=status==0;
        for(i=0;i<n;++i) if(a[i]!=r[i]) { ok=0; printf("MISMATCH Double n=%u B=%u mode=%u i=%u got=%.17Lg ref=%.17Lg\n",n,bs[b],mode,i,(long double)a[i],(long double)r[i]); break; }
        actual_negative_zeros=0; reference_negative_zeros=0;
        for(i=0;i<n;++i) {
            if(a[i]==0 && 1.0L/(long double)a[i]<0) ++actual_negative_zeros;
            if(r[i]==0 && 1.0L/(long double)r[i]<0) ++reference_negative_zeros;
        }
        if(actual_negative_zeros!=reference_negative_zeros) ok=0;
        sprintf(name,"Double n=%u B=%u mode=%u",n,bs[b],mode); check(ok,name);
        check(live==0 && bad_free==0,"Double allocator balance");
    }
    check(lab->pVTbl->SortDouble(lab,0,0,7)==0,"Double null empty");
    check(lab->pVTbl->SortDouble(0,a,0,7)==-1,"Double null me empty");
    check(lab->pVTbl->SortDouble(0,a,1,7)==-1,"Double null me");
    check(lab->pVTbl->SortDouble(lab,0,1,7)==-1,"Double null data");
    check(lab->pVTbl->SortDouble(lab,a,0,0)==-1,"Double zero buckets empty");
    check(lab->pVTbl->SortDouble(lab,a,0,1025)==-1,"Double too many buckets empty");
    for(i=0;i<17;++i) a[i]=saved[i]=(double)(17-i);
    check(lab->pVTbl->SortDouble(lab,a,1000001,7)==-1,"Double count limit");
    check(memcmp(a,saved,sizeof(saved))==0,"Double invalid count unchanged");
    check(lab->pVTbl->SortDouble(lab,a,17,0)==-1,"Double buckets invalid");
    check(memcmp(a,saved,sizeof(saved))==0,"Double invalid buckets unchanged");
    for(k=0;k<64;++k) {
        for(i=0;i<17;++i) a[i]=saved[i]=(double)(17-i);
        calls=0; fail_at=k;
        status=lab->pVTbl->SortDouble(lab,a,17,7);
        fail_at=-1;
        sprintf(name,"Double allocation fail point %d",k);
        if(status==0) { check(live==0 && bad_free==0,name); break; }
        check(status==-2 && memcmp(a,saved,sizeof(saved))==0 && live==0 && bad_free==0,name);
        if(live || bad_free) break;
    }
}
static void invalidDouble(IEcoLab65* lab) {
    double a[3];
    double saved[3];
    double invalids[5];
    double z=0.0;
    int i,k;
    invalids[0]=(double)1000001.0; invalids[1]=(double)-1000001.0;
    invalids[2]=(double)(1.0/z); invalids[3]=(double)(-1.0/z); invalids[4]=(double)(z/z);
    for(k=0;k<5;++k) {
        a[0]=(double)2.0; a[1]=(double)1.0; a[2]=invalids[k];
        for(i=0;i<3;++i) saved[i]=a[i];
        check(lab->pVTbl->SortDouble(lab,a,3,7)==-1,"Double invalid floating tail");
        check(memcmp(a,saved,sizeof(a))==0,"Double invalid floating unchanged");
        check(lab->pVTbl->SortDouble(lab,a+2,1,7)==-1,"Double invalid singleton");
        check(live==0 && bad_free==0,"Double invalid allocator balance");
    }
}
static void testLongDouble(IEcoLab65* lab) {
    long double a[4096];
    long double r[4096];
    long double saved[17];
    uint32_t sizes[]={0,1,2,17,257,4096};
    uint32_t bs[]={1,2,7,128,1024};
    uint32_t n,i,s,b,mode;
    int16_t status;
    int k,ok;
    int actual_negative_zeros,reference_negative_zeros;
    char name[100];
    for(s=0;s<6;++s) for(b=0;b<5;++b) for(mode=0;mode<5;++mode) {
        n=sizes[s];
        for(i=0;i<n;++i) {
            if(mode==0) a[i]=(long double)((double)(random32()%2000001UL)-1000000.0)/(long double)7.0;
            else if(mode==1) a[i]=(long double)((int)i-2000);
            else if(mode==2) a[i]=(long double)(2000-(int)i);
            else if(mode==3) a[i]=(long double)42;
            else a[i]=(long double)((int)(i%7)-3);
        }
        if(n>=5 && mode==0) { a[0]=(long double)-1000000.0; a[1]=(long double)1000000.0; a[2]=(long double)-0.0; a[3]=(long double)0.0; a[4]=(long double)1e-30; }
        for(i=0;i<n;++i) r[i]=a[i];
        qsort(r,n,sizeof(long double),cmpLongDouble);
        status=lab->pVTbl->SortLongDouble(lab,a,n,bs[b]);
        ok=status==0;
        for(i=0;i<n;++i) if(a[i]!=r[i]) { ok=0; printf("MISMATCH LongDouble n=%u B=%u mode=%u i=%u got=%.17Lg ref=%.17Lg\n",n,bs[b],mode,i,(long double)a[i],(long double)r[i]); break; }
        actual_negative_zeros=0; reference_negative_zeros=0;
        for(i=0;i<n;++i) {
            if(a[i]==0 && 1.0L/(long double)a[i]<0) ++actual_negative_zeros;
            if(r[i]==0 && 1.0L/(long double)r[i]<0) ++reference_negative_zeros;
        }
        if(actual_negative_zeros!=reference_negative_zeros) ok=0;
        sprintf(name,"LongDouble n=%u B=%u mode=%u",n,bs[b],mode); check(ok,name);
        check(live==0 && bad_free==0,"LongDouble allocator balance");
    }
    check(lab->pVTbl->SortLongDouble(lab,0,0,7)==0,"LongDouble null empty");
    check(lab->pVTbl->SortLongDouble(0,a,0,7)==-1,"LongDouble null me empty");
    check(lab->pVTbl->SortLongDouble(0,a,1,7)==-1,"LongDouble null me");
    check(lab->pVTbl->SortLongDouble(lab,0,1,7)==-1,"LongDouble null data");
    check(lab->pVTbl->SortLongDouble(lab,a,0,0)==-1,"LongDouble zero buckets empty");
    check(lab->pVTbl->SortLongDouble(lab,a,0,1025)==-1,"LongDouble too many buckets empty");
    for(i=0;i<17;++i) a[i]=saved[i]=(long double)(17-i);
    check(lab->pVTbl->SortLongDouble(lab,a,1000001,7)==-1,"LongDouble count limit");
    check(memcmp(a,saved,sizeof(saved))==0,"LongDouble invalid count unchanged");
    check(lab->pVTbl->SortLongDouble(lab,a,17,0)==-1,"LongDouble buckets invalid");
    check(memcmp(a,saved,sizeof(saved))==0,"LongDouble invalid buckets unchanged");
    for(k=0;k<64;++k) {
        for(i=0;i<17;++i) a[i]=saved[i]=(long double)(17-i);
        calls=0; fail_at=k;
        status=lab->pVTbl->SortLongDouble(lab,a,17,7);
        fail_at=-1;
        sprintf(name,"LongDouble allocation fail point %d",k);
        if(status==0) { check(live==0 && bad_free==0,name); break; }
        check(status==-2 && memcmp(a,saved,sizeof(saved))==0 && live==0 && bad_free==0,name);
        if(live || bad_free) break;
    }
}
static void invalidLongDouble(IEcoLab65* lab) {
    long double a[3];
    long double saved[3];
    long double invalids[5];
    double z=0.0;
    int i,k;
    invalids[0]=(long double)1000001.0; invalids[1]=(long double)-1000001.0;
    invalids[2]=(long double)(1.0/z); invalids[3]=(long double)(-1.0/z); invalids[4]=(long double)(z/z);
    for(k=0;k<5;++k) {
        a[0]=(long double)2.0; a[1]=(long double)1.0; a[2]=invalids[k];
        for(i=0;i<3;++i) saved[i]=a[i];
        check(lab->pVTbl->SortLongDouble(lab,a,3,7)==-1,"LongDouble invalid floating tail");
        check(memcmp(a,saved,sizeof(a))==0,"LongDouble invalid floating unchanged");
        check(lab->pVTbl->SortLongDouble(lab,a+2,1,7)==-1,"LongDouble invalid singleton");
        check(live==0 && bad_free==0,"LongDouble invalid allocator balance");
    }
}
static void maximumInt(IEcoLab65* lab) {
    int32_t* values;
    uint32_t i;
    int ok=1;
    values=(int32_t*)malloc(1000000UL*sizeof(int32_t));
    if(!values) { check(0,"Trusted client allocation for maximum count"); return; }
    for(i=0;i<1000000;++i) values[i]=(int32_t)37;
    if(lab->pVTbl->SortInt(lab,values,1000000,1024)!=0) ok=0;
    for(i=0;i<1000000;++i) if(values[i]!=(int32_t)37) { ok=0; break; }
    check(ok,"Int legal maximum count equal values");
    free(values);
}
static void maximumLong(IEcoLab65* lab) {
    int64_t* values;
    uint32_t i;
    int ok=1;
    values=(int64_t*)malloc(1000000UL*sizeof(int64_t));
    if(!values) { check(0,"Trusted client allocation for maximum count"); return; }
    for(i=0;i<1000000;++i) values[i]=(int64_t)37;
    if(lab->pVTbl->SortLong(lab,values,1000000,1024)!=0) ok=0;
    for(i=0;i<1000000;++i) if(values[i]!=(int64_t)37) { ok=0; break; }
    check(ok,"Long legal maximum count equal values");
    free(values);
}
static void maximumFloat(IEcoLab65* lab) {
    float* values;
    uint32_t i;
    int ok=1;
    values=(float*)malloc(1000000UL*sizeof(float));
    if(!values) { check(0,"Trusted client allocation for maximum count"); return; }
    for(i=0;i<1000000;++i) values[i]=(float)37;
    if(lab->pVTbl->SortFloat(lab,values,1000000,1024)!=0) ok=0;
    for(i=0;i<1000000;++i) if(values[i]!=(float)37) { ok=0; break; }
    check(ok,"Float legal maximum count equal values");
    free(values);
}
static void maximumDouble(IEcoLab65* lab) {
    double* values;
    uint32_t i;
    int ok=1;
    values=(double*)malloc(1000000UL*sizeof(double));
    if(!values) { check(0,"Trusted client allocation for maximum count"); return; }
    for(i=0;i<1000000;++i) values[i]=(double)37;
    if(lab->pVTbl->SortDouble(lab,values,1000000,1024)!=0) ok=0;
    for(i=0;i<1000000;++i) if(values[i]!=(double)37) { ok=0; break; }
    check(ok,"Double legal maximum count equal values");
    free(values);
}
static void maximumLongDouble(IEcoLab65* lab) {
    long double* values;
    uint32_t i;
    int ok=1;
    values=(long double*)malloc(1000000UL*sizeof(long double));
    if(!values) { check(0,"Trusted client allocation for maximum count"); return; }
    for(i=0;i<1000000;++i) values[i]=(long double)37;
    if(lab->pVTbl->SortLongDouble(lab,values,1000000,1024)!=0) ok=0;
    for(i=0;i<1000000;++i) if(values[i]!=(long double)37) { ok=0; break; }
    check(ok,"LongDouble legal maximum count equal values");
    free(values);
}

int16_t EcoMain(IEcoUnknown* system) {
    IEcoSystem1* sys=0;
    IEcoInterfaceBus1* bus=0;
    IEcoLab65* lab=0;
    IEcoLab65* second=0;
    IEcoUnknown* unknown=0;
    UGUID missing={0x01,0x10,{0}};
    uint32_t refs;
    double example[]={0.5,-1.25,0.0,1000000,-1000000,0.5};
    double expected[6];
    int i;
    if (!system || system->pVTbl->QueryInterface(system, &GID_IEcoSystem, (void**)&sys) != 0 || !sys) return 1;
    if (sys->pVTbl->QueryInterface(sys, &IID_IEcoInterfaceBus1, (void**)&bus) != 0 || !bus) { sys->pVTbl->Release(sys); return 2; }
    if (bus->pVTbl->RegisterComponent(bus, &CID_EcoLab65, (IEcoUnknown*)GetIEcoComponentFactoryPtr_AE784DFA9C5E49ECA39F168BF4120672) != 0 ||
        bus->pVTbl->QueryComponent(bus, &CID_EcoLab65, 0, &IID_IEcoLab65, (void**)&lab) != 0 || !lab) {
        bus->pVTbl->Release(bus); sys->pVTbl->Release(sys); return 3;
    }
    check(lab->pVTbl->QueryInterface(lab,&IID_IEcoUnknown,(void**)&unknown)==0 && unknown!=0,"ACOM QueryInterface Unknown");
    if(unknown) unknown->pVTbl->Release(unknown);
    unknown=0;
    check(lab->pVTbl->QueryInterface(lab,&missing,(void**)&unknown)!=0 && unknown==0,"ACOM missing IID");
    check(lab->pVTbl->QueryInterface(lab,0,(void**)&unknown)!=0,"ACOM null IID");
    check(lab->pVTbl->QueryInterface(lab,&IID_IEcoUnknown,0)!=0,"ACOM null output");
    refs=lab->pVTbl->AddRef(lab); check(refs>=2 && lab->pVTbl->Release(lab)==refs-1,"ACOM reference count");
    check(bus->pVTbl->QueryComponent(bus,&CID_EcoLab65,0,&IID_IEcoLab65,(void**)&second)==0 && second!=0,"ACOM second instance");
    if(second) second->pVTbl->Release(second);
    real_mem=((CEcoLab65_F4120672*)lab)->m_pIMem;
    ((CEcoLab65_F4120672*)lab)->m_pIMem=&proxy_mem;
    testInt(lab);
    testLong(lab);
    testFloat(lab);
    testDouble(lab);
    testLongDouble(lab);
    invalidFloat(lab);
    invalidDouble(lab);
    invalidLongDouble(lab);
    for(i=0;i<6;++i) expected[i]=example[i];
    qsort(expected,6,sizeof(double),cmpDouble);
    check(lab->pVTbl->SortDouble(lab,example,6,4)==0,"accuracy example");
    printf("ACCURACY,index,actual,reference,absolute_error\n");
    for(i=0;i<6;++i) printf("ACCURACY,%d,%.6f,%.6f,%.17g\n",i,example[i],expected[i],fabs(example[i]-expected[i]));
    check(live==0 && bad_free==0,"final allocator balance");
    ((CEcoLab65_F4120672*)lab)->m_pIMem=real_mem;
    maximumInt(lab);
    maximumLong(lab);
    maximumFloat(lab);
    maximumDouble(lab);
    maximumLongDouble(lab);
    printf("BUCKET_CHECK cases=%d failures=%d live_allocations=%d invalid_frees=%d sizeof_long=%lu sizeof_long_double=%lu LDBL_MANT_DIG=%d\n",cases,failures,live,bad_free,(unsigned long)sizeof(long),(unsigned long)sizeof(long double),LDBL_MANT_DIG);
    lab->pVTbl->Release(lab); bus->pVTbl->Release(bus); sys->pVTbl->Release(sys);
    return failures ? 4 : 0;
}
