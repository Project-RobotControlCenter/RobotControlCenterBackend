# RobotControlCenterBackend

**Robot Control Center Backend** is the backend of the **Robot Control Center** project. As the name suggests, this program facilitates the management of connections between the frontend, robots, and a database (MongoDB). It allows seamless communication between multiple frontends, robots, and provides essential data storage functionalities.

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [Usage](#usage)
5. [Configuration](#configuration)
6. [Contributing](#contributing)
7. [License](#license)

## Introduction

The **Robot Control Center Backend** is designed to serve as the communication bridge between different components of the **Robot Control Center** project. It manages the connections between multiple **frontend clients**, **robots**, and the **database (MongoDB)**. The backend is responsible for handling robot registrations, frontend sessions, and facilitating data exchange between the robots and the frontend.

## Features
- **Frontend to Robot Connectivity**: Seamlessly connect the frontend to any connected robot.
- **Automatic Robot Management**: Automatically registers new robots in the database and manages robot connection details, such as robot name, MAC address, and IP.
- **Automatic Frontend Management**: Handles frontend connection management and session control, including robot access for each session.
- **Data Transfer**: Allows data transfer between the frontend and connected robots.
- **MongoDB Integration**: Stores essential robot and session data in a MongoDB database for persistent storage.

## Installation

### Prerequisites
Before running the backend, ensure the following dependencies are installed:

- **C++14** or higher
- **Boost.Beast 1.85** or higher (for WebSocket communication)
- **CMake 3.29** or higher (for building the project)

### Steps
1. **Clone the repository**:
    ```bash
    git clone https://github.com/yourusername/RobotControlCenterBackend.git
    cd RobotControlCenterBackend
    ```

2. **Install dependencies**:
    Follow the instructions for your platform to install Boost and CMake.

3. **Build the project**:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

4. **Start MongoDB**:
    Ensure that your MongoDB database is running locally or remotely. The backend requires MongoDB for session and robot data storage.

5. **Start the Backend**:
    Run the following command to start the backend:
    ```bash
    ./robot_backend <local_db_ip> <local_db_port> <network_db_ip> <network_db_port> <db_password> <frontend_port> <robot_port>
    ```

    Replace the placeholders with the appropriate values for your setup:
    - `<local_db_ip>`: IP address of your local MongoDB instance.
    - `<local_db_port>`: MongoDB local port (default: 27017).
    - `<network_db_ip>`: IP address of the network MongoDB instance.
    - `<network_db_port>`: MongoDB network port.
    - `<db_password>`: MongoDB database password.
    - `<frontend_port>`: Port for frontend connections (e.g., 8080).
    - `<robot_port>`: Port for robot connections (e.g., 9090).

Now, the backend is ready to manage robot and frontend connections.

## Usage

Once the backend is running, **robots** and **frontends** can connect to it via their respective ports.

- **Robots**: Upon connection, robots will automatically register with the backend, and their data (such as name, IP, and MAC address) will be stored in MongoDB.
- **Frontends**: Frontend clients can connect and establish sessions, allowing them to interact with the connected robots.

## Configuration

- **Firewall and Ports**: Ensure the necessary ports are open to allow communication between the backend, robots, and frontends. You may need to adjust your firewall settings to allow traffic on:
  - Frontend connection port
  - Robot connection port
- **MongoDB Configuration**: Ensure MongoDB is configured correctly, with the necessary access rights and authentication enabled.

## Contributing

This project is a simple college project, and further development beyond the required features is uncertain. However, if you'd like to contribute or suggest improvements, please feel free to fork the repository and create a pull request.

Here are some ways you can contribute:
- **Bug Fixes**: Help in identifying and fixing bugs.
- **Features**: Suggest and develop new features.
- **Documentation**: Improve the documentation for better clarity.

### How to Contribute
1. Fork the repository.
2. Create a new branch for your feature (`git checkout -b feature/your-feature`).
3. Commit your changes (`git commit -am 'Add new feature'`).
4. Push to the branch (`git push origin feature/your-feature`).
5. Create a new Pull Request.

## License

This project is licensed under the **3-Clause BSD License**.

