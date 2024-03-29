#include <array>

#include <cstdio>
#include <cstring>
#include <ctime>
#include <unistd.h>

#include <core/log_client.h>
static const char* logcatTag = "samp20";
extern uintptr_t g_gameAddr;

// This wrapper class is used to ensure that the file will be fclose()'d at the end of this library's lifetime
class LogFile {
public:
    ~LogFile() {
        if (m_logFile)
            fclose(m_logFile);
    }
    void operator=(std::FILE* filePtr) {
        m_logFile = filePtr;
    }
    auto operator*() {
        return m_logFile;
    }
    operator bool() const {
        return m_logFile != nullptr;
    }
private:
    std::FILE* m_logFile{};
};

static LogFile logFile{};
[[gnu::always_inline]] inline void checkLogFile() {
    char logFilePath[0x60];

    std::snprintf(logFilePath, std::size(logFilePath),
        "%sSAMP.log", reinterpret_cast<const char*>(g_gameAddr + 0x8b46a8));

    logFile = fopen(logFilePath, "a");
    char openedAt[0x2f];

    if (logFile) {
        // It isn't a critical error and occurs somewhat frequently at plugin startup
#ifndef NDEBUG
        __android_log_print(ANDROID_LOG_ERROR, logcatTag,
            "Can't open the log file in (...missing...)%s", logFilePath);
#endif
        return;
    }

    auto ts = time(nullptr);
    const auto timeData = localtime(&ts);

    std::strftime(openedAt, std::size(openedAt), "%T", timeData);
    if (*logFile) {
        std::fprintf(*logFile, "Start time of writing [%s], file pointer: %p from %s\n",
            openedAt, *logFile, logFilePath);
        __android_log_print(ANDROID_LOG_INFO, logcatTag, "Log file opened in %s", logFilePath);
        fflush(*logFile);
    }
}

int userDisplay(const LogId mode, const char* format, va_list va) {
#if NDEBUG
    if (mode == LogId::Debug)
        return 0;
#endif
    va_list cp;
    va_copy(cp, va);

    [[unlikely]] if (!*logFile && g_gameAddr)
        checkLogFile();

    const auto droidRet = __android_log_vprint(
        static_cast<android_LogPriority>(mode), logcatTag, format, cp);

#ifdef NDEBUG
    if (*logFile) {
        std::vfprintf(*logFile, format, cp);
        putc('\n', *logFile);
        fflush(*logFile);
    }
#endif
    va_end(cp);
    return droidRet;
}

void assertAbort(const char* cond, const char* fileName,
    int line, const char* format, ...) {

    thread_local std::array<char, 0x5f> assertBuffer;
    if (!cond)
        return;

    va_list va;
    va_start(va, format);
    vsnprintf(assertBuffer.data(), assertBuffer.size(), format, va);
    va_end(va);

    __android_log_assert(nullptr, logcatTag, "ASSERTION: by %s in %s:%d -> %s", cond, fileName,
        line, assertBuffer.data());
    std::terminate();
}

