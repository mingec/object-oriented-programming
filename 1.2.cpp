#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>

const int numFloors = 40;      // 总楼层数
const int numElevators = 10;   // 电梯数量
const int maxPassengers = 18;  // 每部电梯最大载客量

// 电梯类
class Elevator {
private:
    int currentFloor;             // 当前楼层
    int direction;                // 运行方向，1表示上行，-1表示下行，0表示停止
    int numPassengers;            // 乘客数量
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
    int trips;        // 已乘坐的次数
    int maxTrips;     // 最大乘坐次数

public:
    Passenger(int maxFloor) {
        targetFloor = rand() % maxFloor + 2;  // 生成随机楼层（2到maxFloor之间）
        trips = 0;
        maxTrips = rand() % 10 + 1;  // 随机生成乘坐次数（1到10之间）
    }

    // 返回要去的楼层
    int getTargetFloor() {
        return targetFloor;
    }

    // 乘坐次数加1
    void incrementTrips() {
        trips++;
    }

    // 检查是否达到最大乘坐次数
    bool reachedMaxTrips() {
        return trips >= maxTrips;
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
	srand(time(0));  // 使用当前时间作为随机数种子，确保每次运行时生成的随机数序列不同

	std::vector<Elevator*> elevators;  // 创建一个存储电梯指针的向量

	// 创建指定数量的电梯对象，并将其指针添加到向量中
	for (int i = 0; i < numElevators; i++) {
    	Elevator* elevator = new Elevator();  // 创建新的电梯对象
    	elevators.push_back(elevator);        // 将电梯指针添加到向量中
	}

	std::vector<Passenger*> passengers;  // 创建一个存储乘客指针的向量

	// 创建指定数量的乘客对象，并将其指针添加到向量中
	for (int i = 0; i < 100; i++) {
    	Passenger* passenger = new Passenger(numFloors);  // 创建新的乘客对象，指定最大楼层数
    	passengers.push_back(passenger);                  // 将乘客指针添加到向量中
	}

	int currentTime = 0;  // 初始化当前时间为0

    while (!passengers.empty()) {
        currentTime++;

        // 随机选择一个电梯进行更新
        int elevatorIndex = rand() % elevators.size();
        Elevator* elevator = elevators[elevatorIndex];

        // 更新电梯状态和位置
        if (!elevator->getNumPassengers()) {
            // 如果电梯没有乘客，则随机选择一个乘客进入电梯
            int passengerIndex = rand() % passengers.size();
            int targetFloor = passengers[passengerIndex]->getTargetFloor();

            elevator->addPassenger(targetFloor);

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
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

