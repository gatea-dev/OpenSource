/*
 * KDSA C API library - header file
 * Copyright 2010-2019 Kove IO, Inc.  All rights reserved.
 *
 * This program source is the property of Kove IO, Inc. and contains
 * information which is confidential and proprietary to Kove IO, Inc.  Use
 * of this program source is subject to a license agreement, a copy of which
 * may be obtained from Kove IO, Inc.  No part of this source may be copied,
 * reproduced, disclosed to third parties, or transmitted in any form or by
 * any means, electronic or mechanical for any purpose without the express
 * written consent of Kove IO, Inc.
 */

/* $Id$  <support@kove.net> */

/******************************************************************************

   Introduction and Usage

   This header file describes the functions provided by the Kove Direct
   System Architecture (KDSA) C API library that enables use of a KDSA XPD.

   See the KDSA C API manual for more information.

******************************************************************************/

#ifndef __KOVE_H
#define __KOVE_H

#include <infiniband/verbs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KDSA_FLAGS_USE_EVENT (1L<<0)
#define KDSA_FLAGS_USE_PORT_2 (1L<<2)
#define KDSA_FLAGS_UNSAFE_WRITE (1L<<3)
#define KDSA_FLAGS_INIT_USE_EVENT KDSA_FLAGS_USE_EVENT
#define KDSA_FLAGS_INIT_USE_PORT_2 KDSA_FLAGS_USE_PORT_2
#define KDSA_FLAGS_HANDLE_USE_EVENT KDSA_FLAGS_USE_EVENT
#define KDSA_FLAGS_HANDLE_IO_NOSPIN (1L<<1)
#define KDSA_FLAGS_HANDLE_USE_PORT_2 KDSA_FLAGS_USE_PORT_2
#define KDSA_FLAGS_HANDLE_UNSAFE_WRITE KDSA_FLAGS_UNSAFE_WRITE
#define KDSA_FLAGS_HANDLE_TEST ((1L<<4) | KDSA_FLAGS_HANDLE_NOFAIL)
#define KDSA_FLAGS_HANDLE_FAILFAST (1L<<5)
#define KDSA_FLAGS_HANDLE_NOFAIL_PORT (1L<<6)
#define KDSA_FLAGS_HANDLE_NOFAIL_DEVICE (1L<<7)
#define KDSA_FLAGS_HANDLE_NOFAIL_LINK (1L<<8)
#define KDSA_FLAGS_HANDLE_NOFAIL (KDSA_FLAGS_HANDLE_NOFAIL_PORT | KDSA_FLAGS_HANDLE_NOFAIL_DEVICE | KDSA_FLAGS_HANDLE_NOFAIL_LINK)
#define KDSA_FLAGS_MEMORY_BARRIER (1L<<9)
#define KDSA_FLAGS_HANDLE_MEMORY_BARRIER KDSA_FLAGS_MEMORY_BARRIER

// Everything here and below is experimental and dangerous. Use at your own risk.
#define KDSA_FLAGS_NONPERSIST 1L<<31
#define KDSA_FLAGS_HANDLE_NONPERSIST KDSA_FLAGS_NONPERSIST

/* Debug levels */
#define KDSA_DEBUG_SHOW_ALWAYS (1ull << 63)
#define KDSA_DEBUG_VERBOSE (1ll << 0)
#define KDSA_DEBUG_INFO (1ll << 1)
#define KDSA_DEBUG_WARN (1ll << 2)
#define KDSA_DEBUG_ERROR (1ll << 3)
#define KDSA_DEBUG_CRITICAL ((1ll << 4) | KDSA_DEBUG_SHOW_ALWAYS)
#define KDSA_DEBUG_FATAL ((1ll << 5) | KDSA_DEBUG_SHOW_ALWAYS)
#define KDSA_DEBUG_ALL 0xFFFFFFFFFFFFFFFFllu

/* Debug areas. Optional to OR into debug levels to get area specific
 * verbose and info messages. Used to cut down on spam-like messages */
#define KDSA_DEBUG_COMPLETION (1ll << 32)
#define KDSA_DEBUG_CONNECT (1ll << 33)
#define KDSA_DEBUG_DISCONNECT (1ll << 34)
#define KDSA_DEBUG_MANAGEMENT (1ll << 35)

struct kdsa_vol_handle;
struct kdsa_mem_key;
struct kdsa_async_id;

