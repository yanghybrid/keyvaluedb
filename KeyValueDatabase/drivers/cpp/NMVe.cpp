#include <fcntl.h>
#include <iostream>
#include <sys/ioctl.h>
#include <linux/nvme_ioctl.h>
#include <unistd.h>

int main() {
    // Open NVMe device (e.g., /dev/nvme0n1)
    int fd = open("/dev/nvme0", O_RDWR);
    if (fd < 0) {
        perror("Failed to open NVMe device");
        return -1;
    }

    struct nvme_admin_cmd cmd = {};
    cmd.opcode = 0x06;  // Identify command
    cmd.nsid = 1;
    cmd.addr = reinterpret_cast<uint64_t>(malloc(4096));
    cmd.data_len = 4096;

    // Send the NVMe admin command
    if (ioctl(fd, NVME_IOCTL_ADMIN_CMD, &cmd) < 0) {
        perror("NVMe ioctl failed");
    } else {
        std::cout << "NVMe Identify Command Successful!" << std::endl;
    }

    close(fd);
    return 0;
}
