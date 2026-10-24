/* Static factory; no per-instance template or hidden second initialization. */
#include "CEcoLab2.h"
#include "CEcoLab2Factory.h"

static uint32_t ECOCALLMETHOD factory_addref(IEcoComponentFactory* me) {
    CEcoLab2Factory* factory = (CEcoLab2Factory*)me;
    if (!factory) return 0;
    return ++factory->references;
}

static uint32_t ECOCALLMETHOD factory_release(IEcoComponentFactory* me) {
    CEcoLab2Factory* factory = (CEcoLab2Factory*)me;
    if (!factory || !factory->references) return 0;
    return --factory->references; /* Factory storage is static. */
}

static int16_t ECOCALLMETHOD factory_query(IEcoComponentFactory* me, const UGUID* iid, void** out) {
    if (!out) return ERR_ECO_POINTER;
    *out = 0;
    if (!me || !iid) return ERR_ECO_POINTER;
    if (!IsEqualUGUID(iid, &IID_IEcoUnknown) && !IsEqualUGUID(iid, &IID_IEcoComponentFactory)) {
        return ERR_ECO_NOINTERFACE;
    }
    *out = me;
    factory_addref(me);
    return 0;
}

static int16_t ECOCALLMETHOD factory_alloc(IEcoComponentFactory* me, IEcoUnknown* system,
        IEcoUnknown* outer, const UGUID* iid, void** out) {
    if (!out) return ERR_ECO_POINTER;
    *out = 0;
    if (!me) return ERR_ECO_POINTER;
    return createEcoLab2(system, outer, iid, out);
}

static int16_t ECOCALLMETHOD factory_init(IEcoComponentFactory* me, IEcoUnknown* system, void* value) {
    (void)value;
    return me && system ? ERR_ECO_SUCCESS : ERR_ECO_POINTER;
}

static char_t* ECOCALLMETHOD factory_name(IEcoComponentFactory* me) {
    return me ? "EcoLab2" : 0;
}

static char_t* ECOCALLMETHOD factory_version(IEcoComponentFactory* me) {
    return me ? "2.0.0" : 0;
}

static char_t* ECOCALLMETHOD factory_manufacturer(IEcoComponentFactory* me) {
    return me ? "" : 0;
}

static IEcoComponentFactoryVTbl factory_vtable = {
    factory_query, factory_addref, factory_release, factory_alloc, factory_init,
    factory_name, factory_version, factory_manufacturer
};
static CEcoLab2Factory factory = {&factory_vtable, 0};

#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr(void) {
    return (IEcoComponentFactory*)&factory;
}
#elif ECO_LIB
IEcoComponentFactory* GetIEcoComponentFactoryPtr_C776000EC38746C089AD38BFB1C8BB9C = (IEcoComponentFactory*)&factory;
#endif
