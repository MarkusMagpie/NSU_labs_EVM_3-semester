#include <iostream>
#include <libusb.h>

void printDeviceInfo(libusb_device *dev) {
    libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc); // получение дескриптора USB устройства
    if (r < 0) {
        std::cerr << "Ошибка: не удалось получить дескриптор устройства, код: " << r << std::endl;
        return;
    }

    // Выводим класс устройства, идентификатор производителя и идентификатор изделия
    std::cout << "Класс устройства: " << (int)desc.bDeviceClass << std::endl;
    std::cout << "Идентификатор производителя: " << (int)desc.idVendor << std::endl;
    std::cout << "Идентификатор устройства: " << std::hex << desc.idProduct << std::endl;

    libusb_device_handle *handle; // хендлер где будем хранить конфигурации
    r = libusb_open(dev, &handle); // открыть устройство (начать работать с устройством) и получить хендлер устройства

    // Получаем и выводим серийный номер устройства
    if (r == 0) {
        unsigned char serialNumber[256];
        unsigned char Manufacturer[256];
        unsigned char Product[256];
        // получить дескриптор  устройства в виде строки символов
        if (libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, serialNumber, sizeof(serialNumber)) > 0) { 
            std::cout << "Серийный номер: " << serialNumber << std::endl;
        }

        if (libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, Manufacturer, sizeof(Manufacturer)) > 0) {
            std::cout << "Производитель: " << Manufacturer << std::endl;
        }

        if (libusb_get_string_descriptor_ascii(handle, desc.iProduct, Product, sizeof(Product)) > 0) {
            std::cout << "Изделие: " << Product << std::endl;
        }

        libusb_close(handle);
    } else {
        std::cerr << "Ошибка: не удалось открыть устройство." << std::endl;
    }
    
    std::cout << "--------------------------" << std::endl;
}

int main() {
    libusb_device **devs; // массив указателей на устройства, используется для получения списка устройств
    libusb_context *ctx = NULL; // контекст сессии libusb
    int r; // для возвращаемых значений
    ssize_t cnt; // число найденных USB-устройств

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

// use sudo ./executable to run