#include "serialization.hpp"
#include <iostream>

#include <gtest/gtest.h>

#include <fstream>
void save(const ObjectModel::SerializationContext& c, const std::string& name);
void print(const ObjectModel::SerializationContext& c);

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
    out = Primitive::unpack(byteStream);

    print(byteStream);
}


TEST(SerPrimitive, Bool){
    bool foo = true;
    std::string name("_bool");

    std::unique_ptr<Root> p = Primitive::create(name, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<bool>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(bool) );
}

TEST(SerPrimitive, Int8){
    uint8_t foo = 127;
    std::string name("_int8");

    std::unique_ptr<Root> p = Primitive::create(name, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<uint8_t>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(uint8_t) );
}

TEST(SerPrimitive, Int16){
    uint16_t foo = 6666;
    std::string name("_int16");

    std::unique_ptr<Root> p = Primitive::create(name, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<uint16_t>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(uint16_t) );
}

TEST(SerPrimitive, Int32){
    uint32_t foo = 1000001;
    std::string name("_int32");

    std::unique_ptr<Root> p = Primitive::create(name, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<uint32_t>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(uint32_t) );
}

TEST(SerPrimitive, Int64){
    uint64_t foo = 999999999999;
    std::string name("_int64");

    std::unique_ptr<Root> p = Primitive::create(name, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<uint64_t>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(uint64_t) );
}

TEST(SerPrimitive, Float){
    float foo = 345.675853;
    std::string name("_float");

    std::unique_ptr<Root> p = Primitive::create(name, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<float>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(float) );
}

TEST(SerPrimitive, Double){
    double foo = 345.675853;
    std::string name("_double");

    std::unique_ptr<Root> p = Primitive::create(name, foo);
    std::unique_ptr<Root> p2;

    serDeser(p, p2);

    EXPECT_EQ(foo, ObjectModel::root_cast<double>(*p2));
    EXPECT_EQ(p2->getName(), name);
    EXPECT_EQ(p2->getSize(), fieldOverhead + name.size() + sizeof(double) );
}

TEST(SerArray, Double){
    std::vector<double> foo = {7473.256, 34998.256, 3489.2568, 14589.12, 3434.145, 97.23, 478512.1,
                                83953.256, 8459.125, 1458.1234, 1456879.133};

    for(unsigned i = 0 ; i < 50000; ++i) foo.push_back(3.1415 + 345.14589*(i ^ (i<<1)));


    std::string name("raw dataset");

    std::unique_ptr<Root> p = Array::create(name, foo);
    SerializationContext byteStream;
    p->pack(byteStream);

    std::unique_ptr<Array> out = Array::unpack(byteStream);

    //save(byteStream, "data.dat");

    EXPECT_EQ(foo, ObjectModel::array_cast< std::vector<double> >(*out));
    EXPECT_EQ(out->getName(), name);
    EXPECT_EQ(out->getSize(), (uint16_t)(arrayFieldOverhead + name.size() + sizeof(double) * foo.size()) );
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


void save(const ObjectModel::SerializationContext& c, const std::string& name)
{
    std::ofstream out;
    out.open(name.c_str());

    for (unsigned i = 0; i < c.getData().size(); i++)
    {
        out << c.getData()[i];
    }

    out.close();
}

void print(const ObjectModel::SerializationContext& c)
{
    const std::vector<uint8_t>& data2 = c.getData();
    std::cout << ">>> [";
    for(uint8_t v: data2)
    {
        std::cout << (unsigned)v << " ";
    }
    std::cout << "]" << std::endl;
}
