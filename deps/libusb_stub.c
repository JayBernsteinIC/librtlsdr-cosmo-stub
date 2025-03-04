#include "libusb_stub.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <libc/dlopen/dlfcn.h>
#define MS_ABI_ATTR __attribute__((__ms_abi__))

static struct libusbFuncs {
    int (*ptr_libusb_init)(libusb_context * * ctx);
    void (*ptr_libusb_exit)(libusb_context * ctx);
    void (*ptr_libusb_set_debug)(libusb_context * ctx, int level);
    void (*ptr_libusb_set_log_cb)(libusb_context * ctx, libusb_log_cb cb, int mode);
    const struct libusb_version * (*ptr_libusb_get_version)(void);
    int (*ptr_libusb_has_capability)(uint32_t capability);
    const char * (*ptr_libusb_error_name)(int errcode);
    int (*ptr_libusb_setlocale)(const char * locale);
    const char * (*ptr_libusb_strerror)(int errcode);
    ssize_t (*ptr_libusb_get_device_list)(libusb_context * ctx, libusb_device * * * list);
    void (*ptr_libusb_free_device_list)(libusb_device * * list, int unref_devices);
    libusb_device * (*ptr_libusb_ref_device)(libusb_device * dev);
    void (*ptr_libusb_unref_device)(libusb_device * dev);
    int (*ptr_libusb_get_configuration)(libusb_device_handle * dev, int * config);
    int (*ptr_libusb_get_device_descriptor)(libusb_device * dev, struct libusb_device_descriptor * desc);
    int (*ptr_libusb_get_active_config_descriptor)(libusb_device * dev, struct libusb_config_descriptor * * config);
    int (*ptr_libusb_get_config_descriptor)(libusb_device * dev, uint8_t config_index, struct libusb_config_descriptor * * config);
    int (*ptr_libusb_get_config_descriptor_by_value)(libusb_device * dev, uint8_t bConfigurationValue, struct libusb_config_descriptor * * config);
    void (*ptr_libusb_free_config_descriptor)(struct libusb_config_descriptor * config);
    int (*ptr_libusb_get_ss_endpoint_companion_descriptor)(libusb_context * ctx, const struct libusb_endpoint_descriptor * endpoint, struct libusb_ss_endpoint_companion_descriptor * * ep_comp);
    void (*ptr_libusb_free_ss_endpoint_companion_descriptor)(struct libusb_ss_endpoint_companion_descriptor * ep_comp);
    int (*ptr_libusb_get_bos_descriptor)(libusb_device_handle * dev_handle, struct libusb_bos_descriptor * * bos);
    void (*ptr_libusb_free_bos_descriptor)(struct libusb_bos_descriptor * bos);
    int (*ptr_libusb_get_usb_2_0_extension_descriptor)(libusb_context * ctx, struct libusb_bos_dev_capability_descriptor * dev_cap, struct libusb_usb_2_0_extension_descriptor * * usb_2_0_extension);
    void (*ptr_libusb_free_usb_2_0_extension_descriptor)(struct libusb_usb_2_0_extension_descriptor * usb_2_0_extension);
    int (*ptr_libusb_get_ss_usb_device_capability_descriptor)(libusb_context * ctx, struct libusb_bos_dev_capability_descriptor * dev_cap, struct libusb_ss_usb_device_capability_descriptor * * ss_usb_device_cap);
    void (*ptr_libusb_free_ss_usb_device_capability_descriptor)(struct libusb_ss_usb_device_capability_descriptor * ss_usb_device_cap);
    int (*ptr_libusb_get_container_id_descriptor)(libusb_context * ctx, struct libusb_bos_dev_capability_descriptor * dev_cap, struct libusb_container_id_descriptor * * container_id);
    void (*ptr_libusb_free_container_id_descriptor)(struct libusb_container_id_descriptor * container_id);
    uint8_t (*ptr_libusb_get_bus_number)(libusb_device * dev);
    uint8_t (*ptr_libusb_get_port_number)(libusb_device * dev);
    int (*ptr_libusb_get_port_numbers)(libusb_device * dev, uint8_t * port_numbers, int port_numbers_len);
    int (*ptr_libusb_get_port_path)(libusb_context * ctx, libusb_device * dev, uint8_t * path, uint8_t path_length);
    libusb_device * (*ptr_libusb_get_parent)(libusb_device * dev);
    uint8_t (*ptr_libusb_get_device_address)(libusb_device * dev);
    int (*ptr_libusb_get_device_speed)(libusb_device * dev);
    int (*ptr_libusb_get_max_packet_size)(libusb_device * dev, unsigned char endpoint);
    int (*ptr_libusb_get_max_iso_packet_size)(libusb_device * dev, unsigned char endpoint);
    int (*ptr_libusb_wrap_sys_device)(libusb_context * ctx, intptr_t sys_dev, libusb_device_handle * * dev_handle);
    int (*ptr_libusb_open)(libusb_device * dev, libusb_device_handle * * dev_handle);
    void (*ptr_libusb_close)(libusb_device_handle * dev_handle);
    libusb_device * (*ptr_libusb_get_device)(libusb_device_handle * dev_handle);
    int (*ptr_libusb_set_configuration)(libusb_device_handle * dev_handle, int configuration);
    int (*ptr_libusb_claim_interface)(libusb_device_handle * dev_handle, int interface_number);
    int (*ptr_libusb_release_interface)(libusb_device_handle * dev_handle, int interface_number);
    libusb_device_handle * (*ptr_libusb_open_device_with_vid_pid)(libusb_context * ctx, uint16_t vendor_id, uint16_t product_id);
    int (*ptr_libusb_set_interface_alt_setting)(libusb_device_handle * dev_handle, int interface_number, int alternate_setting);
    int (*ptr_libusb_clear_halt)(libusb_device_handle * dev_handle, unsigned char endpoint);
    int (*ptr_libusb_reset_device)(libusb_device_handle * dev_handle);
    int (*ptr_libusb_alloc_streams)(libusb_device_handle * dev_handle, uint32_t num_streams, unsigned char * endpoints, int num_endpoints);
    int (*ptr_libusb_free_streams)(libusb_device_handle * dev_handle, unsigned char * endpoints, int num_endpoints);
    unsigned char * (*ptr_libusb_dev_mem_alloc)(libusb_device_handle * dev_handle, size_t length);
    int (*ptr_libusb_dev_mem_free)(libusb_device_handle * dev_handle, unsigned char * buffer, size_t length);
    int (*ptr_libusb_kernel_driver_active)(libusb_device_handle * dev_handle, int interface_number);
    int (*ptr_libusb_detach_kernel_driver)(libusb_device_handle * dev_handle, int interface_number);
    int (*ptr_libusb_attach_kernel_driver)(libusb_device_handle * dev_handle, int interface_number);
    int (*ptr_libusb_set_auto_detach_kernel_driver)(libusb_device_handle * dev_handle, int enable);
    struct libusb_transfer * (*ptr_libusb_alloc_transfer)(int iso_packets);
    int (*ptr_libusb_submit_transfer)(struct libusb_transfer * transfer);
    int (*ptr_libusb_cancel_transfer)(struct libusb_transfer * transfer);
    void (*ptr_libusb_free_transfer)(struct libusb_transfer * transfer);
    void (*ptr_libusb_transfer_set_stream_id)(struct libusb_transfer * transfer, uint32_t stream_id);
    uint32_t (*ptr_libusb_transfer_get_stream_id)(struct libusb_transfer * transfer);
    int (*ptr_libusb_control_transfer)(libusb_device_handle * dev_handle, uint8_t request_type, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char * data, uint16_t wLength, unsigned int timeout);
    int (*ptr_libusb_bulk_transfer)(libusb_device_handle * dev_handle, unsigned char endpoint, unsigned char * data, int length, int * actual_length, unsigned int timeout);
    int (*ptr_libusb_interrupt_transfer)(libusb_device_handle * dev_handle, unsigned char endpoint, unsigned char * data, int length, int * actual_length, unsigned int timeout);
    int (*ptr_libusb_get_string_descriptor_ascii)(libusb_device_handle * dev_handle, uint8_t desc_index, unsigned char * data, int length);
    int (*ptr_libusb_try_lock_events)(libusb_context * ctx);
    void (*ptr_libusb_lock_events)(libusb_context * ctx);
    void (*ptr_libusb_unlock_events)(libusb_context * ctx);
    int (*ptr_libusb_event_handling_ok)(libusb_context * ctx);
    int (*ptr_libusb_event_handler_active)(libusb_context * ctx);
    void (*ptr_libusb_interrupt_event_handler)(libusb_context * ctx);
    void (*ptr_libusb_lock_event_waiters)(libusb_context * ctx);
    void (*ptr_libusb_unlock_event_waiters)(libusb_context * ctx);
    int (*ptr_libusb_wait_for_event)(libusb_context * ctx, struct timeval * tv);
    int (*ptr_libusb_handle_events_timeout)(libusb_context * ctx, struct timeval * tv);
    int (*ptr_libusb_handle_events_timeout_completed)(libusb_context * ctx, struct timeval * tv, int * completed);
    int (*ptr_libusb_handle_events)(libusb_context * ctx);
    int (*ptr_libusb_handle_events_completed)(libusb_context * ctx, int * completed);
    int (*ptr_libusb_handle_events_locked)(libusb_context * ctx, struct timeval * tv);
    int (*ptr_libusb_pollfds_handle_timeouts)(libusb_context * ctx);
    int (*ptr_libusb_get_next_timeout)(libusb_context * ctx, struct timeval * tv);
    const struct libusb_pollfd * * (*ptr_libusb_get_pollfds)(libusb_context * ctx);
    void (*ptr_libusb_free_pollfds)(const struct libusb_pollfd * * pollfds);
    void (*ptr_libusb_set_pollfd_notifiers)(libusb_context * ctx, libusb_pollfd_added_cb added_cb, libusb_pollfd_removed_cb removed_cb, void * user_data);
    int (*ptr_libusb_hotplug_register_callback)(libusb_context * ctx, int events, int flags, int vendor_id, int product_id, int dev_class, libusb_hotplug_callback_fn cb_fn, void * user_data, libusb_hotplug_callback_handle * callback_handle);
    void (*ptr_libusb_hotplug_deregister_callback)(libusb_context * ctx, libusb_hotplug_callback_handle callback_handle);
    void * (*ptr_libusb_hotplug_get_user_data)(libusb_context * ctx, libusb_hotplug_callback_handle callback_handle);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_init)(libusb_context * * ctx);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_exit)(libusb_context * ctx);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_set_debug)(libusb_context * ctx, int level);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_set_log_cb)(libusb_context * ctx, libusb_log_cb cb, int mode);
    const struct libusb_version * (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_version)(void);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_has_capability)(uint32_t capability);
    const char * (__attribute__((__ms_abi__)) *ptr_ms_libusb_error_name)(int errcode);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_setlocale)(const char * locale);
    const char * (__attribute__((__ms_abi__)) *ptr_ms_libusb_strerror)(int errcode);
    ssize_t (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_device_list)(libusb_context * ctx, libusb_device * * * list);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_free_device_list)(libusb_device * * list, int unref_devices);
    libusb_device * (__attribute__((__ms_abi__)) *ptr_ms_libusb_ref_device)(libusb_device * dev);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_unref_device)(libusb_device * dev);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_configuration)(libusb_device_handle * dev, int * config);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_device_descriptor)(libusb_device * dev, struct libusb_device_descriptor * desc);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_active_config_descriptor)(libusb_device * dev, struct libusb_config_descriptor * * config);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_config_descriptor)(libusb_device * dev, uint8_t config_index, struct libusb_config_descriptor * * config);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_config_descriptor_by_value)(libusb_device * dev, uint8_t bConfigurationValue, struct libusb_config_descriptor * * config);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_free_config_descriptor)(struct libusb_config_descriptor * config);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_ss_endpoint_companion_descriptor)(libusb_context * ctx, const struct libusb_endpoint_descriptor * endpoint, struct libusb_ss_endpoint_companion_descriptor * * ep_comp);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_free_ss_endpoint_companion_descriptor)(struct libusb_ss_endpoint_companion_descriptor * ep_comp);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_bos_descriptor)(libusb_device_handle * dev_handle, struct libusb_bos_descriptor * * bos);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_free_bos_descriptor)(struct libusb_bos_descriptor * bos);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_usb_2_0_extension_descriptor)(libusb_context * ctx, struct libusb_bos_dev_capability_descriptor * dev_cap, struct libusb_usb_2_0_extension_descriptor * * usb_2_0_extension);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_free_usb_2_0_extension_descriptor)(struct libusb_usb_2_0_extension_descriptor * usb_2_0_extension);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_ss_usb_device_capability_descriptor)(libusb_context * ctx, struct libusb_bos_dev_capability_descriptor * dev_cap, struct libusb_ss_usb_device_capability_descriptor * * ss_usb_device_cap);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_free_ss_usb_device_capability_descriptor)(struct libusb_ss_usb_device_capability_descriptor * ss_usb_device_cap);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_container_id_descriptor)(libusb_context * ctx, struct libusb_bos_dev_capability_descriptor * dev_cap, struct libusb_container_id_descriptor * * container_id);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_free_container_id_descriptor)(struct libusb_container_id_descriptor * container_id);
    uint8_t (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_bus_number)(libusb_device * dev);
    uint8_t (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_port_number)(libusb_device * dev);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_port_numbers)(libusb_device * dev, uint8_t * port_numbers, int port_numbers_len);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_port_path)(libusb_context * ctx, libusb_device * dev, uint8_t * path, uint8_t path_length);
    libusb_device * (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_parent)(libusb_device * dev);
    uint8_t (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_device_address)(libusb_device * dev);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_device_speed)(libusb_device * dev);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_max_packet_size)(libusb_device * dev, unsigned char endpoint);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_max_iso_packet_size)(libusb_device * dev, unsigned char endpoint);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_wrap_sys_device)(libusb_context * ctx, intptr_t sys_dev, libusb_device_handle * * dev_handle);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_open)(libusb_device * dev, libusb_device_handle * * dev_handle);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_close)(libusb_device_handle * dev_handle);
    libusb_device * (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_device)(libusb_device_handle * dev_handle);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_set_configuration)(libusb_device_handle * dev_handle, int configuration);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_claim_interface)(libusb_device_handle * dev_handle, int interface_number);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_release_interface)(libusb_device_handle * dev_handle, int interface_number);
    libusb_device_handle * (__attribute__((__ms_abi__)) *ptr_ms_libusb_open_device_with_vid_pid)(libusb_context * ctx, uint16_t vendor_id, uint16_t product_id);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_set_interface_alt_setting)(libusb_device_handle * dev_handle, int interface_number, int alternate_setting);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_clear_halt)(libusb_device_handle * dev_handle, unsigned char endpoint);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_reset_device)(libusb_device_handle * dev_handle);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_alloc_streams)(libusb_device_handle * dev_handle, uint32_t num_streams, unsigned char * endpoints, int num_endpoints);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_free_streams)(libusb_device_handle * dev_handle, unsigned char * endpoints, int num_endpoints);
    unsigned char * (__attribute__((__ms_abi__)) *ptr_ms_libusb_dev_mem_alloc)(libusb_device_handle * dev_handle, size_t length);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_dev_mem_free)(libusb_device_handle * dev_handle, unsigned char * buffer, size_t length);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_kernel_driver_active)(libusb_device_handle * dev_handle, int interface_number);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_detach_kernel_driver)(libusb_device_handle * dev_handle, int interface_number);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_attach_kernel_driver)(libusb_device_handle * dev_handle, int interface_number);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_set_auto_detach_kernel_driver)(libusb_device_handle * dev_handle, int enable);
    struct libusb_transfer * (__attribute__((__ms_abi__)) *ptr_ms_libusb_alloc_transfer)(int iso_packets);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_submit_transfer)(struct libusb_transfer * transfer);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_cancel_transfer)(struct libusb_transfer * transfer);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_free_transfer)(struct libusb_transfer * transfer);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_transfer_set_stream_id)(struct libusb_transfer * transfer, uint32_t stream_id);
    uint32_t (__attribute__((__ms_abi__)) *ptr_ms_libusb_transfer_get_stream_id)(struct libusb_transfer * transfer);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_control_transfer)(libusb_device_handle * dev_handle, uint8_t request_type, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char * data, uint16_t wLength, unsigned int timeout);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_bulk_transfer)(libusb_device_handle * dev_handle, unsigned char endpoint, unsigned char * data, int length, int * actual_length, unsigned int timeout);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_interrupt_transfer)(libusb_device_handle * dev_handle, unsigned char endpoint, unsigned char * data, int length, int * actual_length, unsigned int timeout);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_string_descriptor_ascii)(libusb_device_handle * dev_handle, uint8_t desc_index, unsigned char * data, int length);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_try_lock_events)(libusb_context * ctx);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_lock_events)(libusb_context * ctx);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_unlock_events)(libusb_context * ctx);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_event_handling_ok)(libusb_context * ctx);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_event_handler_active)(libusb_context * ctx);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_interrupt_event_handler)(libusb_context * ctx);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_lock_event_waiters)(libusb_context * ctx);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_unlock_event_waiters)(libusb_context * ctx);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_wait_for_event)(libusb_context * ctx, struct timeval * tv);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_handle_events_timeout)(libusb_context * ctx, struct timeval * tv);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_handle_events_timeout_completed)(libusb_context * ctx, struct timeval * tv, int * completed);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_handle_events)(libusb_context * ctx);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_handle_events_completed)(libusb_context * ctx, int * completed);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_handle_events_locked)(libusb_context * ctx, struct timeval * tv);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_pollfds_handle_timeouts)(libusb_context * ctx);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_next_timeout)(libusb_context * ctx, struct timeval * tv);
    const struct libusb_pollfd * * (__attribute__((__ms_abi__)) *ptr_ms_libusb_get_pollfds)(libusb_context * ctx);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_free_pollfds)(const struct libusb_pollfd * * pollfds);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_set_pollfd_notifiers)(libusb_context * ctx, libusb_pollfd_added_cb added_cb, libusb_pollfd_removed_cb removed_cb, void * user_data);
    int (__attribute__((__ms_abi__)) *ptr_ms_libusb_hotplug_register_callback)(libusb_context * ctx, int events, int flags, int vendor_id, int product_id, int dev_class, libusb_hotplug_callback_fn cb_fn, void * user_data, libusb_hotplug_callback_handle * callback_handle);
    void (__attribute__((__ms_abi__)) *ptr_ms_libusb_hotplug_deregister_callback)(libusb_context * ctx, libusb_hotplug_callback_handle callback_handle);
    void * (__attribute__((__ms_abi__)) *ptr_ms_libusb_hotplug_get_user_data)(libusb_context * ctx, libusb_hotplug_callback_handle callback_handle);
} stub_funcs;

