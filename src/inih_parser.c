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
        } else if (MATCH("server", "key_max_length")) {
                pconfig->key_max_length = atoi(value);
        } else if (MATCH("server", "buffer_max_length")) {
                pconfig->buffer_max_length = atoi(value);
        } else if (MATCH("server", "show_client_query")) {
                pconfig->show_client_query = atoi(value);
        } else {
                return 0;  /* unknown section/name, error */
        }
        return 1;
}