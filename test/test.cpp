#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

// 函数声明
int setupSerialPort(const std::string& port, int baudrate);
std::vector<unsigned char> readFromSerial(int fd);
void printHex(const std::vector<unsigned char>& data);

int main() {
    std::string serialPort = "/dev/ttyUSB0";  // 设置串口设备名
    int baudrate = B115200;  // 设置波特率为 115200

    // 设置串口
    int fd = setupSerialPort(serialPort, baudrate);
    if (fd == -1) {
        std::cerr << "Failed to open serial port.\n";
        return 1;
    }

    while (true) {
        // 从串口读取数据
        std::vector<unsigned char> data = readFromSerial(fd);
        if (data.empty()) {
            continue;
        }

        // 输出数据（以十六进制格式）
        printHex(data);
    }

    // 关闭串口
    close(fd);

    return 0;
}

// 设置串口
int setupSerialPort(const std::string& port, int baudrate) {
    int fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        std::cerr << "Failed to open serial port: " << std::strerror(errno) << "\n";
        return -1;
    }

    // 配置串口选项
    struct termios options;
    tcgetattr(fd, &options);
    cfsetospeed(&options, baudrate);
    cfsetispeed(&options, baudrate);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;
    tcsetattr(fd, TCSANOW, &options);

    return fd;
}

// 从串口读取数据
std::vector<unsigned char> readFromSerial(int fd) {
    std::vector<unsigned char> data;

    unsigned char buffer[1024];
    int bytesRead = read(fd, buffer, sizeof(buffer));

    if (bytesRead > 0) {
        for (int i = 0; i < bytesRead; i++) {
            data.push_back(buffer[i]);
        }
    }

    return data;
}

// 输出数据（以十六进制格式）
void printHex(const std::vector<unsigned char>& data) {
    for (const unsigned char& byte : data) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;
}