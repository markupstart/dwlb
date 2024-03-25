/* See LICENSE file for copyright and license details. */
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pulse/simple.h>
#include <pulse/error.h>

#include "../util.h" // Assuming this has 'calculate_percentage'

// Global variables for PulseAudio
int error;
pa_mainloop *pa_ml;
pa_mainloop_api *pa_mlapi;
pa_context *pa_context;

// Function to query sink info
void get_volume_info(pa_context *context) {
    // ... (same as before)
}

// Callback to process sink information
void sink_info_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
    // ... (same as before)
}

// Callback for volume changes
void volume_change_callback(pa_context *c, const pa_subscription_event_t *event, uint32_t index, void *userdata) {
    // ... (same as before) 
}

int main() {
    // PulseAudio initialization
    pa_ml = pa_mainloop_new();
    pa_mlapi = pa_mainloop_get_api(pa_ml);
    pa_context = pa_context_new(pa_mlapi, "Volume Status Monitor");

    if (pa_context_connect(pa_context, NULL, 0, NULL) < 0) {
        fprintf(stderr, "Failed to connect to PulseAudio: %s\n", pa_strerror(error));
        return 1; // Handle error appropriately
    }

    // Subscribe to volume change events
    pa_subscription_mask_t mask = PA_SUBSCRIPTION_MASK_SINK;  
    pa_operation *op = pa_context_subscribe(pa_context, mask, volume_change_callback, NULL);
    pa_operation_unref(op);

    // Start the PulseAudio main loop
    int ret;  
    if (pa_mainloop_run(pa_ml, &ret) < 0) {
        fprintf(stderr, "pa_mainloop_run() failed: %s\n", pa_strerror(error));
        return 1; 
    }

    // PulseAudio cleanup
    pa_operation_unref(op); // Unsubscribe
    pa_context_disconnect(pa_context);
    pa_context_unref(pa_context);
    pa_mainloop_free(pa_ml);

    return 0;
} 

