#include <stdio.h>
#include "FEConsumer_C.h"
#include "c/MD_Message.h"
#include <string.h>
#include <signal.h>

bool keep_running = false;
const FieldID MESSAGE_TYPE = 1750;
const FieldID VENDOR_SYMBOL = 1010;
const FieldID ISO_CODE = 17;
const FieldID MSG_GENESIS = 1409;
const FieldID LAST_PRICE = 50;
const FieldID UNOFFICIAL_PRICE = 1288;
const FieldID BID = 509;
const FieldID ASK = 609;

void sighandler(int signal) {
    keep_running = false;
}

void event_handler(const Event* e, void* closure) {
    Error* err;
    const char* event_values;
    Event_EventType event_type;
    Event_get_error(e, &err);
    Event_get_all_values(e, &event_values);
    Event_get_type(e, &event_type);
    Error_ErrorCode err_code;
    const char* err_desc;
    Error_get_error_code(err, &err_code);
    Error_get_description(err, &err_desc);
    printf("Received Event of type %i (Error %i:%s) %s\n", event_type,
        (int)err_code, err_desc, event_values);
}

void print_message(const char *topic, MD_Message *msg, void* closure) {
    char msg_type_filter = *((char*)closure);
    char message_type = '*';
    bool success = MD_Message_get_char(msg, MESSAGE_TYPE, &message_type);
    if (!success) {
        printf("Error while reading message type from msg for %s\n", topic);
    }
    if (msg_type_filter == '*' || msg_type_filter == message_type) {
        MD_Message_print(msg);
    }
}

void print_help() {
    printf("\nValid arguments:");
    printf("\n-U [user]");
    printf("\n\tRequired. Username to use for authentication");
    printf("\n-S [serial]");
    printf("\n\tRequired. Serial to use for authentication");
    printf("\n-D [device id]");
    printf("\n\tRequired. Device ID to use for authentication");
    printf("\n-t [topic]");
    printf("\n\tRequired. Comma separated list of topics to subscribe to");
    printf("\n-h [host]");
    printf("\n\tRequired. Authentication server to connect to, of the form \"<hostname>:<port>\"");
    printf("\n-P [path]");
    printf("\n\tPath to the OTP file to be used for authentication. Defaults to \"\"");
    printf("\n-m [msg-type]");
    printf("\n\tMessage type filter, prints only message that match that type");
    printf("\n-d [delayed]");
    printf("\n\tRequest delayed data");
    printf("\n-c [canned]");
    printf("\n\tRequest canned data");
    printf("\n-s [snapshot]");
    printf("\n\tRequest snapshot messages prior to streaming data");
    printf("\n-H,--help [help]");
    printf("\n\tDisplays this message");
    printf("\n\nSample usage:");
    printf("\n\tsimple_subscriber.exe -U Username -S Serial -D DeviceID -t topic -h host");
}

bool print_err(bool success, bool no_error, Error* err, const char* method_name) {
    if (!success) {
        printf("Invalid arguments passed to %s\n", method_name);
        return true;
    } else if (!no_error) {
        Error_ErrorCode code;
        const char* desc;
        Error_get_error_code(err, &code);
        Error_get_description(err, &desc);
        printf("Unable to %s (%i: %s)", method_name, code, desc);
        return true;
    }
    return false;
}

