//
// Created by marcin on 27.07.2021.
//
#include "canhacker.h"
#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include "lib.h"
#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include <atomic.h>

static const char CR = '\r';
static const char BEL = 7;
static const uint16_t TIMESTAMP_LIMIT = 0xEA60;

static enum CAN_CLOCK canClock = MCP_8MHZ;
static bool timestampEnabled = false;
static bool listenOnly = false;
static bool loopback = false;
static enum CAN_SPEED bitrate;
static bool isConnected = false;
static FILE *stream;
static FILE *debugStream;
extern volatile unsigned long timer1_millis;

enum COMMAND {
    COMMAND_SET_BITRATE = 'S', // set CAN bit rate
    COMMAND_SET_BTR = 's', // set CAN bit rate via
    COMMAND_OPEN_CAN_CHAN = 'O', // open CAN channel
    COMMAND_CLOSE_CAN_CHAN = 'C', // close CAN channel
    COMMAND_SEND_11BIT_ID = 't', // send CAN message with 11bit ID
    COMMAND_SEND_29BIT_ID = 'T', // send CAN message with 29bit ID
    COMMAND_SEND_R11BIT_ID = 'r', // send CAN remote message with 11bit ID
    COMMAND_SEND_R29BIT_ID = 'R', // send CAN remote message with 29bit ID
    COMMAND_READ_STATUS = 'F', // read status flag byte
    COMMAND_SET_ACR = 'M', // set Acceptance Code Register
    COMMAND_SET_AMR = 'm', // set Acceptance Mask Register
    COMMAND_GET_VERSION = 'V', // get hardware and software version
    COMMAND_GET_SW_VERSION = 'v', // get software version only
    COMMAND_GET_SERIAL = 'N', // get device serial number
    COMMAND_TIME_STAMP = 'Z', // toggle time stamp setting
    COMMAND_READ_ECR = 'E', // read Error Capture Register
    COMMAND_READ_ALCR = 'A', // read Arbritation Lost Capture Register
    COMMAND_READ_REG = 'G', // read register conten from SJA1000
    COMMAND_WRITE_REG = 'W', // write register content to SJA1000
    COMMAND_LISTEN_ONLY = 'L'  // switch to listen only mode
};

static enum ERROR canhacker_parseTransmit(const char *buffer, int length, struct can_frame *frame);

static enum ERROR canhacker_createTransmit(const struct can_frame *frame, char *buffer, int length);

static uint16_t canhacker_getTimestamp(void);

static enum ERROR canhacker_setFilter(uint32_t filter);

static enum ERROR canhacker_setFilterMask(uint32_t mask);

static enum ERROR canhacker_connectCan(void);

static enum ERROR canhacker_disconnectCan(void);

static bool canhacker_isConnected(void);

static enum ERROR canhacker_writeCan(const struct can_frame *frame);

static enum ERROR canhacker_writeStream(char character);

static enum ERROR canhacker_writeStreamFromBuffer(const char *buffer);

static enum ERROR canhacker_writeDebugStream(char character);

static enum ERROR canhacker_writeDebugStreamFromBuffer(const char *buffer);

static enum ERROR canhacker_writeDebugStreamInt(int buffer);

static enum ERROR canhacker_writeDebugStreamFromBufferWithSize(const uint8_t *buffer, size_t size);

static enum ERROR canhacker_writePgmDebugStream(PGM_P ifsh);

static enum ERROR canhacker_receiveSetBitrateCommand(const char *buffer, int length);

static enum ERROR canhacker_receiveTransmitCommand(const char *buffer, int length);

static enum ERROR canhacker_receiveTimestampCommand(const char *buffer, int length);

static enum ERROR canhacker_receiveCloseCommand(const char *buffer, int length);

static enum ERROR canhacker_receiveOpenCommand(const char *buffer, int length);

static enum ERROR canhacker_receiveListenOnlyCommand(const char *buffer, int length);

