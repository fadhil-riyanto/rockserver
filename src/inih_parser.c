#include "../header/inih_reader.h"
#include <stdlib.h>
#include <string.h>

int inih_cb(void* user, const char* section, const char* name,
                        const char* value)
{
        struct config* pconfig = (struct config*)user;

        #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
        if (MATCH("server", "conf_port")) {
                pconfig->conf_port = atoi(value);
        } else if (MATCH("server", "max_conn")) {
                pconfig->max_conn = atoi(value);
        } else if (MATCH("server", "max_events")) {
                pconfig->max_events = atoi(value);
        } else if (MATCH("server", "child_maxevents")) {
                pconfig->child_maxevents = atoi(value);
        } else if (MATCH("server", "db_path")) {
                pconfig->db_path = strdup(value);
        } else if (MATCH("server", "io_uring_queue_depth")) {
                pconfig->io_uring_queue_depth = atoi(value);
        } 
        else {
                return 0;  /* unknown section/name, error */
        }
        return 1;
}