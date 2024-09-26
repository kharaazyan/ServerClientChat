Here’s a comprehensive README file that covers both the TCP server-client application and the UDP communication code you provided. 

---

# Socket Communication Application

## Overview

This project implements a basic socket communication system in C, allowing clients to communicate with a server using both TCP and UDP protocols. The server can handle multiple clients, facilitating message passing between them. This system is designed to showcase multi-threading capabilities and effective handling of socket communications.

## Components

### 1. TCP Server-Client Implementation

The first component is a multi-threaded TCP server that can handle two clients simultaneously. It enables clients to send messages to each other through the server.

#### Features:
- **Multi-threading**: Each client is handled in a separate thread.
- **Client connection management**: The server accepts up to two clients at a time.
- **Message forwarding**: Messages received from one client are sent to the other client.

#### Key Functions:
- **handle_client(void* arg)**: A thread function that handles communication for a connected client.

### 2. TCP Client Implementation

The second part consists of a TCP client that connects to the server and allows users to send and receive messages in real-time.

#### Features:
- **Real-time message handling**: Clients can send messages while simultaneously receiving messages from the server.
- **User-friendly interface**: Messages are displayed in the terminal for clarity.

#### Key Functions:
- **receive_messages(void* arg)**: A thread function to receive messages from the server.
- **send_messages(void* arg)**: A thread function to send messages to the server.

### Installation

1. **Prerequisites:**
   - A C compiler (like gcc).
   - A Linux-based operating system (the code is tailored for Unix-like environments).

2. **Compiling the Code:**
   - Use the following commands to compile the TCP server-client code:
     ```bash
     gcc -o tcp_server tcp_server.c -lpthread
     gcc -o tcp_client tcp_client.c -lpthread
     ```
   - Replace `tcp_server.c` and `tcp_client.c` with the respective filenames of your code.

3. **Running the Server and Client:**
   - First, run the server in one terminal:
     ```bash
     ./tcp_server
     ```
   - Then, run the client in another terminal:
     ```bash
     ./tcp_client
     ```

### Usage

- Once both the server and client are running, you can start sending messages from the client.
- The server will relay messages between the connected clients.

### Notes
- Ensure that the server is running before starting the client.
- The maximum number of clients that can be connected simultaneously is defined in the server code. Adjust the implementation if more clients are needed.

### License
This project is licensed under the MIT License. Feel free to use, modify, and distribute it as you wish.

---

Feel free to modify any sections based on specific needs or preferences!
