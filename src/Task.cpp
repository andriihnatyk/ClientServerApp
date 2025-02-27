#include "Task.h"
#include <iostream>

Task::Task(NetworkHandler &net) : network(net) {}

void SumTask::execute() {
    int number1, number2;
    std::string input;

    network.sendMessage("Please enter number 1: ");
    input = network.receiveMessage();
    if (input.empty()) {
        network.sendMessage("Client disconnected. Ending task.");
        return;
    }

    try {
        number1 = std::stoi(input);
    } catch (const std::exception& e) {
        network.sendMessage("Invalid input. Please send a number.");
        return;
    }

    network.sendMessage("Please enter number 2: ");
    input = network.receiveMessage();
    if (input.empty()) {
        network.sendMessage("Client disconnected. Ending task.");
        return;
    }

    try {
        number2 = std::stoi(input);
    } catch (const std::exception& e) {
        network.sendMessage("Invalid input. Please send a number.");
        return;
    }

    network.sendMessage("The sum is: " + std::to_string(number1 + number2) );
}