void *libusb;

void initialize_libusb(void) {
    char *candidates_libusb[] = { "libusb-1.0.so", "libusb-1.0.dll", "libusb-1.0.so.0.3.0", "libusb-1.0.so.0" };
    libusb = try_find_lib(candidates_libusb, 3);

    if (!libusb) {
        fprintf(stderr, "Unable to locate libusb, exiting!");
        exit(1);
    }

    stub_funcs.ptr_libusb_init = try_find_sym(libusb, "libusb_init");
    stub_funcs.ptr_libusb_exit = try_find_sym(libusb, "libusb_exit");
    stub_funcs.ptr_libusb_set_debug = try_find_sym(libusb, "libusb_set_debug");
    stub_funcs.ptr_libusb_set_log_cb = try_find_sym(libusb, "libusb_set_log_cb");
    stub_funcs.ptr_libusb_get_version = try_find_sym(libusb, "libusb_get_version");
    stub_funcs.ptr_libusb_has_capability = try_find_sym(libusb, "libusb_has_capability");
    stub_funcs.ptr_libusb_error_name = try_find_sym(libusb, "libusb_error_name");
    stub_funcs.ptr_libusb_setlocale = try_find_sym(libusb, "libusb_setlocale");
    stub_funcs.ptr_libusb_strerror = try_find_sym(libusb, "libusb_strerror");
    stub_funcs.ptr_libusb_get_device_list = try_find_sym(libusb, "libusb_get_device_list");
    stub_funcs.ptr_libusb_free_device_list = try_find_sym(libusb, "libusb_free_device_list");
    stub_funcs.ptr_libusb_ref_device = try_find_sym(libusb, "libusb_ref_device");
    stub_funcs.ptr_libusb_unref_device = try_find_sym(libusb, "libusb_unref_device");
    stub_funcs.ptr_libusb_get_configuration = try_find_sym(libusb, "libusb_get_configuration");
    stub_funcs.ptr_libusb_get_device_descriptor = try_find_sym(libusb, "libusb_get_device_descriptor");
    stub_funcs.ptr_libusb_get_active_config_descriptor = try_find_sym(libusb, "libusb_get_active_config_descriptor");
    stub_funcs.ptr_libusb_get_config_descriptor = try_find_sym(libusb, "libusb_get_config_descriptor");
    stub_funcs.ptr_libusb_get_config_descriptor_by_value = try_find_sym(libusb, "libusb_get_config_descriptor_by_value");
    stub_funcs.ptr_libusb_free_config_descriptor = try_find_sym(libusb, "libusb_free_config_descriptor");
    stub_funcs.ptr_libusb_get_ss_endpoint_companion_descriptor = try_find_sym(libusb, "libusb_get_ss_endpoint_companion_descriptor");
    stub_funcs.ptr_libusb_free_ss_endpoint_companion_descriptor = try_find_sym(libusb, "libusb_free_ss_endpoint_companion_descriptor");
    stub_funcs.ptr_libusb_get_bos_descriptor = try_find_sym(libusb, "libusb_get_bos_descriptor");
    stub_funcs.ptr_libusb_free_bos_descriptor = try_find_sym(libusb, "libusb_free_bos_descriptor");
    stub_funcs.ptr_libusb_get_usb_2_0_extension_descriptor = try_find_sym(libusb, "libusb_get_usb_2_0_extension_descriptor");
    stub_funcs.ptr_libusb_free_usb_2_0_extension_descriptor = try_find_sym(libusb, "libusb_free_usb_2_0_extension_descriptor");
    stub_funcs.ptr_libusb_get_ss_usb_device_capability_descriptor = try_find_sym(libusb, "libusb_get_ss_usb_device_capability_descriptor");
    stub_funcs.ptr_libusb_free_ss_usb_device_capability_descriptor = try_find_sym(libusb, "libusb_free_ss_usb_device_capability_descriptor");
    stub_funcs.ptr_libusb_get_container_id_descriptor = try_find_sym(libusb, "libusb_get_container_id_descriptor");
    stub_funcs.ptr_libusb_free_container_id_descriptor = try_find_sym(libusb, "libusb_free_container_id_descriptor");
    stub_funcs.ptr_libusb_get_bus_number = try_find_sym(libusb, "libusb_get_bus_number");
    stub_funcs.ptr_libusb_get_port_number = try_find_sym(libusb, "libusb_get_port_number");
    stub_funcs.ptr_libusb_get_port_numbers = try_find_sym(libusb, "libusb_get_port_numbers");
    stub_funcs.ptr_libusb_get_port_path = try_find_sym(libusb, "libusb_get_port_path");
    stub_funcs.ptr_libusb_get_parent = try_find_sym(libusb, "libusb_get_parent");
    stub_funcs.ptr_libusb_get_device_address = try_find_sym(libusb, "libusb_get_device_address");
    stub_funcs.ptr_libusb_get_device_speed = try_find_sym(libusb, "libusb_get_device_speed");
    stub_funcs.ptr_libusb_get_max_packet_size = try_find_sym(libusb, "libusb_get_max_packet_size");
    stub_funcs.ptr_libusb_get_max_iso_packet_size = try_find_sym(libusb, "libusb_get_max_iso_packet_size");
    stub_funcs.ptr_libusb_wrap_sys_device = try_find_sym(libusb, "libusb_wrap_sys_device");
    stub_funcs.ptr_libusb_open = try_find_sym(libusb, "libusb_open");
    stub_funcs.ptr_libusb_close = try_find_sym(libusb, "libusb_close");
    stub_funcs.ptr_libusb_get_device = try_find_sym(libusb, "libusb_get_device");
    stub_funcs.ptr_libusb_set_configuration = try_find_sym(libusb, "libusb_set_configuration");
    stub_funcs.ptr_libusb_claim_interface = try_find_sym(libusb, "libusb_claim_interface");
    stub_funcs.ptr_libusb_release_interface = try_find_sym(libusb, "libusb_release_interface");
    stub_funcs.ptr_libusb_open_device_with_vid_pid = try_find_sym(libusb, "libusb_open_device_with_vid_pid");
    stub_funcs.ptr_libusb_set_interface_alt_setting = try_find_sym(libusb, "libusb_set_interface_alt_setting");
    stub_funcs.ptr_libusb_clear_halt = try_find_sym(libusb, "libusb_clear_halt");
    stub_funcs.ptr_libusb_reset_device = try_find_sym(libusb, "libusb_reset_device");
    stub_funcs.ptr_libusb_alloc_streams = try_find_sym(libusb, "libusb_alloc_streams");
    stub_funcs.ptr_libusb_free_streams = try_find_sym(libusb, "libusb_free_streams");
    stub_funcs.ptr_libusb_dev_mem_alloc = try_find_sym(libusb, "libusb_dev_mem_alloc");
    stub_funcs.ptr_libusb_dev_mem_free = try_find_sym(libusb, "libusb_dev_mem_free");
    stub_funcs.ptr_libusb_kernel_driver_active = try_find_sym(libusb, "libusb_kernel_driver_active");
    stub_funcs.ptr_libusb_detach_kernel_driver = try_find_sym(libusb, "libusb_detach_kernel_driver");
    stub_funcs.ptr_libusb_attach_kernel_driver = try_find_sym(libusb, "libusb_attach_kernel_driver");
    stub_funcs.ptr_libusb_set_auto_detach_kernel_driver = try_find_sym(libusb, "libusb_set_auto_detach_kernel_driver");
    stub_funcs.ptr_libusb_alloc_transfer = try_find_sym(libusb, "libusb_alloc_transfer");
    stub_funcs.ptr_libusb_submit_transfer = try_find_sym(libusb, "libusb_submit_transfer");
    stub_funcs.ptr_libusb_cancel_transfer = try_find_sym(libusb, "libusb_cancel_transfer");
    stub_funcs.ptr_libusb_free_transfer = try_find_sym(libusb, "libusb_free_transfer");
    stub_funcs.ptr_libusb_transfer_set_stream_id = try_find_sym(libusb, "libusb_transfer_set_stream_id");
    stub_funcs.ptr_libusb_transfer_get_stream_id = try_find_sym(libusb, "libusb_transfer_get_stream_id");
    stub_funcs.ptr_libusb_control_transfer = try_find_sym(libusb, "libusb_control_transfer");
    stub_funcs.ptr_libusb_bulk_transfer = try_find_sym(libusb, "libusb_bulk_transfer");
    stub_funcs.ptr_libusb_interrupt_transfer = try_find_sym(libusb, "libusb_interrupt_transfer");
    stub_funcs.ptr_libusb_get_string_descriptor_ascii = try_find_sym(libusb, "libusb_get_string_descriptor_ascii");
    stub_funcs.ptr_libusb_try_lock_events = try_find_sym(libusb, "libusb_try_lock_events");
    stub_funcs.ptr_libusb_lock_events = try_find_sym(libusb, "libusb_lock_events");
    stub_funcs.ptr_libusb_unlock_events = try_find_sym(libusb, "libusb_unlock_events");
    stub_funcs.ptr_libusb_event_handling_ok = try_find_sym(libusb, "libusb_event_handling_ok");
    stub_funcs.ptr_libusb_event_handler_active = try_find_sym(libusb, "libusb_event_handler_active");
    stub_funcs.ptr_libusb_interrupt_event_handler = try_find_sym(libusb, "libusb_interrupt_event_handler");
    stub_funcs.ptr_libusb_lock_event_waiters = try_find_sym(libusb, "libusb_lock_event_waiters");
    stub_funcs.ptr_libusb_unlock_event_waiters = try_find_sym(libusb, "libusb_unlock_event_waiters");
    stub_funcs.ptr_libusb_wait_for_event = try_find_sym(libusb, "libusb_wait_for_event");
    stub_funcs.ptr_libusb_handle_events_timeout = try_find_sym(libusb, "libusb_handle_events_timeout");
    stub_funcs.ptr_libusb_handle_events_timeout_completed = try_find_sym(libusb, "libusb_handle_events_timeout_completed");
    stub_funcs.ptr_libusb_handle_events = try_find_sym(libusb, "libusb_handle_events");
    stub_funcs.ptr_libusb_handle_events_completed = try_find_sym(libusb, "libusb_handle_events_completed");
    stub_funcs.ptr_libusb_handle_events_locked = try_find_sym(libusb, "libusb_handle_events_locked");
    stub_funcs.ptr_libusb_pollfds_handle_timeouts = try_find_sym(libusb, "libusb_pollfds_handle_timeouts");
    stub_funcs.ptr_libusb_get_next_timeout = try_find_sym(libusb, "libusb_get_next_timeout");
    stub_funcs.ptr_libusb_get_pollfds = try_find_sym(libusb, "libusb_get_pollfds");
    stub_funcs.ptr_libusb_free_pollfds = try_find_sym(libusb, "libusb_free_pollfds");
    stub_funcs.ptr_libusb_set_pollfd_notifiers = try_find_sym(libusb, "libusb_set_pollfd_notifiers");
    stub_funcs.ptr_libusb_hotplug_register_callback = try_find_sym(libusb, "libusb_hotplug_register_callback");
    stub_funcs.ptr_libusb_hotplug_deregister_callback = try_find_sym(libusb, "libusb_hotplug_deregister_callback");
    stub_funcs.ptr_libusb_hotplug_get_user_data = try_find_sym(libusb, "libusb_hotplug_get_user_data");
    stub_funcs.ptr_ms_libusb_init = try_find_sym(libusb, "libusb_init");
    stub_funcs.ptr_ms_libusb_exit = try_find_sym(libusb, "libusb_exit");
    stub_funcs.ptr_ms_libusb_set_debug = try_find_sym(libusb, "libusb_set_debug");
    stub_funcs.ptr_ms_libusb_set_log_cb = try_find_sym(libusb, "libusb_set_log_cb");
    stub_funcs.ptr_ms_libusb_get_version = try_find_sym(libusb, "libusb_get_version");
    stub_funcs.ptr_ms_libusb_has_capability = try_find_sym(libusb, "libusb_has_capability");
    stub_funcs.ptr_ms_libusb_error_name = try_find_sym(libusb, "libusb_error_name");
    stub_funcs.ptr_ms_libusb_setlocale = try_find_sym(libusb, "libusb_setlocale");
    stub_funcs.ptr_ms_libusb_strerror = try_find_sym(libusb, "libusb_strerror");
    stub_funcs.ptr_ms_libusb_get_device_list = try_find_sym(libusb, "libusb_get_device_list");
    stub_funcs.ptr_ms_libusb_free_device_list = try_find_sym(libusb, "libusb_free_device_list");
    stub_funcs.ptr_ms_libusb_ref_device = try_find_sym(libusb, "libusb_ref_device");
    stub_funcs.ptr_ms_libusb_unref_device = try_find_sym(libusb, "libusb_unref_device");
    stub_funcs.ptr_ms_libusb_get_configuration = try_find_sym(libusb, "libusb_get_configuration");
    stub_funcs.ptr_ms_libusb_get_device_descriptor = try_find_sym(libusb, "libusb_get_device_descriptor");
    stub_funcs.ptr_ms_libusb_get_active_config_descriptor = try_find_sym(libusb, "libusb_get_active_config_descriptor");
    stub_funcs.ptr_ms_libusb_get_config_descriptor = try_find_sym(libusb, "libusb_get_config_descriptor");
    stub_funcs.ptr_ms_libusb_get_config_descriptor_by_value = try_find_sym(libusb, "libusb_get_config_descriptor_by_value");
    stub_funcs.ptr_ms_libusb_free_config_descriptor = try_find_sym(libusb, "libusb_free_config_descriptor");
    stub_funcs.ptr_ms_libusb_get_ss_endpoint_companion_descriptor = try_find_sym(libusb, "libusb_get_ss_endpoint_companion_descriptor");
    stub_funcs.ptr_ms_libusb_free_ss_endpoint_companion_descriptor = try_find_sym(libusb, "libusb_free_ss_endpoint_companion_descriptor");
    stub_funcs.ptr_ms_libusb_get_bos_descriptor = try_find_sym(libusb, "libusb_get_bos_descriptor");
    stub_funcs.ptr_ms_libusb_free_bos_descriptor = try_find_sym(libusb, "libusb_free_bos_descriptor");
    stub_funcs.ptr_ms_libusb_get_usb_2_0_extension_descriptor = try_find_sym(libusb, "libusb_get_usb_2_0_extension_descriptor");
    stub_funcs.ptr_ms_libusb_free_usb_2_0_extension_descriptor = try_find_sym(libusb, "libusb_free_usb_2_0_extension_descriptor");
    stub_funcs.ptr_ms_libusb_get_ss_usb_device_capability_descriptor = try_find_sym(libusb, "libusb_get_ss_usb_device_capability_descriptor");
    stub_funcs.ptr_ms_libusb_free_ss_usb_device_capability_descriptor = try_find_sym(libusb, "libusb_free_ss_usb_device_capability_descriptor");
    stub_funcs.ptr_ms_libusb_get_container_id_descriptor = try_find_sym(libusb, "libusb_get_container_id_descriptor");
    stub_funcs.ptr_ms_libusb_free_container_id_descriptor = try_find_sym(libusb, "libusb_free_container_id_descriptor");
    stub_funcs.ptr_ms_libusb_get_bus_number = try_find_sym(libusb, "libusb_get_bus_number");
    stub_funcs.ptr_ms_libusb_get_port_number = try_find_sym(libusb, "libusb_get_port_number");
    stub_funcs.ptr_ms_libusb_get_port_numbers = try_find_sym(libusb, "libusb_get_port_numbers");
    stub_funcs.ptr_ms_libusb_get_port_path = try_find_sym(libusb, "libusb_get_port_path");
    stub_funcs.ptr_ms_libusb_get_parent = try_find_sym(libusb, "libusb_get_parent");
    stub_funcs.ptr_ms_libusb_get_device_address = try_find_sym(libusb, "libusb_get_device_address");
    stub_funcs.ptr_ms_libusb_get_device_speed = try_find_sym(libusb, "libusb_get_device_speed");
    stub_funcs.ptr_ms_libusb_get_max_packet_size = try_find_sym(libusb, "libusb_get_max_packet_size");
    stub_funcs.ptr_ms_libusb_get_max_iso_packet_size = try_find_sym(libusb, "libusb_get_max_iso_packet_size");
    stub_funcs.ptr_ms_libusb_wrap_sys_device = try_find_sym(libusb, "libusb_wrap_sys_device");
    stub_funcs.ptr_ms_libusb_open = try_find_sym(libusb, "libusb_open");
    stub_funcs.ptr_ms_libusb_close = try_find_sym(libusb, "libusb_close");
    stub_funcs.ptr_ms_libusb_get_device = try_find_sym(libusb, "libusb_get_device");
    stub_funcs.ptr_ms_libusb_set_configuration = try_find_sym(libusb, "libusb_set_configuration");
    stub_funcs.ptr_ms_libusb_claim_interface = try_find_sym(libusb, "libusb_claim_interface");
    stub_funcs.ptr_ms_libusb_release_interface = try_find_sym(libusb, "libusb_release_interface");
    stub_funcs.ptr_ms_libusb_open_device_with_vid_pid = try_find_sym(libusb, "libusb_open_device_with_vid_pid");
    stub_funcs.ptr_ms_libusb_set_interface_alt_setting = try_find_sym(libusb, "libusb_set_interface_alt_setting");
    stub_funcs.ptr_ms_libusb_clear_halt = try_find_sym(libusb, "libusb_clear_halt");
    stub_funcs.ptr_ms_libusb_reset_device = try_find_sym(libusb, "libusb_reset_device");
    stub_funcs.ptr_ms_libusb_alloc_streams = try_find_sym(libusb, "libusb_alloc_streams");
    stub_funcs.ptr_ms_libusb_free_streams = try_find_sym(libusb, "libusb_free_streams");
    stub_funcs.ptr_ms_libusb_dev_mem_alloc = try_find_sym(libusb, "libusb_dev_mem_alloc");
    stub_funcs.ptr_ms_libusb_dev_mem_free = try_find_sym(libusb, "libusb_dev_mem_free");
    stub_funcs.ptr_ms_libusb_kernel_driver_active = try_find_sym(libusb, "libusb_kernel_driver_active");
    stub_funcs.ptr_ms_libusb_detach_kernel_driver = try_find_sym(libusb, "libusb_detach_kernel_driver");
    stub_funcs.ptr_ms_libusb_attach_kernel_driver = try_find_sym(libusb, "libusb_attach_kernel_driver");
    stub_funcs.ptr_ms_libusb_set_auto_detach_kernel_driver = try_find_sym(libusb, "libusb_set_auto_detach_kernel_driver");
    stub_funcs.ptr_ms_libusb_alloc_transfer = try_find_sym(libusb, "libusb_alloc_transfer");
    stub_funcs.ptr_ms_libusb_submit_transfer = try_find_sym(libusb, "libusb_submit_transfer");
    stub_funcs.ptr_ms_libusb_cancel_transfer = try_find_sym(libusb, "libusb_cancel_transfer");
    stub_funcs.ptr_ms_libusb_free_transfer = try_find_sym(libusb, "libusb_free_transfer");
    stub_funcs.ptr_ms_libusb_transfer_set_stream_id = try_find_sym(libusb, "libusb_transfer_set_stream_id");
    stub_funcs.ptr_ms_libusb_transfer_get_stream_id = try_find_sym(libusb, "libusb_transfer_get_stream_id");
    stub_funcs.ptr_ms_libusb_control_transfer = try_find_sym(libusb, "libusb_control_transfer");
    stub_funcs.ptr_ms_libusb_bulk_transfer = try_find_sym(libusb, "libusb_bulk_transfer");
    stub_funcs.ptr_ms_libusb_interrupt_transfer = try_find_sym(libusb, "libusb_interrupt_transfer");
    stub_funcs.ptr_ms_libusb_get_string_descriptor_ascii = try_find_sym(libusb, "libusb_get_string_descriptor_ascii");
    stub_funcs.ptr_ms_libusb_try_lock_events = try_find_sym(libusb, "libusb_try_lock_events");
    stub_funcs.ptr_ms_libusb_lock_events = try_find_sym(libusb, "libusb_lock_events");
    stub_funcs.ptr_ms_libusb_unlock_events = try_find_sym(libusb, "libusb_unlock_events");
    stub_funcs.ptr_ms_libusb_event_handling_ok = try_find_sym(libusb, "libusb_event_handling_ok");
    stub_funcs.ptr_ms_libusb_event_handler_active = try_find_sym(libusb, "libusb_event_handler_active");
    stub_funcs.ptr_ms_libusb_interrupt_event_handler = try_find_sym(libusb, "libusb_interrupt_event_handler");
    stub_funcs.ptr_ms_libusb_lock_event_waiters = try_find_sym(libusb, "libusb_lock_event_waiters");
    stub_funcs.ptr_ms_libusb_unlock_event_waiters = try_find_sym(libusb, "libusb_unlock_event_waiters");
    stub_funcs.ptr_ms_libusb_wait_for_event = try_find_sym(libusb, "libusb_wait_for_event");
    stub_funcs.ptr_ms_libusb_handle_events_timeout = try_find_sym(libusb, "libusb_handle_events_timeout");
    stub_funcs.ptr_ms_libusb_handle_events_timeout_completed = try_find_sym(libusb, "libusb_handle_events_timeout_completed");
    stub_funcs.ptr_ms_libusb_handle_events = try_find_sym(libusb, "libusb_handle_events");
    stub_funcs.ptr_ms_libusb_handle_events_completed = try_find_sym(libusb, "libusb_handle_events_completed");
    stub_funcs.ptr_ms_libusb_handle_events_locked = try_find_sym(libusb, "libusb_handle_events_locked");
    stub_funcs.ptr_ms_libusb_pollfds_handle_timeouts = try_find_sym(libusb, "libusb_pollfds_handle_timeouts");
    stub_funcs.ptr_ms_libusb_get_next_timeout = try_find_sym(libusb, "libusb_get_next_timeout");
    stub_funcs.ptr_ms_libusb_get_pollfds = try_find_sym(libusb, "libusb_get_pollfds");
    stub_funcs.ptr_ms_libusb_free_pollfds = try_find_sym(libusb, "libusb_free_pollfds");
    stub_funcs.ptr_ms_libusb_set_pollfd_notifiers = try_find_sym(libusb, "libusb_set_pollfd_notifiers");
    stub_funcs.ptr_ms_libusb_hotplug_register_callback = try_find_sym(libusb, "libusb_hotplug_register_callback");
    stub_funcs.ptr_ms_libusb_hotplug_deregister_callback = try_find_sym(libusb, "libusb_hotplug_deregister_callback");
    stub_funcs.ptr_ms_libusb_hotplug_get_user_data = try_find_sym(libusb, "libusb_hotplug_get_user_data");
}

