
#include <iostream>
#include <vector>

using namespace std;

class BinaryNumber {
public:
    // Конструктор по умолчанию
    BinaryNumber() : size(32) {}

    BinaryNumber(int value, int size = 32) {
        if (size != 8 && size != 16 && size != 32) {
            throw invalid_argument("Invalid size");
        }

        if ((value >= (1 << size)) || (value < -(1 << (size - 1)))) {
            throw overflow_error("Overflow");
        }

        this->size = size;
        this->value = value;
        this->bits.resize(size);

        // Перевод десятичного числа в двоичное
        for (int i = size - 1; i >= 0; i--) {
            bits[i] = (value & (1 << i)) != 0;
        }
    }

    BinaryNumber(const vector<bool>& bits, int size) {
        if (bits.size() != size) {
            throw invalid_argument("Invalid size");
        }

        this->size = size;
        this->bits = bits;

        // Вычисление десятичного представления
        value = 0;
        for (int i = 0; i < size; i++) {
            if (bits[i]) {
                value |= (1 << i);
            }
        }
    }

    void printBinary() const {
        for (int i = size - 1; i >= 0; i--) {
            cout << (bits[i] ? '1' : '0');
        }
        cout << endl;
    }

    void printDecimal() const {
        cout << value << endl;
    }

    BinaryNumber operator+(const BinaryNumber& other) const {
        BinaryNumber result(0, size);
        vector<bool> sumBits(size, false);

        int carry = 0;

        for (int i = 0; i < sumBits.size(); ++i) {
            int bitSum = bits[i] + other.bits[i] + carry;

            sumBits[i] = bitSum % 2;
            carry = bitSum / 2;
        }

        if (carry) {
            sumBits.push_back(1); // Увеличиваем размер, если есть переполнение
        }

        if (sumBits.size() > size) {
            throw overflow_error("Overflow");
        }

        result = BinaryNumber(sumBits, size);
        return result;
    }

    BinaryNumber operator-(const BinaryNumber& other) const {

        BinaryNumber result(0, size);
        vector<bool> diffBits(size, false);

        int borrow = 0;

        for (int i = 0 ; i <diffBits.size(); i++) {
            // Рассчитываем разность для текущего бита
            int diff = bits[i] - other.bits[i] - borrow;

            // Если разность меньше 0, добавляем 2 и устанавливаем флаг заимствования
            if (diff < 0) {
                diff += 2;
                borrow = 1;
            } else {
                borrow = 0;
            }

            // Записываем разность в итоговый вектор
            diffBits[i] = diff;
        }

        if (borrow) {
            throw underflow_error("Underflow"); // Проверка на переполнение
        }

        result = BinaryNumber(diffBits, size);
        return result;
    }

    BinaryNumber operator*(const BinaryNumber& other) const {
        BinaryNumber result(0, size * 2);
        vector<bool> productBits(size * 2, false);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int product = bits[i] * other.bits[j];
                int index = i + j;

                // Учитываем сумму в текущей позиции
                int sum = productBits[index] + product;

                // Обновляем биты результата и перенос
                productBits[index] = sum % 2;
                if (sum > 1) {
                    // Проверяем, не вышли ли за пределы размера массива
                    if (index + 1 < size * 2) {
                        productBits[index + 1] = true;
                    } else {
                        // Если произошло переполнение, выбрасываем исключение
                        throw overflow_error("Overflow");
                    }
                }
            }
        }

        // Создаем объект BinaryNumber на основе вектора с результатом умножения
        result = BinaryNumber(productBits, size * 2);
        return result;
    }

private:
    int value;
    vector<bool> bits;
    int size;
};

int main() {
    int num1, num2;
    char op;
    int size;

    cout << "Enter two numbers: ";
    cin >> num1 >> num2;

    cout << "Choose operation (+, -, *): ";
    cin >> op;

    cout << "Enter the size of the binary numbers (8, 16, 32): ";
    cin >> size;

    BinaryNumber bn1(num1, size);
    BinaryNumber bn2(num2, size);

    cout << "First binary number: ";
    bn1.printBinary();

    cout << "Second binary number: ";
    bn2.printBinary();

    BinaryNumber result;

    try {
        switch (op) {
            case '+':
                result = bn1 + bn2;
                break;
            case '-':
                result = bn1 - bn2;
                break;
            case '*':
                result = bn1 * bn2;
                break;
            default:
                throw invalid_argument("Invalid operation");
        }
    }
    catch (exception& e) {
        cout << "Mistake: " << e.what() << endl;
        return 1;
    }

    cout << "The result is in binary format: ";
    result.printBinary();

    cout << "The result is in decimal format: ";
    result.printDecimal();

    return 0;
}