typedef uint64_t kdsa_serno_t;
typedef struct kdsa_vol_handle * kdsa_vol_handle_t;
typedef struct kdsa_mem_key * kdsa_mem_key_t;
typedef uint64_t kdsa_vol_offset_t;
typedef uint64_t kdsa_size_t;
typedef int64_t kdsa_size64_t;
typedef uint8_t kdsa_volumeid_t[16];
typedef struct kdsa_async_id * kdsa_async_id_t;
typedef signed char kdsa_device_name_t[IBV_SYSFS_NAME_MAX];

typedef struct
{
    char dev_name[IBV_SYSFS_NAME_MAX];
    int  port;
} kdsa_port_def_t;

typedef struct
{
    int64_t ret_code;
    int64_t errno_value;
    kdsa_async_id_t async_id;
    int64_t atomic_result;
} kdsa_async_status_t;

int kdsa_prt_volumeid(kdsa_volumeid_t volume, size_t size, char* str);

int kdsa_prt_serno(kdsa_serno_t serno, size_t size, char* str);

int kdsa_prt_device_name(kdsa_device_name_t device_name, size_t size, char* str);

int kdsa_prt_mem_key(kdsa_mem_key_t key, size_t size, char* str);

int kdsa_parse_serno(const char* serno_string, kdsa_serno_t *serno);

int kdsa_parse_volumeid(const char* volumeid_string, kdsa_volumeid_t volume_id);

int kdsa_parse_connection_specifier(const char* connection_specifier, 
                                    kdsa_device_name_t local_device_name, 
                                    uint8_t *local_port_num, 
                                    kdsa_serno_t *serial_number, 
                                    uint8_t *link_number, 
                                    kdsa_volumeid_t volume_id);

int kdsa_list_local_devices(int max, kdsa_device_name_t *devices);

int kdsa_get_unique_ports(kdsa_port_def_t **array, int *size);

int kdsa_get_serials(kdsa_device_name_t device, uint8_t local_port, int max, kdsa_serno_t *sernos, uint64_t *links);

int kdsa_get_serial_links(kdsa_device_name_t device, uint8_t local_port, kdsa_serno_t serial, uint64_t *links);

int kdsa_get_volume_count(kdsa_device_name_t device, uint8_t local_port, kdsa_serno_t serno, uint8_t link_no);

int kdsa_get_volumes(kdsa_device_name_t device, uint8_t local_port, kdsa_serno_t serno, 
                     uint8_t link_no, int max, kdsa_volumeid_t *volumes);

int kdsa_get_volume_size(kdsa_vol_handle_t handle, kdsa_size_t *size);

int kdsa_connect(const char* connection_string, uint32_t flags, kdsa_vol_handle_t *handle);

int kdsa_disconnect(kdsa_vol_handle_t handle);

int kdsa_set_handle_flags(kdsa_vol_handle_t handle, uint32_t flags, uint32_t *old_flags);

int kdsa_get_handle_flags(kdsa_vol_handle_t handle, uint32_t *flags);

int kdsa_register_mem(kdsa_vol_handle_t handle, void* addr, size_t size, kdsa_mem_key_t *key);

int kdsa_deregister_mem(kdsa_mem_key_t key);

int kdsa_async_write(kdsa_vol_handle_t handle, kdsa_mem_key_t key, 
                     kdsa_vol_offset_t off, const void* buf, 
                     kdsa_size_t bytes, kdsa_async_id_t *async_id);

int kdsa_async_write_ext(kdsa_vol_handle_t handle, kdsa_mem_key_t key, 
                     kdsa_vol_offset_t off, const void* buf, kdsa_size_t bytes,
		     uint64_t flags, kdsa_async_id_t *async_id);

int kdsa_async_read(kdsa_vol_handle_t handle, kdsa_mem_key_t key, 
                    kdsa_vol_offset_t off, void* buf, 
                    kdsa_size_t bytes, kdsa_async_id_t *async_id);

int kdsa_async_read_ext(kdsa_vol_handle_t handle, kdsa_mem_key_t key, 
                    kdsa_vol_offset_t off, void* buf, kdsa_size_t bytes,
		    uint64_t flags, kdsa_async_id_t *async_id);

int kdsa_async_compare_and_swap(kdsa_vol_handle_t handle, kdsa_vol_offset_t off, 
                                uint64_t compare, uint64_t swap, kdsa_async_id_t *async_id);

int kdsa_async_fetch_and_add(kdsa_vol_handle_t handle, kdsa_vol_offset_t off, 
                             uint64_t add, kdsa_async_id_t *async_id);

int kdsa_async_memory_barrier(kdsa_vol_handle_t handle, kdsa_async_id_t *async_id);