int (libusb_init)(libusb_context * * ctx) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_init(ctx);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_init(ctx); 
	}}
void (libusb_exit)(libusb_context * ctx) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_exit(ctx);  
	}
	else {
		stub_funcs.ptr_ms_libusb_exit(ctx); 
	}}
void (libusb_set_debug)(libusb_context * ctx, int level) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_set_debug(ctx, level);  
	}
	else {
		stub_funcs.ptr_ms_libusb_set_debug(ctx, level); 
	}}
void (libusb_set_log_cb)(libusb_context * ctx, libusb_log_cb cb, int mode) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_set_log_cb(ctx, cb, mode);  
	}
	else {
		stub_funcs.ptr_ms_libusb_set_log_cb(ctx, cb, mode); 
	}}
const struct libusb_version * (libusb_get_version)(void) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_version();  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_version(); 
	}}
int (libusb_has_capability)(uint32_t capability) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_has_capability(capability);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_has_capability(capability); 
	}}
const char * (libusb_error_name)(int errcode) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_error_name(errcode);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_error_name(errcode); 
	}}
int (libusb_setlocale)(const char * locale) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_setlocale(locale);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_setlocale(locale); 
	}}
const char * (libusb_strerror)(int errcode) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_strerror(errcode);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_strerror(errcode); 
	}}
