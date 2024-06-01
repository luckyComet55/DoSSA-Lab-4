/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab4
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab4
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab4.h"
#include "IdEcoLab4.h"
#include "IEcoConnectionPointContainer.h"


/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoLab4
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab4_QueryInterface(/* in */ struct IEcoLab4* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab4* pCMe = (CEcoLab4*)me;
    IEcoUnknown* nonDelegating = (IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk;
    int16_t result = 0;

    if (me == 0 || ppv == 0) {
        return -1;
    }

    result = nonDelegating->pVTbl->QueryInterface(nonDelegating, riid, ppv);
    if (result != 0 && pCMe->m_pIUnkOuter != 0) {
        result = pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
    }

    return result;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoLab4
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab4_AddRef(/* in */ struct IEcoLab4* me) {
    CEcoLab4* pCMe = (CEcoLab4*)me;

    if (me == 0 ) {
        return -1;
    }

    return pCMe->m_pVTblINondelegatingUnk->AddRef((IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk);
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoLab4
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab4_Release(/* in */ struct IEcoLab4* me) {
    CEcoLab4* pCMe = (CEcoLab4*)me;

    if (me == 0 ) {
        return -1;
    }

    return pCMe->m_pVTblINondelegatingUnk->AddRef((IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk);
}

int16_t ECOCALLMETHOD CEcoLab4_NondelegatingQueryInterface(/* in */ struct IEcoUnknown * me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab4* pCMe = (CEcoLab4*)((uint64_t)me - sizeof(struct IEcoLab4*));
    int16_t result;
    if (me == 0 || ppv == 0) {
        return -1;
    }
    if (IsEqualUGUID(riid, &IID_IEcoLab4) ) {
        *ppv = &pCMe->m_pVTblIEcoLab4;
        ++pCMe->m_cRef;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblINondelegatingUnk;
        ++pCMe->m_cRef;
    }
    else {
        *ppv = 0;
        return -1;
    }
    return 0;
}

uint32_t ECOCALLMETHOD CEcoLab4_NondelegatingAddRef(/* in */ struct IEcoUnknown* me) {
    CEcoLab4* pCMe = (CEcoLab4*)((uint64_t)me - sizeof(struct IEcoLab4*) - sizeof(struct IEcoCalculatorY*) - sizeof(struct IEcoCalculatorX*));

    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

uint32_t ECOCALLMETHOD CEcoLab4_NondelegatingRelease(/* in */ struct IEcoUnknown* me) {
    CEcoLab4* pCMe = (CEcoLab4*)((uint64_t)me - sizeof(struct IEcoLab4*) - sizeof(struct IEcoCalculatorY*) - sizeof(struct IEcoCalculatorX*));

    if (me == 0 ) {
        return -1;
    }

    --pCMe->m_cRef;

    if ( pCMe->m_cRef == 0 ) {
        if (pCMe->m_pInnerUnknown != 0) {
            if (pCMe->m_pInnerUnknown->pVTbl->Release(pCMe->m_pInnerUnknown) == 0) {
                pCMe->m_pInnerUnknown = 0;
            } else {
                pCMe->m_cRef = 1;
            }
        }
        if (pCMe->m_cRef == 0) {
            deleteCEcoLab4((IEcoLab4*)pCMe);
        }
        return 0;
    }
    return pCMe->m_cRef;
}

uint64_t ECOCALLMETHOD CEcoLab4_alloc(
    struct IEcoLab4* me,
    size_t size_to_alloc
) {
    CEcoLab4* pCMe = (CEcoLab4*) me;
    printf("alloc %lld bytes\n", size_to_alloc);
    return 0;
}

int16_t ECOCALLMETHOD CEcoLab4_dealloc(
    struct IEcoLab4* me,
    uint64_t ptr
) {
    CEcoLab4* pCMe = (CEcoLab4*) me;
    printf("dealloc at %lld\n", ptr);
    return 0;
}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD initCEcoLab4(/*in*/ struct IEcoLab4* me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab4* pCMe = (CEcoLab4*)me;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoUnknown* pOuterUnknown = (IEcoUnknown*) me;
    int16_t result = -1;

    if (me == 0 ) {
        return result;
    }

    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab4, pOuterUnknown, &IID_IEcoUnknown, (void**)&pCMe->m_pInnerUnknown);

    if (me == 0 ) {
        return result;
    }

    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    pIBus->pVTbl->Release(pIBus);
	
    return result;
}

/* Create Virtual Table IEcoLab4 */
IEcoLab4VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
    CEcoLab4_QueryInterface,
    CEcoLab4_AddRef,
    CEcoLab4_Release,
    CEcoLab4_alloc,
    CEcoLab4_dealloc
};

IEcoUnknownVTbl g_x000000000000000000000000000000AAVTblLab1 = {
        CEcoLab4_NondelegatingQueryInterface,
        CEcoLab4_NondelegatingAddRef,
        CEcoLab4_NondelegatingRelease
};


/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoLab4(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab4** ppIEcoLab4) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab4* pCMe = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;
	
    if (ppIEcoLab4 == 0 || pIUnkSystem == 0) {
        return result;
    }

    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem1, (void **)&pISys);

    if (result != 0 && pISys == 0) {
        return result;
    }

    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    if (result != 0 && pIMem == 0) {
        pISys->pVTbl->Release(pISys);
        return result;
    }

    pCMe = (CEcoLab4*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab4));

    pCMe->m_pISys = pISys;

    pCMe->m_pIMem = pIMem;

    pCMe->m_cRef = 1;

    pCMe->m_pVTblIEcoLab4 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

    pCMe->m_pVTblINondelegatingUnk = &g_x000000000000000000000000000000AAVTblLab1;

    pCMe->m_pIUnkOuter = 0;
    if (pIUnkOuter != 0) {
        pCMe->m_pIUnkOuter = pIUnkOuter;
    } else {
        pCMe->m_pIUnkOuter = (IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk;
    }

    pCMe->m_Name = 0;

    pCMe->m_pInnerUnknown = 0;

    *ppIEcoLab4 = (IEcoLab4*)pCMe;

    pIBus->pVTbl->Release(pIBus);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoLab4(/* in */ IEcoLab4* pIEcoLab4) {
    CEcoLab4* pCMe = (CEcoLab4*)pIEcoLab4;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab4 != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}