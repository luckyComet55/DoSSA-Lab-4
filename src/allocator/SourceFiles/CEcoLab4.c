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

    if (me == 0 || ppv == 0) {
        return -1;
    }

    if (IsEqualUGUID(riid, &IID_IEcoLab4)) {
        *ppv = &pCMe->m_pVTblIEcoLab4;
        pCMe->m_pVTblIEcoLab4->AddRef((IEcoLab4*)pCMe);
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblIEcoLab4;
        pCMe->m_pVTblIEcoLab4->AddRef((IEcoLab4*)pCMe);
    } else {
        *ppv = 0;
        return -1;
    }

    return 0;
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

    return ++pCMe->m_cRef;
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

    --pCMe->m_cRef;

    if (pCMe->m_cRef == 0) {
        deleteCEcoLab4(me);
    }

    return pCMe->m_cRef;
}

ChunkPtr createMemoryChunk(IEcoMemoryAllocator1* pIMem, char* chunk_ptr, uint32_t chunk_size, ChunkPtr next) {
    ChunkPtr new_chunk = 0;

    new_chunk = pIMem->pVTbl->Alloc(pIMem, sizeof(Chunk));

    if (new_chunk == 0) {
        return 0;
    }

    new_chunk->chunk_size = chunk_size;
    new_chunk->is_free = TRUE;
    new_chunk->ptr = chunk_ptr;
    new_chunk->next = next;

    return new_chunk;
}

void deleteChunkList(IEcoMemoryAllocator1* pIMem, ChunkPtr start_node) {
    ChunkPtr current_ptr = start_node;
    ChunkPtr next_ptr = 0;

    if (start_node == 0) {
        return;
    }

    do {
        next_ptr = current_ptr->next;
        pIMem->pVTbl->Free(pIMem, (void*) current_ptr);
        current_ptr = next_ptr;
    } while (current_ptr != 0);

    return;
}

void freeChunk(ChunkPtr chunk) {
    if (chunk == 0) {
        return;
    }
    chunk->is_free = TRUE;
}

void mergeChunks(IEcoMemoryAllocator1* pIMem, ChunkPtr chunk_left, ChunkPtr chunk_right) {
    if (chunk_left == 0 || chunk_right == 0) {
        return;
    }

    if (chunk_left->is_free != TRUE) {
        printf("can't merge chunk %ld: chunk is in use\n", chunk_left->ptr);
        return;
    } else if (chunk_right->is_free != TRUE) {
        printf("can't merge chunk %ld: chunk is in use\n", chunk_left->ptr);
        return;
    }

    chunk_left->chunk_size += chunk_right->chunk_size;
    chunk_left->next = chunk_right->next;

    printf("merged chunks %ld (size %ld) and %ld (size %ld)\n", chunk_left->ptr, chunk_left->chunk_size, chunk_right->ptr, chunk_right->chunk_size);
    
    pIMem->pVTbl->Free(pIMem, chunk_right);
}

void vacuumBuffer(IEcoMemoryAllocator1* pIMem, ChunkPtr chunkList) {
    ChunkPtr current = chunkList;
    ChunkPtr next = 0;

    if (current == 0) {
        return;
    }

    next = current->next;

    while (next != 0) {
        while (next != 0 && current->is_free == TRUE && next->is_free == TRUE) {
            mergeChunks(pIMem, current, next);
            next = current->next;
        }
        if (current->next == 0) {
            break;
        }
        current = current->next;
        next = current->next;
    }
}


void* ECOCALLMETHOD CEcoLab4_alloc(
    struct IEcoLab4* me,
    uint32_t size_to_alloc
) {
    CEcoLab4* pCMe = (CEcoLab4*) me;
    IEcoMemoryAllocator1* pIMem = pCMe->m_pIMem;
    ChunkPtr bestFitChunk = pCMe->chunk_list;
    ChunkPtr newChunk = 0;
    uint32_t size_delta = 0;
    uint32_t min_size_delta = (uint32_t)(-1);
    bool_t found_chunk = FALSE;

    while (bestFitChunk != 0) {
        if (size_to_alloc > bestFitChunk->chunk_size) {
            bestFitChunk = bestFitChunk->next;
            continue;
        }

        size_delta = bestFitChunk->chunk_size - size_to_alloc;

        if (size_delta < min_size_delta) {
            min_size_delta = size_delta;
            found_chunk = TRUE;
        }

        bestFitChunk = bestFitChunk->next;
    }

    if (found_chunk == FALSE) {
        printf("could not allocate %ld bytes\n", size_to_alloc);
        return 0;
    }

    newChunk = createMemoryChunk(pIMem, bestFitChunk->ptr + size_to_alloc, bestFitChunk->chunk_size - size_to_alloc, bestFitChunk->next);

    bestFitChunk->is_free = FALSE;
    bestFitChunk->chunk_size = size_to_alloc;
    bestFitChunk->next = newChunk;

    return (void*)bestFitChunk->ptr;
}

int16_t ECOCALLMETHOD CEcoLab4_dealloc(
    struct IEcoLab4* me,
    void* ptr
) {
    CEcoLab4* pCMe = (CEcoLab4*) me;
    IEcoMemoryAllocator1* pIMem = pCMe->m_pIMem;
    ChunkPtr chunk_ptr = pCMe->chunk_list;
    bool_t freed_chunk = FALSE;
    char* ptr_actual = (char*) ptr;

    while (chunk_ptr != 0) {
        if (chunk_ptr->ptr == ptr_actual && chunk_ptr->is_free == FALSE) {
            printf("found chunk to free: %ld (size %ld)\n", chunk_ptr->ptr, chunk_ptr->chunk_size);
            chunk_ptr->is_free = TRUE;
            freed_chunk = TRUE;
            break;
        }
        chunk_ptr = chunk_ptr->next;
    }

    if (freed_chunk == FALSE) {
        printf("error while deallocating ptr %ld: could not deallocate\n", ptr_actual);
        return -1;
    }

    vacuumBuffer(pIMem, pCMe->chunk_list);

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
    int16_t result = -1;

    if (me == 0) {
        return result;
    }

    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

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

    pCMe->m_Name = 0;

    pCMe->buffer_default_size = 2 * sizeof(int) * 1024 * 1024;

    pCMe->buffer = (char*)pIMem->pVTbl->Alloc(pIMem, pCMe->buffer_default_size);

    if (pCMe->buffer == 0) {
        printf("error while allocating allocator buffer of size %ld\n", pCMe->buffer_default_size);
        return -1;
    } else {
        printf("successfully allocated buffer of size %ld at ptr %ld\n", pCMe->buffer_default_size, pCMe->buffer);
    }

    pCMe->chunk_list = createMemoryChunk(pIMem, pCMe->buffer, pCMe->buffer_default_size, 0);

    if (pCMe->chunk_list == 0) {
        printf("error while initializing chink list\n");
        return -1;
    }

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

        // delete chunk list
        deleteChunkList(pIMem, pCMe->chunk_list);
        // free memory allocated for buffer
        pIMem->pVTbl->Free(pIMem, pCMe->buffer);

        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
