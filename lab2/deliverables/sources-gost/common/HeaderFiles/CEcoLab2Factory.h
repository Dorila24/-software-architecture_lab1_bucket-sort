/* Private state of the statically stored component factory. */
#ifndef ARPO_C_EcoLab2_FACTORY_H
#define ARPO_C_EcoLab2_FACTORY_H

#include "IEcoSystem1.h"

typedef struct CEcoLab2Factory {
    IEcoComponentFactoryVTbl* vtable;
    uint32_t references;
} CEcoLab2Factory;

#endif