ssize_t (libusb_get_device_list)(libusb_context * ctx, libusb_device * * * list) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_device_list(ctx, list);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_device_list(ctx, list); 
	}}
void (libusb_free_device_list)(libusb_device * * list, int unref_devices) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_free_device_list(list, unref_devices);  
	}
	else {
		stub_funcs.ptr_ms_libusb_free_device_list(list, unref_devices); 
	}}
libusb_device * (libusb_ref_device)(libusb_device * dev) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_ref_device(dev);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_ref_device(dev); 
	}}
void (libusb_unref_device)(libusb_device * dev) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_unref_device(dev);  
	}
	else {
		stub_funcs.ptr_ms_libusb_unref_device(dev); 
	}}
int (libusb_get_configuration)(libusb_device_handle * dev, int * config) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_configuration(dev, config);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_configuration(dev, config); 
	}}
int (libusb_get_device_descriptor)(libusb_device * dev, struct libusb_device_descriptor * desc) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_device_descriptor(dev, desc);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_device_descriptor(dev, desc); 
	}}
int (libusb_get_active_config_descriptor)(libusb_device * dev, struct libusb_config_descriptor * * config) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_active_config_descriptor(dev, config);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_active_config_descriptor(dev, config); 
	}}
int (libusb_get_config_descriptor)(libusb_device * dev, uint8_t config_index, struct libusb_config_descriptor * * config) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_config_descriptor(dev, config_index, config);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_config_descriptor(dev, config_index, config); 
	}}