int main( int argc, const char* argv[] )
{
    signal(SIGINT, &sighandler);

    char msg_type_filter = '*';
    const char* topics = "";
    const char *host = NULL, *user = NULL, *serial = NULL, *device = NULL, *path = "";
    bool delayed = false, snapshot = false, canned = false;

    bool keep_parsing = true;
    for (int index = 1; index < argc && keep_parsing; index++) {
        if (argv[index][0] != '-') { continue; }
        char opt = argv[index][1];
        if (opt == '-') {
            if (!strcmp(argv[index], "--help")) {
                print_help();
                return 0;
            } else if (!strcmp(argv[index], "--")) {
                keep_parsing = false;
                continue;
            } else {
                printf("Unexpected argument %s while parsing arguments\n", argv[index]);
                print_help();
                return 1;
            }
        }
        if (index == argc - 1 && (opt != 'd' && opt != 'c' && opt != 's' && opt != 'M' && opt != 'H')) {
            printf("Error while parsing arguments. Expected argument after %s\n", argv[index]);
            print_help();
            return 1;
        }
        switch (opt) {
        case 'U':
            user = argv[++index];
            break;
        case 'S':
            serial = argv[++index];
            break;
        case 'D':
            device = argv[++index];
            break;
        case 'P':
            path = argv[++index];
            break;
        case 't':
            topics = argv[++index];
            break;
        case 'h':
            host = argv[++index];
            break;
        case 'm':
            msg_type_filter = argv[++index][0];
            break;
        case 'd':
            delayed = true;
            break;
        case 'c':
            canned = true;
            break;
        case 's':
            snapshot = true;
            break;
        case 'H':
            print_help();
            return 0;
        }
    }

    if (!host || !user || !serial || !topics || !device) {
        printf("Missing required arguments. One or more of [host, user, serial, device, topics] is missing\n");
        print_help();
        return 1;
    }

    FEConsumer* consumer = FEConsumer_new(1, 1);
    Error* err = Error_new();
    bool success, no_error;

    success = FEConsumer_authenticate(consumer, host, user, serial, device, "", "", path, false, err);
    success &= Error_NoError(err, &no_error);
    if (print_err(success, no_error, err, "Authenticate")) {
        Error_delete(err);
        FEConsumer_delete(consumer);
        return 1;
    };
    FEConsumer_event_cb(consumer, &event_handler, NULL);
    
    success = FEConsumer_connect(consumer, err);
    success &= Error_NoError(err, &no_error);
    if (print_err(success, no_error, err, "Connect")) {
        Error_delete(err);
        FEConsumer_delete(consumer);
        return 1;
    };

    success = FEConsumer_log_in(consumer, err);
    success &= Error_NoError(err, &no_error);
    print_err(success, no_error, err, "Login");

    FEConsumer_SocketType fd;
    success = FEConsumer_register_callback(consumer, "", &print_message, &msg_type_filter, &fd, err);
    success &= Error_NoError(err, &no_error);
    print_err(success, no_error, err, "Register callback");

    FEConsumer_DataMode data_mode = LIVE;
    if (delayed) {
        printf("Requesting DELAYED\n");
        data_mode = DELAYED;
    } else if (canned) {
        printf("Requesting CANNED\n");
        data_mode = CANNED;
    }

    FEConsumer_SnapshotMode snap_mode = NO_SNAPSHOT;
    if (snapshot) {
        printf("Requesting Snapshots\n");
        snap_mode = REQUEST_SNAPSHOT;
    }

    //Iterate through topics
    //char* next_topic = NULL;
    //char delimiter = ',';
    //char* topic = strtok_r(topics, &delimiter, &next_topic);
    //while (topic != NULL) {
    //    success = FEConsumer_subscribe(consumer, topic, snap_mode, data_mode, &err);
    //    success &= Error_NoError(err, &no_error);
    //    if (!success || !no_error) {
    //        printf("Unable to subscribe to %s\n", topic);
    //    } else {
    //        keep_running = true;
    //    }
    //    topic = strtok_r(NULL, &delimiter, &next_topic);
    //}
    if (!FEConsumer_subscribe(consumer, topics, snap_mode, data_mode, err) || !Error_NoError(err, &no_error) || !no_error) {
        printf("Unable to subscribe to %s\n", topics);
    } else {
        keep_running = true;
    }

    if (!keep_running) {
        printf("No successful subscriptions. Exiting.\n");
        FEConsumer_disconnect(consumer, err);
        FEConsumer_delete(consumer);
        Error_delete(err);
        return 1;
    }

    while (keep_running) {
        FEConsumer_dispatch(consumer, 1000, err);
    }
    printf("Shutting down\n");
    FEConsumer_disconnect(consumer, err);
    FEConsumer_delete(consumer);
    Error_delete(err);
    printf("Good-bye\n");
    return 0;
}
