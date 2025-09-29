#ifndef __WD_H__
#define __WD_H__

#include <stddef.h>
#include <sys/types.h>

typedef enum wd_status
{
    SUCCESS,
    FAIL
} wd_status_e;

/* "Make me immortal" */
/* Start watchdog protection
* interval - heartbeat interval (seconds)
* tolerance - number of missed heartbeats allowed
* argv/argc - original argv/argc of the user process (may be NULL)
*/
wd_status_e WDStart(unsigned int interval, unsigned int tolerance, const char **argv, int argc);
/* "Do not resuscitate" */
/* Stop watchdog protection cleanly */
wd_status_e WDStop(void);

#endif /* __WD_H__ */
