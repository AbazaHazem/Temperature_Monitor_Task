#include "pubsub.h"
#include <stdio.h>
#include <string.h>

#define MAX_TOPICS 10
#define MAX_SUBSCRIBERS 10
#define MAX_MESSAGE_LENGTH 64

typedef struct {
    char topic[MAX_MESSAGE_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    uint8_t readyToPublish;
} PubSubMessage;

typedef struct {
    char topic[MAX_MESSAGE_LENGTH];
    SubscriberCallback callback;
} Subscriber;

static PubSubMessage topics[MAX_TOPICS] = {0};
static Subscriber subscribers[MAX_SUBSCRIBERS] = {0};
static uint8_t topicCount = 0;
static uint8_t subscriberCount = 0;

void publish(const char* topic, const char* message) {
    for (uint8_t i = 0; i < topicCount; i++) {
        if (strcmp(topics[i].topic, topic) == 0) {
            strncpy(topics[i].message, message, MAX_MESSAGE_LENGTH);
            topics[i].readyToPublish = 1;
            return;
        }
    }

    if (topicCount < MAX_TOPICS) {
        strncpy(topics[topicCount].topic, topic, MAX_MESSAGE_LENGTH);
        strncpy(topics[topicCount].message, message, MAX_MESSAGE_LENGTH);
        topics[topicCount].readyToPublish = 1;
        topicCount++;
    }
}

void subscribe(const char* topic, SubscriberCallback callback) {
    if (subscriberCount < MAX_SUBSCRIBERS) {
        strncpy(subscribers[subscriberCount].topic, topic, MAX_MESSAGE_LENGTH);
        subscribers[subscriberCount].callback = callback;
        subscriberCount++;
    }
}

void processPubSub(void) {
    for (uint8_t i = 0; i < topicCount; i++) {
        if (topics[i].readyToPublish) {
            for (uint8_t j = 0; j < subscriberCount; j++) {
                if (strcmp(topics[i].topic, subscribers[j].topic) == 0) {
                    subscribers[j].callback(topics[i].message);
                }
            }
            topics[i].readyToPublish = 0;
        }
    }
}