int kdsa_async_memset(kdsa_vol_handle_t handle, kdsa_vol_offset_t off, int value, 
                      kdsa_size64_t length, kdsa_async_id_t *async_id);

int kdsa_async_memcpy(kdsa_vol_handle_t handle, kdsa_vol_offset_t dst, kdsa_vol_offset_t src, 
                      kdsa_size64_t n, kdsa_async_id_t *async_id);

int kdsa_async_status(int count, kdsa_async_status_t *status_array);

int kdsa_async_return(kdsa_async_id_t async_id, uint64_t *result);

int kdsa_write(kdsa_vol_handle_t handle, kdsa_mem_key_t key, kdsa_vol_offset_t off, const void* buf, kdsa_size_t bytes);

int kdsa_write_ext(kdsa_vol_handle_t handle, kdsa_mem_key_t key, kdsa_vol_offset_t off, const void* buf, kdsa_size_t bytes, uint64_t flags);

int kdsa_read(kdsa_vol_handle_t handle, kdsa_mem_key_t key, kdsa_vol_offset_t off, void* buf, kdsa_size_t bytes);

int kdsa_read_ext(kdsa_vol_handle_t handle, kdsa_mem_key_t key, kdsa_vol_offset_t off, void* buf, kdsa_size_t bytes, uint64_t flags);

int kdsa_compare_and_swap(kdsa_vol_handle_t handle, kdsa_vol_offset_t off, 
                          uint64_t compare, uint64_t swap, uint64_t *result);
 
int kdsa_fetch_and_add(kdsa_vol_handle_t handle, kdsa_vol_offset_t off, uint64_t add, uint64_t *result);

int kdsa_memory_barrier(kdsa_vol_handle_t handle);

int kdsa_memset(kdsa_vol_handle_t handle, kdsa_vol_offset_t off, int value, kdsa_size64_t length);

int kdsa_memcpy(kdsa_vol_handle_t handle, kdsa_vol_offset_t dst, kdsa_vol_offset_t src, kdsa_size64_t n);

int kdsa_write_dual(kdsa_vol_handle_t handle1, kdsa_mem_key_t key1, kdsa_vol_handle_t handle2, 
                    kdsa_mem_key_t key2, kdsa_vol_offset_t off, const void* buf, kdsa_size_t bytes);

int kdsa_write_dual_ext(kdsa_vol_handle_t handle1, kdsa_mem_key_t key1, kdsa_vol_handle_t handle2, 
                    kdsa_mem_key_t key2, kdsa_vol_offset_t off, const void* buf, kdsa_size_t bytes,
		    uint64_t flags);

int kdsa_async_write_dual(kdsa_vol_handle_t handle1, kdsa_mem_key_t key1, kdsa_vol_handle_t handle2,
                          kdsa_mem_key_t key2, kdsa_vol_offset_t off, const void* buf, kdsa_size_t bytes, 
                          kdsa_async_id_t *async_id);

int kdsa_async_write_dual_ext(kdsa_vol_handle_t handle1, kdsa_mem_key_t key1, kdsa_vol_handle_t handle2,
                          kdsa_mem_key_t key2, kdsa_vol_offset_t off, const void* buf, kdsa_size_t bytes, 
                          uint64_t flags, kdsa_async_id_t *async_id);

int kdsa_sync_region(kdsa_vol_handle_t src, kdsa_vol_handle_t dst, kdsa_vol_offset_t off, kdsa_size64_t bytes);

int kdsa_sync_volumes(kdsa_vol_handle_t src, kdsa_vol_handle_t dst);

int kdsa_write_unregistered(kdsa_vol_handle_t handle, kdsa_vol_offset_t off, const void* buf, kdsa_size_t bytes);

int kdsa_read_unregistered(kdsa_vol_handle_t handle, kdsa_vol_offset_t off, void* buf, kdsa_size_t bytes);

uint64_t kdsa_set_debug(uint64_t new_debug);
uint64_t kdsa_get_debug(void);

int kdsa_set_read_buffer_size(kdsa_vol_handle_t handle, size_t new_read_buffer_size);
int kdsa_set_write_buffer_size(kdsa_vol_handle_t handle, size_t new_write_buffer_size);

int kdsa_set_timeout(kdsa_vol_handle_t handle, struct timespec timeout, struct timespec *old_timeout);
int kdsa_get_timeout(kdsa_vol_handle_t handle, struct timespec *old_timeout);

#ifdef __cplusplus
}
#endif

#endif //ifndef __KOVE_H
