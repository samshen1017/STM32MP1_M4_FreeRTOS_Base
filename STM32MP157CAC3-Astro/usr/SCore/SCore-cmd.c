#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"

#include "SObject.h"
#include "SCoreConf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#if defined(S_USING_MODULE)
#include "SModule.h"

static BaseType_t list_module_cmd(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    SModule *mod = NULL;
    struct rlist_node *node = NULL;
    SObject_Info *info = NULL;

    int maxlen = SOBJ_NAME_SIZE;
    const char *item_title = "SModule";
    printf("%-*.*s Status\r\n", maxlen, SOBJ_NAME_SIZE, item_title);
    for (int i = 0; i < maxlen; i++)
    {
        printf("-");
    }
    printf(" ----------\r\n");

    info = SObject_get_info(SObject_Module);
    assert(info != NULL);
    for (node = info->obj_list.next; node != &(info->obj_list); node = node->next)
    {
        mod = (SModule *)rlist_entry(node, SObject, list);
        printf("%-*.*s %-20s\r\n", maxlen, SOBJ_NAME_SIZE, mod->parent.name, SModule_state_to_str(mod->state));
    }

    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;
}

static const CLI_Command_Definition_t list_module = {
    "list-mod",
    "list-mod\r\n",
    list_module_cmd, /* The function to run. */
    -1               /* The user can enter any number of commands. */
};

#endif

static BaseType_t list_obj_cmd(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    SObject *obj = NULL;
    struct rlist_node *node = NULL;
    SObject_Info *info = NULL;

    int maxlen = SOBJ_NAME_SIZE;
    const char *item_title = "SObject";
    printf("%-*.s         type         lock\r\n", maxlen, item_title);
    for (int i = 0; i < maxlen; i++)
    {
        printf("-");
    }
    printf(" -------------------- ----------\r\n");

/* SModule */
#ifdef S_USING_MODULE
    info = SObject_get_info(SObject_Module);
    assert(info != NULL);
    for (node = info->obj_list.next; node != &(info->obj_list); node = node->next)
    {
        obj = rlist_entry(node, SObject, list);
        printf("%-*.*s %-20s\r\n", maxlen, SOBJ_NAME_SIZE, obj->name, SObject_type_to_str(obj->type));
    }
#endif

/* SSession */
#ifdef S_USING_SESSION
    info = SObject_get_info(SObject_Session);
    assert(info != NULL);
    for (node = info->obj_list.next; node != &(info->obj_list); node = node->next)
    {
        obj = rlist_entry(node, SObject, list);
        printf("%-*.*s %-20s\r\n", maxlen, SOBJ_NAME_SIZE, obj->name, SObject_type_to_str(obj->type));
    }
#endif

/* StaticSection */
#ifdef S_USING_STATICSECTION
    info = SObject_get_info(SObject_StaticSection);
    assert(info != NULL);
    for (node = info->obj_list.next; node != &(info->obj_list); node = node->next)
    {
        obj = rlist_entry(node, SObject, list);
        printf("%-*.*s %-20s\r\n", maxlen, SOBJ_NAME_SIZE, obj->name, SObject_type_to_str(obj->type));
    }
#endif
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    return pdFALSE;
}

static const CLI_Command_Definition_t list_obj = {
    "list-obj",
    "list-obj\r\n",
    list_obj_cmd, /* The function to run. */
    -1            /* The user can enter any number of commands. */
};

void vRegisterSCoreCLICommands(void)
{
    FreeRTOS_CLIRegisterCommand(&list_obj);

#if defined(S_USING_MODULE)
    FreeRTOS_CLIRegisterCommand(&list_module);
#endif
}
