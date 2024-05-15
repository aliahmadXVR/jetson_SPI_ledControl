/*
Compilation instruction
g++ -o spi_loop_spidev spi_loop_spidev.cpp

*/
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <cstring>

// SPI device settings
static const char *device = "/dev/spidev0.0";
static const uint8_t mode = 0;
static const uint8_t bits_per_word = 8;
static const uint32_t speed_hz = 1000000;

int spi_fd;

void transfer(uint8_t *tx_buffer, uint8_t *rx_buffer, size_t len) {
    struct spi_ioc_transfer tr;
    std::memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (unsigned long)tx_buffer;
    tr.rx_buf = (unsigned long)rx_buffer;
    tr.len = len;
    tr.speed_hz = speed_hz;
    tr.bits_per_word = bits_per_word;
    tr.delay_usecs = 0;

    int status = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    if (status < 0) {
        perror("SPI transfer failed");
    }
}

void loopback_test(uint8_t *data, size_t len) {
    uint8_t rx_buffer[len];
    transfer(data, rx_buffer, len);

    std::cout << "Sent: ";
    for (size_t i = 0; i < len; ++i) {
        std::cout << std::hex << static_cast<int>(data[i]) << " ";
    }
    std::cout << std::endl;

    std::cout << "Received: ";
    for (size_t i = 0; i < len; ++i) {
        std::cout << std::hex << static_cast<int>(rx_buffer[i]) << " ";
    }
    std::cout << std::endl;
}

int main() {
    spi_fd = open(device, O_RDWR);
    if (spi_fd < 0) {
        perror("Can't open SPI device");
        return -1;
    }

    // Set SPI mode
    int ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1) {
        perror("Can't set SPI mode");
        return -1;
    }

    // Set bits per word
    ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
    if (ret == -1) {
        perror("Can't set bits per word");
        return -1;
    }

    // Set max speed
    ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed_hz);
    if (ret == -1) {
        perror("Can't set max speed hz");
        return -1;
    }

    // Example data to send
    const size_t len = 4;
    uint8_t data_to_send[len] = {0xAA, 0xBB, 0xCC, 0xDD};

    while (1){
    // Perform loopback test
    loopback_test(data_to_send, len);
    sleep(1);
    }
  
    close(spi_fd);

    return 0;
}
