#ifndef TESTCONTAINERS_HPP
#define TESTCONTAINERS_HPP

#include <cstdint>
#include <filesystem>
#include <string>
#include <utility>
#include <expected>

namespace testcontainers {

namespace {
extern "C" {
#include "testcontainers-c/container.h"
}
}

struct TcpPort {
    uint16_t underlying;
};

struct UdpPort {
    uint16_t underlying;
};

enum struct HttpMethod {
    Get,
    Post,
    Head,
};

class Container {
public:
    constexpr Container(std::string image) : request_id(tc_container_create(image.c_str())) {}

    constexpr ~Container() {
        tc_container_terminate(request_id);
    }

    std::expected<void, std::string> run() {
        return std::unexpected{"unimplemented"};
    }

    void with_file(std::filesystem::path source, std::filesystem::path destination) {
        tc_container_with_file(request_id, source.c_str(), destination.c_str());
    }

    void wait_for_http(TcpPort port, std::filesystem::path endpoint) {
        tc_container_with_wait_for_http(request_id, port.underlying, endpoint.c_str());
    }

    void expose_port(TcpPort port) {
        tc_container_with_exposed_tcp_port(request_id, port.underlying);
    }

    void expose_port(UdpPort port);

    std::pair<uint16_t, std::expected<std::string, std::string>> send_http(HttpMethod method, TcpPort port, std::filesystem::path endpoint) {
        return {0, std::unexpected{"unimplemented"}};
    }

private:
    int request_id;
};

}

#endif // !TESTCONTAINERS_HPP
