#ifndef PUBSUB_H
#define PUBSUB_H

#include <stdint.h>

typedef void (*SubscriberCallback)(const char* message);

void publish(const char* topic, const char* message);
void subscribe(const char* topic, SubscriberCallback callback);
void processPubSub(void);

#endif // PUBSUB_H
