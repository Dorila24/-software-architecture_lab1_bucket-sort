/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoLab2
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoLab2
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

#ifndef __ID_ECOLAB2_H__
#define __ID_ECOLAB2_H__

#include "IEcoBase1.h"
#include "IEcoLab2.h"

/* EcoLab2 CID = {C776000E-C387-46C0-89AD-38BFB1C8BB9C} */
#ifndef __CID_EcoLab2
static const UGUID CID_EcoLab2 = {0x01, 0x10, {0xC7, 0x76, 0x00, 0x0E, 0xC3, 0x87, 0x46, 0xC0, 0x89, 0xAD, 0x38, 0xBF, 0xB1, 0xC8, 0xBB, 0x9C}};
#endif /* __CID_EcoLab2 */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_C776000EC38746C089AD38BFB1C8BB9C;
#endif

#endif /* __ID_ECOLAB2_H__ */

