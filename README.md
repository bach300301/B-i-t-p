# Bai-tap
## Bài tập lớn Cấu trúc dữ liệu và Giải thuật

- Giới thiệu bài toán: Đo độ trùng khớp giữa tất cả các cặp tập tin văn bản trong một thư mục dữ liệu. 
Đây cũng được coi là một trường hợp đơn giản nhất của bài toán phát hiện đạo văn.

## Các đơn vị so khớp:
-    1-grams: một từ đơn.
-    2-grams: hai từ đơn liên tiếp.
-    3-grams: ba từ đơn liên tiếp.
-    1-gram và 2-grams: một từ đơn và hai từ đơn liên tiếp.
-    1-gram và 2-gram và 3-grams: một từ đơn, hai từ đơn liên tiếp, và ba từ đơn liên tiếp.

## Quy tắc tách từ: (Tham khảo http://www.cplusplus.com/reference/cstring/strtok/)
-    Tách các từ giữa các dấu câu, ký tự đặc biệt (.,:;`'/+-(){}[]<>*&^%$#@!?~/\\=).
-    Tách các từ giữa các dấu khoảng trắng ( ).
-    Tách các từ giữa các dấu xuống hàng (\r\t\n).
-    Tách các từ giữa các số (1234567890) (có nghĩa là loại bỏ các ký tự số).
-    Vậy mẫu tách từ cuối cùng  là:
        ".,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890"
- Lưu ý: Chuyển tất cả ký tự tách được về ký tự thường.

## Tiêu chí đo độ trùng khớp: 
- Số trường hợp đơn vị so khớp trong hai tập tin văn bản giống nhau. 
- Số trường hợp trùng khớp của đơn vị w (có thể là 1-grams, 2-grams, hoặc 3-grams) là f_w = min(f_w_A, f_w_B) 
(trong đó: w đại diện cho các đơn vị so khớp, f là tần số của đơn vị so khớp) trong đó A đại diện cho tập 
tin thứ nhất và B đại diện trong tập tin thứ hai.
- Ví dụ tập tin tin A ("Ba\nthuong con vi con giong me"), B ("Me con thuong con vi con giong ba") sau khi xử lý, 
chúng ta thu được các đơn vị 1-grams kèm theo tần số:
-    Tập tin A: dict_A = {"ba": 1, "thuong": 1, "con": 2, "vi": 1, "giong": 1, "me": 1}
-    Tập tin B: dict_B = {"me": 1, "thuong": 1, "con": 3, "vi": 1, "giong": 1, "ba": 1}
- Như vậy số trường hợp trùng sẽ là: f_ba + f_thuong + f_con + f_vi + f_giong + f_me = 1 + 1 + 2 + 1 + 1 + 1 = 7
- Ví dụ tập tin tin A ("Ba\nthuong con vi con giong me"), B ("Me con thuong con vi con giong ba") sau khi xử lý, 
chúng ta thu được các đơn vị 2-grams kèm theo tần số xuất hiện như sau:
 -   Tập tin A: dict_A = {"ba thuong": 1, "thuong con": 1, "con vi": 1, "vi con": 1, "con giong": 1, "giong me": 1}
 -   Tập tin B: dict_B = {"me con": 1, "con thuong": 1, "thuong con": 1, "con vi": 1, "vi con": 1, "con giong": 1, "giong ba": 1}
- Như vậy số trường hợp trùng sẽ là: f_thuongcon + f_convi + f_vicon + f_congiong =  1 + 1 + 1 + 1 = 4
- Ví dụ tập tin tin A ("Ba thuong con vi con giong me"), B ("Me thuong con vi con giong ba") sau khi xử lý, 
chúng ta có được tổng số trường hợp trùng của A và B nếu xét trên cả hai đơn vị 1-grams và 2-grams sẽ là 7 + 4 = 11.

## Cấu trúc dữ liệu sử dụng để tìm kiếm:
-    Cấu trúc dữ liệu vector<donVi> hoặc là (donVi[]) trong đó donVi {int tanSo; char *chuoi} (chỗ này tùy ý các bạn làm).
-    Cấu trúc dữ liệu binary search tree (sử dụng hàm strcmp từ thư viện <string.h> để so sánh hai chuỗi với nhau).
-    Cấu trúc dữ liệu bảng băm (tự cài cấu trúc bảng băm và sử dụng thư viện map).

- So sánh thời gian (giây) chạy tương đối của chương trình giữa các cấu trúc dữ liệu trên thư mục sm_doc_set. 
- Ghi rõ cấu hình máy chạy các chương trình (Hệ điều hành, CPUs, RAM). Tham khảo thư viện <chrono> để ghi lại thời gian thực thi.

## Yêu cầu về mã chương trình (source code):
- Các tham số đầu vào bắt buộc của chương trình
-    Tham số "-d [STRING]" có ý nghĩa: đường dẫn đến thư mục cần liệt kê các cặp có độ trùng khớp cao. Ví dụ: -d "C:\\sm_doc_set\\*.txt"
-    Tham số "-a [STRING]" có ý nghĩa: cấu trúc dữ liệu sử dụng cho chương trình
        vector: cấu trúc dữ liệu vector, list, array tùy cách các bạn dùng 
        bst: cấu trúc cây nhị phân
        map_from_scratch: cấu trúc bảng băm do tự các bạn cài
        map: cấu trúc bảng băm có sẵn trong C++
-    Tham số "-n [INT|INT-INT]" có ý nghĩa: rút trích các ngrams có độ dài là N hoặc có độ dài từ N đến M. 
    - Ví dụ: -n 1: chỉ tính 1-grams, -n 1-3: tính các 1-grams, 2-grams và 3-grams.
-    Tham số "-k [INT]" có ý nghĩa: -1 hiển thị tất cả các cặp và độ trùng khớp tương ứng; 
    hoặc k là một số lớn hơn 0 thì hiển thị k cặp tập tin có độ trùng khớp cao nhất. 
    - Ví dụ: -k -1: in tất  cả các kết quả; -k 5: in 5 kết quả có độ trùng khớp cao nhất.
- Ví dụ một lời gọi chương trình bằng dòng lệnh:
    ten_file_thuc_thi.exe -d C:\sm_doc_set\ -a vector -n 1-2 -k 5