static enum ERROR canhacker_receiveSetAcrCommand(const char *buffer, int length);

static enum ERROR canhacker_receiveSetAmrCommand(const char *buffer, int length);

static unsigned long millis(void);

const char hex_asc_upper[] = "0123456789ABCDEF";

#define hex_asc_upper_lo(x)    hex_asc_upper[((x) & 0x0F)]
#define hex_asc_upper_hi(x)    hex_asc_upper[((x) & 0xF0) >> 4]

unsigned long millis() {
    unsigned long millis_return;
    ENTER_CRITICAL(R);
    millis_return = timer1_millis;
    EXIT_CRITICAL(R);
    return millis_return;
}

static inline void put_hex_byte(char *buf, uint8_t byte) {
    buf[0] = hex_asc_upper_hi(byte);
    buf[1] = hex_asc_upper_lo(byte);
}

static inline void put_id(char *buf, int end_offset, canid_t id) {
    /* build 3 (SFF) or 8 (EFF) digit CAN identifier */
    while (end_offset >= 0) {
        buf[end_offset--] = hex_asc_upper[id & 0xF];
        id >>= 4;
    }
}

#define put_sff_id(buf, id) put_id(buf, 2, id)
#define put_eff_id(buf, id) put_id(buf, 7, id)

void CanHacker(FILE *_stream, FILE *_debugStream) {
    stream = _stream;
    debugStream = _debugStream;
    canhacker_writePgmDebugStream(PSTR("Initialization\n"));
    MCP2515();
    reset();
    setConfigMode();
}

FILE *getInterfaceStream() {
    return stream;
}

void setClock(enum CAN_CLOCK clock) {
    canClock = clock;
}

static enum ERROR canhacker_connectCan() {
    enum MCP2515_ERROR error = setBitrateWithCANClock(bitrate, canClock);
    if (error != MCP2515_ERROR_OK) {
        canhacker_writePgmDebugStream(PSTR("setBitRate error:\n"));
        canhacker_writeDebugStreamInt((int) error);
        canhacker_writeDebugStream('\n');
        return ERROR_MCP2515_INIT_BITRATE;
    }
    if (loopback) {
        error = setLoopbackMode();
    } else if (listenOnly) {
        error = setListenOnlyMode();
    } else {
        error = setNormalMode();
    }
    if (error != MCP2515_ERROR_OK) {
        return ERROR_MCP2515_INIT_SET_MODE;
    }
    isConnected = true;
    return ERROR_OK;
}

static enum ERROR canhacker_disconnectCan() {
    isConnected = false;
    setConfigMode();
    return ERROR_OK;
}

static bool canhacker_isConnected() {
    return isConnected;
}

static enum ERROR canhacker_writeCan(const struct can_frame *frame) {
    if (sendMessage(frame) != MCP2515_ERROR_OK) {
        return ERROR_MCP2515_SEND;
    }
    return ERROR_OK;
}

enum ERROR pollReceiveCan() {
    if (!isConnected) {
        return ERROR_OK;
    }
    while (checkReceive()) {
        struct can_frame frame;
        if (readMessage(&frame) != MCP2515_ERROR_OK) {
            return ERROR_MCP2515_READ;
        }
        enum ERROR error = receiveCanFrame(&frame);
        if (error != ERROR_OK) {
            return error;
        }
    }
    return ERROR_OK;
}

enum ERROR receiveCan(enum RXBn rxBuffer) {
    if (!isConnected) {
        return ERROR_OK;
    }
    struct can_frame frame;
    enum MCP2515_ERROR result = readMessageThroughRXBn(rxBuffer, &frame);
    if (result == MCP2515_ERROR_NOMSG) {
        return ERROR_OK;
    }
    if (result != MCP2515_ERROR_OK) {
        return ERROR_MCP2515_READ;
    }
    return receiveCanFrame(&frame);
}

static uint16_t canhacker_getTimestamp() {
    return millis() % TIMESTAMP_LIMIT;
}

