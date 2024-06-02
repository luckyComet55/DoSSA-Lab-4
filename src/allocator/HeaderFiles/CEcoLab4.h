#ifndef __C_ECOLAB4_H_
#define __C_ECOLAB4_H_

#include "IEcoLab4.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"


typedef struct Chunk {

    bool_t is_free;

    char* ptr;

    uint32_t chunk_size;

    struct Chunk* next;

} Chunk, *ChunkPtr;

typedef struct CEcoLab4 {

    IEcoLab4VTbl* m_pVTblIEcoLab4;

    uint32_t m_cRef;

    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;

    /* Системный интерфейс */
    IEcoSystem1* m_pISys;

    /* Данные экземпляра */
    char_t* m_Name;

    ChunkPtr chunk_list;

    uint32_t buffer_default_size;

    char* buffer;

} CEcoLab4, *CEcoLab4Ptr;

/* Инициализация экземпляра */
int16_t ECOCALLMETHOD initCEcoLab4(/*in*/ struct IEcoLab4* me, /* in */ IEcoUnknown *pIUnkSystem);
/* Создание экземпляра */
int16_t ECOCALLMETHOD createCEcoLab4(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab4** ppIEcoLab4);
/* Удаление */
void ECOCALLMETHOD deleteCEcoLab4(/* in */ IEcoLab4* pIEcoLab4);

#endif