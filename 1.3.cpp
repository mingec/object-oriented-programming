#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>

const int numFloors = 40;  // ��¥����
const int numElevators = 10;  // ��������
const int maxPassengers = 18;  // ÿ����������ؿ���

// ������
class Elevator {
private:
    int currentFloor;  // ��ǰ¥��
    int direction;  // ���з���1��ʾ���У�-1��ʾ���У�0��ʾֹͣ
    int numPassengers;  // �˿�����
    std::vector<int> passengers;  // �洢�˿�Ҫȥ��¥��

public:
    Elevator() {
        currentFloor = 1;
        direction = 0;
        numPassengers = 0;
    }

    // ��ȡ��ǰ¥��
    int getCurrentFloor() {
        return currentFloor;
    }

    // ��ȡ���з���
    int getDirection() {
        return direction;
    }

    // ��ȡ�˿�����
    int getNumPassengers() {
        return numPassengers;
    }

    // ��ȡ��index���˿�Ҫȥ��¥��
    int getPassengerFloor(int index) {
        return passengers[index];
    }

    // ��ӳ˿�Ҫȥ��¥��
    void addPassenger(int floor) {
        if (numPassengers < maxPassengers) {
            passengers.push_back(floor);
            numPassengers++;
        }
        else {
            std::cout << "��������Ա��" << std::endl;
        }
    }

    // �Ƴ��˿�
    void removePassenger(int index) {
        if (index >= 0 && index < numPassengers) {
            passengers.erase(passengers.begin() + index);
            numPassengers--;
        }
    }

    // ���µ���״̬��λ��
    void update(int targetFloor) {
        if (currentFloor < targetFloor) {
            direction = 1;  // ����
        }
        else if (currentFloor > targetFloor) {
            direction = -1;  // ����
        }
        else {
            direction = 0;  // ֹͣ
        }

        currentFloor += direction;
    }
};

// �˿���
class Passenger {
private:
    int targetFloor;  // Ҫȥ��¥��

public:
    Passenger(int maxFloor) {
        targetFloor = rand() % maxFloor + 2;  // �������¥�㣨2��maxFloor֮�䣩
    }

    // ����Ҫȥ��¥��
    int getTargetFloor() {
        return targetFloor;
    }
};

// ��ʾ������Ϣ
void displaySimulationInfo(const std::vector<Elevator*>& elevators, int currentTime) {
    std::cout << "��ǰʱ�䣺" << currentTime << std::endl;

    for (int i = 0; i < elevators.size(); i++) {
        Elevator* elevator = elevators[i];

        std::cout << "����" << (i + 1) << ": ";
        std::cout << "��ǰ¥�㣺" << elevator->getCurrentFloor() << " ";
        std::cout << "���з���" << elevator->getDirection() << " ";
        std::cout << "�˿�������" << elevator->getNumPassengers() << " ";
        std::cout << "�˿�Ŀ��¥�㣺";

        for (int j = 0; j < elevator->getNumPassengers(); j++) {
            std::cout << elevator->getPassengerFloor(j) << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

// ѡ������ʵĵ����ó˿͵ȴ�
void selectElevator(std::vector<Elevator*>& elevators, Passenger* passenger) {
    Elevator* selectedElevator = nullptr;

    // �������ݣ�ѡ������ʵĵ���
    for (Elevator* elevator : elevators) {
        int currentFloor = elevator->getCurrentFloor();
        int direction = elevator->getDirection();

        // E0: �ɵ���ÿ��
        if (currentFloor == passenger->getTargetFloor()) {
            selectedElevator = elevator;
            break;
        }

        // E1: �ɵ���ÿ��
        if (elevator->getNumPassengers() == 0 && direction == 0) {
            selectedElevator = elevator;
            break;
        }

        // E2: �ɵ���1��25~40��
        if (currentFloor == 1 || (currentFloor >= 25 && currentFloor <= 40)) {
            selectedElevator = elevator;
        }

        // E3: �ɵ���1��25~40��
        if (direction == 0 && currentFloor == 1 && (passenger->getTargetFloor() >= 25 && passenger->getTargetFloor() <= 40)) {
            selectedElevator = elevator;
        }

        // E4: �ɵ���1~25��
        if (currentFloor >= 1 && currentFloor <= 25) {
            selectedElevator = elevator;
        }

        // E5: �ɵ���1~25��
        if (direction == 0 && currentFloor >= 1 && currentFloor <= 25 && passenger->getTargetFloor() <= 25) {
            selectedElevator = elevator;
        }

        // E6: �ɵ���1��2~40���е�ż����
        if (currentFloor == 1 || (currentFloor >= 2 && currentFloor <= 40 && currentFloor % 2 == 0)) {
            selectedElevator = elevator;
        }

        // E7: �ɵ���1��2~40���е�ż����
        if (direction == 0 && currentFloor == 1 && (passenger->getTargetFloor() >= 2 && passenger->getTargetFloor() <= 40 && passenger->getTargetFloor() % 2 == 0)) {
            selectedElevator = elevator;
        }

        // E8: �ɵ���1~39���е�������
        if (currentFloor >= 1 && currentFloor <= 39 && currentFloor % 2 == 1) {
            selectedElevator = elevator;
        }

        // E9: �ɵ���1~39���е�������
        if (direction == 0 && currentFloor >= 1 && currentFloor <= 39 && currentFloor % 2 == 1 && passenger->getTargetFloor() % 2 == 1) {
            selectedElevator = elevator;
        }
    }

    // ����ҵ����ʵĵ��ݣ����ó˿͵ȴ�
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

        // ���ѡ��һ�����ݽ��и���
        int elevatorIndex = rand() % elevators.size();
        Elevator* elevator = elevators[elevatorIndex];

        // ���µ���״̬��λ��
        if (!elevator->getNumPassengers()) {
            // �������û�г˿ͣ������ѡ��һ���˿ͽ������
            int passengerIndex = rand() % passengers.size();
            Passenger* passenger = passengers[passengerIndex];

            selectElevator(elevators, passenger);

            passengers.erase(passengers.begin() + passengerIndex);
        }
        else {
            // ��������г˿ͣ����ƶ����ݲ�����˿�Ŀ��¥��
            int targetFloor = elevator->getPassengerFloor(0);
            elevator->update(targetFloor);

            if (elevator->getCurrentFloor() == targetFloor) {
                elevator->removePassenger(0);
            }
        }

        // ��ʾ������Ϣ
        displaySimulationInfo(elevators, currentTime);

        // ��ͣһ����
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // �ͷ��ڴ�
    for (int i = 0; i < elevators.size(); i++) {
        delete elevators[i];
    }

    for (int i = 0; i < passengers.size(); i++) {
        delete passengers[i];
    }

    return 0;
}