static enum ERROR canhacker_receiveSetBitrateCommand(const char *buffer, int length) {
    if (isConnected) {
        canhacker_writePgmDebugStream(PSTR("\"Bitrate command cannot be called while connected\\n\""));
        canhacker_writeStream(BEL);
        return ERROR_CONNECTED;
    }
    if (length < 2) {
        canhacker_writeStream(BEL);
        canhacker_writePgmDebugStream(PSTR("Bitrate command must by 2 bytes long\n"));
        canhacker_writeDebugStreamFromBufferWithSize((const uint8_t *) buffer, length);
        canhacker_writeDebugStream('\n');
        return ERROR_INVALID_COMMAND;
    }
    switch (buffer[1]) {
        case '0':
            canhacker_writePgmDebugStream(PSTR("Set bitrate 10KBPS\n"));
            bitrate = CAN_10KBPS;
            break;
        case '1':
            canhacker_writePgmDebugStream(PSTR("Set bitrate 20KBPS\n"));
            bitrate = CAN_20KBPS;
            break;
        case '2':
            canhacker_writePgmDebugStream(PSTR("Set bitrate 50KBPS\n"));
            bitrate = CAN_50KBPS;
            break;
        case '3':
            canhacker_writePgmDebugStream(PSTR("Set bitrate 100KBPS\n"));
            bitrate = CAN_100KBPS;
            break;
        case '4':
            canhacker_writePgmDebugStream(PSTR("Set bitrate 125KBPS\n"));
            bitrate = CAN_125KBPS;
            break;
        case '5':
            canhacker_writePgmDebugStream(PSTR("Set bitrate 250KBPS\n"));
            bitrate = CAN_250KBPS;
            break;
        case '6':
            canhacker_writePgmDebugStream(PSTR("Set bitrate 500KBPS\n"));
            bitrate = CAN_500KBPS;
            break;
        case '7':
            canhacker_writePgmDebugStream(PSTR("Bitrate 7 is not supported\n"));
            canhacker_writeDebugStreamFromBufferWithSize((const uint8_t *) buffer, length);
            canhacker_writeDebugStream('\n');
            canhacker_writeStream(BEL);
            return ERROR_INVALID_COMMAND;
            break;
        case '8':
            canhacker_writePgmDebugStream(PSTR("Set bitrate 1000KBPS\n"));
            bitrate = CAN_1000KBPS;
            break;
        default:
            canhacker_writePgmDebugStream(PSTR("Unexpected bitrate\n"));
            canhacker_writeDebugStreamFromBufferWithSize((const uint8_t *) buffer, length);
            canhacker_writeDebugStream('\n');
            canhacker_writeStream(BEL);
            return ERROR_INVALID_COMMAND;
            break;
    }
    return canhacker_writeStream(CR);
}

enum ERROR processInterrupt() {
    if (!isConnected) {
        return ERROR_OK;
    }
    uint8_t irq = getInterrupts();
    if (irq & CANINTF_ERRIF) {
        clearRXnOVR();
    }
    if (irq & CANINTF_RX0IF) {
        enum ERROR error = receiveCan(RXB0);
        if (error != ERROR_OK) {
            return error;
        }
    }
    if (irq & CANINTF_RX1IF) {
        enum ERROR error = receiveCan(RXB1);
        if (error != ERROR_OK) {
            return error;
        }
    }
    if (irq & CANINTF_WAKIF) {
        fprintf_P(debugStream, "MCP_WAKIF\r\n");
        clearInterrupts();
        fflush(debugStream);
    }
    if (irq & CANINTF_ERRIF) {
        fprintf_P(debugStream, "ERRIF\r\n");
        clearMERR();
        fflush(debugStream);
    }
    if (irq & CANINTF_MERRF) {
        fprintf_P(debugStream, "MERRF\r\n");
        clearInterrupts();
        fflush(debugStream);
    }
    return ERROR_OK;
}

