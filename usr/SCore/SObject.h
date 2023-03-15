#ifndef SOBJECT_H
#define SOBJECT_H

#include <FreeRTOS.h>
#include <semphr.h>

#include "SCoreConf.h"
#include "rlist.h"

typedef enum
{
    SObject_Null = 0,
#ifdef S_USING_MODULE
    SObject_Module,
#endif
#ifdef S_USING_SESSION
    SObject_Session,
#endif
#ifdef S_USING_CONFIG
    SObject_Config,
#endif
#ifdef S_USING_SCHEDULER
    SObject_Scheduler,
#endif
#ifdef S_USING_STATICSECTION
    SObject_StaticSection,
#endif
    SObject_Unknown,
} SObject_Class_Type;

#define SOBJ_NAME_SIZE 12

typedef struct
{
    char name[SOBJ_NAME_SIZE];
    SObject_Class_Type type;
    SemaphoreHandle_t lock;
    rlist_t list; // kernel list node
} SObject;

typedef struct
{
    SObject_Class_Type type;
    rlist_t obj_list; // head
} SObject_Info;

SObject *SObject_find(SObject_Class_Type type, const char *name);

_err_t SObject_init(SObject *obj, SObject_Class_Type type, const char *name);
_err_t SObject_detach(SObject *obj);

SObject *SObject_allocate(SObject_Class_Type type, const char *name);
void SObject_delete(SObject *obj);

_err_t SObject_take(SObject *obj);
_err_t SObject_release(SObject *obj);

SObject_Info *SObject_get_info(SObject_Class_Type type);
size_t SObject_get_type(SObject *obj);
const char *SObject_type_to_str(SObject_Class_Type t);

#endif // S_OBJECT_H