int (libusb_get_config_descriptor_by_value)(libusb_device * dev, uint8_t bConfigurationValue, struct libusb_config_descriptor * * config) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_config_descriptor_by_value(dev, bConfigurationValue, config);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_config_descriptor_by_value(dev, bConfigurationValue, config); 
	}}
void (libusb_free_config_descriptor)(struct libusb_config_descriptor * config) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_free_config_descriptor(config);  
	}
	else {
		stub_funcs.ptr_ms_libusb_free_config_descriptor(config); 
	}}
int (libusb_get_ss_endpoint_companion_descriptor)(libusb_context * ctx, const struct libusb_endpoint_descriptor * endpoint, struct libusb_ss_endpoint_companion_descriptor * * ep_comp) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_ss_endpoint_companion_descriptor(ctx, endpoint, ep_comp);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_ss_endpoint_companion_descriptor(ctx, endpoint, ep_comp); 
	}}
void (libusb_free_ss_endpoint_companion_descriptor)(struct libusb_ss_endpoint_companion_descriptor * ep_comp) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_free_ss_endpoint_companion_descriptor(ep_comp);  
	}
	else {
		stub_funcs.ptr_ms_libusb_free_ss_endpoint_companion_descriptor(ep_comp); 
	}}
int (libusb_get_bos_descriptor)(libusb_device_handle * dev_handle, struct libusb_bos_descriptor * * bos) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_bos_descriptor(dev_handle, bos);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_bos_descriptor(dev_handle, bos); 
	}}
