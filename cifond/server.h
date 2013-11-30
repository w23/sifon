#ifndef __SERVER_H__
#define __SERVER_H__

struct keeper_t;

int server_run(struct keeper_t *keeper, const char *bind_host, int port);

#endif /*ifndef __SERVER_H__*/
