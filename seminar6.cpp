#include <iostream>

typedef unsigned char byte;
#define interface struct

interface IInputStream {
	// Возвращает false, если поток закончился
	virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
	virtual void Write(byte value) = 0;
};

void Encode(IInputStream& original, IOutputStream& compressed);
// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original);


class BitReader {
public:
    BitReader(IInputStream& stream) : in(stream), pos(0), buf(0) {
    }

    // 1010 1001
    bool ReadBit(int& value) {
        if (pos == 0) {
            bool res = in.Read(buf);
            if (!res) {
                return false;
            }
        }
        value = (buf >> pos) & 1;
        pos++;
        if (pos == 8) {
            pos = 0;
        }
        return true;
    }
private:
    IInputStream& in;

    int pos;
    byte buf;

};

class BitWriter {
public:
    BitWriter(IOutputStream& stream) : out(stream), pos(0), buf(0) {

    }
    // buf = 0000 0001
    // pos = 1
    void WriteBit(byte value) {
        value = value & 1;
        buf = buf | (value << pos);
        pos++;
        if (pos == 8) {
            flush();
        }
    }

    void flush() {
        out.Write(buf);
        buf = 0;
        pos = 0;
    }
private:
    IOutputStream& out;

    int pos;
    byte buf;

};


class InputStream : public IInputStream {
public:
    InputStream(std::vector<byte>& input) : arr(input), pos(0) {

    }
	// Возвращает false, если поток закончился
	virtual bool Read(byte& value) override {
        if (pos < arr.size()) {
            value = arr[pos++];
            return true;
        }
        return false;
    }

private:
    std::vector<byte>& arr;
    int pos;
};

class OutputStream : public IOutputStream {
public:
    OutputStream(std::vector<byte>& output) : arr(output) {
    }

	virtual void Write(byte value) override {
        arr.push_back(value);
    }
private:
    std::vector<byte>& arr;
};



int main() {
    std::vector<byte> in_vec;

    InputStream input_stream(in_vec);
    OutputStream output_stream(in_vec);

    BitReader bit_reader(input_stream);
    BitWriter bit_writer(output_stream);


    bit_writer.WriteBit(1);
    bit_writer.WriteBit(0);
    bit_writer.WriteBit(1);
    bit_writer.WriteBit(0);

    bit_writer.WriteBit(1);
    bit_writer.WriteBit(0);
    bit_writer.WriteBit(1);
    bit_writer.WriteBit(0); // flush

    bit_writer.WriteBit(1);
    bit_writer.WriteBit(0);
    bit_writer.WriteBit(1);
    bit_writer.WriteBit(0);

    bit_writer.WriteBit(1);
    bit_writer.WriteBit(1);
    bit_writer.flush();

    int value = 0;
    while (bit_reader.ReadBit(value)) {
        std::cout << (int)value;
    }

    int k =0;
    k -=- 1;

}
