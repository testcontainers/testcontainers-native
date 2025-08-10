#ifndef TESTCONTAINERS_HPP
#define TESTCONTAINERS_HPP

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <iterator>
#include <memory>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <variant>

namespace testcontainers {

namespace {
extern "C" {
#include "testcontainers-c/container.h"
}
}

struct TcpPort {
    uint16_t underlying;
};

enum struct HttpMethod {
    Get,
    Post,
    Head,
};

class Builder;

auto materialize(const auto& in) {
#if __cplusplus >= 202300L
    return std::ranges::to<std::string>(in | std::ranges::views::filter([](char c) { return c != 0x00; }));
#else
    auto out = std::string{};
    std::ranges::for_each(in | std::ranges::views::filter([](char c) { return c != 0x00; }), [&](char c) { out += c; });
    return out;
#endif
}

class Container {
public:
    /// A `Container` cannot be explicitly created; use the
    /// `Container::Builder` instead.
    constexpr Container() = delete;

    /// A `Container` cannot be copied.
    explicit constexpr Container(const Container&) = delete;

    /// A `Container` cannot be copied.
    constexpr Container& operator=(const Container&) = delete;

    explicit constexpr Container(Container&& other) noexcept : container_id(-1) { *this = std::move(other); }

    constexpr Container& operator=(Container&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        std::swap(container_id, other.container_id);

        return *this;
    }

    constexpr ~Container() {
        if (container_id != -1) {
            tc_container_terminate(container_id);
        }
    }

    /// Send an HTTP request, type denoted by `method`, to the
    /// container. Perform the request on port `port` and to
    /// endpoint `endpoint`.
    std::pair<int, std::string> send_http(HttpMethod method, TcpPort port, const std::filesystem::path& endpoint) {
        switch (method) {
            case HttpMethod::Get:
                return send_http_get(port, endpoint);
            case HttpMethod::Post:
                return {0, "unimplemented"};
            case HttpMethod::Head:
                return {0, "unimplemented"};
        }

        std::unreachable();
    }

private:
    /// The `Configuration` structure holds all expected
    /// configuration related to the instantiation of a `Container`
    /// instance.
    struct Configuration {
        using Endpoint = std::filesystem::path;
        using Http     = std::pair<TcpPort, Endpoint>;
        using Timeout  = std::chrono::milliseconds;
        using WaitFor  = std::variant<Timeout, Http>;

        int     request_id     = -1;
        WaitFor wait_for_start = std::chrono::milliseconds{1000};
    };

    /// The `Awaiter` is for different types of waiting. It may be
    /// visited (with `std::visit`) when used on e.g. a
    /// `Container::Configuration::WaitFor` instance.
    struct Awaiter {
        explicit constexpr Awaiter(int container_id) : container_id(container_id) {}

        void operator()(const Configuration::Timeout& timeout) { std::this_thread::sleep_for(timeout); }

        void operator()(const Configuration::Http& http) {
            const auto& [port, endpoint] = http;
            tc_container_with_wait_for_http(container_id, port.underlying, endpoint.c_str());
        }

    private:
        int container_id;
    };

    /// Instantiate a container, and optionally wait for it to be
    /// ready.
    explicit constexpr Container(Configuration configuration) {
        char* error = nullptr;

        // Awaiting HTTP-events must occur before the container is
        // running.
        if (std::holds_alternative<Configuration::Http>(configuration.wait_for_start)) {
            Awaiter{container_id}(std::get<Configuration::Http>(configuration.wait_for_start));
        }

        if (container_id = tc_container_run(configuration.request_id, error); container_id == -1) {
            throw std::runtime_error{error};
        }

        // Awaiting a timeout must occur after a container is
        // running.
        if (std::holds_alternative<Configuration::Timeout>(configuration.wait_for_start)) {
            Awaiter{container_id}(std::get<Configuration::Timeout>(configuration.wait_for_start));
        }
    }

    /// Send an HTTP GET request to the container, on port `port`
    /// and with endpoint `endpoint`.
    constexpr std::pair<int, std::string> send_http_get(TcpPort port, const std::filesystem::path& endpoint) const {
        static constexpr size_t response_size     = 0xFFFF;
        static constexpr size_t error_size        = 0xFFFF;
        auto                    response_body_raw = std::array<char, response_size>{0x00};
        auto                    error_raw         = std::array<char, error_size>{0x00};

        auto code = tc_container_send_http_get(container_id, port.underlying, endpoint.c_str(),
                                               response_body_raw.data(), error_raw.data());

        if (code == -1) {
            auto error = materialize(error_raw);
            return std::pair{code, error};
        }

        auto response_body = materialize(response_body_raw);
        return std::pair{code, response_body};
    }

    int container_id;

public:
    /// `Builder` is the mechanism to instantiate `Container`s. It
    /// serves to configure the setup, and provides an interface to
    /// simply check for errors upon container creation.
    class Builder {
    public:
        constexpr Builder(const std::string& image)
            : configuration(Container::Configuration{.request_id = tc_container_create(image.c_str())}) {}

        /// Add a file from host `source` to container path
        /// `destination`.
        Builder with_file(std::filesystem::path source, std::filesystem::path destination) {
            tc_container_with_file(configuration.request_id, source.c_str(), destination.c_str());
            return *this;
        }

        /// Expose a TCP port out from the container.
        Builder expose_port(TcpPort port) {
            tc_container_with_exposed_tcp_port(configuration.request_id, port.underlying);
            return *this;
        }

        /// Add a timeout duration that's waited-for during
        /// container startup.
        Builder wait_for(std::chrono::milliseconds duration) {
            configuration.wait_for_start = duration;
            return *this;
        }

        /// Add an endpoint to act as health-check, determining if
        /// the container is ready for requests.
        Builder wait_for_http(TcpPort port, std::filesystem::path endpoint) {
            configuration.wait_for_start = std::pair{port, endpoint};
            return *this;
        }

        /// Construct a `Container` instance with the configured
        /// options.
        std::optional<Container> build() {
            try {
                return std::make_optional(Container{configuration});
            } catch (const std::runtime_error& exception) {
                return std::nullopt;
            }
        }

    private:
        Configuration configuration;
    };
};

}

#endif  // !TESTCONTAINERS_HPP
