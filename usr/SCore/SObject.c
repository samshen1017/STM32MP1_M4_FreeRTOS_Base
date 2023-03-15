#include "SObject.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <FreeRTOS.h>

#define OBJ_CONTAINER_LIST_INIT(c)                                 \
    {                                                              \
        &(obj_container[c].obj_list), &(obj_container[c].obj_list) \
    }

static SObject_Info obj_container[SObject_Unknown] = {
    {SObject_Null, OBJ_CONTAINER_LIST_INIT(SObject_Null)},
#ifdef S_USING_MODULE
    {SObject_Module, OBJ_CONTAINER_LIST_INIT(SObject_Module)},
#endif
#ifdef S_USING_SESSION
    {SObject_Session, OBJ_CONTAINER_LIST_INIT(SObject_Session)},
#endif
#ifdef S_USING_CONFIG
    {SObject_Config, OBJ_CONTAINER_LIST_INIT(SObject_Config)},
#endif
#ifdef S_USING_SCHEDULER
    {SObject_Scheduler, OBJ_CONTAINER_LIST_INIT(SObject_Scheduler)},
#endif
#ifdef S_USING_STATICSECTION
    {SObject_StaticSection, OBJ_CONTAINER_LIST_INIT(SObject_StaticSection)},
#endif
};

/* 静态初始化 */
_err_t SObject_init(SObject *obj, SObject_Class_Type type, const char *name)
{
    /* get object information */
    SObject_Info *info = SObject_get_info(type);

    assert(obj != NULL);
    assert(info != NULL);

    /* initialize object's parameters */
    /* set object type */
    obj->type = type;

    /* copy name */
    strncpy(obj->name, name, SOBJ_NAME_SIZE);

    /* initialize lock */
    obj->lock = xSemaphoreCreateMutex();
    if (obj->lock == NULL)
    {
        printf("xSemaphoreCreateMutex failed.\r\n");
        return _ERROR;
    }

    /* insert object into information object list */
    rlist_insert_after(&(info->obj_list), &(obj->list));
    return _EOK;
}

_err_t SObject_detach(SObject *obj)
{
    /* object check */
    assert(obj != NULL);

    /* reset object type */
    obj->type = SObject_Null;
    /* remove from list */
    rlist_remove(&(obj->list));
    /* delete mutex */
    vSemaphoreDelete(obj->lock);

    return _EOK;
}

/* 动态初始化 */
SObject *SObject_allocate(SObject_Class_Type type, const char *name)
{
    _err_t res;

    SObject *obj = (SObject *)pvPortMalloc(sizeof(SObject));
    if (obj == NULL)
    {
        printf("no memory can be allocated.\r\n");
        /* no memory can be allocated */
        goto _NO_MEM;
    }

    /* clean memory data of object */
    memset(obj, 0, sizeof(SObject));

    res = SObject_init(obj, type, name);
    if (res != _EOK)
    {
        printf("SObject init failed.\r\n");
        goto _INIT_ERR;
    }

    /* return object */
    return obj;

_INIT_ERR:
    vPortFree(obj);

_NO_MEM:
    return NULL;
}

void SObject_delete(SObject *obj)
{
    /* object check */
    assert(obj != NULL);

    SObject_detach(obj);

    /* free the memory of object */
    vPortFree(obj);
}

SObject_Info *SObject_get_info(SObject_Class_Type type)
{
    int index;
    for (index = 0; index < SObject_Unknown; index++)
    {
        if (obj_container[index].type == type)
        {
            return &obj_container[index];
        }
    }
    return NULL;
}

size_t SObject_get_type(SObject *obj)
{
    assert(obj != NULL);
    return obj->type;
}

SObject *SObject_find(SObject_Class_Type type, const char *name)
{
    SObject *obj = NULL;
    struct rlist_node *node = NULL;
    SObject_Info *info = NULL;

    /* parameter check */
    if ((name == NULL) || (type > SObject_Unknown))
        return NULL;
    /* try to find object */
    if (info == NULL)
    {
        info = SObject_get_info((SObject_Class_Type)type);
        assert(info != NULL);
    }
    for (node = info->obj_list.next; node != &(info->obj_list); node = node->next)
    {
        obj = rlist_entry(node, SObject, list);
        if (strncmp(obj->name, name, SOBJ_NAME_SIZE) == 0)
        {
            return obj;
        }
    }
    return NULL;
}

_err_t SObject_take(SObject *obj)
{
    assert(obj != NULL);
    if (xSemaphoreTake(obj->lock, portMAX_DELAY) == pdPASS)
    {
        return _EOK;
    }
    return _ERROR;
}

_err_t SObject_release(SObject *obj)
{
    assert(obj != NULL);
    if (xSemaphoreGive(obj->lock) == pdPASS)
    {
        return _EOK;
    }
    return _ERROR;
}

const char *SObject_type_to_str(SObject_Class_Type t)
{
    /* 用于枚举转换字符串 */
#define ENUM_TYPE_CASE(x) \
    case x:               \
        return (#x);

    switch (t)
    {
#ifdef S_USING_MODULE
        ENUM_TYPE_CASE(SObject_Module)
#endif
#ifdef S_USING_SESSION
        ENUM_TYPE_CASE(SObject_Session)
#endif
#ifdef S_USING_THREAD
        ENUM_TYPE_CASE(SObject_Thread)
#endif
#ifdef S_USING_CONFIG
        ENUM_TYPE_CASE(SObject_Config)
#endif
#ifdef S_USING_DATA
        ENUM_TYPE_CASE(SObject_Data)
#endif
#ifdef S_USING_SENSOR
        ENUM_TYPE_CASE(SObject_Sensor)
#endif
#ifdef S_USING_SEMAPHORE
        ENUM_TYPE_CASE(SObject_Semaphore)
#endif
#ifdef S_USING_MSGQUEUE
        ENUM_TYPE_CASE(SObject_MsgQueue)
#endif
#ifdef S_USING_MUTEX
        ENUM_TYPE_CASE(SObject_Mutex)
#endif
#ifdef S_USING_SCHEDULER
        ENUM_TYPE_CASE(SObject_Scheduler)
#endif
#ifdef S_USING_STATICSECTION
        ENUM_TYPE_CASE(SObject_StaticSection)
#endif
#ifdef S_USING_EVENT
        ENUM_TYPE_CASE(SObject_Event)
#endif
    default:
        break;
    }
    return "SObject_Unknown";
}
