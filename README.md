# RobotControlCenterBackend

**Robot Control Center Backend** is the backend for the **Robot Control Center** project. This program facilitates managing connections between the frontend, robots, and a MongoDB database, enabling seamless communication and persistent storage functionalities.

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [Usage](#usage)
5. [Configuration](#configuration)
6. [JSON Structures](#json-structures)
7. [Communication Flows](#communication-flows)
8. [License](#license)

---

## Introduction

The **Robot Control Center Backend** serves as a communication bridge for the **Robot Control Center** project. It manages connections between **frontend clients**, **robots**, and the **database (MongoDB)**. The backend handles robot registrations, frontend sessions, and facilitates data exchange between these entities.

---

## Features
- **Frontend to Robot Connectivity**: Enables direct interaction between the frontend and connected robots.
- **Automatic Robot Management**: Registers new robots in the database and manages connection details (e.g., name, MAC address, IP).
- **Session Management**: Handles frontend connections and assigns robots to sessions.
- **Data Exchange**: Facilitates communication and data transfer between the frontend and robots.
- **Persistent Storage**: Stores robot and session data in a MongoDB database.

---

## Installation

### Prerequisites
Ensure the following dependencies are installed:
- **C++14** or higher
- **Boost.Beast 1.85** or higher (for WebSocket communication)
- **CMake 3.29** or higher (for building the project)
- **MongoDB** (for data storage)

### Steps
1. **Clone the repository**:
    ```bash
    git clone https://github.com/yourusername/RobotControlCenterBackend.git
    cd RobotControlCenterBackend
    ```

2. **Install dependencies**:
    Install Boost and CMake according to your platform's instructions.

3. **Build the project**:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

4. **Start MongoDB**:
    Ensure your MongoDB instance is running locally or remotely.

5. **Run the backend**:
    ```bash
    ./robot_backend <local_db_ip> <local_db_port> <network_db_ip> <network_db_port> <db_password> <frontend_port> <robot_port>
    ```
    Replace placeholders with appropriate values.

---

## Usage

Once the backend is running:
- **Robots**: Automatically register upon connection. Robot data is stored in MongoDB.
- **Frontends**: Connect to the backend and manage sessions to interact with robots.

---

## Configuration

- **Ports**:
  - Ensure ports for frontend and robot connections are open.
  - Adjust firewall settings if necessary.
- **MongoDB**:
  - Configure MongoDB access with proper authentication.

---

## JSON Structures

The backend uses structured JSON messages for communication between components. Below are the key message formats:

### `getAllRobots`
- **Description**: Fetch data for all connected robots.
- **Request**:
    ```json
    {
        "message_type": "getAllRobots",
        "data": {}
    }
    ```
- **Response**:
    ```json
    {
        "message_type": "AllRobotsInfo",
        "data": {
            "robots_info": [
                {
                    "mac_address": "string",
                    "robot_name": "string",
                    "robot_ip": "string",
                    "robot_port": "number"
                }
            ]
        }
    }
    ```

### `connectToRobot`
- **Description**: Connect a session to a specific robot by MAC address.
- **Request**:
    ```json
    {
        "message_type": "connectToRobot",
        "data": {
            "mac_address": "string"
        }
    }
    ```

### `disconnectFromRobot`
- **Description**: Disconnect the session from its current robot.
- **Request**:
    ```json
    {
        "message_type": "disconnectFromRobot",
        "data": {}
    }
    ```
- **Response when no robot is connected**:
    ```json
    {
        "message_type": "NoRobotConnected"
    }
    ```

### `robotControl`
- **Description**: Forward control commands from the frontend to a robot.
- **Request**:
    ```json
    {
        "message_type": "robotControl",
        "data": {
            // Command-specific details
        }
    }
    ```

---

## Communication Flows

### Frontend ↔ Session
- **Purpose**: Manage robot connections and retrieve robot data.
- **Messages**: `getAllRobots`, `connectToRobot`, `disconnectFromRobot`, `robotControl`.

### Session ↔ Robot
- **Purpose**: Forward control commands to robots and relay their responses.
- **Messages**: Text and binary messages are parsed and forwarded as needed.

### Robot ↔ Database
- **Purpose**: Store and retrieve robot data (e.g., MAC address, IP).
- **Messages**: BSON queries used for MongoDB interactions.

---

---

## License

This project is licensed under the **3-Clause BSD License**.
