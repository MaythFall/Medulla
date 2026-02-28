#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <stdexcept>
#include <vector>
#include <functional>
#include "Types.hpp"

#if _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <unistd.h>
#include <csignal>

namespace medulla {

    class Server {
        private:
        static constexpr int BACKLOG = 10;
        int socket_descriptor;
        sockaddr_in address;

        void read_message(SecureBuffer<uint8_t>& buffer) {
            uint32_t size;
            ssize_t read_len = 0;
            while (read_len < sizeof(uint32_t)) {
                ssize_t curr_read = read(socket_descriptor, (uint8_t*)&size + read_len, sizeof(uint32_t) - read_len);
                if (curr_read <= 0) {
                    close(socket_descriptor);
                    throw std::runtime_error("Failed to read from socket stream");
                }
                read_len += curr_read;
            }
            size = ntohl(size);
            buffer.resize(size);
            for (read_len = 0; read_len < size;) {
                ssize_t curr_read = read(socket_descriptor, buffer.data() + read_len, size - read_len);
                if (curr_read <= 0) {
                    close(socket_descriptor);
                    throw std::runtime_error("Failed to read from socket stream");
                }
                read_len += curr_read;
            }
        }

        public:

        void send_message(SecureBuffer<uint8_t>& data) {
            uint32_t loc_size = data.size();
            uint32_t net_size = htonl(loc_size);
            ssize_t write_len = 0;
            while (write_len < sizeof(uint32_t)) {
                ssize_t curr_write = write(socket_descriptor, (uint8_t*)&net_size + write_len, sizeof(uint32_t) - write_len);
                if (curr_write <= 0) {
                    close(socket_descriptor);
                    throw std::runtime_error("Failed to write to socket stream");
                }
                write_len += curr_write;
            }
            for (write_len = 0; write_len < loc_size;) {
                ssize_t curr_write = write(socket_descriptor, reinterpret_cast<void*>(data.data() + write_len), loc_size - write_len);
                if (curr_write <= 0) {
                    close(socket_descriptor);
                    throw std::runtime_error("Failed to write to socket stream");
                }
                write_len += curr_write;
            }
        }

        void runSocket(std::function<void(Server&, SecureBuffer<uint8_t>&)> action, SecureBuffer<uint8_t>& buffer) {
            signal(SIGCHLD, SIG_IGN);
            socklen_t len = sizeof(address);
            while (socket_descriptor != -1) {
                int child_fd = -1;
                child_fd = accept(socket_descriptor, (sockaddr*)&address, &len);
                if (child_fd == -1) throw std::runtime_error("Failed to access connection request");
                int proccess_id = fork();
                if (proccess_id == -1) {throw std::runtime_error("Failed to create child process");}
                else if (proccess_id == 0) {
                    close(socket_descriptor);
                    socket_descriptor = child_fd;
                    read_message(buffer);
                    action(*this, buffer);
                    close(socket_descriptor);
                    exit(0);
                } else {
                    close(child_fd);
                }
            }
        }

        Server(int port_num = 12578) {
            socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
            if (socket_descriptor == -1) throw std::runtime_error("Failed to create socket");

            int opt = 1, err = 1;
            err = setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
            if (err == -1) throw std::runtime_error("Failed to set Socket");
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = htonl(INADDR_ANY);
            address.sin_port = htons(port_num);
            err = bind(socket_descriptor, (sockaddr*)&address, sizeof(address));
            if (err == -1) throw std::runtime_error("Failed to bind Socket");
            err = listen(socket_descriptor, BACKLOG);
            if (err == -1) throw std::runtime_error("Failed to open Socket");
        }

        ~Server() {
            if (socket_descriptor != -1) close(socket_descriptor);
        }
    };
}
#endif