static enum ERROR canhacker_setFilter(uint32_t filter) {
    if (isConnected) {
        canhacker_writePgmDebugStream(PSTR("Filter cannot be set while connected\n"));
        return ERROR_CONNECTED;
    }
    enum RXF filters[] = {RXF0, RXF1, RXF2, RXF3, RXF4, RXF5};
    for (uint8_t i = 0; i < 6; i++) {
        enum MCP2515_ERROR result = setFilter(filters[i], false, filter);
        if (result != MCP2515_ERROR_OK) {
            return ERROR_MCP2515_FILTER;
        }
    }
    return ERROR_OK;
}

static enum ERROR canhacker_setFilterMask(uint32_t mask) {
    if (isConnected) {
        canhacker_writePgmDebugStream(PSTR("Filter mask cannot be set while connected\n"));
        return ERROR_CONNECTED;
    }
    enum MASK masks[] = {MASK0, MASK1};
    for (uint8_t i = 0; i < 2; i++) {
        enum MCP2515_ERROR result = setFilterMask(masks[i], false, mask);
        if (result != MCP2515_ERROR_OK) {
            return ERROR_MCP2515_FILTER;
        }
    }
    return ERROR_OK;
}

static enum ERROR canhacker_writeStream(char character) {
    char str[2];
    str[0] = character;
    str[1] = '\0';
    return canhacker_writeStreamFromBuffer(str);
}

static enum ERROR canhacker_writeStreamFromBuffer(const char *buffer) {
    size_t printed = fputs(buffer, stream);
    if (printed != strlen(buffer))
        return ERROR_SERIAL_TX_OVERRUN;
    return ERROR_OK;
}

static enum ERROR canhacker_writeDebugStream(const char character) {
    if (debugStream != NULL) {
        putc(character, debugStream);
    }
    return ERROR_OK;
}

static enum ERROR canhacker_writeDebugStreamFromBuffer(const char *buffer) {
    if (debugStream != NULL) {
        fputs(buffer, debugStream);
    }
    return ERROR_OK;
}

static enum ERROR canhacker_writeDebugStreamFromBufferWithSize(const uint8_t *buffer, size_t size) {
    if (debugStream != NULL) {
        fwrite(buffer, sizeof buffer[0], size, debugStream);
    }
    return ERROR_OK;
}

static enum ERROR canhacker_writeDebugStreamInt(int buffer) {
    if (debugStream != NULL) {
        fputc(buffer, debugStream);
    }
    return ERROR_OK;
}

enum ERROR receiveCommand(const char *buffer, int length) {
    switch (buffer[0]) {
        case COMMAND_GET_SERIAL: {
            return canhacker_writeStreamFromBuffer(CANHACKER_SERIAL_RESPONSE);
        }
        case COMMAND_GET_SW_VERSION: {
            return canhacker_writeStreamFromBuffer(CANHACKER_SW_VERSION_RESPONSE);
        }
        case COMMAND_GET_VERSION: {
            return canhacker_writeStreamFromBuffer(CANHACKER_VERSION_RESPONSE);
        }
        case COMMAND_SEND_11BIT_ID:
        case COMMAND_SEND_29BIT_ID:
        case COMMAND_SEND_R11BIT_ID:
        case COMMAND_SEND_R29BIT_ID:
            return canhacker_receiveTransmitCommand(buffer, length);
        case COMMAND_CLOSE_CAN_CHAN:
            return canhacker_receiveCloseCommand(buffer, length);
        case COMMAND_OPEN_CAN_CHAN:
            return canhacker_receiveOpenCommand(buffer, length);
        case COMMAND_SET_BITRATE:
            return canhacker_receiveSetBitrateCommand(buffer, length);
        case COMMAND_SET_ACR:
            return canhacker_receiveSetAcrCommand(buffer, length);
        case COMMAND_SET_AMR:
            return canhacker_receiveSetAmrCommand(buffer, length);
        case COMMAND_SET_BTR:
            if (isConnected) {
                canhacker_writeStream(BEL);
                canhacker_writePgmDebugStream(PSTR("\"SET_BTR command cannot be called while connected\\n\""));
                return ERROR_CONNECTED;
            }
            canhacker_writePgmDebugStream("SET_BTR not supported\n");
            return canhacker_writeStream(CR);
        case COMMAND_LISTEN_ONLY:
            return canhacker_receiveListenOnlyCommand(buffer, length);
        case COMMAND_TIME_STAMP:
            return canhacker_receiveTimestampCommand(buffer, length);
        case COMMAND_WRITE_REG:
        case COMMAND_READ_REG: {
            return canhacker_writeStream(CR);
        }
        case COMMAND_READ_STATUS:
        case COMMAND_READ_ECR:
        case COMMAND_READ_ALCR: {
            if (!isConnected) {
                canhacker_writePgmDebugStream(PSTR("Read status, ecr, alcr while not connected\n"));
            }
        }
    }
}

