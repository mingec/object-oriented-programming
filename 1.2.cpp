#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>

const int numFloors = 40;      // ��¥����
const int numElevators = 10;   // ��������
const int maxPassengers = 18;  // ÿ����������ؿ���

// ������
class Elevator {
private:
    int currentFloor;             // ��ǰ¥��
    int direction;                // ���з���1��ʾ���У�-1��ʾ���У�0��ʾֹͣ
    int numPassengers;            // �˿�����
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

    // ���ӳ˿�Ҫȥ��¥��
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
    int trips;        // �ѳ����Ĵ���
    int maxTrips;     // ����������

public:
    Passenger(int maxFloor) {
        targetFloor = rand() % maxFloor + 2;  // �������¥�㣨2��maxFloor֮�䣩
        trips = 0;
        maxTrips = rand() % 10 + 1;  // ������ɳ���������1��10֮�䣩
    }

    // ����Ҫȥ��¥��
    int getTargetFloor() {
        return targetFloor;
    }

    // ����������1
    void incrementTrips() {
        trips++;
    }

    // ����Ƿ�ﵽ����������
    bool reachedMaxTrips() {
        return trips >= maxTrips;
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
	srand(time(0));  // ʹ�õ�ǰʱ����Ϊ��������ӣ�ȷ��ÿ������ʱ���ɵ���������в�ͬ

	std::vector<Elevator*> elevators;  // ����һ���洢����ָ�������

	// ����ָ�������ĵ��ݶ��󣬲�����ָ�����ӵ�������
	for (int i = 0; i < numElevators; i++) {
    	Elevator* elevator = new Elevator();  // �����µĵ��ݶ���
    	elevators.push_back(elevator);        // ������ָ�����ӵ�������
	}

	std::vector<Passenger*> passengers;  // ����һ���洢�˿�ָ�������

	// ����ָ�������ĳ˿Ͷ��󣬲�����ָ�����ӵ�������
	for (int i = 0; i < 100; i++) {
    	Passenger* passenger = new Passenger(numFloors);  // �����µĳ˿Ͷ���ָ�����¥����
    	passengers.push_back(passenger);                  // ���˿�ָ�����ӵ�������
	}

	int currentTime = 0;  // ��ʼ����ǰʱ��Ϊ0

    while (!passengers.empty()) {
        currentTime++;

        // ���ѡ��һ�����ݽ��и���
        int elevatorIndex = rand() % elevators.size();
        Elevator* elevator = elevators[elevatorIndex];

        // ���µ���״̬��λ��
        if (!elevator->getNumPassengers()) {
            // �������û�г˿ͣ������ѡ��һ���˿ͽ������
            int passengerIndex = rand() % passengers.size();
            int targetFloor = passengers[passengerIndex]->getTargetFloor();

            elevator->addPassenger(targetFloor);

            passengers.erase(passengers.begin() + passengerIndex);
        }
        else {
            // ��������г˿ͣ����ƶ����ݲ������˿�Ŀ��¥��
            int targetFloor = elevator->getPassengerFloor(0);
            elevator->update(targetFloor);

            if (elevator->getCurrentFloor() == targetFloor) {
                elevator->removePassenger(0);
            }
        }

        // ��ʾ������Ϣ
        displaySimulationInfo(elevators, currentTime);

        // ��ͣһ����
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
