// JNI header for native interface communication
#include <jni.h>
// String manipulation utilities
#include <string>
// Standard reverse algorithm
#include <algorithm>
// Integer limits for overflow detection
#include <climits>
// Android logging system for debugging
#include <android/log.h>

// Custom identifier for filtering native logs in Logcat
#define ENGINE_LOG_TAG "NATIVE_COMPUTE"
// Shorthand for information-level logging
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, ENGINE_LOG_TAG, __VA_ARGS__)
// Shorthand for error-level logging
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, ENGINE_LOG_TAG, __VA_ARGS__)

// =============================================
// DEMONSTRATION 1: Engine Status Retrieval
// Purpose: Shows basic string return from C++
// =============================================
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_lab22_1nativecalculator_MainActivity_fetchEngineStatus(
        JNIEnv* env_context,
        jobject /* java_caller */) {

    LOG_INFO("Status request received from application layer");
    return env_context->NewStringUTF("Native Compute Engine Active v2.0");
}

// =============================================
// DEMONSTRATION 2: Exponentiation with Safety
// Purpose: Shows integer parameter handling and validation
// =============================================
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_lab22_1nativecalculator_MainActivity_exponentiate(
        JNIEnv* env_context,
        jobject /* java_caller */,
        jint base_value,
        jint power_value) {

    // Guard clause: reject negative exponents
    if (power_value < 0) {
        LOG_ERROR("Rejected negative exponent: %d", power_value);
        return -1;
    }

    // Guard clause: handle mathematically undefined case
    if (base_value == 0 && power_value == 0) {
        LOG_ERROR("Undefined computation: 0^0 requested");
        return -2;
    }

    // Perform exponentiation with overflow protection
    long long accumulator = 1;
    for (int iteration = 0; iteration < power_value; iteration++) {
        accumulator *= base_value;

        // Detect if result exceeds integer storage capacity
        if (accumulator > INT_MAX) {
            LOG_ERROR("Overflow detected: %d^%d exceeds INT_MAX", base_value, power_value);
            return -3;
        }
    }

    LOG_INFO("Computation successful: %d^%d = %lld", base_value, power_value, accumulator);
    return static_cast<jint>(accumulator);
}

// =============================================
// DEMONSTRATION 3: String Transformation
// Purpose: Shows Java String ↔ C++ string conversion
// =============================================
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_lab22_1nativecalculator_MainActivity_processText(
        JNIEnv* env_context,
        jobject /* java_caller */,
        jstring input_text) {

    // Validate input is not null
    if (input_text == nullptr) {
        LOG_ERROR("Received null string reference");
        return env_context->NewStringUTF("ERROR: Null input rejected");
    }

    // Extract native characters from Java string
    const char* raw_buffer = env_context->GetStringUTFChars(input_text, nullptr);

    if (raw_buffer == nullptr) {
        LOG_ERROR("Memory allocation failed for string conversion");
        return env_context->NewStringUTF("ERROR: Memory allocation failed");
    }

    // Create modifiable C++ string copy
    std::string mutable_copy(raw_buffer);

    // Release Java string resources back to JVM
    env_context->ReleaseStringUTFChars(input_text, raw_buffer);

    // Apply reversal transformation
    std::reverse(mutable_copy.begin(), mutable_copy.end());

    LOG_INFO("Text transformation completed successfully");
    return env_context->NewStringUTF(mutable_copy.c_str());
}

// =============================================
// DEMONSTRATION 4: Array Statistical Analysis
// Purpose: Shows Java array handling and element access
// =============================================
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_lab22_1nativecalculator_MainActivity_calculateAverage(
        JNIEnv* env_context,
        jobject /* java_caller */,
        jintArray data_array) {

    // Validate array reference exists
    if (data_array == nullptr) {
        LOG_ERROR("Null array reference provided");
        return -1;
    }

    // Determine array dimensions
    jsize element_count = env_context->GetArrayLength(data_array);

    // Handle edge case: empty array
    if (element_count == 0) {
        LOG_INFO("Empty array received, average is 0");
        return 0;
    }

    // Obtain direct memory access to array elements
    jint* raw_data = env_context->GetIntArrayElements(data_array, nullptr);

    if (raw_data == nullptr) {
        LOG_ERROR("Failed to acquire array memory lock");
        return -2;
    }

    // Compute arithmetic mean
    long long running_total = 0;
    for (jsize index = 0; index < element_count; index++) {
        running_total += raw_data[index];
    }

    // Release array lock back to Java VM
    env_context->ReleaseIntArrayElements(data_array, raw_data, JNI_ABORT);

    // Calculate integer average
    jint average_result = static_cast<jint>(running_total / element_count);

    LOG_INFO("Average computed: %d from %d elements (total: %lld)",
             average_result, element_count, running_total);
    return average_result;
}