void (libusb_free_bos_descriptor)(struct libusb_bos_descriptor * bos) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_free_bos_descriptor(bos);  
	}
	else {
		stub_funcs.ptr_ms_libusb_free_bos_descriptor(bos); 
	}}
int (libusb_get_usb_2_0_extension_descriptor)(libusb_context * ctx, struct libusb_bos_dev_capability_descriptor * dev_cap, struct libusb_usb_2_0_extension_descriptor * * usb_2_0_extension) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_usb_2_0_extension_descriptor(ctx, dev_cap, usb_2_0_extension);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_usb_2_0_extension_descriptor(ctx, dev_cap, usb_2_0_extension); 
	}}
void (libusb_free_usb_2_0_extension_descriptor)(struct libusb_usb_2_0_extension_descriptor * usb_2_0_extension) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_free_usb_2_0_extension_descriptor(usb_2_0_extension);  
	}
	else {
		stub_funcs.ptr_ms_libusb_free_usb_2_0_extension_descriptor(usb_2_0_extension); 
	}}
int (libusb_get_ss_usb_device_capability_descriptor)(libusb_context * ctx, struct libusb_bos_dev_capability_descriptor * dev_cap, struct libusb_ss_usb_device_capability_descriptor * * ss_usb_device_cap) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_ss_usb_device_capability_descriptor(ctx, dev_cap, ss_usb_device_cap);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_ss_usb_device_capability_descriptor(ctx, dev_cap, ss_usb_device_cap); 
	}}
