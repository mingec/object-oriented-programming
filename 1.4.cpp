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
    int idleTime;  // ����ʱ��
    int runningTime;  // ����ʱ��

public:
    Elevator() {
        currentFloor = 1;
        direction = 0;
        numPassengers = 0;
        idleTime = 0;
        runningTime = 0;
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

    // ��ȡ����ʱ��
    int getIdleTime() {
        return idleTime;
    }

    // ��ȡ����ʱ��
    int getRunningTime() {
        return runningTime;
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

    // ���ӿ���ʱ��
    void increaseIdleTime() {
        idleTime++;
    }

    // ��������ʱ��
    void increaseRunningTime() {
        runningTime++;
    }
};

// �˿���
class Passenger {
private:
    int targetFloor;  // Ҫȥ��¥��
    int waitingTime;  // �ȴ�ʱ��

public:
    Passenger(int maxFloor) {
        targetFloor = rand() % maxFloor + 2;  // �������¥�㣨2��maxFloor֮�䣩
        waitingTime = 0;
    }

    // ����Ҫȥ��¥��
    int getTargetFloor() {
        return targetFloor;
    }

    // ���ӵȴ�ʱ��
    void increaseWaitingTime() {
        waitingTime++;
    }

    // ��ȡ�ȴ�ʱ��
    int getWaitingTime() {
        return waitingTime;
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

        // ���µ���״̬��λ��
        for (int i = 0; i < elevators.size(); i++) {
            Elevator* elevator = elevators[i];

            if (elevator->getNumPassengers() == 0) {
                // ����Ϊ����״̬
                elevator->increaseIdleTime();
            }
            else {
                // �����г˿�
                int targetFloor = elevator->getPassengerFloor(0);
                elevator->update(targetFloor);
                elevator->increaseRunningTime();

                if (elevator->getCurrentFloor() == targetFloor) {
                    elevator->removePassenger(0);
                }
            }
        }

        // ���ѡ��һ�����ݽ��и���
        int elevatorIndex = rand() % elevators.size();
        Elevator* elevator = elevators[elevatorIndex];

        // �������û�г˿ͣ������ѡ��һ���˿ͽ������
        if (elevator->getNumPassengers() == 0 && !passengers.empty()) {
            int passengerIndex = rand() % passengers.size();
            int targetFloor = passengers[passengerIndex]->getTargetFloor();

            elevator->addPassenger(targetFloor);

            passengers.erase(passengers.begin() + passengerIndex);
        }

        // ���³˿͵ĵȴ�ʱ��
        for (int i = 0; i < passengers.size(); i++) {
            passengers[i]->increaseWaitingTime();
        }

        // ��ʾ������Ϣ
        displaySimulationInfo(elevators, currentTime);

        // ��ͣһ����
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // ͳ�Ʒ���
    std::cout << "����ͳ����Ϣ��" << std::endl;
    for (int i = 0; i < elevators.size(); i++) {
        Elevator* elevator = elevators[i];
        std::cout << "����" << (i + 1) << ": " << std::endl;
        std::cout << "����ʱ�䣺" << elevator->getIdleTime() << "��ʱ�䵥λ" << std::endl;
        std::cout << "����ʱ�䣺" << elevator->getRunningTime() << "��ʱ�䵥λ" << std::endl;
        std::cout << std::endl;
    }

    std::cout << "�˿͵ȴ�ʱ�䣺" << std::endl;
    for (int i = 0; i < passengers.size(); i++) {
        Passenger* passenger = passengers[i];
        std::cout << "�˿�" << (i + 1) << ": ";
        std::cout << "�ȴ�ʱ�䣺" << passenger->getWaitingTime() << "��ʱ�䵥λ" << std::endl;
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

