# TCP Client Server Chat

# Sever

## Header files

```c++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
```

- `unistd` - Gives access POSIX api (read, write, close, etc.)
- `arpa/inet.h` - Provides functions for network programming, particularly related to IPv4 and IPv6 addresses

### Variables

```c++
struct sockaddr_in {
    sa_family_t    sin_family; // Address family (AF_INET for IPv4)
    in_port_t      sin_port;   // Port number (in network byte order)
    struct in_addr sin_addr;   // IP address (in network byte order)
};
```

- `AF_INET` is used to mark that sockaddr_in is a IPv4 address (`AF_INET6` for IPv6)
- `sin_addr` is usually assigned value `INADDR_ANY` ie, listen to any available interfaces (0.0.0.0)
- `sin_port` is assigned `htons(PORT)`
    - `htons` (Host TO Network Short) - s a function that converts a port number from host byte order to network byte
      order (big-endian).

### Bind

he bind() function in C associates a socket with a specific IP address and port number on the local machine. This is
necessary for the server to listen for incoming connections on a specific port.

#### Function protocol
```c++
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

> What Happens Without bind()?
> 
> If you don't call bind(), the OS automatically assigns a random port when calling listen().
> 
> But for a server, you must bind to a specific port so clients know where to connect.


