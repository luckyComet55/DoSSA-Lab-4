#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab4.h"
#include <stdlib.h>
#include <stdio.h>

int16_t test(IEcoLab4* this) {
    int16_t result = 0;
    int* ptr = 0;
    int* arr_ptr = 0;
    size_t allocating_size = sizeof(int);
    size_t arr_size = 10 * sizeof(int);

    ptr = (int*)this->pVTbl->alloc(this, allocating_size);
    if (ptr != 0) {
        printf("sucessfully allocated %ld bytes at %lld\n", allocating_size, ptr);
    } else {
        printf("error while allocating %ld bytes, terminating\n", allocating_size);
        result = -1;
        goto Termination;
    }

    arr_ptr = (int*)this->pVTbl->alloc(this, arr_size);
    if (arr_ptr != 0) {
        printf("sucessfully allocated %ld bytes at %lld\n", arr_size, arr_ptr);
    } else {
        printf("error while allocating %ld bytes, terminating\n", arr_size);
        result = -1;
        goto Termination;
    }

    result = this->pVTbl->dealloc(this, ptr);
    if (result == 0) {
        printf("sucessfully deallocated at %lld\n", ptr);
    } else {
        printf("error while deallocating at %lld, terminating\n", ptr);
        result = -1;
        goto Termination;
    }

    result = this->pVTbl->dealloc(this, arr_ptr);
    if (result == 0) {
        printf("sucessfully deallocated at %lld\n", arr_ptr);
    } else {
        printf("error while deallocating at %lld, terminating\n", arr_ptr);
        result = -1;
        goto Termination;
    }

Termination:

    return result;
}

int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    char_t* copyName = 0;
    FILE* out_file;
    errno_t err_code = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab4* pIEcoLab4 = 0;

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem1, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab4, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3C);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif
    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }


    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab4, 0, &IID_IEcoLab4, (void**) &pIEcoLab4);
    if (result != 0 || pIEcoLab4 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

    // утановка ГПСЧ
    srand(time(0));

    result = test(pIEcoLab4);

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab4 != 0) {
        pIEcoLab4->pVTbl->Release(pIEcoLab4);
    }


    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}