enum ERROR receiveCanFrame(const struct can_frame *frame) {
    char out[35];
    enum ERROR error = canhacker_createTransmit(frame, out, 35);
    if (error != ERROR_OK) {
        return error;
    }
    return canhacker_writeStreamFromBuffer(out);
}

static enum ERROR canhacker_parseTransmit(const char *buffer, int length, struct can_frame *frame) {
    if (length < MIN_MESSAGE_LENGTH) {
        canhacker_writePgmDebugStream(PSTR("\"Transmit message lenght < minimum\\n\""));
        canhacker_writeDebugStreamFromBufferWithSize((const uint8_t *) buffer, length);
        canhacker_writeDebugStream('\n');
        return ERROR_INVALID_COMMAND;
    }
    int isExtended = 0;
    int isRTR = 0;

    switch (buffer[0]) {
        case 't':
            break;
        case 'T':
            isExtended = 1;
            break;
        case 'r':
            isRTR = 1;
            break;
        case 'R':
            isExtended = 1;
            isRTR = 1;
            break;
        default:
            canhacker_writePgmDebugStream(PSTR("\"Unexpected type of transmit command\\n\""));
            canhacker_writeDebugStreamFromBufferWithSize((const uint8_t *) buffer, length);
            canhacker_writeDebugStream('\n');
            return ERROR_INVALID_COMMAND;
    }
    int offset = 1;

    canid_t id = 0;
    int idChars = isExtended ? 8 : 3;
    for (int i = 0; i < idChars; i++) {
        id <<= 4;
        id += hexCharToByte(buffer[offset++]);
    }
    if (isRTR) {
        id |= CAN_RTR_FLAG;
    }
    if (isExtended) {
        id |= CAN_EFF_FLAG;
    }
    frame->can_id = id;
    uint8_t dlc = hexCharToByte(buffer[offset++]);
    if (dlc > 8) {
        canhacker_writePgmDebugStream(PSTR("DLC > 8\n"));
        canhacker_writeDebugStreamFromBufferWithSize((const uint8_t *) buffer, length);
        canhacker_writeDebugStream('\n');
        return ERROR_INVALID_COMMAND;
    }
    if (dlc == 0) {
        canhacker_writePgmDebugStream(PSTR("DLC = 0\n"));
        canhacker_writeDebugStreamFromBufferWithSize((const uint8_t *) buffer, length);
        canhacker_writeDebugStream('\n');
        return ERROR_INVALID_COMMAND;
    }
    frame->can_dlc = dlc;
    if (!isRTR) {
        for (int i = 0; i < dlc; i++) {
            char hiHex = buffer[offset++];
            char loHex = buffer[offset++];
            frame->data[i] = hexCharToByte(loHex) + (hexCharToByte(hiHex) << 4);
        }
    }
    return ERROR_OK;
}

