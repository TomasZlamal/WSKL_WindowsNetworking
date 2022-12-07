#ifndef ERRORS_HPP
#define ERRORS_HPP
#include <exception>
#include <string>
namespace WSKL {
    class SocketException : std::exception {
    public:
        SocketException(const char* message)
            : msg_(message) {}
        SocketException(const std::string& message)
            : msg_(message) {}
        virtual ~SocketException() noexcept {}
        virtual const char* c_str() const noexcept {
            return msg_.c_str();
        }

    protected:
        std::string msg_;
    };
}
#endif