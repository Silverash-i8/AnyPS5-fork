#include "prx/libc/include/general/VabiMacros.hpp"
#include "SceTypes.hpp"
#include <cstdint>
#include <cstdlib>

extern "C" {
int32_t APS5_VABI sceSystemGestureOpen(int32_t input_type, const void* param);
int APS5_VABI sceSystemGestureClose(int32_t gesture_handle);
int APS5_VABI sceSystemGestureCreateTouchRecognizer(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer, int32_t type, const SystemGestureRectangle* rectangle, const void* param);
int APS5_VABI sceSystemGestureAppendTouchRecognizer(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer);
int APS5_VABI sceSystemGestureGetTouchRecognizerInformation(int32_t gesture_handle, const SystemGestureTouchRecognizer* recognizer, SystemGestureTouchRecognizerInformation* information);
int APS5_VABI sceSystemGestureUpdateTouchRecognizerRectangle(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer, const SystemGestureRectangle* rectangle);
int APS5_VABI sceSystemGestureUpdateAllTouchRecognizer(int32_t gesture_handle);
int APS5_VABI sceSystemGestureGetTouchEvents(int32_t gesture_handle, const SystemGestureTouchRecognizer* recognizer, SystemGestureTouchEvent* event_buffer, uint32_t capacity_of_buffer, uint32_t* number_of_event);
int APS5_VABI sceSystemGestureGetPrimitiveTouchEvents(int32_t gesture_handle, SystemGesturePrimitiveTouchEvent* event_buffer, uint32_t capacity_of_buffer, uint32_t* number_of_event);
int APS5_VABI sceSystemGestureGetPrimitiveTouchEventsCount(int32_t gesture_handle);
int APS5_VABI sceSystemGestureGetPrimitiveTouchEventByIndex(int32_t gesture_handle, uint32_t index, SystemGesturePrimitiveTouchEvent* event);
int APS5_VABI sceSystemGestureResetPrimitiveTouchRecognizer(int32_t gesture_handle);
int APS5_VABI sceSystemGestureResetTouchRecognizer(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer);
int APS5_VABI sceSystemGestureRemoveTouchRecognizer(int32_t gesture_handle, SystemGestureTouchRecognizer* recognizer);
}

namespace {

void Require(bool value) { if (!value) std::abort(); }

constexpr int kInvalidArgument = static_cast<int>(0x80D10002);
constexpr int kInvalidHandle = static_cast<int>(0x80D10003);
constexpr int kIndexOutOfArray = static_cast<int>(0x80D10005);

}

int main() {
    const int32_t handle = sceSystemGestureOpen(0, nullptr);
    Require(handle > 0);

    SystemGestureTouchRecognizer recognizer{};
    const SystemGestureRectangle area{10.0f, 20.0f, 300.0f, 400.0f, {}};
    Require(sceSystemGestureCreateTouchRecognizer(handle + 1, &recognizer, 2, &area, nullptr) == kInvalidHandle);
    Require(sceSystemGestureCreateTouchRecognizer(handle, nullptr, 2, &area, nullptr) == kInvalidArgument);
    Require(sceSystemGestureCreateTouchRecognizer(handle, &recognizer, 2, &area, nullptr) == 0);
    Require(sceSystemGestureAppendTouchRecognizer(handle, &recognizer) == 0);

    SystemGestureTouchRecognizerInformation information{};
    Require(sceSystemGestureGetTouchRecognizerInformation(handle, &recognizer, nullptr) == kInvalidArgument);
    Require(sceSystemGestureGetTouchRecognizerInformation(handle, &recognizer, &information) == 0);
    Require(information.gesture_type == 2 && information.rectangle.x == 10.0f && information.rectangle.height == 400.0f && information.updated_time == 0);

    const SystemGestureRectangle moved{1.0f, 2.0f, 3.0f, 4.0f, {}};
    Require(sceSystemGestureUpdateTouchRecognizerRectangle(handle, &recognizer, &moved) == 0);
    Require(sceSystemGestureGetTouchRecognizerInformation(handle, &recognizer, &information) == 0);
    Require(information.gesture_type == 2 && information.rectangle.x == 1.0f && information.rectangle.width == 3.0f);

    Require(sceSystemGestureUpdateAllTouchRecognizer(handle) == 0);
    Require(sceSystemGestureUpdateAllTouchRecognizer(handle + 1) == kInvalidHandle);

    SystemGestureTouchEvent events[4]{};
    uint32_t count = 7;
    Require(sceSystemGestureGetTouchEvents(handle, &recognizer, events, 4, nullptr) == kInvalidArgument);
    Require(sceSystemGestureGetTouchEvents(handle, &recognizer, events, 4, &count) == 0 && count == 0);

    SystemGesturePrimitiveTouchEvent primitives[4]{};
    count = 7;
    Require(sceSystemGestureGetPrimitiveTouchEvents(handle, primitives, 4, &count) == 0 && count == 0);
    Require(sceSystemGestureGetPrimitiveTouchEventsCount(handle) == 0);
    Require(sceSystemGestureGetPrimitiveTouchEventByIndex(handle, 0, &primitives[0]) == kIndexOutOfArray);

    Require(sceSystemGestureResetPrimitiveTouchRecognizer(handle) == 0);
    Require(sceSystemGestureResetTouchRecognizer(handle, &recognizer) == 0);
    Require(sceSystemGestureRemoveTouchRecognizer(handle, &recognizer) == 0);
    Require(sceSystemGestureRemoveTouchRecognizer(handle + 1, &recognizer) == kInvalidHandle);
    Require(sceSystemGestureClose(handle) == 0);
}
