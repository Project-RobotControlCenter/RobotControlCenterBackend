//
// Created by elejandc on 11/9/24.
//

#ifndef ROBOTMANAGER_H
#define ROBOTMANAGER_H

class RobotManager {
public:
    RobotManager(const RobotManager&) = delete;
    RobotManager& operator=(const RobotManager&) = delete;

    static RobotManager& getInstance() {
        static RobotManager _instance;
        return _instance;
    }
private:
    RobotManager();
    ~RobotManager();
};



#endif //ROBOTMANAGER_H
