/* Private state of the statically stored component factory. */
#ifndef ARPO_C_EcoLab65_FACTORY_H
#define ARPO_C_EcoLab65_FACTORY_H

#include "IEcoSystem1.h"

typedef struct CEcoLab65Factory {
    IEcoComponentFactoryVTbl* vtable;
    uint32_t references;
} CEcoLab65Factory;

#endif
