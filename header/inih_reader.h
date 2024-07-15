#ifndef INIH_CONFIG_H
#define INIH_CONFIG_H

struct config {
        int conf_port;
        int max_conn;
        int max_events;
        int child_maxevents;
};

int inih_cb(void* user, const char* section, const char* name,
                        const char* value);

#endif