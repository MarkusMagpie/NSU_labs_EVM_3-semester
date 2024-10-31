#include <iostream>
#include <libusb.h>

void printDeviceInfo(libusb_device *dev) {
    libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
        std::cerr << "Ошибка: не удалось получить дескриптор устройства, код: " << r << std::endl;
        return;
    }

    // Выводим класс устройства, идентификатор производителя и идентификатор изделия
    std::cout << "Класс устройства: " << (int)desc.bDeviceClass << std::endl;
    std::cout << "ID производителя: " << std::hex << desc.idVendor << std::endl;
    std::cout << "ID изделия: " << std::hex << desc.idProduct << std::endl;

    // Получаем и выводим серийный номер устройства
    if (desc.iSerialNumber) {
        libusb_device_handle *handle;
        r = libusb_open(dev, &handle);
        if (r == 0) {
            unsigned char serialNumber[256];
            int length = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, serialNumber, sizeof(serialNumber));
            if (length > 0) {
                std::cout << "Серийный номер: " << serialNumber << std::endl;
            } else {
                std::cerr << "Ошибка: не удалось получить серийный номер." << std::endl;
            }
            libusb_close(handle);
        } else {
            std::cerr << "Ошибка: не удалось открыть устройство для получения серийного номера." << std::endl;
        }
    } else {
        std::cout << "Серийный номер отсутствует." << std::endl;
    }
    
    std::cout << "--------------------------" << std::endl;
}

int main() {
    libusb_device **devs;
    libusb_context *ctx = NULL;
    int r;
    ssize_t cnt;

    // Инициализируем libusb
    r = libusb_init(&ctx);
    if (r < 0) {
        std::cerr << "Ошибка: не удалось инициализировать libusb, код: " << r << std::endl;
        return 1;
    }

    // Получаем список подключенных USB-устройств
    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        std::cerr << "Ошибка: не удалось получить список устройств." << std::endl;
        libusb_exit(ctx);
        return 1;
    }

    std::cout << "Найдено USB-устройств: " << cnt << std::endl;
    std::cout << "==========================" << std::endl;

    for (ssize_t i = 0; i < cnt; i++) {
        printDeviceInfo(devs[i]);
    }

    // Освобождаем ресурсы
    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);

    return 0;
}