static enum ERROR canhacker_createTransmit(const struct can_frame *frame, char *buffer, int length) {
    int offset;
    int len = frame->can_dlc;

    int isRTR = (frame->can_id & CAN_RTR_FLAG) ? 1 : 0;

    if (frame->can_id & CAN_ERR_FLAG) {
        return ERROR_ERROR_FRAME_NOT_SUPPORTED;
    } else if (frame->can_id & CAN_EFF_FLAG) {
        buffer[0] = isRTR ? 'R' : 'T';
        put_eff_id(buffer + 1, frame->can_id & CAN_EFF_MASK);
        offset = 9;
    } else {
        buffer[0] = isRTR ? 'r' : 't';
        put_sff_id(buffer + 1, frame->can_id & CAN_SFF_MASK);
        offset = 4;
    }

    buffer[offset++] = hex_asc_upper_lo(frame->can_dlc);

    if (!isRTR) {
        int i;
        for (i = 0; i < len; i++) {
            put_hex_byte(buffer + offset, frame->data[i]);
            offset += 2;
        }
    }

    if (timestampEnabled) {
        uint16_t ts = canhacker_getTimestamp();
        put_hex_byte(buffer + offset, ts >> 8);
        offset += 2;
        put_hex_byte(buffer + offset, ts);
        offset += 2;
    }

    buffer[offset++] = CR;
    buffer[offset] = '\0';

    if (offset >= length) {
        return ERROR_BUFFER_OVERFLOW;
    }
    return ERROR_OK;
}

enum ERROR sendFrame(const struct can_frame *frame) {
    return canhacker_writeCan(frame);
}

static enum ERROR canhacker_receiveTransmitCommand(const char *buffer, int length) {
    if (!isConnected) {
        canhacker_writePgmDebugStream(PSTR("Transmit command while not connected\n"));
        return ERROR_NOT_CONNECTED;
    }
    if (listenOnly) {
        return ERROR_LISTEN_ONLY;
    }
    struct can_frame frame;
    enum ERROR error = canhacker_parseTransmit(buffer, length, &frame);
    if (error != ERROR_OK) {
        return error;
    }
    error = canhacker_writeCan(&frame);
    if (error != ERROR_OK) {
        return error;
    }
    return canhacker_writeStream(CR);
}

enum ERROR canhacker_receiveTimestampCommand(const char *buffer, const int length) {
    if (length != 2) {
        canhacker_writeStream(BEL);
        canhacker_writePgmDebugStream(PSTR("Timestamp command must be 2 bytes long\n"));
        canhacker_writeDebugStreamFromBufferWithSize((const uint8_t *) buffer, length);
        canhacker_writeDebugStream('\n');
        return ERROR_INVALID_COMMAND;
    }
    switch (buffer[1]) {
        case '0':
            timestampEnabled = false;
            return canhacker_writeStream(CR);
        case '1':
            timestampEnabled = true;
            return canhacker_writeStream(CR);
        default:
            canhacker_writeStream(BEL);
            canhacker_writePgmDebugStream(PSTR("Timestamp cammand must have value 0 or 1\n"));
            canhacker_writeDebugStreamFromBufferWithSize((const uint8_t *) buffer, length);
            canhacker_writeDebugStream('\n');
            return ERROR_INVALID_COMMAND;
    }
    return ERROR_OK;
}

enum ERROR canhacker_receiveCloseCommand(const char *buffer, const int length) {
    canhacker_writePgmDebugStream(PSTR("receiveCloseCommand\n"));

    if (length < 1) {
        return ERROR_INVALID_COMMAND;
    }

    if (buffer[0] != COMMAND_CLOSE_CAN_CHAN) {
        return ERROR_INVALID_COMMAND;
    }

    if (!isConnected) {
        return canhacker_writeDebugStream(BEL);
    }
    enum ERROR error = canhacker_disconnectCan();
    if (error != ERROR_OK) {
        return error;
    }
    canhacker_writeStream(CR);
}

