 #include <opencv2/opencv.hpp>

//#include <opencv2/core.hpp> // basic building blocks
//#include <opencv2/imgcodecs.hpp> // reading/writing functions (imshow)
//#include <opencv2/highgui.hpp> // GUI functions (namedWindow; destroyAllWindows)
//#include <opencv2/imgproc.hpp> // image processing (blur)

#include <iostream> // for console input/output
#include <chrono> // for time measurement

int main() {
    // std::cout << "OpenCV version: " << CV_VERSION << std::endl;

    // открытие видеопотока с камеры (0 – первая подключенная камера)
    cv::VideoCapture capture(0);
    if (!capture.isOpened()) {
        std::cerr << "Error! Couldn't open camera" << std::endl;
        return 0;
    }

    // cv::namedWindow("Оригинальное изображение", cv::WINDOW_AUTOSIZE);
    // cv::namedWindow("Отредактированное изображение", cv::WINDOW_AUTOSIZE);

    cv::Mat frame, redacted_frame; // переменные для хранения каров
    int frame_count = 0; // счетчик кадров
    auto start = std::chrono::high_resolution_clock::now(); // начальное время

    while (true) {
        // считывание очередного кадра
        capture >> frame;
        if (frame.empty()) {
            std::cerr << "Error! Cannot capture the frame" << std::endl;
            break;
        }

        // редактирование изображения - сглаживание
        cv::blur(frame, redacted_frame, cv::Size(5, 5));

        frame_count++;

        auto current = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current - start;
        double fps = frame_count / elapsed.count();

        std::string fps_text = "FPS: " + std::to_string(fps);
        cv::putText(frame, fps_text, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);

        cv::imshow("Original image", frame);
        cv::imshow("Redacted image", redacted_frame);

        // проверка нажатия Esc для выхода 
        char c = (char)cv::waitKey(33); // 33 мс ожидаем нажатие клавиши
        if (c == 27) break;
    }

    // вывод продолжительности работы программы
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double fps = frame_count / elapsed.count();
    std::cout << "Frames counted: " << frame_count << std::endl;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    std::cout << "Amount of frames per second (FPS): " << fps << std::endl;

    // закрытие видеопотока
    capture.release();
    cv::destroyAllWindows();

    return 0;
}

// запуск программы
// g++ main.cpp `pkg-config --cflags --libs opencv4`

// проверил работу камеры с помощью команд:
// cheese
// guvcview
// v4l2-ctl --list-devices

// https://docs.opencv.org/4.x/dd/d6e/tutorial_windows_visual_studio_opencv.html