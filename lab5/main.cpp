//#include <opencv2/opencv.hpp>

#include <opencv2/core.hpp> // basic building blocks (cv::Mat)
#include <opencv2/highgui.hpp> // GUI functions (namedWindow; destroyAllWindows; imshow)
#include <opencv2/imgproc.hpp> // image processing (blur)

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

    cv::Mat frame, redacted_frame; // переменные для хранения каров

    int frame_count = 0; // счетчик кадров
    int frame_count_origin = 0;

    auto start = std::chrono::high_resolution_clock::now(); // начальное время
    auto origin_start = start;

    std::string fps_text, current_time;

    while (true) {
        // считывание кадра
        capture >> frame;
        if (frame.empty()) {
            std::cerr << "Error! Cannot capture the frame" << std::endl;
            break;
        }

        // редактирование изображения - сглаживание
        cv::blur(frame, redacted_frame, cv::Size(5, 5));

        // черно-белый эффект
        cv::applyColorMap(frame, redacted_frame, cv::ColormapTypes::COLORMAP_BONE);

        frame_count++;
        if (frame_count % 100 == 0) {
            auto current = std::chrono::high_resolution_clock::now();
            frame_count_origin += frame_count;

			std::chrono::duration<double> elapsed = current - start;
            start = current;
			double fps = frame_count / elapsed.count();
            frame_count = 0;

			// обновление текста
			fps_text = "FPS: " + std::to_string(fps);
            current_time = "Elapsed Time: " + std::to_string(elapsed.count());
        }

        cv::putText(frame, fps_text, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
        cv::putText(frame, current_time, cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);

        cv::imshow("Original image", frame);
        cv::imshow("Redacted image", redacted_frame);

        // проверка нажатия Esc для выхода 
        char c = (char)cv::waitKey(33); // 33 мс ожидаем нажатие клавиши
        if (c == 27) break;
    }

    frame_count_origin += frame_count;

    // вывод продолжительности работы программы
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - origin_start;
    double fps = frame_count_origin / elapsed.count();
    std::cout << "Frames counted: " << frame_count_origin << std::endl;
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