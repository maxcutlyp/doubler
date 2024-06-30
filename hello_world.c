#include <stdlib.h>
#include <stdio.h>
#include <lv2/core/lv2.h>

#define URI "http://example.com/helloworld"

#define SKIP_MS 10

typedef struct {
    float *buf;
    uint32_t end;
    uint32_t length;
} RingBuffer;

static inline void rb_push(RingBuffer *rb, float value) {
    rb->end = (rb->end + 1) % rb->length;
    rb->buf[rb->end] = value;
}

static inline void rb_push_many(RingBuffer *rb, float *values, uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
        rb->buf[(rb->end + i) % rb->length] = values[i];
    }
    rb->end = (rb->end + n) % rb->length;
}

static inline float rb_get(RingBuffer *rb) {
    return rb->buf[(rb->end + 1) % rb->length];
}

void rb_init(RingBuffer *rb, uint32_t length) {
    rb->buf = (float *)calloc(length, sizeof(float));
    rb->length = length;
    rb->end = 0;
}

void rb_free(RingBuffer *rb) {
    free(rb->buf);
}


typedef struct {
    float *input;
    float *output;
    double rate;
    RingBuffer overflow;
} HelloWorld;

static LV2_Handle instantiate(
    const LV2_Descriptor *descriptor,
    double rate,
    const char *bundle_path,
    const LV2_Feature* const* features
) {
    (void)descriptor;
    (void)bundle_path;
    (void)features;

    HelloWorld *plugin = (HelloWorld *)malloc(sizeof(HelloWorld));
    plugin->rate = rate;

    rb_init(&plugin->overflow, rate * SKIP_MS * 0.001);

    return (LV2_Handle)plugin;
}

static void connect_port(
    LV2_Handle instance,
    uint32_t port,
    void *data_location
) {
    HelloWorld *plugin = (HelloWorld *)instance;
    switch (port) {
        case 0: {
            plugin->input = (float *)data_location;
        }; break;
        case 1: {
            plugin->output = (float *)data_location;
        }; break;
    }
}

static void activate(LV2_Handle instance) {
    (void)instance;
}

static void run(LV2_Handle instance, uint32_t n_samples) {
    HelloWorld *plugin = (HelloWorld *)instance;

    for (uint32_t i = 0; i < n_samples; i++) {
        plugin->output[i] = rb_get(&plugin->overflow) + plugin->input[i];
        rb_push(&plugin->overflow, plugin->input[i]);
    }
}

static void deactivate(LV2_Handle instance) {
    (void)instance;
}

static void cleanup(LV2_Handle instance) {
    HelloWorld *plugin = (HelloWorld *)instance;
    rb_free(&plugin->overflow);

    free(instance);
}

static const void *extension_data(const char *uri) {
    (void)uri;
    return NULL;
}

static const LV2_Descriptor descriptor = {
    URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data,
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor *lv2_descriptor(uint32_t index) {
    return (index == 0) ? &descriptor : NULL;
}


