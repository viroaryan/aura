// ==========================================
// Aura Standard Library (aura_stdlib.h)
// Multi-platform Socket & System Bindings
// ==========================================

#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif

// ==========================================
// 1. Generic Size and Array helpers
// ==========================================

template <typename T>
inline auto size(const T& container) {
    return container.size();
}

// ==========================================
// 2. Implicit String Coercion Overloads
// ==========================================

inline std::string toAuraString(double val) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
}

inline std::string toAuraString(int val) {
    return std::to_string(val);
}

inline std::string toAuraString(bool val) {
    return val ? "true" : "false";
}

// std::string + primitive
inline std::string operator+(const std::string& str, int val) { return str + toAuraString(val); }
inline std::string operator+(const std::string& str, double val) { return str + toAuraString(val); }
inline std::string operator+(const std::string& str, bool val) { return str + toAuraString(val); }

// primitive + std::string
inline std::string operator+(int val, const std::string& str) { return toAuraString(val) + str; }
inline std::string operator+(double val, const std::string& str) { return toAuraString(val) + str; }
inline std::string operator+(bool val, const std::string& str) { return toAuraString(val) + str; }


// ==========================================
// 3. User Input & File System Helpers
// ==========================================

inline std::string input(const std::string& prompt) {
    std::cout << prompt;
    std::string result;
    std::getline(std::cin, result);
    return result;
}

inline std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[Aura Runtime Error] Failed to open file: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

inline void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[Aura Runtime Error] Failed to write to file: " << filename << std::endl;
        return;
    }
    file << content;
}

inline void appendFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "[Aura Runtime Error] Failed to append to file: " << filename << std::endl;
        return;
    }
    file << content;
}


// ==========================================
// 4. Dynamic HTTP Web Server
// ==========================================

inline void startServer(int port, const std::string& htmlContent) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[Aura Runtime Error] Failed to initialize Windows Sockets (Winsock)" << std::endl;
        return;
    }
#endif

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "[Aura Runtime Error] Socket creation failed" << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "[Aura Runtime Error] Port binding failed for port " << port << std::endl;
#ifdef _WIN32
        closesocket(server_fd);
        WSACleanup();
#else
        close(server_fd);
#endif
        return;
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "[Aura Runtime Error] Listening on port " << port << " failed" << std::endl;
        return;
    }

    std::cout << "\n=============================================" << std::endl;
    std::cout << "🚀 Aura Native Web Server is Live!" << std::endl;
    std::cout << "🔗 Access it at: http://localhost:" << port << std::endl;
    std::cout << "=============================================\n" << std::endl;

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) {
            continue;
        }

        char buffer[2048] = {0};
#ifdef _WIN32
        recv(client_fd, buffer, sizeof(buffer) - 1, 0);
#else
        read(client_fd, buffer, sizeof(buffer) - 1);
#endif

        // Build standard HTTP Header Response and append dynamically generated HTML
        std::string httpResponse = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " + std::to_string(htmlContent.length()) + "\r\n"
            "Connection: close\r\n"
            "\r\n" + 
            htmlContent;

        send(client_fd, httpResponse.c_str(), httpResponse.length(), 0);

#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
    }
}
