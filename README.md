# Client-Server Application

## 📌 Description
This project is a **client-server application** that uses **TCP/IP** and **WinSock2** for communication.  
The server accepts connections from client and executes tasks, such as:
- ✅ Calculating the sum of two numbers
- 🔄 Finding the maximum number in an array (planned feature)

---

## 🚀 **How to Run the Project?**
### 1️⃣ **Compile the Server**
```bash
g++ src/main.cpp src/NetworkHandler.cpp src/Task.cpp -o bin/server.exe -lws2_32
```
### 2️⃣ **Compile the Client**
```bash
g++ src/client.cpp -o bin/client.exe -lws2_32
```
### 3️⃣ **Run the Server**
```bash
./bin/server.exe
```
### 4️⃣ **Run the Client**
```bash
./bin/client.exe
```
### 🛠 Technologies Used
- C++ (Standard 11)
- WinSock2 (Windows API for Networking)
- TCP/IP Communication
- GitHub for Version Control
