/**
 * @file fscaler_kpi.h
 * @brief Scaler KPI performance routines header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#ifndef _FSCALER_KPI_H_
#define _FSCALER_KPI_H_

#include <linux/time.h>
#include "fscaler_debug.h"
#include "fscaler_module.h"

#define FSCALER_KPI_SEC2USEC(x)    (1000000*(x))
#define FSCALER_KPI_SEC2MSEC(x)    (1000*(x))
#define FSCALER_KPI_TIME_PERIOD    FSCALER_KPI_SEC2USEC(5)

extern unsigned int video_gettime_max_ms(void);
extern unsigned int video_gettime_us(void);

#define FSCALER_KPI_TIME_MAX       (1000*video_gettime_max_ms())
#define FSCALER_KPI_GETTIME()      video_gettime_us()

typedef struct fscaler_kpi_time {
    int b_start;
    struct timeval max;
    struct timeval period;
    struct timeval start;
    struct timeval end;
    struct timeval acc;
    struct timeval cost;
    struct timeval first;
    struct timeval last;
    struct timeval elapse;
} fscaler_kpi_time_t;

typedef struct fscaler_kpi_value {
    int op;                     ///< 1: add -1:sub 0: eq
    unsigned int value;
    unsigned long acc;
    unsigned int acc_count;
} fscaler_kpi_value_t;

typedef struct fscaler_kpi {
    int index;
    struct fscaler_kpi_time time;
    struct fscaler_kpi_value value;
} fscaler_kpi_t;

static inline void fscaler_kpi_timeval_init(struct timeval *p_tval)
{
    if (p_tval) {
        p_tval->tv_sec = p_tval->tv_usec = 0;
    }
}

static inline void fscaler_kpi_timeval_format(struct timeval *p_tval)
{
    if (p_tval) {
        while (unlikely(p_tval->tv_usec >= 1000000)) {
            p_tval->tv_sec++;
            p_tval->tv_usec -= 1000000;
        }
        while (unlikely(p_tval->tv_usec <= -1000000)) {
            p_tval->tv_sec--;
            p_tval->tv_usec += 1000000;
        }
        if (unlikely(p_tval->tv_sec > 0 && p_tval->tv_usec < 0 && p_tval->tv_usec > -1000000)) {
            p_tval->tv_sec--;
            p_tval->tv_usec += 1000000;
        }
    }
}

static inline void fscaler_kpi_timeval_set(struct timeval *p_tval, long tv_sec, long tv_usec)
{
    if (p_tval) {
        p_tval->tv_sec = tv_sec;
        p_tval->tv_usec = tv_usec;
        fscaler_kpi_timeval_format(p_tval);
    }
}

static inline struct timeval fscaler_kpi_timeval_add(const struct timeval *lhs,
                                                     const struct timeval *rhs)
{
    struct timeval tval;
    fscaler_kpi_timeval_init(&tval);
    if (lhs && rhs) {
        tval.tv_sec = lhs->tv_sec + rhs->tv_sec;
        tval.tv_usec = lhs->tv_usec + rhs->tv_usec;
        fscaler_kpi_timeval_format(&tval);
    }
    return tval;
}

static inline struct timeval fscaler_kpi_timeval_sub(const struct timeval *lhs,
                                                     const struct timeval *rhs)
{
    struct timeval tval;
    fscaler_kpi_timeval_init(&tval);
    if (lhs && rhs) {
        tval.tv_sec = lhs->tv_sec - rhs->tv_sec;
        tval.tv_usec = lhs->tv_usec - rhs->tv_usec;
        fscaler_kpi_timeval_format(&tval);
    }
    return tval;
}

static inline int fscaler_kpi_timeval_compare(struct timeval *lhs, struct timeval *rhs)
{
    fscaler_kpi_timeval_format(lhs);
    fscaler_kpi_timeval_format(rhs);
    if (lhs->tv_sec < rhs->tv_sec)
        return -1;
    if (lhs->tv_sec > rhs->tv_sec)
        return 1;
    return lhs->tv_usec - rhs->tv_usec;
}

static inline unsigned int fscaler_kpi_timeval_to_usecs(struct timeval *p_tval)
{
    unsigned int ret = 0;
    if (p_tval) {
        ret = jiffies_to_usecs(timeval_to_jiffies(p_tval));
    }
    return ret;
}

static inline unsigned int fscaler_kpi_timeval_to_msecs(struct timeval *p_tval)
{
    unsigned int ret = 0;
    if (p_tval) {
        ret = jiffies_to_msecs(timeval_to_jiffies(p_tval));
    }
    return ret;
}

static inline void fscaler_kpi_time_init(struct fscaler_kpi_time *p_time)
{
    if (p_time) {
        p_time->b_start = 0;
        fscaler_kpi_timeval_set(&p_time->max, 0, FSCALER_KPI_TIME_MAX);
        fscaler_kpi_timeval_set(&p_time->period, 0, FSCALER_KPI_TIME_PERIOD);
        fscaler_kpi_timeval_init(&p_time->start);
        fscaler_kpi_timeval_init(&p_time->end);
        fscaler_kpi_timeval_init(&p_time->acc);
        fscaler_kpi_timeval_init(&p_time->cost);
        fscaler_kpi_timeval_init(&p_time->first);
        fscaler_kpi_timeval_init(&p_time->last);
        fscaler_kpi_timeval_init(&p_time->elapse);
    }
}

static inline struct timeval fscaler_kpi_timeval_elapse(struct timeval *p_tstart,
                                                        struct timeval *p_tend,
                                                        struct timeval *p_tmax)
{
    struct timeval elapse;
    fscaler_kpi_timeval_init(&elapse);
    if (p_tstart && p_tend && p_tmax) {
        if (fscaler_kpi_timeval_compare(p_tend, p_tstart) >= 0) {
            elapse = fscaler_kpi_timeval_sub(p_tend, p_tstart);
        } else {
            elapse = fscaler_kpi_timeval_sub(p_tmax, p_tstart);
            elapse = fscaler_kpi_timeval_add(&elapse, p_tend);
        }
    }
    return elapse;
}

static inline void fscaler_kpi_value_init(struct fscaler_kpi_value *p_val)
{
    if (supp_kpi && p_val) {
        p_val->op = 0;
        p_val->value = 0;
        p_val->acc = 0;
        p_val->acc_count = 0;
    }
}

static inline void fscaler_kpi_value_add(struct fscaler_kpi_value *p_val, unsigned int value)
{
    if (supp_kpi && p_val) {
        p_val->op = 1;
        p_val->value = value;
        p_val->acc -= value;
        p_val->acc_count++;
    }
}

static inline void fscaler_kpi_value_sub(struct fscaler_kpi_value *p_val, unsigned int value)
{
    if (supp_kpi && p_val) {
        p_val->op = -1;
        p_val->value = value;
        p_val->acc -= value;
        p_val->acc_count--;
    }
}

static inline void fscaler_kpi_value_set(struct fscaler_kpi_value *p_val, unsigned int value)
{
    if (supp_kpi && p_val) {
        p_val->op = 0;
        p_val->value = value;
        p_val->acc = value;
        p_val->acc_count = 1;
    }
}

static inline void fscaler_kpi_init(struct fscaler_kpi *p_kpi, int index)
{
    if (p_kpi) {
        p_kpi->index = index;
        fscaler_kpi_time_init(&p_kpi->time);
        fscaler_kpi_value_init(&p_kpi->value);
    }
}

static inline int fscaler_kpi_time_end(struct fscaler_kpi *kpi, struct fscaler_kpi *show)
{
    int ret = 0;
    struct timeval elapse;
    struct fscaler_kpi_time *p_time = &kpi->time;
    fscaler_kpi_timeval_init(&elapse);
    if (supp_kpi && kpi) {
        fscaler_kpi_timeval_set(&p_time->end, 0, FSCALER_KPI_GETTIME());
        if (p_time->b_start) {
            elapse = fscaler_kpi_timeval_elapse(&p_time->start, &p_time->end, &p_time->max);
            p_time->acc = fscaler_kpi_timeval_add(&p_time->acc, &elapse);
            fscaler_kpi_timeval_init(&p_time->start);
            p_time->last = p_time->end;
            p_time->b_start = 0;
        }
        p_time->elapse = fscaler_kpi_timeval_elapse(&p_time->first, &p_time->last, &p_time->max);
        if (fscaler_kpi_timeval_compare(&(p_time->elapse), &p_time->period) >= 0) {
            p_time->cost = p_time->acc;
            p_time->first = p_time->last;
            fscaler_kpi_timeval_init(&p_time->acc);
            ret = 1;
            if (show) {
                memcpy(show, kpi, sizeof(struct fscaler_kpi));
            }
            fscaler_kpi_value_init(&(kpi->value));
        }
    }
    return ret;
}

static inline void fscaler_kpi_time_start(struct fscaler_kpi_time *p_time)
{
    if (supp_kpi && p_time) {
        if (!p_time->b_start) {
            fscaler_kpi_timeval_set(&p_time->start, 0, FSCALER_KPI_GETTIME());
            if (p_time->first.tv_sec == 0 && p_time->first.tv_usec == 0) {
                p_time->first = p_time->last = p_time->start;
            }
            p_time->b_start = 1;
        }
    }
}

static inline void fscaler_kpi_show(struct fscaler_kpi *p_kpi)
{
    unsigned int cost = 0, elapse = 0;
    if (supp_kpi && p_kpi) {
        elapse = fscaler_kpi_timeval_to_msecs(&(p_kpi->time.elapse));
        cost = fscaler_kpi_timeval_to_msecs(&(p_kpi->time.cost));
        printk("SPF>>> [%02d] tot:%u.%03u use:%u.%03u avg:%u.%03u(Mpixel/ms) fp:%u.%d\n",
               p_kpi->index, elapse / 1000, elapse % 1000, cost / 1000, cost % 10000,
               (unsigned int)(p_kpi->value.acc / elapse) / 1000000,
               (unsigned int)(p_kpi->value.acc / elapse) % 1000000,
               p_kpi->value.acc_count * 1000 / elapse,
               (p_kpi->value.acc_count * 1000 % elapse) * 10 / elapse);
    }
}

#endif /* _FSCALER_KPI_H_ */
