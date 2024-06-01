#ifndef __I_ECOLAB4_H__
#define __I_ECOLAB4_H__

#include "IEcoBase1.h"

/* IEcoLab1 IID = {277FC00C-3562-4096-AFCF-C125B94EEC90} */
#ifndef __IID_IEcoLab4
static const UGUID IID_IEcoLab4 = {0x01, 0x10, 0x27, 0x7F, 0xC0, 0x0C, 0x35, 0x62, 0x40, 0x96, 0xAF, 0xCF, 0xC1, 0x25, 0xB9, 0x4E, 0xEC, 0x90};
#endif /* __IID_IEcoLab4 */

typedef struct IEcoLab4VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ struct IEcoLab4* me, /* in */ const UGUID* riid, /* out */ void **ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ struct IEcoLab4* me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ struct IEcoLab4* me);

    uint64_t (ECOCALLMETHOD *alloc)(
        struct IEcoLab4* me,
        size_t size_to_alloc
    );

    int16_t (ECOCALLMETHOD *dealloc)(
        struct IEcoLab4* me,
        uint64_t ptr
    );

} IEcoLab4VTbl, *IEcoLab4VTblPtr;

interface IEcoLab4 {
    struct IEcoLab4VTbl *pVTbl;
} IEcoLab4;


#endif /* __I_ECOLAB4_H__ */
