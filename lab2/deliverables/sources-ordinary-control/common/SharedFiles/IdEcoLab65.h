/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoLab65
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoLab65
 * </description>
 *
 * <reference>
 *
 * </reference>
 *
 * <author>
 *   Copyright (c) 2026 . All rights reserved.
 * </author>
 *
 */

#ifndef __ID_ECOLAB65_H__
#define __ID_ECOLAB65_H__

#include "IEcoBase1.h"
#include "IEcoLab65.h"

/* EcoLab65 CID = {AE784DFA-9C5E-49EC-A39F-168BF4120672} */
#ifndef __CID_EcoLab65
static const UGUID CID_EcoLab65 = {0x01, 0x10, {0xAE, 0x78, 0x4D, 0xFA, 0x9C, 0x5E, 0x49, 0xEC, 0xA3, 0x9F, 0x16, 0x8B, 0xF4, 0x12, 0x06, 0x72}};
#endif /* __CID_EcoLab65 */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_AE784DFA9C5E49ECA39F168BF4120672;
#endif

#endif /* __ID_ECOLAB65_H__ */

