#include "serialization.hpp"
#include <iostream>

#include <gtest/gtest.h>

#include <fstream>
void retriveNsave(const ObjectModel::SerializationContext& c, ObjectModel::Root* r);
void save(const char*, const std::vector<uint8_t>& vector);

using ObjectModel::Array;
using ObjectModel::Primitive;
using ObjectModel::Root;
using ObjectModel::Type;
using ObjectModel::SerializationContext;

const unsigned fieldOverhead = 5;
const unsigned arrayFieldOverhead = 7;


void serDeser(const std::unique_ptr<Root>& in, std::unique_ptr<Root>& out)
{
    SerializationContext byteStream;

    in->pack(byteStream);
    retriveNsave(byteStream, in.get());
    out = Primitive::unpack(byteStream);

//    const std::vector<uint8_t>& data2 = byteStream.getData();
//    for(uint8_t v: data2)
//    {
//        std::cout << (unsigned)v << " ";
//    }
//    std::cout << std::endl;
}

//void serDeserArray(const std::unique_ptr<Root>& in, std::unique_ptr<Root>& out)
//{
//    SerializationContext byteStream;
//    in->pack(byteStream);
//    out = Array::unpack(byteStream);
//
////    const std::vector<uint8_t>& data2 = byteStream.getData();
////    for(uint8_t v: data2)
////    {
////        std::cout << (unsigned)v << " ";
////    }
////    std::cout << std::endl;
//}

TEST(SerPrimitive, Bool){
    bool foo = true;
    std::string name("_bool");

    std::unique_ptr<Root> p = Primitive::create(name, Type::BOOL, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<bool>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(bool) );
}

TEST(SerPrimitive, Int8){
    uint8_t foo = 127;
    std::string name("_int8");

    std::unique_ptr<Root> p = Primitive::create(name, Type::I8, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<uint8_t>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(uint8_t) );
}

TEST(SerPrimitive, Int16){
    uint16_t foo = 6666;
    std::string name("_int16");

    std::unique_ptr<Root> p = Primitive::create(name, Type::I16, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<uint16_t>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(uint16_t) );
}

TEST(SerPrimitive, Int32){
    uint32_t foo = 1000001;
    std::string name("_int32");

    std::unique_ptr<Root> p = Primitive::create(name, Type::I32, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<uint32_t>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(uint32_t) );
}

TEST(SerPrimitive, Int64){
    uint64_t foo = 999999999999;
    std::string name("_int64");

    std::unique_ptr<Root> p = Primitive::create(name, Type::I64, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<uint64_t>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(uint64_t) );
}

TEST(SerPrimitive, Float){
    float foo = 345.675853;
    std::string name("_float");

    std::unique_ptr<Root> p = Primitive::create(name, Type::FLOAT, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<float>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(float) );
}

TEST(SerPrimitive, Double){
    double foo = 345.675853;
    std::string name("_double");

    std::unique_ptr<Root> p = Primitive::create(name, Type::DOUBLE, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<double>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(double) );
}

TEST(SerArray, Int32){
    std::vector<double> foo = {7473.256, 34998.256, 3489.2568, 14589.12, 3434.145, 97.23, 478512.1,
                                83953.256, 8459.125, 1458.1234, 1456879.133};

    for(unsigned i = 0 ; i < 50000; ++i) foo.push_back(3.1415 + 345.14589*(i ^ (i<<1)));


    std::string name(std::string("array") + std::to_string(foo.size()) + "_double");

    std::unique_ptr<Root> p = Array::create(name, Type::DOUBLE, foo);
    SerializationContext byteStream;
    p->pack(byteStream);

//    const std::vector<uint8_t>& data2 = byteStream.getData();
//    for(uint8_t v: data2)
//    {
//        std::cout << (unsigned)v << " ";
//    }
//    std::cout << std::endl;

    std::unique_ptr<Array> out = Array::unpack(byteStream);

    retriveNsave(byteStream, p.get());
//    std::unique_ptr<Root> p2;
//
//    serDeserArray(p, p2);

    EXPECT_EQ(foo, ObjectModel::array_cast< std::vector<double> >(*out));
    EXPECT_EQ(out->getName(), name);
    EXPECT_EQ(out->getSize(), (uint16_t)(arrayFieldOverhead + name.size() + sizeof(double) * foo.size()) );
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


void retriveNsave(const ObjectModel::SerializationContext& c, ObjectModel::Root* r)
{
    std::string name = r->getName().substr(0, r->getName().length()).append(".abc");
    save(name.c_str(), c.getData());
}

void save(const char* file, const std::vector<uint8_t>& buffer)
{
    std::ofstream out;
    out.open(file);

    for (unsigned i = 0; i < buffer.size(); i++)
    {
        out << buffer[i];
    }

    out.close();
}
