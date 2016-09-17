#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "dispatch-table.h"
#include "handlers.h"
#include "structs.h"
#include "list.h"

// dispatch-table.c
// borrows heavily from borja sotomayor's code at
// https://github.com/uchicago-cs/cmsc23300/blob/master/samples/dispatch_table/dispatch-str-two.c

typedef int (*handler_function)(cmdValueStruct cmdValueInstance, user* myUser, list* userList);

typedef struct handler_entry
{
    char *name;
    handler_function func;
}handler_entry;

#define HANDLER_ENTRY(NAME) { #NAME, handle_ ## NAME}

handler_entry handlers[] = {
                                HANDLER_ENTRY(NICK), 
                            	HANDLER_ENTRY(USER), 
                            	HANDLER_ENTRY(QUIT),
                                HANDLER_ENTRY(PRIVMSG),
                                HANDLER_ENTRY(NOTICE),
                                HANDLER_ENTRY(LUSERS),
                                HANDLER_ENTRY(WHOIS),
                                HANDLER_ENTRY(MOTD),
                                HANDLER_ENTRY(PING),
                                HANDLER_ENTRY(PONG),
                                HANDLER_ENTRY(UNKNOWN),
				                HANDLER_ENTRY(JOIN),
                                HANDLER_ENTRY(PART),
                                HANDLER_ENTRY(TOPIC),
                                HANDLER_ENTRY(OPER),
                                HANDLER_ENTRY(MODE),
				HANDLER_ENTRY(AWAY),
				HANDLER_ENTRY(NAMES),
                                HANDLER_ENTRY(LIST),
                                HANDLER_ENTRY(WHO)
        					}; 

int num_handlers = sizeof(handlers) / sizeof(handler_entry);
int dispatch(cmdValueStruct cmdValueInstance, user* myUser, list* userList)
{
	char *code = cmdValueInstance.cmd;
    int rc, i;

    for(i=0; i<num_handlers; i++)
        if (!strcmp(handlers[i].name, code))
        {
	
	
            rc = handlers[i].func(cmdValueInstance, myUser, userList);
	
	
            printf("handler %d returned code %d\n", i, rc);
            break;
        }

    if(i == num_handlers)
    {
            rc = handle_UNKNOWN(cmdValueInstance, myUser, userList);
            printf("handler handled unknown command\n");
    }

    return 1;
}
