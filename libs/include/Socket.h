#include <arpa/inet.h>
#include <inttypes.h>
#include <sys/socket.h>

class Socket {
public:
  enum class type { UDP, TCP };
  static const int buflen_max = 9000;

  class Endpoint {
  public:
    const char *ipaddr;
    uint16_t port;
    Endpoint(const char *ip_address, uint16_t port_number)
        : ipaddr(ip_address), port(port_number) {}
  };

  Socket(Socket::type type);
  int Buflen(uint16_t buflen);

  int Local(const char *ipaddr, int port);
  int Remote(const char *ipaddr, int port);

  int Receive();
  int Receive(void *buffer, int rcvlen);

  int Send();

private:
  int s_{-1};
  uint16_t buflen_{buflen_max};
  struct sockaddr_in local_;
  struct sockaddr_in remote_;
  char buffer_[buflen_max];

protected:
  Socket();
};

class UDPServer : public Socket {
public:
  UDPServer(Endpoint local) : Socket(Socket::type::UDP) {
    Local(local.ipaddr, local.port);
  };
};

class UDPClient : public Socket {
public:
  UDPClient(Endpoint local, struct Endpoint remote)
      : Socket(Socket::type::UDP) {
    Local(local.ipaddr, local.port);
    Remote(remote.ipaddr, remote.port);
  };
};