void (libusb_free_ss_usb_device_capability_descriptor)(struct libusb_ss_usb_device_capability_descriptor * ss_usb_device_cap) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_free_ss_usb_device_capability_descriptor(ss_usb_device_cap);  
	}
	else {
		stub_funcs.ptr_ms_libusb_free_ss_usb_device_capability_descriptor(ss_usb_device_cap); 
	}}
int (libusb_get_container_id_descriptor)(libusb_context * ctx, struct libusb_bos_dev_capability_descriptor * dev_cap, struct libusb_container_id_descriptor * * container_id) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_container_id_descriptor(ctx, dev_cap, container_id);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_container_id_descriptor(ctx, dev_cap, container_id); 
	}}
void (libusb_free_container_id_descriptor)(struct libusb_container_id_descriptor * container_id) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_free_container_id_descriptor(container_id);  
	}
	else {
		stub_funcs.ptr_ms_libusb_free_container_id_descriptor(container_id); 
	}}
uint8_t (libusb_get_bus_number)(libusb_device * dev) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_bus_number(dev);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_bus_number(dev); 
	}}
uint8_t (libusb_get_port_number)(libusb_device * dev) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_port_number(dev);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_port_number(dev); 
	}}
int (libusb_get_port_numbers)(libusb_device * dev, uint8_t * port_numbers, int port_numbers_len) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_port_numbers(dev, port_numbers, port_numbers_len);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_port_numbers(dev, port_numbers, port_numbers_len); 
	}}
int (libusb_get_port_path)(libusb_context * ctx, libusb_device * dev, uint8_t * path, uint8_t path_length) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_port_path(ctx, dev, path, path_length);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_port_path(ctx, dev, path, path_length); 
	}}
libusb_device * (libusb_get_parent)(libusb_device * dev) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_parent(dev);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_parent(dev); 
	}}
uint8_t (libusb_get_device_address)(libusb_device * dev) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_device_address(dev);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_device_address(dev); 
	}}
int (libusb_get_device_speed)(libusb_device * dev) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_device_speed(dev);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_device_speed(dev); 
	}}
int (libusb_get_max_packet_size)(libusb_device * dev, unsigned char endpoint) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_max_packet_size(dev, endpoint);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_max_packet_size(dev, endpoint); 
	}}
int (libusb_get_max_iso_packet_size)(libusb_device * dev, unsigned char endpoint) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_max_iso_packet_size(dev, endpoint);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_max_iso_packet_size(dev, endpoint); 
	}}
int (libusb_wrap_sys_device)(libusb_context * ctx, intptr_t sys_dev, libusb_device_handle * * dev_handle) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_wrap_sys_device(ctx, sys_dev, dev_handle);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_wrap_sys_device(ctx, sys_dev, dev_handle); 
	}}
int (libusb_open)(libusb_device * dev, libusb_device_handle * * dev_handle) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_open(dev, dev_handle);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_open(dev, dev_handle); 
	}}
void (libusb_close)(libusb_device_handle * dev_handle) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_close(dev_handle);  
	}
	else {
		stub_funcs.ptr_ms_libusb_close(dev_handle); 
	}}
libusb_device * (libusb_get_device)(libusb_device_handle * dev_handle) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_device(dev_handle);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_device(dev_handle); 
	}}
int (libusb_set_configuration)(libusb_device_handle * dev_handle, int configuration) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_set_configuration(dev_handle, configuration);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_set_configuration(dev_handle, configuration); 
	}}
int (libusb_claim_interface)(libusb_device_handle * dev_handle, int interface_number) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_claim_interface(dev_handle, interface_number);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_claim_interface(dev_handle, interface_number); 
	}}
int (libusb_release_interface)(libusb_device_handle * dev_handle, int interface_number) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_release_interface(dev_handle, interface_number);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_release_interface(dev_handle, interface_number); 
	}}
libusb_device_handle * (libusb_open_device_with_vid_pid)(libusb_context * ctx, uint16_t vendor_id, uint16_t product_id) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_open_device_with_vid_pid(ctx, vendor_id, product_id);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_open_device_with_vid_pid(ctx, vendor_id, product_id); 
	}}
int (libusb_set_interface_alt_setting)(libusb_device_handle * dev_handle, int interface_number, int alternate_setting) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_set_interface_alt_setting(dev_handle, interface_number, alternate_setting);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_set_interface_alt_setting(dev_handle, interface_number, alternate_setting); 
	}}
int (libusb_clear_halt)(libusb_device_handle * dev_handle, unsigned char endpoint) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_clear_halt(dev_handle, endpoint);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_clear_halt(dev_handle, endpoint); 
	}}
int (libusb_reset_device)(libusb_device_handle * dev_handle) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_reset_device(dev_handle);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_reset_device(dev_handle); 
	}}
int (libusb_alloc_streams)(libusb_device_handle * dev_handle, uint32_t num_streams, unsigned char * endpoints, int num_endpoints) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_alloc_streams(dev_handle, num_streams, endpoints, num_endpoints);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_alloc_streams(dev_handle, num_streams, endpoints, num_endpoints); 
	}}
int (libusb_free_streams)(libusb_device_handle * dev_handle, unsigned char * endpoints, int num_endpoints) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_free_streams(dev_handle, endpoints, num_endpoints);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_free_streams(dev_handle, endpoints, num_endpoints); 
	}}
unsigned char * (libusb_dev_mem_alloc)(libusb_device_handle * dev_handle, size_t length) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_dev_mem_alloc(dev_handle, length);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_dev_mem_alloc(dev_handle, length); 
	}}
int (libusb_dev_mem_free)(libusb_device_handle * dev_handle, unsigned char * buffer, size_t length) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_dev_mem_free(dev_handle, buffer, length);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_dev_mem_free(dev_handle, buffer, length); 
	}}
int (libusb_kernel_driver_active)(libusb_device_handle * dev_handle, int interface_number) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_kernel_driver_active(dev_handle, interface_number);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_kernel_driver_active(dev_handle, interface_number); 
	}}
int (libusb_detach_kernel_driver)(libusb_device_handle * dev_handle, int interface_number) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_detach_kernel_driver(dev_handle, interface_number);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_detach_kernel_driver(dev_handle, interface_number); 
	}}
int (libusb_attach_kernel_driver)(libusb_device_handle * dev_handle, int interface_number) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_attach_kernel_driver(dev_handle, interface_number);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_attach_kernel_driver(dev_handle, interface_number); 
	}}
int (libusb_set_auto_detach_kernel_driver)(libusb_device_handle * dev_handle, int enable) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_set_auto_detach_kernel_driver(dev_handle, enable);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_set_auto_detach_kernel_driver(dev_handle, enable); 
	}}
