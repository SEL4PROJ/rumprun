/*
 * Copyright 2017, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#pragma once
#include <autoconf.h>
#include <vka/vka.h>
#include <vspace/vspace.h>
#include <sel4utils/thread.h>
#include <sel4utils/process.h>
#include <sel4utils/mapping.h>
#include <sync/bin_sem.h>

#include <sel4platsupport/timer.h>
#include <platsupport/timer.h>

#include <sel4/kernel.h>

struct env {
    /* An initialised vka that may be used by the test. */
    vka_t vka;
    /* virtual memory management interface */
    vspace_t vspace;
    /* initialised timer */
    seL4_timer_t *timer;
    /* abstract interface over application init */
    simple_t simple;
    /* notification for timer */
    vka_object_t timer_notification;
    vka_object_t pci_notification;
    vka_object_t halt_notification;
    vka_object_t spl_notification;
    sel4utils_thread_t timing_thread;
    sel4utils_thread_t pci_thread;

    sync_bin_sem_t spl_semaphore;
    sync_bin_sem_t halt_semaphore;
    /* IO Ops */
    ps_io_ops_t io_ops;
    /* Irq Handler caps for PCI devices */
    seL4_CPtr caps[BMK_MAXINTR];
    /* Thread local storage base ptr */
    void *tls_base_ptr;
    /* Rumprun cmdline */
    // char cmdline[4096];
    /* Priority level for disabling interrupt thread */
    volatile int spldepth;
    /* Guard from preventing interrupt thread from disabling itsel4 */
    volatile int mask_the_mask;
    /* The PCI interrupt handler thread should wake up the runner thread */
    volatile bool should_wakeup;

};
typedef struct env *env_t;

extern struct env env;

int arch_init_timer(env_t env);
void x86_initclocks(void);
void simple_init_rumprun(simple_t *simple, seL4_CPtr endpoint);
int custom_simple_vspace_bootstrap_frames(simple_t *simple, vspace_t *vspace, sel4utils_alloc_data_t *alloc_data,
                            vka_t *vka);
int custom_get_priority(simple_t *simple);
char *custom_get_cmdline(simple_t *simple);