enum ERROR canhacker_receiveOpenCommand(const char *buffer, const int length) {

    if (length != 1) {
        return ERROR_INVALID_COMMAND;
    }

    if (buffer[0] != COMMAND_OPEN_CAN_CHAN) {
        return ERROR_INVALID_COMMAND;
    }

    canhacker_writePgmDebugStream(PSTR("receiveOpenCommand\n"));
    enum ERROR error = canhacker_connectCan();
    if (error != ERROR_OK) {
        return error;
    }
    canhacker_writeStream(CR);
}

enum ERROR canhacker_receiveListenOnlyCommand(const char *buffer, const int length) {

    if (length != 1) {
        return ERROR_INVALID_COMMAND;
    }

    if (buffer[0] != COMMAND_LISTEN_ONLY) {
        return ERROR_INVALID_COMMAND;
    }

    canhacker_writePgmDebugStream(PSTR("receiveListenOnlyCommand\n"));
    if (isConnected) {
        canhacker_writeStream(BEL);
        canhacker_writePgmDebugStream(PSTR("ListenOnly command cannot be called while connected\n"));
        return ERROR_CONNECTED;
    }
    listenOnly = true;
    return canhacker_writeStream(CR);
}

enum ERROR canhacker_receiveSetAcrCommand(const char *buffer, const int length) {
    if (length != 9) {
        canhacker_writeStream(BEL);
        canhacker_writePgmDebugStream(PSTR("ACR command must by 9 bytes long\n"));
        canhacker_writeDebugStreamFromBufferWithSize((const uint8_t*)buffer, length);
        canhacker_writeDebugStream('\n');
        return ERROR_INVALID_COMMAND;
    }
    uint32_t id = 0;
    for (int i=1; i<=8; i++) {
        id <<= 4;
        id += hexCharToByte(buffer[i]);
    }

    bool beenConnected = canhacker_isConnected();
    enum ERROR error;

    if (beenConnected) {
        error = canhacker_disconnectCan();
        if (error != ERROR_OK) {
            return error;
        }
    }

    error = setFilter(id);
    if (error != ERROR_OK) {
        return error;
    }

    if (beenConnected) {
        error = canhacker_connectCan();
        if (error != ERROR_OK) {
            return error;
        }
    }
    return canhacker_writeStream(CR);
}

enum ERROR canhacker_receiveSetAmrCommand(const char *buffer, const int length) {
    if (length != 9) {
        canhacker_writeStream(BEL);
        canhacker_writePgmDebugStream(PSTR("AMR command must by 9 bytes long\n"));
        canhacker_writeDebugStreamFromBufferWithSize((const uint8_t*)buffer, length);
        canhacker_writeDebugStream('\n');
        return ERROR_INVALID_COMMAND;
    }
    uint32_t id = 0;
    for (int i=1; i<=8; i++) {
        id <<= 4;
        id += hexCharToByte(buffer[i]);
    }

    bool beenConnected = canhacker_isConnected();
    enum ERROR error;

    if (beenConnected) {
        error = canhacker_disconnectCan();
        if (error != ERROR_OK) {
            return error;
        }
    }

    error = canhacker_setFilterMask(id);
    if (error != ERROR_OK) {
        return error;
    }

    if (beenConnected) {
        error = canhacker_connectCan();
        if (error != ERROR_OK) {
            return error;
        }
    }
    return canhacker_writeStream(CR);
}

enum ERROR enableLoopback() {
    if (isConnected) {
        canhacker_writePgmDebugStream(PSTR("Loopback cannot be changed while connected\n"));
        return ERROR_CONNECTED;
    }

    loopback = true;

    return ERROR_OK;
}

enum ERROR disableLoopback(void) {
    if (isConnected) {
        canhacker_writePgmDebugStream(PSTR("Loopback cannot be changed while connected\n"));
        return ERROR_CONNECTED;
    }
    loopback = false;
    return ERROR_OK;
}