struct libusb_transfer * (libusb_alloc_transfer)(int iso_packets) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_alloc_transfer(iso_packets);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_alloc_transfer(iso_packets); 
	}}
int (libusb_submit_transfer)(struct libusb_transfer * transfer) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_submit_transfer(transfer);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_submit_transfer(transfer); 
	}}
int (libusb_cancel_transfer)(struct libusb_transfer * transfer) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_cancel_transfer(transfer);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_cancel_transfer(transfer); 
	}}
void (libusb_free_transfer)(struct libusb_transfer * transfer) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_free_transfer(transfer);  
	}
	else {
		stub_funcs.ptr_ms_libusb_free_transfer(transfer); 
	}}
void (libusb_transfer_set_stream_id)(struct libusb_transfer * transfer, uint32_t stream_id) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_transfer_set_stream_id(transfer, stream_id);  
	}
	else {
		stub_funcs.ptr_ms_libusb_transfer_set_stream_id(transfer, stream_id); 
	}}
uint32_t (libusb_transfer_get_stream_id)(struct libusb_transfer * transfer) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_transfer_get_stream_id(transfer);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_transfer_get_stream_id(transfer); 
	}}
int (libusb_control_transfer)(libusb_device_handle * dev_handle, uint8_t request_type, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char * data, uint16_t wLength, unsigned int timeout) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_control_transfer(dev_handle, request_type, bRequest, wValue, wIndex, data, wLength, timeout);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_control_transfer(dev_handle, request_type, bRequest, wValue, wIndex, data, wLength, timeout); 
	}}
int (libusb_bulk_transfer)(libusb_device_handle * dev_handle, unsigned char endpoint, unsigned char * data, int length, int * actual_length, unsigned int timeout) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_bulk_transfer(dev_handle, endpoint, data, length, actual_length, timeout);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_bulk_transfer(dev_handle, endpoint, data, length, actual_length, timeout); 
	}}
int (libusb_interrupt_transfer)(libusb_device_handle * dev_handle, unsigned char endpoint, unsigned char * data, int length, int * actual_length, unsigned int timeout) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_interrupt_transfer(dev_handle, endpoint, data, length, actual_length, timeout);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_interrupt_transfer(dev_handle, endpoint, data, length, actual_length, timeout); 
	}}
int (libusb_get_string_descriptor_ascii)(libusb_device_handle * dev_handle, uint8_t desc_index, unsigned char * data, int length) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_string_descriptor_ascii(dev_handle, desc_index, data, length);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_string_descriptor_ascii(dev_handle, desc_index, data, length); 
	}}
int (libusb_try_lock_events)(libusb_context * ctx) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_try_lock_events(ctx);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_try_lock_events(ctx); 
	}}
void (libusb_lock_events)(libusb_context * ctx) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_lock_events(ctx);  
	}
	else {
		stub_funcs.ptr_ms_libusb_lock_events(ctx); 
	}}
void (libusb_unlock_events)(libusb_context * ctx) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_unlock_events(ctx);  
	}
	else {
		stub_funcs.ptr_ms_libusb_unlock_events(ctx); 
	}}
int (libusb_event_handling_ok)(libusb_context * ctx) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_event_handling_ok(ctx);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_event_handling_ok(ctx); 
	}}
int (libusb_event_handler_active)(libusb_context * ctx) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_event_handler_active(ctx);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_event_handler_active(ctx); 
	}}
void (libusb_interrupt_event_handler)(libusb_context * ctx) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_interrupt_event_handler(ctx);  
	}
	else {
		stub_funcs.ptr_ms_libusb_interrupt_event_handler(ctx); 
	}}
void (libusb_lock_event_waiters)(libusb_context * ctx) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_lock_event_waiters(ctx);  
	}
	else {
		stub_funcs.ptr_ms_libusb_lock_event_waiters(ctx); 
	}}
void (libusb_unlock_event_waiters)(libusb_context * ctx) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_unlock_event_waiters(ctx);  
	}
	else {
		stub_funcs.ptr_ms_libusb_unlock_event_waiters(ctx); 
	}}
int (libusb_wait_for_event)(libusb_context * ctx, struct timeval * tv) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_wait_for_event(ctx, tv);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_wait_for_event(ctx, tv); 
	}}
int (libusb_handle_events_timeout)(libusb_context * ctx, struct timeval * tv) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_handle_events_timeout(ctx, tv);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_handle_events_timeout(ctx, tv); 
	}}
int (libusb_handle_events_timeout_completed)(libusb_context * ctx, struct timeval * tv, int * completed) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_handle_events_timeout_completed(ctx, tv, completed);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_handle_events_timeout_completed(ctx, tv, completed); 
	}}
int (libusb_handle_events)(libusb_context * ctx) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_handle_events(ctx);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_handle_events(ctx); 
	}}
int (libusb_handle_events_completed)(libusb_context * ctx, int * completed) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_handle_events_completed(ctx, completed);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_handle_events_completed(ctx, completed); 
	}}
int (libusb_handle_events_locked)(libusb_context * ctx, struct timeval * tv) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_handle_events_locked(ctx, tv);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_handle_events_locked(ctx, tv); 
	}}
int (libusb_pollfds_handle_timeouts)(libusb_context * ctx) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_pollfds_handle_timeouts(ctx);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_pollfds_handle_timeouts(ctx); 
	}}
int (libusb_get_next_timeout)(libusb_context * ctx, struct timeval * tv) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_next_timeout(ctx, tv);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_next_timeout(ctx, tv); 
	}}
const struct libusb_pollfd * * (libusb_get_pollfds)(libusb_context * ctx) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_get_pollfds(ctx);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_get_pollfds(ctx); 
	}}
void (libusb_free_pollfds)(const struct libusb_pollfd * * pollfds) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_free_pollfds(pollfds);  
	}
	else {
		stub_funcs.ptr_ms_libusb_free_pollfds(pollfds); 
	}}
void (libusb_set_pollfd_notifiers)(libusb_context * ctx, libusb_pollfd_added_cb added_cb, libusb_pollfd_removed_cb removed_cb, void * user_data) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_set_pollfd_notifiers(ctx, added_cb, removed_cb, user_data);  
	}
	else {
		stub_funcs.ptr_ms_libusb_set_pollfd_notifiers(ctx, added_cb, removed_cb, user_data); 
	}}
int (libusb_hotplug_register_callback)(libusb_context * ctx, int events, int flags, int vendor_id, int product_id, int dev_class, libusb_hotplug_callback_fn cb_fn, void * user_data, libusb_hotplug_callback_handle * callback_handle) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_hotplug_register_callback(ctx, events, flags, vendor_id, product_id, dev_class, cb_fn, user_data, callback_handle);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_hotplug_register_callback(ctx, events, flags, vendor_id, product_id, dev_class, cb_fn, user_data, callback_handle); 
	}}
void (libusb_hotplug_deregister_callback)(libusb_context * ctx, libusb_hotplug_callback_handle callback_handle) { 
	if((IsLinux())){
		stub_funcs.ptr_libusb_hotplug_deregister_callback(ctx, callback_handle);  
	}
	else {
		stub_funcs.ptr_ms_libusb_hotplug_deregister_callback(ctx, callback_handle); 
	}}
void * (libusb_hotplug_get_user_data)(libusb_context * ctx, libusb_hotplug_callback_handle callback_handle) { 
	if((IsLinux())){
		return stub_funcs.ptr_libusb_hotplug_get_user_data(ctx, callback_handle);  
	}
	else {
		return stub_funcs.ptr_ms_libusb_hotplug_get_user_data(ctx, callback_handle); 
	}}

void close_libusb(void) {
    cosmo_dlclose(libusb);
}
