#include <cstdint>
#include <cstddef>
#include <cstring>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"

// No touch input is emulated: recognizers exist but never report events.
static constexpr int32_t GESTURE_HANDLE = 1;
static constexpr int SCE_SYSTEM_GESTURE_ERROR_INVALID_ARGUMENT = static_cast<int>(0x80D10002);
static constexpr int SCE_SYSTEM_GESTURE_ERROR_INVALID_HANDLE = static_cast<int>(0x80D10003);
static constexpr int SCE_SYSTEM_GESTURE_ERROR_INDEX_OUT_OF_ARRAY = static_cast<int>(0x80D10005);

namespace {

struct RecognizerState {
    std::int32_t type;
    SystemGestureRectangle rectangle;
};

static_assert(sizeof(RecognizerState) <= sizeof(SystemGestureTouchRecognizer));

int Check(int32_t gesture_handle, const void* argument) {
    if (gesture_handle != GESTURE_HANDLE) return SCE_SYSTEM_GESTURE_ERROR_INVALID_HANDLE;
    return argument == nullptr ? SCE_SYSTEM_GESTURE_ERROR_INVALID_ARGUMENT : 0;
}

int NoEvents(int32_t gesture_handle, const void* recognizer, uint32_t* number_of_event) {
    if (const int error = Check(gesture_handle, recognizer)) return error;
    if (number_of_event == nullptr) return SCE_SYSTEM_GESTURE_ERROR_INVALID_ARGUMENT;
    *number_of_event = 0;
    return 0;
}

}

