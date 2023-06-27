#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>

const int numFloors = 40;  // 总楼层数
const int numElevators = 10;  // 电梯数量
const int maxPassengers = 18;  // 每部电梯最大载客量

// 电梯类
class Elevator {
private:
    int currentFloor;  // 当前楼层
    int direction;  // 运行方向，1表示上行，-1表示下行，0表示停止
    int numPassengers;  // 乘客数量
    std::vector<int> passengers;  // 存储乘客要去的楼层
    int idleTime;  // 空闲时间
    int runningTime;  // 运行时间

public:
    Elevator() {
        currentFloor = 1;
        direction = 0;
        numPassengers = 0;
        idleTime = 0;
        runningTime = 0;
    }

    // 获取当前楼层
    int getCurrentFloor() {
        return currentFloor;
    }

    // 获取运行方向
    int getDirection() {
        return direction;
    }

    // 获取乘客数量
    int getNumPassengers() {
        return numPassengers;
    }

    // 获取第index个乘客要去的楼层
    int getPassengerFloor(int index) {
        return passengers[index];
    }

    // 获取空闲时间
    int getIdleTime() {
        return idleTime;
    }

    // 获取运行时间
    int getRunningTime() {
        return runningTime;
    }

    // 添加乘客要去的楼层
    void addPassenger(int floor) {
        if (numPassengers < maxPassengers) {
            passengers.push_back(floor);
            numPassengers++;
        }
        else {
            std::cout << "电梯已满员！" << std::endl;
        }
    }

    // 移除乘客
    void removePassenger(int index) {
        if (index >= 0 && index < numPassengers) {
            passengers.erase(passengers.begin() + index);
            numPassengers--;
        }
    }

    // 更新电梯状态和位置
    void update(int targetFloor) {
        if (currentFloor < targetFloor) {
            direction = 1;  // 上行
        }
        else if (currentFloor > targetFloor) {
            direction = -1;  // 下行
        }
        else {
            direction = 0;  // 停止
        }

        currentFloor += direction;
    }

    // 增加空闲时间
    void increaseIdleTime() {
        idleTime++;
    }

    // 增加运行时间
    void increaseRunningTime() {
        runningTime++;
    }
};

// 乘客类
class Passenger {
private:
    int targetFloor;  // 要去的楼层
    int waitingTime;  // 等待时间

public:
    Passenger(int maxFloor) {
        targetFloor = rand() % maxFloor + 2;  // 生成随机楼层（2到maxFloor之间）
        waitingTime = 0;
    }

    // 返回要去的楼层
    int getTargetFloor() {
        return targetFloor;
    }

    // 增加等待时间
    void increaseWaitingTime() {
        waitingTime++;
    }

    // 获取等待时间
    int getWaitingTime() {
        return waitingTime;
    }
};

// 显示仿真信息
void displaySimulationInfo(const std::vector<Elevator*>& elevators, int currentTime) {
    std::cout << "当前时间：" << currentTime << std::endl;

    for (int i = 0; i < elevators.size(); i++) {
        Elevator* elevator = elevators[i];

        std::cout << "电梯" << (i + 1) << ": ";
        std::cout << "当前楼层：" << elevator->getCurrentFloor() << " ";
        std::cout << "运行方向：" << elevator->getDirection() << " ";
        std::cout << "乘客数量：" << elevator->getNumPassengers() << " ";
        std::cout << "乘客目标楼层：";

        for (int j = 0; j < elevator->getNumPassengers(); j++) {
            std::cout << elevator->getPassengerFloor(j) << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

int main() {
    srand(time(0));

    std::vector<Elevator*> elevators;
    for (int i = 0; i < numElevators; i++) {
        Elevator* elevator = new Elevator();
        elevators.push_back(elevator);
    }

    std::vector<Passenger*> passengers;

    for (int i = 0; i < 100; i++) {
        Passenger* passenger = new Passenger(numFloors);
        passengers.push_back(passenger);
    }

    int currentTime = 0;

    while (!passengers.empty()) {
        currentTime++;

        // 更新电梯状态和位置
        for (int i = 0; i < elevators.size(); i++) {
            Elevator* elevator = elevators[i];

            if (elevator->getNumPassengers() == 0) {
                // 电梯为空闲状态
                elevator->increaseIdleTime();
            }
            else {
                // 电梯有乘客
                int targetFloor = elevator->getPassengerFloor(0);
                elevator->update(targetFloor);
                elevator->increaseRunningTime();

                if (elevator->getCurrentFloor() == targetFloor) {
                    elevator->removePassenger(0);
                }
            }
        }

        // 随机选择一个电梯进行更新
        int elevatorIndex = rand() % elevators.size();
        Elevator* elevator = elevators[elevatorIndex];

        // 如果电梯没有乘客，则随机选择一个乘客进入电梯
        if (elevator->getNumPassengers() == 0 && !passengers.empty()) {
            int passengerIndex = rand() % passengers.size();
            int targetFloor = passengers[passengerIndex]->getTargetFloor();

            elevator->addPassenger(targetFloor);

            passengers.erase(passengers.begin() + passengerIndex);
        }

        // 更新乘客的等待时间
        for (int i = 0; i < passengers.size(); i++) {
            passengers[i]->increaseWaitingTime();
        }

        // 显示仿真信息
        displaySimulationInfo(elevators, currentTime);

        // 暂停一秒钟
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // 统计分析
    std::cout << "电梯统计信息：" << std::endl;
    for (int i = 0; i < elevators.size(); i++) {
        Elevator* elevator = elevators[i];
        std::cout << "电梯" << (i + 1) << ": " << std::endl;
        std::cout << "空闲时间：" << elevator->getIdleTime() << "个时间单位" << std::endl;
        std::cout << "运行时间：" << elevator->getRunningTime() << "个时间单位" << std::endl;
        std::cout << std::endl;
    }

    std::cout << "乘客等待时间：" << std::endl;
    for (int i = 0; i < passengers.size(); i++) {
        Passenger* passenger = passengers[i];
        std::cout << "乘客" << (i + 1) << ": ";
        std::cout << "等待时间：" << passenger->getWaitingTime() << "个时间单位" << std::endl;
    }

    // 释放内存
    for (int i = 0; i < elevators.size(); i++) {
        delete elevators[i];
    }

    for (int i = 0; i < passengers.size(); i++) {
        delete passengers[i];
    }

    return 0;
}

