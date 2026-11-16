



VIdeo: https://youtu.be/XyqSipbJLDE?si=mHxNMQGHq-bCAO1T




## 1. Giới thiệu

Video này trình diễn hoạt động thực tế của module Remote I/O do mình tự thiết kế và phát triển, sử dụng vi điều khiển STM32 và giao thức truyền thông Modbus RTU trên giao diện RS485.

Module được thiết kế với:
- **4 Digital Inputs (4DI):** Tiếp nhận tín hiệu đầu vào số từ các cảm biến hoặc thiết bị ngoại vi.
- **4 Digital Outputs (4DO):** Điều khiển các thiết bị đầu ra thông qua mạch driver và relay.
- **Giao tiếp RS485:** Trao đổi dữ liệu với thiết bị điều khiển thông qua giao thức Modbus RTU.

## 2. Nguyên lý hoạt động

Module hoạt động theo mô hình Modbus RTU Slave. Thiết bị điều khiển trung tâm đóng vai trò Modbus Master, gửi yêu cầu đọc dữ liệu hoặc điều khiển đầu ra thông qua đường truyền RS485.

Quá trình hoạt động:

1. Thiết bị Master gửi yêu cầu Modbus RTU đến module.
2. Vi điều khiển STM32 tiếp nhận và xử lý khung dữ liệu.
3. Đối với yêu cầu đọc, module phản hồi trạng thái của các ngõ vào hoặc ngõ ra.
4. Đối với yêu cầu ghi, STM32 cập nhật trạng thái ngõ ra và điều khiển mạch relay tương ứng.
5. Module gửi phản hồi về Master theo giao thức Modbus RTU.

## 3. Nội dung demo

Trong video, OpenPLC chạy trên máy tính được sử dụng làm Modbus RTU Master để kiểm tra hoạt động của module.

Các nội dung được trình diễn gồm:
- Thiết lập giao tiếp giữa OpenPLC và module thông qua bộ chuyển đổi USB-to-RS485.
- Đọc trạng thái các ngõ vào số DI.
- Điều khiển các ngõ ra DO thông qua chương trình PLC.
- Kiểm tra khả năng phản hồi và trao đổi dữ liệu giữa Master và module.

## 4. Khả năng ứng dụng

Module không giới hạn trong các hệ thống sử dụng PLC. Bất kỳ thiết bị nào hỗ trợ Modbus RTU Master qua giao diện RS485 và có cấu hình truyền thông tương thích đều có thể giao tiếp với module.

Một số nền tảng có thể sử dụng:
- PLC công nghiệp.
- HMI có chức năng Modbus Master.
- Máy tính công nghiệp (IPC).
- Vi điều khiển hoặc hệ thống nhúng có hỗ trợ Modbus RTU.

## 5. Kết luận

Demo nhằm kiểm chứng khả năng giao tiếp Modbus RTU, đọc dữ liệu đầu vào và điều khiển đầu ra của module trong một hệ thống điều khiển thực tế.

Sản phẩm hướng đến khả năng mở rộng I/O linh hoạt, dễ tích hợp với nhiều nền tảng điều khiển khác nhau thông qua giao thức Modbus RTU/RS485.
