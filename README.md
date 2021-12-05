## Bài Báo Cáo Lab 1 

### NHÓM THỰC HIỆN: 


| STT | MSSV | Họ tên | 
| :----: | :-: | :-: |
| 1 | 19521509 | Nguyễn Văn Hiếu |
| 2 | 19521485 | Phạm Phúc Hậu |
| 3 | 19522354 | Bùi Văn Tình |
# 1.1. Truyền dữ liệu không sử dụng TLS
- File: server.c : Dùng để chạy trên Ubuntu Server, cho phép mở port, blind, listen và xử lý các thông điệp từ client là Hello+ thông điệp.
- File: client.c: Dùng để chạy trên máy Ubuntu Desktop, khi chạy file thì điền ip của máy client khi có thông báo nhập và nhập thông điệp cần gửi.
# 1.2. Truyền dữ liệu sử dụng TLS
- File: openssl_server.c : Dùng để chạy trên Ubuntu Server, Khi chạy cần điền vào số port cần mở  ,blind, listen và xử lý các thông điệp từ client là Hello+ thông điệp.
- File: openssl_client.c: Dùng để chạy trên máy Ubuntu Desktop, khi chạy file thì điền ip của máy client và port cần mở khi có thông báo nhập và nhập thông điệp cần gửi.
