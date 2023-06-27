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

public:
    Elevator() {
        currentFloor = 1;
        direction = 0;
        numPassengers = 0;
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
};

// 乘客类
class Passenger {
private:
    int targetFloor;  // 要去的楼层

public:
    Passenger(int maxFloor) {
        targetFloor = rand() % maxFloor + 2;  // 生成随机楼层（2到maxFloor之间）
    }

    // 返回要去的楼层
    int getTargetFloor() {
        return targetFloor;
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

// 选择最合适的电梯让乘客等待
void selectElevator(std::vector<Elevator*>& elevators, Passenger* passenger) {
    Elevator* selectedElevator = nullptr;

    // 遍历电梯，选择最合适的电梯
    for (Elevator* elevator : elevators) {
        int currentFloor = elevator->getCurrentFloor();
        int direction = elevator->getDirection();

        // E0: 可到达每层
        if (currentFloor == passenger->getTargetFloor()) {
            selectedElevator = elevator;
            break;
        }

        // E1: 可到达每层
        if (elevator->getNumPassengers() == 0 && direction == 0) {
            selectedElevator = elevator;
            break;
        }

        // E2: 可到达1、25~40层
        if (currentFloor == 1 || (currentFloor >= 25 && currentFloor <= 40)) {
            selectedElevator = elevator;
        }

        // E3: 可到达1、25~40层
        if (direction == 0 && currentFloor == 1 && (passenger->getTargetFloor() >= 25 && passenger->getTargetFloor() <= 40)) {
            selectedElevator = elevator;
        }

        // E4: 可到达1~25层
        if (currentFloor >= 1 && currentFloor <= 25) {
            selectedElevator = elevator;
        }

        // E5: 可到达1~25层
        if (direction == 0 && currentFloor >= 1 && currentFloor <= 25 && passenger->getTargetFloor() <= 25) {
            selectedElevator = elevator;
        }

        // E6: 可到达1、2~40层中的偶数层
        if (currentFloor == 1 || (currentFloor >= 2 && currentFloor <= 40 && currentFloor % 2 == 0)) {
            selectedElevator = elevator;
        }

        // E7: 可到达1、2~40层中的偶数层
        if (direction == 0 && currentFloor == 1 && (passenger->getTargetFloor() >= 2 && passenger->getTargetFloor() <= 40 && passenger->getTargetFloor() % 2 == 0)) {
            selectedElevator = elevator;
        }

        // E8: 可到达1~39层中的奇数层
        if (currentFloor >= 1 && currentFloor <= 39 && currentFloor % 2 == 1) {
            selectedElevator = elevator;
        }

        // E9: 可到达1~39层中的奇数层
        if (direction == 0 && currentFloor >= 1 && currentFloor <= 39 && currentFloor % 2 == 1 && passenger->getTargetFloor() % 2 == 1) {
            selectedElevator = elevator;
        }
    }

    // 如果找到合适的电梯，则让乘客等待
    if (selectedElevator != nullptr) {
        selectedElevator->addPassenger(passenger->getTargetFloor());
    }
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

        // 随机选择一个电梯进行更新
        int elevatorIndex = rand() % elevators.size();
        Elevator* elevator = elevators[elevatorIndex];

        // 更新电梯状态和位置
        if (!elevator->getNumPassengers()) {
            // 如果电梯没有乘客，则随机选择一个乘客进入电梯
            int passengerIndex = rand() % passengers.size();
            Passenger* passenger = passengers[passengerIndex];

            selectElevator(elevators, passenger);

            passengers.erase(passengers.begin() + passengerIndex);
        }
        else {
            // 如果电梯有乘客，则移动电梯并处理乘客目标楼层
            int targetFloor = elevator->getPassengerFloor(0);
            elevator->update(targetFloor);

            if (elevator->getCurrentFloor() == targetFloor) {
                elevator->removePassenger(0);
            }
        }

        // 显示仿真信息
        displaySimulationInfo(elevators, currentTime);

        // 暂停一秒钟
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
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