extern "C" {

int APS5_VABI sceSystemGestureAppendTouchRecognizer(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer) {
    return Check(gesture_handle, recognizer);
}

int APS5_VABI sceSystemGestureClose(int32_t gesture_handle) {
    return gesture_handle == GESTURE_HANDLE ? 0 : SCE_SYSTEM_GESTURE_ERROR_INVALID_HANDLE;
}

int APS5_VABI sceSystemGestureCreateTouchRecognizer(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer, int32_t type, const SystemGestureRectangle* rectangle, const void* param) {
    (void)param;
    if (const int error = Check(gesture_handle, recognizer)) return error;
    RecognizerState state{type, {}};
    if (rectangle != nullptr) state.rectangle = *rectangle;
    std::memset(recognizer, 0, sizeof(*recognizer));
    std::memcpy(recognizer, &state, sizeof(state));
    return 0;
}

int APS5_VABI sceSystemGestureFinalizePrimitiveTouchRecognizer(void) {
    return 0;
}

int APS5_VABI sceSystemGestureGetPrimitiveTouchEventByIndex(int32_t gesture_handle, uint32_t index, SystemGesturePrimitiveTouchEvent* event) {
    (void)index;
    if (const int error = Check(gesture_handle, event)) return error;
    return SCE_SYSTEM_GESTURE_ERROR_INDEX_OUT_OF_ARRAY;
}

int APS5_VABI sceSystemGestureGetPrimitiveTouchEventByPrimitiveID(int32_t gesture_handle, uint16_t primitiveId, SystemGesturePrimitiveTouchEvent* event) {
 (void)gesture_handle;
 (void)primitiveId;
 (void)event;
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceSystemGestureGetPrimitiveTouchEvents(int32_t gesture_handle, SystemGesturePrimitiveTouchEvent* event_buffer, uint32_t capacity_of_buffer, uint32_t* number_of_event) {
    (void)capacity_of_buffer;
    return NoEvents(gesture_handle, event_buffer, number_of_event);
}

int APS5_VABI sceSystemGestureGetPrimitiveTouchEventsCount(int32_t gesture_handle) {
    return gesture_handle == GESTURE_HANDLE ? 0 : SCE_SYSTEM_GESTURE_ERROR_INVALID_HANDLE;
}

int APS5_VABI sceSystemGestureGetTouchEventByEventID(int32_t gesture_handle, const SystemGestureTouchRecognizer* recognizer, uint32_t eventId, SystemGestureTouchEvent* event) {
 (void)gesture_handle;
 (void)recognizer;
 (void)eventId;
 (void)event;
 NotImplemented_nid_no_patch(__func__);
 return 0;
}

int APS5_VABI sceSystemGestureGetTouchEventByIndex(int32_t gesture_handle, const SystemGestureTouchRecognizer* recognizer, uint32_t index, SystemGestureTouchEvent* event) {
    (void)gesture_handle;
    (void)recognizer;
    (void)index;
    (void)event;
    return SCE_SYSTEM_GESTURE_ERROR_INDEX_OUT_OF_ARRAY;
}

int APS5_VABI sceSystemGestureGetTouchEvents(int32_t gesture_handle, const SystemGestureTouchRecognizer* recognizer, SystemGestureTouchEvent* event_buffer, uint32_t capacity_of_buffer, uint32_t* number_of_event) {
    (void)event_buffer;
    (void)capacity_of_buffer;
    return NoEvents(gesture_handle, recognizer, number_of_event);
}

int APS5_VABI sceSystemGestureGetTouchEventsCount(int32_t gesture_handle, const SystemGestureTouchRecognizer* recognizer) {
    (void)recognizer;
    return gesture_handle == GESTURE_HANDLE ? 0 : SCE_SYSTEM_GESTURE_ERROR_INVALID_HANDLE;
}

int APS5_VABI sceSystemGestureGetTouchRecognizerInformation(int32_t gesture_handle, const SystemGestureTouchRecognizer* recognizer, SystemGestureTouchRecognizerInformation* information) {
    if (const int error = Check(gesture_handle, recognizer)) return error;
    if (information == nullptr) return SCE_SYSTEM_GESTURE_ERROR_INVALID_ARGUMENT;
    RecognizerState state;
    std::memcpy(&state, recognizer, sizeof(state));
    *information = {};
    information->gesture_type = state.type;
    information->rectangle = state.rectangle;
    return 0;
}

int APS5_VABI sceSystemGestureInitializePrimitiveTouchRecognizer(const void* param) {
    (void)param;
    return 0;
}

int32_t APS5_VABI sceSystemGestureOpen(int32_t input_type, const void* param) {
    (void)input_type;
    (void)param;
    return GESTURE_HANDLE;
}

int APS5_VABI sceSystemGestureRemoveTouchRecognizer(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer) {
    return Check(gesture_handle, recognizer);
}

int APS5_VABI sceSystemGestureResetPrimitiveTouchRecognizer(int32_t gesture_handle) {
    return gesture_handle == GESTURE_HANDLE ? 0 : SCE_SYSTEM_GESTURE_ERROR_INVALID_HANDLE;
}

int APS5_VABI sceSystemGestureResetTouchRecognizer(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer) {
    return Check(gesture_handle, recognizer);
}

int APS5_VABI sceSystemGestureUpdateAllTouchRecognizer(int32_t gesture_handle) {
    return gesture_handle == GESTURE_HANDLE ? 0 : SCE_SYSTEM_GESTURE_ERROR_INVALID_HANDLE;
}

int APS5_VABI sceSystemGestureUpdatePrimitiveTouchRecognizer(int32_t gesture_handle, const void* param) {
    (void)param;
    return gesture_handle == GESTURE_HANDLE ? 0 : SCE_SYSTEM_GESTURE_ERROR_INVALID_HANDLE;
}

int APS5_VABI sceSystemGestureUpdateTouchRecognizer(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer) {
    (void)recognizer;
    return gesture_handle == GESTURE_HANDLE ? 0 : SCE_SYSTEM_GESTURE_ERROR_INVALID_HANDLE;
}

int APS5_VABI sceSystemGestureUpdateTouchRecognizerRectangle(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer, const SystemGestureRectangle* rectangle) {
    if (const int error = Check(gesture_handle, recognizer)) return error;
    if (rectangle == nullptr) return SCE_SYSTEM_GESTURE_ERROR_INVALID_ARGUMENT;
    RecognizerState state;
    std::memcpy(&state, recognizer, sizeof(state));
    state.rectangle = *rectangle;
    std::memcpy(recognizer, &state, sizeof(state));
    return 0;
}

}
